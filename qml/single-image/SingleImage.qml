import QtQuick 2.9
import Ubuntu.Components 1.3
import ImageProcessing 1.0
import Ubuntu.Content 1.3
import Ubuntu.Components.Popups 1.3
import "../components"

Page {
    id: singleImagePage
    width: parent.width

    property string currentImage
    property int currentIndex
    property string currentID

    header: SingleImageHeader {
        id: singleImageHeader
    }

    UbuntuShape {
        id: image
        aspect: UbuntuShape.Flat

        anchors{
            //horizontalCenter: parent.horizontalCenter
            topMargin: singleImageHeader.height + gridmargin
            fill: parent
            leftMargin: gridmargin
            rightMargin: gridmargin
        }

        source: Image {
            sourceSize.width: parent.width
            sourceSize.height: parent.height
            source: currentImage
        }

        sourceFillMode: UbuntuShape.PreserveAspectFit
    }

    Connections {
        target: singleImageHeader

        onDeleteImage: {
            ImageProcessing.removeImage(currentID);
            mainPage.imageModel.remove(currentIndex);
            pageStack.pop();
        }

        onSaveImage: {
            //TODO: move this code to a general function
            //TODO: save as pdf or as an image?
            var url = ImageProcessing.exportAsPdf(currentID);
            console.log("Share:",url);
            var sharePopup = PopupUtils.open(shareDialog, singleImagePage, {"contentType" : ContentType.Documents});
            sharePopup.items.push(contentItemComponent.createObject(singleImagePage, {"url" : url, "text": currentID}));
        }
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

    Component.onDestruction: {
        currentImage = "";
        //TODO: Find a more elegant way of reseting this int
        currentIndex = -1;
        currentID = "";
    }
}
