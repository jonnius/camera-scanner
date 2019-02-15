import QtQuick 2.6
import Ubuntu.Components 1.3
import Ubuntu.Content 1.3

PageHeader {
    title: i18n.tr('Camera Scanner')

    signal deleteImage()
    signal saveImage()

    StyleHints {
        foregroundColor: fgColor
        backgroundColor: bgColor
        dividerColor.visible: false
    }

    trailingActionBar {
        numberOfSlots: 3
        actions: [

            Action {
                iconName: "delete"
                shortcut: "supr"
                text: i18n.tr("Delete")

                onTriggered: deleteImage();
            },

            Action {
                iconName: "edit"
                shortcut: "Ctrl+e"
                text: i18n.tr("Edit")

                onTriggered: {
                    console.log("Not implemented: Edit image page")
                }
            },

            Action {
                iconName: "save"
                shortcut: "Ctrl+s"
                text: i18n.tr("Save")

                onTriggered: saveImage();
            }
        ]
    }
}
