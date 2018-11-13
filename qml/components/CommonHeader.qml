import QtQuick 2.6
import Ubuntu.Components 1.3


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
					mainPageStack.push(Qt.resolvedUrl("../about.qml"));
				}
			},
			
			Action {
				iconName: "add"
				shortcut: "Ctrl+a"
				text: i18n.tr("Add")
				onTriggered: {
					Qt.inputMethod.hide();
					mainPageStack.push(Qt.resolvedUrl("../SettingsPage.qml"));
				}
			},
			Action {
				iconName: "save"
				shortcut: "Ctrl+s"
				text: i18n.tr("Save")
				onTriggered: {
					mainPageStack.push(Qt.resolvedUrl("../about.qml"));
				}
			}
		]
	}
}
