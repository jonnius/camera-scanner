/*
 * Copyright (C) 2014-2016 Canonical Ltd
 *
 * This file is part of Ubuntu Clock App
 *
 * Ubuntu Clock App is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * Ubuntu Clock App is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.6
import Ubuntu.Components 1.3

/*
 Component which displays an empty state (approved by design). It offers an
 icon, title and subtitle to describe the empty state.
*/

Item {
    id: emptyState

    // Public APIs
    property alias iconName: emptyIcon.name
    property alias title: emptyLabel.text
    property alias subTitle: emptySublabel.text

    height: childrenRect.height

    Label {
        id: emptyLabel
        textSize: Label.Large
        font.weight: Font.Normal
        width: parent.width
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        color: bgColor
    }

    Icon {
        id: emptyIcon

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: emptyLabel.bottom
            topMargin: units.gu(4)
        }

        height: units.gu(17)
        width: height
        color: bgColor
        asynchronous: true
    }

    Label {
        id: emptySublabel
        width: parent.width - gridmargin * 6
        wrapMode: Text.WordWrap

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: emptyIcon.bottom
            topMargin: units.gu(6)
        }

        horizontalAlignment: Text.AlignHCenter
        color: UbuntuColors.ash
    }
}
