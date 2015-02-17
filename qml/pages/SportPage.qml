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
import QtGraphicalEffects 1.0
import harbour.stator.LocationReader 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.All

    property string sport: ""
    property string sportImage

    property bool paused: true
    property alias updateInterval: locationReader.updateInterval
    property bool pageActive: Qt.application.active && page.status === PageStatus.Active

    onPageActiveChanged: {
        durationTimer.running = !paused && pageActive
        locationReader.refreshGuiNotifications = !paused && pageActive
    }

    onPausedChanged: {
        durationTimer.running = !paused && pageActive
        locationReader.refreshGuiNotifications = !paused && pageActive
    }

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
                    durationTimer.running = !paused && pageActive
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
                value: "00:00:00"

                Timer {
                    id: durationTimer
                    interval: 1000
                    repeat: true
                    running: !paused && pageActive
                    onTriggered: duration.value = locationReader.duration
                 }
            }

            KeyValue {
                id: distance
                key: qsTr("Distance")
                value: "0.00"
                unit: qsTr("km")
            }

            KeyValue {
                id: currentSpeed
                key: qsTr("Current Speed")
                value: "0.00"
                unit: qsTr("km/h")
            }

            KeyValue {
                id: averageSpeed
                key: qsTr("Average Speed")
                value: "0.00"
                unit: qsTr("km/h")
            }
        }

        Image {
            id: statusImage
            source: sportImage
            width: Theme.iconSizeLarge
            height: Theme.iconSizeLarge

            anchors {
                top: parent.top
                topMargin: Theme.paddingLarge
                horizontalCenter: parent.horizontalCenter
            }
        }

        ColorOverlay {
            anchors.fill: statusImage
            source: statusImage
            color: Theme.highlightColor
        }

        Image {
            id: notImage
            source: "qrc:/images/sports/not.svg"
            width: Theme.iconSizeLarge
            height: Theme.iconSizeLarge

            anchors {
                top: parent.top
                topMargin: Theme.paddingLarge
                horizontalCenter: parent.horizontalCenter
            }

            visible: paused
        }

        VerticalScrollDecorator { flickable: flickable }
    }

    LocationReader {
        id: locationReader
        onRefreshGui: {
            distance.value = locationReader.distance
            currentSpeed.value = locationReader.currentSpeed
            averageSpeed.value = locationReader.averageSpeed
        }
    }
}
