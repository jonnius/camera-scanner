import QtQuick 2.6
import QtQuick.Layouts 1.3
import Ubuntu.Components 1.3
import ImageProcessing 1.0
import "components"

MainView {
    id: mainView
    objectName: 'mainView'
    applicationName: 'camerascanner.jonius'
    automaticOrientation: true

    width: units.gu(45)
    height: units.gu(75)

    readonly property color bgColor: "#0e8cba"
    readonly property color fgColor: "#f7f7f7"
    readonly property color txtColor: "#3d3d3d"

    property var activeTransfer: null
    property double gridmargin: units.gu(1)
    property double mingridwidth: units.gu(15)

    PageStack {
        id: pageStack
        Component.onCompleted: pageStack.push(mainPage)
    }

    MainPage {
        id: mainPage
        anchors.fill: parent
    }

    Component.onCompleted: ImageProcessing.restoreCache()
}
