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

/**
 * @brief Minimial horizontal accuracy to process a positioning event, in meters.
 * If the value is higher, the event will be ignored.
 */
const int MIN_HORIZONTAL_ACCURACY = 100;

LocationReader::LocationReader(QObject *parent) :
    QObject(parent),
    m_positionSource(QGeoPositionInfoSource::createDefaultSource(this)),
    m_state(STATE_BEGINNING),
    m_elapsedTimer(),
    m_partialDuration(0),
    m_distance(0),
    m_currentSpeed(0),
    m_lastPosition(),
    m_refreshGuiNotifications(true)
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
        qDebug() << "Update interval:" << m_positionSource->updateInterval();
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
    if(m_positionSource == NULL) {
        qWarning() << "Position source is NULL while getting update interval";
        // TODO: notification to the user
        return 0;
    }

    return m_positionSource->updateInterval();
}

void LocationReader::setUpdateInterval(int millis)
{
    if(m_positionSource == NULL) {
        qWarning() << "Position source is NULL while setting update interval";
        // TODO: notification to the user
        return;
    }

    qDebug() << "Setting GPS update interval:" << millis << "ms";
    m_positionSource->setUpdateInterval(millis);
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
        m_state = STATE_BEGINNING;
        m_positionSource->startUpdates();
        m_elapsedTimer.start();
    } else {
        qDebug() << "Disabling location updates";
        m_positionSource->stopUpdates();
        m_partialDuration += m_elapsedTimer.elapsed();
        m_elapsedTimer.invalidate();
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
#ifdef QT_DEBUG
    dumpPositionInfo(info);
#endif // QT_DEBUG

    qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();

    if(now - MAX_TIME_DIFFERENCE > info.timestamp().toMSecsSinceEpoch()) {
        qDebug() << "Position is too old, ignoring event:" << info.timestamp();
        return;
    }

    if(info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy)
            && info.attribute(QGeoPositionInfo::HorizontalAccuracy) > MIN_HORIZONTAL_ACCURACY) {
        qDebug() << "Too low horizontal accuracy, ignoring event:"
                 << info.attribute(QGeoPositionInfo::HorizontalAccuracy) << "m";
        return;
    }

    if(m_state == STATE_MULTIPLE_EVENTS) {
        m_distance += m_lastPosition.distanceTo(info.coordinate());
        m_currentSpeed = info.hasAttribute(QGeoPositionInfo::GroundSpeed)
            ? info.attribute(QGeoPositionInfo::GroundSpeed) : 0;

        if(m_refreshGuiNotifications) {
            emit refreshGui();
        }
    }

    m_state = STATE_MULTIPLE_EVENTS;
    m_lastPosition = info.coordinate();
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

    qDebug() << "----";
}

bool LocationReader::refreshGuiNotifications() const
{
    return m_refreshGuiNotifications;
}

void LocationReader::setRefreshGuiNotifications(bool refreshGuiNotifications)
{
    m_refreshGuiNotifications = refreshGuiNotifications;
}

qint64 LocationReader::rawDuration() const {
    if(m_elapsedTimer.isValid()) {
        return m_partialDuration + m_elapsedTimer.elapsed();
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
    QString result;
    return result.sprintf("%0.1f", meters / 1000.0);
}

QString LocationReader::formatSpeed(qreal metersPerSecond) const {
    QString result;
    return result.sprintf("%0.1f", metersPerSecond * 3.6);
}
