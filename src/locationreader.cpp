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

#include <QtPositioning/QGeoPositionInfoSource>
#include <QtPositioning/QGeoPositionInfo>
#include <QtPositioning/QGeoCoordinate>

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QIODevice>

#include "locationreader.h"

LocationReader::LocationReader(QObject *parent) :
    QObject(parent)
{
    QFile file("/home/nemo/stator.log");
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);

    QGeoPositionInfoSource* source = QGeoPositionInfoSource::createDefaultSource(this);

    if(source != NULL) {
        out << "starting updates" << endl;
        source->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        source->setUpdateInterval(5000);

        connect(source, SIGNAL(error(QGeoPositionInfoSource::Error)),
                this, SLOT(error(QGeoPositionInfoSource::Error)));
        connect(source, SIGNAL(updateTimeout()),
                this, SLOT(updateTimeout()));
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));

        source->startUpdates();
    } else {
        out << "impossible to start updates" << endl;
    }

    file.close();
}

LocationReader::~LocationReader()
{
    QFile file("/home/nemo/stator.log");
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);

    out << "Destructor" << endl;

    file.close();
}

void LocationReader::updateTimeout()
{
    QFile file("/home/nemo/stator.log");
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);

    out << "Update timeout" << endl;

    file.close();
}

void LocationReader::error(QGeoPositionInfoSource::Error positioningError)
{
    QFile file("/home/nemo/stator.log");
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);

    out << "Error:" << positioningError << endl;

    file.close();
}

void LocationReader::positionUpdated(const QGeoPositionInfo &info)
{
    QFile file("/home/nemo/stator.log");
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);

    out << "Timestamp: " << info.timestamp().toString() << endl;

    QGeoCoordinate coordinate = info.coordinate();
    out << "QGeoCoordinate:" << coordinate.toString() << endl;
    out << "Altitude: " << coordinate.altitude() << endl;
    out << "Latitude: " << coordinate.latitude() << endl;
    out << "Longitude: " << coordinate.longitude() << endl;

    out << "Direction: " << (info.hasAttribute(QGeoPositionInfo::Direction)
                           ? QString::number(info.attribute(QGeoPositionInfo::Direction))
                           : "undefined") << endl;
    out << "GroundSpeed: " << (info.hasAttribute(QGeoPositionInfo::GroundSpeed)
                             ? QString::number(info.attribute(QGeoPositionInfo::GroundSpeed))
                             : "undefined")<< endl;
    out << "VerticalSpeed: " << (info.hasAttribute(QGeoPositionInfo::VerticalSpeed)
                               ? QString::number(info.attribute(QGeoPositionInfo::VerticalSpeed))
                               : "undefined")<< endl;
    out << "MagneticVariation: " << (info.hasAttribute(QGeoPositionInfo::MagneticVariation)
                                   ? QString::number(info.attribute(QGeoPositionInfo::MagneticVariation))
                                   : "undefined")<< endl;
    out << "HorizontalAccuracy: " << (info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy)
                                    ? QString::number(info.attribute(QGeoPositionInfo::HorizontalAccuracy))
                                    : "undefined")<< endl;
    out << "VerticalAccuracy: " << (info.hasAttribute(QGeoPositionInfo::VerticalAccuracy)
                                  ? QString::number(info.attribute(QGeoPositionInfo::VerticalAccuracy))
                                  : "undefined")<< endl;
    out << "----" << endl;

    file.close();
}
