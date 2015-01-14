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

    int updateInterval() const;
    void setUpdateInterval(int millis);

signals:
    /**
     * @brief Duration of sport activity changed.
     * @param millis new duration in milliseconds
     */
    void durationUpdated(qint64 millis);

    /**
     * @brief Current ground speed changed.
     * @param metersPerSecond speed in meters per second
     */
    void currentSpeedUpdated(qreal metersPerSecond);

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
    /**
     * @brief Source of GEO position information, may be NULL on error.
     */
    QGeoPositionInfoSource* positionSource;

    /**
     * @brief State of location reader to prevent computations on invalid data.
     */
    LocationReaderState state;

    /**
     * @brief Total duration of sport activity.
     */
    qint64 duration;

    /**
     * @brief Current ground speed in meters per second.
     */
    qreal currentSpeed;

    /**
     * @brief Timestamp of last valid position event, in milliseconds.
     */
    qint64 lastTimestamp;
};

#endif // LOCATIONREADER_H
