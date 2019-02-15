import QtQuick 2.6
import Ubuntu.Components 1.3
import Ubuntu.Content 1.3
import ImageProcessing 1.0
import Ubuntu.Components.Popups 1.3

PageHeader {
    title: i18n.tr('Camera Scanner')

    StyleHints {
        foregroundColor: fgColor
        backgroundColor: bgColor
        dividerColor.visible: false
    }

    trailingActionBar {
        numberOfSlots: 3
        actions: [

            Action {
                iconName: "info"
                shortcut: "Ctrl+i"
                text: i18n.tr("Information")

                onTriggered: {
                    pageStack.push(Qt.resolvedUrl("../InfoPage.qml"));
                }
            },

            Action {
                iconName: "add"
                shortcut: "Ctrl+a"
                text: i18n.tr("Add")

                onTriggered: {
                    Qt.inputMethod.hide();
                    pageStack.push(Qt.resolvedUrl("../ImportPage.qml"),{"contentType": ContentType.Pictures, "handler": ContentHandler.Source})
                }
            },

            Action {
                iconName: "save"
                shortcut: "Ctrl+s"
                text: i18n.tr("Save")

                onTriggered: {
                    var url = ImageProcessing.exportAllAsPdf()
                    console.log("Share:",url)
                    var sharePopup = PopupUtils.open(shareDialog, mainPage, {"contentType" : ContentType.Documents})
                    sharePopup.items.push(contentItemComponent.createObject(mainPage, {"url" : url, "text": "export"}))
                }
            }
        ]
    }
}
