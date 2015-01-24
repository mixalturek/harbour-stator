#
# Copyright (C) 2015 Michal Turek
#
# This file is part of Stator application.
# https://github.com/mixalturek/harbour-stator/
#
# Stator is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3 of the License.
#
# Stator is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Stator.  If not, see <http://www.gnu.org/licenses/>.
#

TARGET = harbour-stator

CONFIG += sailfishapp positioning
LIBS += -lQt5Positioning

SOURCES += src/harbour-stator.cpp \
    src/locationreader.cpp

HEADERS += \
    src/locationreader.h

OTHER_FILES += qml/harbour-stator.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-stator.spec \
    rpm/harbour-stator.yaml \
    translations/*.ts \
    harbour-stator.desktop \
    qml/pages/AboutPage.qml \
    qml/components/HeaderAndText.qml \
    qml/pages/MainPage.qml \
    rpm/harbour-stator.changes \
    translations/harbour-stator-cs_CZ.ts \
    qml/pages/SportPage.qml \
    README.md \
    qml/components/SportIcon.qml \
    qml/components/KeyValue.qml \
    qml/pages/ResultsPage.qml \
    resources/images/sports/walking.svg \
    resources/images/sports/running.svg \
    resources/images/sports/skating.svg \
    resources/images/sports/skiing.svg \
    resources/images/sports/biking.svg \
    resources/images/sports/riding.svg \
    resources/images/sports/driving.svg \
    resources/images/sports/training.svg \
    resources/images/sports/README.txt \
    resources/images/sports/TEMPLATE.svg \
    .gitignore

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-stator-cs_CZ.ts

RESOURCES += \
    resources/resources.qrc
