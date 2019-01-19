import QtQuick 2.9
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import "./components"

Page {
    id: infoPage
    anchors.fill: parent

    header: PageHeader {
        id: header
        title: i18n.tr('Info about Camera Scanner')
        StyleHints {
            foregroundColor: fgColor
            backgroundColor: bgColor
            dividerColor.visible: false
        }
    }


    ScrollView {
        id: scrollView
        width: parent.width
        height: parent.height - header.height
        anchors.top: header.bottom
        contentItem: Column {
            width: infoPage.width

            Icon {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: parent.width / 4
                width: parent.width / 2
                height: width
                source: "../assets/logo.svg"
            }

            SettingsListItem {
                name: i18n.tr("Report a bug")
                icon: "stock_message"
                onClicked: Qt.openUrlExternally("https://github.com/jonnius/camera-scanner/issues")
            }

            SettingsListItem {
                name: i18n.tr("Contributors")
                icon: "contact-group"
                onClicked: Qt.openUrlExternally("https://github.com/jonnius/camera-scanner/graphs/contributors")
            }

            SettingsListItem {
                name: i18n.tr("Source code")
                icon: "text-xml-symbolic"
                onClicked: Qt.openUrlExternally("https://github.com/jonnius/camera-scanner")
            }

            SettingsListItem {
                name: i18n.tr("License")
                icon: "x-office-document-symbolic"
                onClicked: Qt.openUrlExternally("https://github.com/jonnius/camera-scanner/blob/master/LICENSE")
            }

        }
    }

}
