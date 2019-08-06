import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import ImageProcessing 1.0

Dialog {
    id: root

    title: i18n.tr("Delete all")
           
    Label { 
        text: i18n.tr("Are you sure you want to delete all images?")
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }
    
    Button {
        id: okButton
        objectName: "okButton"
        text: i18n.tr("OK")
        color: UbuntuColors.red

        onClicked: {
            ImageProcessing.removeAll()
            mainPage.imageModel.clear();
            PopupUtils.close(root)
        }
    }

    Button {
        id: cancelButton
        objectName: "cancelButton"
        text: i18n.tr("Cancel")

        color: UbuntuColors.graphite

        onClicked: {
            PopupUtils.close(root)
        }
    }
}
