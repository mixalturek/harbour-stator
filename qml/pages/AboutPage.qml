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
    id: aboutPage

    SilicaFlickable {
        id: aboutPageFlickable
        anchors.fill: parent
        contentHeight: aboutColumn.height

        Column {
            PageHeader {
                title: qsTr("About")
            }

            id: aboutColumn
            anchors {
                left: parent.left;
                right: parent.right
            }
            height: childrenRect.height
            spacing: Theme.paddingLarge

            HeaderAndText {
                header: qsTr("Stator")
                text: qsTr("Stator is a sport tracker application that uses \
embeded GPS unit to continuously measure position and speed, analyze them \
and monitor your performance.")
            }

            HeaderAndText {
                header: qsTr("Version")
                text: "0.1.0-SNAPSHOT"
            }

            HeaderAndText {
                header: qsTr("Author")
                text: "Michal Turek"
            }

            BackgroundItem {
                contentHeight: licenseSection.height
                height: contentHeight

                HeaderAndText {
                    id: licenseSection
                    header: qsTr("License")
                    text: qsTr("GNU GPL version 3")
                }

                onClicked: {
                    Qt.openUrlExternally("http://www.gnu.org/copyleft/gpl.html");
                }
            }

            BackgroundItem {
                contentHeight: sourceCodeSection.height
                height: contentHeight

                HeaderAndText {
                    id: sourceCodeSection
                    header: qsTr("Source Code")
                    text: "https://github.com/mixalturek/harbour-stator/"
                }

                onClicked: {
                    Qt.openUrlExternally("https://github.com/mixalturek/harbour-stator/");
                }
            }
        }
    }

    VerticalScrollDecorator { flickable: aboutPageFlickable }
}
