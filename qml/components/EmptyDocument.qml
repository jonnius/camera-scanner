/*
  Copyright (C) 2015, 2016 Stefano Verzegnassi

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

import QtQuick 2.6
import Ubuntu.Components 1.3

Item {
    anchors.fill: parent

    EmptyState {
        id: state
        anchors {
            topMargin: units.gu(8)
            left: parent.left
            right: parent.right
            margins: units.gu(2)
            verticalCenter: parent.verticalCenter
        }
        title: i18n.tr("Empty Project")
        subTitle: i18n.tr("Please, tap on the + icon to add images to the project")
        iconName: "insert-image"

    }
}
