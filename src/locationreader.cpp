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

/**
 * @brief Maximal time difference from now to consider a positioning event
 * to be valid, in milliseconds.
 */
const int MAX_TIME_DIFFERENCE = 60000;

LocationReader::LocationReader(QObject *parent) :
    QObject(parent),
    positionSource(QGeoPositionInfoSource::createDefaultSource(this)),
    state(STATE_BEGINNING),
    elapsedTimer(),
    partialDuration(0),
    distance(0),
    lastPosition()
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

int LocationReader::updateInterval() const
{
    if(positionSource == NULL) {
        qWarning() << "Position source is NULL while getting update interval";
        // TODO: notification to the user
        return 0;
    }

    return positionSource->updateInterval();
}

void LocationReader::setUpdateInterval(int millis)
{
    if(positionSource == NULL) {
        qWarning() << "Position source is NULL while setting update interval";
        // TODO: notification to the user
        return;
    }

    qDebug() << "Setting GPS update interval:" << millis << "ms";
    positionSource->setUpdateInterval(millis);
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
        elapsedTimer.start();
    } else {
        qDebug() << "Disabling location updates";
        positionSource->stopUpdates();
        partialDuration += elapsedTimer.elapsed();
        elapsedTimer.invalidate();
    }
}

qint64 LocationReader::duration() const {
    if(elapsedTimer.isValid()) {
        return partialDuration + elapsedTimer.elapsed();
    } else {
        return partialDuration;
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
    qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();

    if(now - MAX_TIME_DIFFERENCE > info.timestamp().toMSecsSinceEpoch()) {
        qDebug() << "Position is too old, ignoring event:" << info.timestamp();
        return;
    }

    dumpPositionInfo(info);

    if(state == STATE_MULTIPLE_EVENTS) {
        distance += lastPosition.distanceTo(info.coordinate());
        emit distanceUpdated(distance);

        qreal currentSpeed = info.hasAttribute(QGeoPositionInfo::GroundSpeed)
            ? info.attribute(QGeoPositionInfo::GroundSpeed) : 0;
        emit currentSpeedUpdated(currentSpeed);

        quint64 currentDuration = duration();
        if(currentDuration != 0) {
            emit averageSpeedUpdated(distance / (currentDuration / 1000.0));
        }
    }

    state = STATE_MULTIPLE_EVENTS;
    lastPosition = info.coordinate();
}

void LocationReader::dumpPositionInfo(const QGeoPositionInfo &info) {
    qDebug() << "----";
    qDebug() << "Timestamp:" << info.timestamp();
    qDebug() << "Coordinate:" << info.coordinate();

    if(info.hasAttribute(QGeoPositionInfo::GroundSpeed)) {
        qDebug() << "GroundSpeed:" << info.attribute(QGeoPositionInfo::GroundSpeed);
    }

    if(info.hasAttribute(QGeoPositionInfo::VerticalSpeed)) {
        qDebug() << "VerticalSpeed:" << info.attribute(QGeoPositionInfo::VerticalSpeed);
    }

    if(info.hasAttribute(QGeoPositionInfo::MagneticVariation)) {
        qDebug() << "MagneticVariation:" << info.attribute(QGeoPositionInfo::MagneticVariation);
    }

    if(info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy)) {
        qDebug() << "HorizontalAccuracy:" << info.attribute(QGeoPositionInfo::HorizontalAccuracy);
    }

    if(info.hasAttribute(QGeoPositionInfo::VerticalAccuracy)) {
        qDebug() << "VerticalAccuracy:" << info.attribute(QGeoPositionInfo::VerticalAccuracy);
    }

    qDebug() << "----";
}
