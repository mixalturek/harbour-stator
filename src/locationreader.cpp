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

#include <QDebug>
#include <QtPositioning/QGeoPositionInfoSource>
#include <QtPositioning/QGeoPositionInfo>
#include <QtPositioning/QGeoCoordinate>

#include "locationreader.h"

LocationReader::LocationReader(QObject *parent) :
    QObject(parent)
{
    QGeoPositionInfoSource* source = QGeoPositionInfoSource::createDefaultSource(this);

    if(source != NULL) {
        qDebug() << "Starting location updates";
        source->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        // source->setUpdateInterval(5000);

        connect(source, SIGNAL(error(QGeoPositionInfoSource::Error)),
                this, SLOT(error(QGeoPositionInfoSource::Error)));
        connect(source, SIGNAL(updateTimeout()),
                this, SLOT(updateTimeout()));
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));

        source->startUpdates();
    } else {
        qWarning() << "Starting of location updates failed";
    }
}

LocationReader::~LocationReader()
{
    qDebug() << "Location reader destructor";
}

void LocationReader::updateTimeout()
{
    qWarning() << "Location update timed out";
}

void LocationReader::error(QGeoPositionInfoSource::Error positioningError)
{
    qWarning() << "Error:" << positioningError;
}

void LocationReader::positionUpdated(const QGeoPositionInfo &info)
{
    qDebug() << "Timestamp:" << info.timestamp();

    QGeoCoordinate coordinate = info.coordinate();
    qDebug() << "QGeoCoordinate:" << coordinate;
    qDebug() << "Latitude:" << coordinate.latitude();
    qDebug() << "Longitude:" << coordinate.longitude();
    qDebug() << "Altitude:" << coordinate.altitude();

    qDebug() << "Direction:" << (info.hasAttribute(QGeoPositionInfo::Direction)
        ? QString::number(info.attribute(QGeoPositionInfo::Direction)) : "undefined");
    qDebug() << "GroundSpeed:" << (info.hasAttribute(QGeoPositionInfo::GroundSpeed)
        ? QString::number(info.attribute(QGeoPositionInfo::GroundSpeed)) : "undefined");
    qDebug() << "VerticalSpeed:" << (info.hasAttribute(QGeoPositionInfo::VerticalSpeed)
        ? QString::number(info.attribute(QGeoPositionInfo::VerticalSpeed)) : "undefined");
    qDebug() << "MagneticVariation:" << (info.hasAttribute(QGeoPositionInfo::MagneticVariation)
        ? QString::number(info.attribute(QGeoPositionInfo::MagneticVariation)) : "undefined");
    qDebug() << "HorizontalAccuracy:" << (info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy)
        ? QString::number(info.attribute(QGeoPositionInfo::HorizontalAccuracy)) : "undefined");
    qDebug() << "VerticalAccuracy:" << (info.hasAttribute(QGeoPositionInfo::VerticalAccuracy)
        ? QString::number(info.attribute(QGeoPositionInfo::VerticalAccuracy)): "undefined");
    qDebug() << "----";
}
