import QtQuick 2.9
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3

ListItem {
    property var name: ""
    property var value: ""
    property var icon: "settings"
    property var rightIcon: ""
    height: layout.height

    ListItemLayout {
        id: layout
        title.text: name
        subtitle.text: value
        Icon {
            name: icon
            width: units.gu(3)
            height: units.gu(3)
            visible: icon !== ""
            SlotsLayout.position: SlotsLayout.Leading
        }

        Icon {
            SlotsLayout.position: SlotsLayout.Trailing
            name: rightIcon
            visible: rightIcon !== ""
            width: units.gu(2)
            height: width
        }
    }
}
