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

#ifndef LOCATIONREADER_H
#define LOCATIONREADER_H

#include <QObject>
#include <QElapsedTimer>
#include <QtPositioning/QGeoPositionInfoSource>
#include <QtPositioning/QGeoPositionInfo>
#include "locationreaderstate.h"

/**
 * @brief Reader of GEO position.
 */
class LocationReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval)
    Q_PROPERTY(qint64 duration READ duration)

public:
    /**
     * @brief Constructor.
     * @param parent optional parent owner
     */
    explicit LocationReader(QObject *parent = 0);

    /**
     * @brief Destructor.
     */
    virtual ~LocationReader();

    /**
     * @brief Get update interval.
     * @return update interval in milliseconds
     */
    int updateInterval() const;

    /**
     * @brief Set update interval.
     * @param millis update interval in milliseconds
     */
    void setUpdateInterval(int millis);

    /**
     * @brief Get total duration.
     * @return duration in milliseconds
     */
    qint64 duration() const;

signals:
    /**
     * @brief Total distance updated.
     * @param meters distance in meters
     */
    void distanceUpdated(qreal meters);

    /**
     * @brief Current ground speed changed.
     * @param metersPerSecond speed in meters per second
     */
    void currentSpeedUpdated(qreal metersPerSecond);

    /**
     * @brief Average ground speed changed.
     * @param metersPerSecond speed in meters per second
     */
    void averageSpeedUpdated(qreal metersPerSecond);

public slots:
    /**
     * @brief Enable or disable position updates.
     * @param enable true enable, false disable
     */
    void enableUpdates(bool enable);

private slots:
    void error(QGeoPositionInfoSource::Error positioningError);
    void updateTimeout();
    void positionUpdated(const QGeoPositionInfo &info);

private:
    void dumpPositionInfo(const QGeoPositionInfo &info);

private:
    /**
     * @brief Source of GEO position information, may be NULL on error.
     */
    QGeoPositionInfoSource* m_positionSource;

    /**
     * @brief State of location reader to prevent computations on invalid data.
     */
    LocationReaderState m_state;

    /**
     * @brief Timer for duration calculation.
     */
    QElapsedTimer m_elapsedTimer;

    /**
     * @brief Partial duration of sport activity, in milliseconds.
     */
    qint64 m_partialDuration;

    /**
     * @brief Total distance reached during sport activity, in meters.
     */
    qreal m_distance;

    /**
     * @brief Position in last valid position event.
     */
    QGeoCoordinate m_lastPosition;
};

#endif // LOCATIONREADER_H
