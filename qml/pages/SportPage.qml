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
import "../components"

Page {
    id: page
    // allowedOrientations: Orientation.All

    property string sport: ""

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: page.width
            spacing: Theme.paddingMedium

            anchors {
                right: parent.right
                left: parent.left
                margins: Theme.paddingLarge
            }

            PageHeader {
                title: sport
            }

            KeyValue {
                id: duration
                key: qsTr("Duration")
                value: "00:42:42"
            }

            KeyValue {
                id: distance
                key: qsTr("Distance")
                value: "14.23"
                unit: qsTr("km")
            }

            KeyValue {
                id: altitude
                key: qsTr("Altitude")
                value: "↑ 200, ↓ 42"
                unit: qsTr("m")
            }

            KeyValue {
                id: currentSpeed
                key: qsTr("Current Speed")
                value: "2.3"
                unit: qsTr("km/h")
            }

            KeyValue {
                id: averageSpeed
                key: qsTr("Average Speed")
                value: "2.1"
                unit: qsTr("km/h")
            }
        }

        VerticalScrollDecorator { flickable: flickable }
    }
}
