/*
 * Copyright (C) 2015 Michal Turek
 *
 * This file is part of Stator application.
 * https://github.com/mixalturek/harbour-stator/
 *
 * Stator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Stator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Stator.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    property alias imageSource: image.source
    property alias text: text.text

    width: image.width;
    height: image.height + text.height
    color: "transparent"

    Image {
        id: image
        width: Theme.iconSizeLarge
        height: Theme.iconSizeLarge
    }

    Label {
        id: text
        font.pixelSize: Theme.fontSizeExtraSmall
        color: Theme.primaryColor
        anchors.top: image.bottom
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
    }
}
