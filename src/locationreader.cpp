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
    QObject(parent),
    positionSource(QGeoPositionInfoSource::createDefaultSource(this)),
    state(STATE_BEGINNING),
    duration(0),
    lastTimestamp(0)
{
    if(positionSource != NULL) {
        positionSource->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);

        connect(positionSource, SIGNAL(error(QGeoPositionInfoSource::Error)),
                this, SLOT(error(QGeoPositionInfoSource::Error)));
        connect(positionSource, SIGNAL(updateTimeout()),
                this, SLOT(updateTimeout()));
        connect(positionSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));

        qDebug() << "Available position sources:" << QGeoPositionInfoSource::availableSources();
        qDebug() << "Used position source:" << positionSource->sourceName();
        qDebug() << "Minimum update interval:" << positionSource->minimumUpdateInterval();
        qDebug() << "Update interval:" << positionSource->updateInterval();
    } else {
        qWarning() << "Creating of QGeoPositionInfoSource failed";
    }
}

LocationReader::~LocationReader()
{
    qDebug() << "Location reader destructor";

    if(positionSource != NULL) {
        positionSource->stopUpdates();

        // Released by Qt parent mechanism
        // delete positionSource;
        // positionSource = NULL;
    }
}

void LocationReader::enableUpdates(bool enable)
{
    if(positionSource == NULL) {
        qWarning() << "Position source is NULL while enabling updates";
        // TODO: notification to the user
        return;
    }

    if(enable) {
        qDebug() << "Enabling location updates";
        state = STATE_BEGINNING;
        positionSource->startUpdates();
    } else {
        qDebug() << "Disabling location updates";
        positionSource->stopUpdates();
    }
}

void LocationReader::updateTimeout()
{
    qWarning() << "Position update timeout";
    // TODO: notification to the user
}

void LocationReader::error(QGeoPositionInfoSource::Error positioningError)
{
    qWarning() << "Position error:" << positioningError;
    // TODO: notification to the user
}

void LocationReader::positionUpdated(const QGeoPositionInfo &info)
{
    qDebug() << "----";
    qDebug() << "Position updated";
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

    if(state == STATE_BEGINNING) {
        state = STATE_FIRST_EVENT;
        return;
    }

    qint64 timestamp = info.timestamp().toMSecsSinceEpoch();

    if(state == STATE_MULTIPLE_EVENTS) {
        duration += timestamp - lastTimestamp;
        emit durationUpdated(duration);
    }

    state = STATE_MULTIPLE_EVENTS;
    lastTimestamp = timestamp;
}
