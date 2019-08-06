import QtQuick 2.6
import Ubuntu.Components 1.3
import QtQml.Models 2.1
import QtGraphicalEffects 1.0
import ImageProcessing 1.0
import Ubuntu.Content 1.3
import Ubuntu.Components.Popups 1.3
import "../components"
import "../single-image"

Page {
    id: mainPage
    property alias imageModel: imageModel
    signal exportCompleted ()

    header: CommonHeader {
        id: mainHeader
    }

    function imageAdded ( id ) {
        imageModel.append (
            {
                imageID: id,
                imgout: "image://documents/"+id
            }
        )
    }

    Component {
        id: deleteAllDialog
        
        DeleteAllImgDialog {}
    }
    
    Connections {
        target: mainHeader

        onDeleteAllImage: {
            PopupUtils.open(deleteAllDialog)
        }
    }
    
    Connections {
        target: ImageProcessing
        onImageAdded: imageAdded (id)
    }

    ListModel {
        id: imageModel
    }

    Component {
        id: shareDialog

        ContentShareDialog {
            Component.onDestruction: exportCompleted()
        }
    }

    Component {
        id: contentItemComponent
        ContentItem { }
    }

    DelegateModel {
        id: visualModel
        model: imageModel

        delegate: MouseArea {
            id: delegateRoot
            property int visualIndex: DelegateModel.itemsIndex
            property var imageID: model.imageID
            width: gridview.cellWidth
            height: gridview.cellHeight
            drag.smoothed: true

            Item {
                id: icon
                width: gridview.cellWidth
                height: gridview.cellHeight
                anchors {
                    horizontalCenter: parent.horizontalCenter;
                    verticalCenter: parent.verticalCenter
                }

                Drag.active: delegateRoot.drag.active
                Drag.source: delegateRoot
                Drag.hotSpot.x: 36
                Drag.hotSpot.y: 36

                states: [
                    State {
                        when: icon.Drag.active
                        ParentChange {
                            target: icon
                            parent: mainView
                        }

                        AnchorChanges {
                            target: icon;
                            anchors.horizontalCenter: undefined;
                            anchors.verticalCenter: undefined
                        }
                    }
                ]

                Image {
                    id: mainImg

                    anchors {
                        fill: parent
                        leftMargin: mainView.gridmargin
                        rightMargin: mainView.gridmargin
                        topMargin: 1.5*mainView.gridmargin
                        bottomMargin: 1.5*mainView.gridmargin
                    }

                    asynchronous: true
                    fillMode: Image.PreserveAspectFit
                    source: Qt.resolvedUrl(model.imgout)

                    // Prevent blurry SVGs
                    sourceSize.width: 2*mainView.mingridwidth
                    sourceSize.height: 3*mainView.mingridwidth

                    /* Overlay for when image is pressed */
                    Rectangle {
                        id: overlay
                        anchors.fill: parent
                        color: "#000"
                        border.color: UbuntuColors.orange
                        border.width: 0
                        opacity: delegateRoot.pressed ? 0.3 : 0

                        Behavior on opacity {
                            NumberAnimation {
                                duration: UbuntuAnimation.SlowDuration
                            }
                        }
                    }
                }

                DropShadow {
                    anchors.fill: mainImg
                    horizontalOffset: 5
                    verticalOffset: 5
                    radius: 18
                    samples: 25
                    transparentBorder: true
                    color: "#80000000"
                    source: mainImg
                }
            }

            DropArea {
                anchors { fill: parent; margins: 15 }

                onEntered: {
                    visualModel.items.move(drag.source.visualIndex, delegateRoot.visualIndex)
                    imageModel.move(drag.source.visualIndex, delegateRoot.visualIndex, 1)
                }
            }

            onPressAndHold: {
                overlay.border.width = 10
                mouse.accepted = false
                delegateRoot.drag.target = icon
            }

            onReleased: {
                pageStack.push(Qt.resolvedUrl("../single-image/SingleImage.qml"), {"currentImage": Qt.resolvedUrl(model.imgout), "currentID": model.imageID, "currentIndex": index})
            }
        }
    }

    GridView {
        id: gridview

        anchors {
            top: mainHeader.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: units.gu(2)
            leftMargin: mainView.gridmargin
            rightMargin: mainView.gridmargin
        }

        height: mainView.height/2
        clip: true
        cellWidth: width / Math.floor(width/mainView.mingridwidth)
        cellHeight: cellWidth*1.4

        displaced: Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
        }

        model: visualModel
    }

    Loader {
        id: emptyStateLoader
        anchors.fill: parent
        active: imageModel.count === 0
        source: Qt.resolvedUrl("EmptyDocument.qml")
    }
}
