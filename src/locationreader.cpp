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
const int MAX_TIME_DIFFERENCE = 5 * 60 * 1000;

/**
 * @brief Number of position events to consider the position is valid.
 */
const int EVENTS_POSITION_VALID = 4;

/**
 * @brief Artificial value of invalid timestamp.
 */
const int INVALID_TIMESTAMP = -1;

LocationReader::LocationReader(QObject *parent) :
    QObject(parent),
    m_positionSource(QGeoPositionInfoSource::createDefaultSource(this)),
    m_numEvents(0),
    m_startTime(INVALID_TIMESTAMP),
    m_partialDuration(0),
    m_distance(0),
    m_currentSpeed(0),
    m_lastPosition(),
    m_lastPositionAccuracy(0),
    m_refreshGuiNotifications(true),
    m_updateInterval(0),
    m_lastTimestamp(0)
{
    if(m_positionSource != NULL) {
        m_positionSource->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);

        connect(m_positionSource, SIGNAL(error(QGeoPositionInfoSource::Error)),
                this, SLOT(error(QGeoPositionInfoSource::Error)));
        connect(m_positionSource, SIGNAL(updateTimeout()),
                this, SLOT(updateTimeout()));
        connect(m_positionSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));

        qDebug() << "Available position sources:" << QGeoPositionInfoSource::availableSources();
        qDebug() << "Used position source:" << m_positionSource->sourceName();
        qDebug() << "Minimum update interval:" << m_positionSource->minimumUpdateInterval();
    } else {
        qWarning() << "Creating of QGeoPositionInfoSource failed";
    }
}

LocationReader::~LocationReader()
{
    qDebug() << "Location reader destructor";

    if(m_positionSource != NULL) {
        m_positionSource->stopUpdates();

        // Released by Qt parent mechanism
        // delete m_positionSource;
        // m_positionSource = NULL;
    }
}

int LocationReader::updateInterval() const
{
    return m_updateInterval;
}

void LocationReader::setUpdateInterval(int millis)
{
    m_updateInterval = millis;
}

void LocationReader::enableUpdates(bool enable)
{
    if(m_positionSource == NULL) {
        qWarning() << "Position source is NULL while enabling updates";
        // TODO: notification to the user
        return;
    }

    if(enable) {
        qDebug() << "Enabling location updates";
        m_numEvents = 0;
        m_positionSource->startUpdates();
        m_positionSource->setUpdateInterval(0);
    } else {
        qDebug() << "Disabling location updates";
        m_positionSource->stopUpdates();

        if(m_startTime != INVALID_TIMESTAMP) {
            m_partialDuration += currentTime() - m_startTime;
            m_startTime = INVALID_TIMESTAMP;
        }
    }
}

void LocationReader::updateTimeout()
{
    qWarning() << "Position update timeout";
    m_numEvents = 0;
    m_positionSource->setUpdateInterval(0);
}

void LocationReader::error(QGeoPositionInfoSource::Error positioningError)
{
    qWarning() << "Position error:" << positioningError;
    // TODO: notification to the user
}

void LocationReader::positionUpdated(const QGeoPositionInfo &info)
{
#ifdef QT_DEBUG
    dumpPositionInfo(info);
#endif // QT_DEBUG

    if(!info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy)) {
        qDebug() << "Horizontal accuracy is missing, ignoring event";
        return;
    }

    qint64 now = currentTime();

    if(now - MAX_TIME_DIFFERENCE > info.timestamp().toMSecsSinceEpoch()) {
        qDebug() << "Position is too old, ignoring event:" << info.timestamp();
        return;
    }

    ++m_numEvents;
    QGeoCoordinate coordinate = info.coordinate();

    if(m_numEvents == EVENTS_POSITION_VALID) {
        qDebug() << "Position is considered valid, setting long term update interval:" << m_updateInterval;
        m_positionSource->setUpdateInterval(m_updateInterval);

        if(m_startTime == INVALID_TIMESTAMP) {
            m_startTime = now;
        }
    }

    qreal accuracy = info.attribute(QGeoPositionInfo::HorizontalAccuracy);

    if(m_numEvents > EVENTS_POSITION_VALID) {
        qreal distanceIncrement = m_lastPosition.distanceTo(coordinate);

        if(distanceIncrement > m_lastPositionAccuracy + accuracy) {
            m_distance += distanceIncrement;
            m_currentSpeed = distanceIncrement / (now - m_lastTimestamp) * 1000;

            optionallyRefreshGui();

            m_lastPosition = coordinate;
            m_lastPositionAccuracy = accuracy;
            m_lastTimestamp = now;

#ifdef QT_DEBUG
            dumpState();
#endif // QT_DEBUG
        } else {
            qDebug() << "Distance is lower than accuracy, ignoring event:"
                     << distanceIncrement << "<" << m_lastPositionAccuracy << "+" << accuracy;
        }
    } else {
        m_lastPosition = coordinate;
        m_lastPositionAccuracy = accuracy;
        m_lastTimestamp = now;
    }
}

void LocationReader::dumpPositionInfo(const QGeoPositionInfo &info) const {
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
}

void LocationReader::dumpState() const {
    qDebug() << "Duration:" << duration();
    qDebug() << "Distance:" << distance();
    qDebug() << "Current speed:" << currentSpeed();
    qDebug() << "Average speed:" << averageSpeed();
}

bool LocationReader::refreshGuiNotifications() const
{
    return m_refreshGuiNotifications;
}

void LocationReader::setRefreshGuiNotifications(bool refreshGuiNotifications)
{
    qDebug() << "Setting refresh GUI flag:" << refreshGuiNotifications;
    m_refreshGuiNotifications = refreshGuiNotifications;
    optionallyRefreshGui();
}

void LocationReader::optionallyRefreshGui() const {
    if(m_refreshGuiNotifications) {
        qDebug() << "Refreshing GUI";
        emit refreshGui();
    }
}

qint64 LocationReader::rawDuration() const {
    if(m_startTime != INVALID_TIMESTAMP) {
        return m_partialDuration + currentTime() - m_startTime;
    } else {
        return m_partialDuration;
    }
}

QString LocationReader::duration() const
{
    return formatDuration(rawDuration());
}

QString LocationReader::distance() const
{
    return formatDistance(m_distance);
}

QString LocationReader::currentSpeed() const
{
    return formatSpeed(m_currentSpeed);
}

QString LocationReader::averageSpeed() const
{
    quint64 currentDuration = rawDuration();

    if(currentDuration != 0) {
        return formatSpeed(m_distance / (currentDuration / 1000.0));
    } else {
        return formatSpeed(0);
    }
}

QString LocationReader::formatDuration(qint64 millis) const {
    int hours = millis / 3600000;
    int hoursRemainder = millis % 3600000;
    int minutes = hoursRemainder / 60000;
    int minutesRemainder = hoursRemainder % 60000;
    int seconds = minutesRemainder / 1000;

    QString result;
    return result.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
}

QString LocationReader::formatDistance(qreal meters) const {
    return QString::number(meters / 1000.0, 'f', 2);
}

QString LocationReader::formatSpeed(qreal metersPerSecond) const {
    return QString::number(metersPerSecond * 3.6, 'f', 2);
}

qint64 LocationReader::currentTime() const {
    return QDateTime::currentDateTime().currentMSecsSinceEpoch();
}
