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
    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        contentHeight: column.height

        Column {
            id: column
            width: page.width

            PageHeader {
                title: qsTr("Stator")
                description: qsTr("Sport Tracker")
            }

            anchors {
                right: parent.right
                left: parent.left
                margins: Theme.paddingLarge
            }

            Flow {
                width: parent.width
                spacing: Theme.paddingLarge

                SportIcon {
                    imageSource: "qrc:/images/sports/walking.svg"
                    text: qsTr("Walking")
                    updateInterval: 45000
                }

                SportIcon {
                    imageSource: "qrc:/images/sports/running.svg"
                    text: qsTr("Running")
                    updateInterval: 30000
                }

                SportIcon {
                    imageSource: "qrc:/images/sports/skating.svg"
                    text: qsTr("Skating")
                    updateInterval: 20000
                }

                SportIcon {
                    imageSource: "qrc:/images/sports/skiing.svg"
                    text: qsTr("Skiing")
                    updateInterval: 20000
                }

                SportIcon {
                    imageSource: "qrc:/images/sports/biking.svg"
                    text: qsTr("Biking")
                    updateInterval: 15000
                }

                SportIcon {
                    imageSource: "qrc:/images/sports/riding.svg"
                    text: qsTr("Riding")
                    updateInterval: 15000
                }

                SportIcon {
                    imageSource: "qrc:/images/sports/driving.svg"
                    text: qsTr("Driving")
                    updateInterval: 3000
                }

                SportIcon {
                    imageSource: "qrc:/images/sports/training.svg"
                    text: qsTr("Training")
                    updateInterval: 0
                }
            }
        }
    }
}
