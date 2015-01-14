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
import harbour.stator.LocationReader 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.All

    property string sport: ""
    property bool paused: true
    property alias updateInterval: locationReader.updateInterval

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: paused ? qsTr("Go") : qsTr("Pause")
                onClicked: {
                    paused = !paused
                    locationReader.enableUpdates(!paused)
                }
            }
        }

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
                description: paused ? qsTr("Paused") : qsTr("Active")
            }

            KeyValue {
                id: duration
                key: qsTr("Duration")
                value: formatDuration(0)
            }

            KeyValue {
                id: distance
                key: qsTr("Distance")
                value: formatDistance(0)
                unit: qsTr("km")
            }

            KeyValue {
                id: altitude
                key: qsTr("Altitude")
                value: "↑ 0, ↓ 0"
                unit: qsTr("m")
            }

            KeyValue {
                id: currentSpeed
                key: qsTr("Current Speed")
                value: formatSpeed(0)
                unit: qsTr("km/h")
            }

            KeyValue {
                id: averageSpeed
                key: qsTr("Average Speed")
                value: formatSpeed(0)
                unit: qsTr("km/h")
            }
        }

        VerticalScrollDecorator { flickable: flickable }
    }

    LocationReader {
        id: locationReader
        onDurationUpdated: duration.value = formatDuration(millis)
        onDistanceUpdated: distance.value = formatDistance(meters)
        onCurrentSpeedUpdated: currentSpeed.value = formatSpeed(metersPerSecond)
        onAverageSpeedUpdated: averageSpeed.value = formatSpeed(metersPerSecond)
    }

    function formatDuration(millis) {
        var hours = Math.floor(millis / 3600000);
        var hoursRemainder = millis % 3600000;
        var minutes = Math.floor(hoursRemainder / 60000);
        var minutesRemainder = hoursRemainder % 60000;
        var seconds = Math.floor(minutesRemainder / 1000);

        hours = (hours < 10) ? '0' + hours : hours;
        minutes = (minutes < 10) ? '0' + minutes : minutes;
        seconds = (seconds < 10) ? '0' + seconds : seconds;

        return hours + ":" + minutes + ":" + seconds
    }

    function formatDistance(meters) {
        return (meters / 1000.0).toFixed(1)
    }

    function formatSpeed(metersPerSecond) {
        return (metersPerSecond * 3.6).toFixed(1)
    }
}
