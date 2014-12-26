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
//import QtPositioning 5.0
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
            }

            anchors {
                right: parent.right
                left: parent.left
            }

            Label {
                // TODO:
                x: Theme.paddingLarge
                text: qsTr("TODO")
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            Flow {
                width: parent.width
                spacing: Theme.paddingLarge

                ImageAndText {
                    imageSource: "../images/sports/walk.svg"
                    text: qsTr("Walk")
                }

                ImageAndText {
                    imageSource: "../images/sports/run.svg"
                    text: qsTr("Run")
                }

                ImageAndText {
                    imageSource: "../images/sports/skate.svg"
                    text: qsTr("Skate")
                }

                ImageAndText {
                    imageSource: "../images/sports/bike.svg"
                    text: qsTr("Bike")
                }

                ImageAndText {
                    imageSource: "../images/sports/walk.svg"
                    text: qsTr("Walk")
                }

                ImageAndText {
                    imageSource: "../images/sports/run.svg"
                    text: qsTr("Run")
                }

                ImageAndText {
                    imageSource: "../images/sports/skate.svg"
                    text: qsTr("Skate")
                }

                ImageAndText {
                    imageSource: "../images/sports/bike.svg"
                    text: qsTr("Bike")
                }
             }
        }

//        Component.onCompleted: positionSource.start()

//        PositionSource {
//            id: positionSource
//        }
    }
}
