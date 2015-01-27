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


/**
 * @brief Reader of GEO position.
 */
class LocationReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval)
    Q_PROPERTY(QString duration READ duration)
    Q_PROPERTY(QString distance READ distance)
    Q_PROPERTY(QString currentSpeed READ currentSpeed)
    Q_PROPERTY(QString averageSpeed READ averageSpeed)
    Q_PROPERTY(bool refreshGuiNotifications READ refreshGuiNotifications WRITE setRefreshGuiNotifications)

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

    bool refreshGuiNotifications() const;
    void setRefreshGuiNotifications(bool refreshGuiNotifications);

    QString duration() const;
    QString distance() const;
    QString currentSpeed() const;
    QString averageSpeed() const;

signals:
    /**
     * @brief Refresh GUI, parameter values update.
     */
    void refreshGui() const;

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
    void dumpPositionInfo(const QGeoPositionInfo &info) const;
    void dumpState() const;
    void optionallyRefreshGui() const;
    qint64 rawDuration() const;
    QString formatDuration(qint64 millis) const;
    QString formatDistance(qreal meters) const;
    QString formatSpeed(qreal metersPerSecond) const;

private:
    /**
     * @brief Source of GEO position information, may be NULL on error.
     */
    QGeoPositionInfoSource* m_positionSource;

    /**
     * @brief Number of received positioning events.
     */
    int m_numEvents;

    /**
     * @brief Timestamp of measurement start, in milliseconds.
     */
    qint64 m_startTime;

    /**
     * @brief Partial duration of sport activity, in milliseconds.
     */
    qint64 m_partialDuration;

    /**
     * @brief Total distance reached during sport activity, in meters.
     */
    qreal m_distance;

    /**
     * @brief Current speed, in meters per second.
     */
    qreal m_currentSpeed;

    /**
     * @brief Position in last valid position event.
     */
    QGeoCoordinate m_lastPosition;

    /**
     * @brief Enable flag for GUI refreshing.
     */
    bool m_refreshGuiNotifications;

    /**
     * @brief GPS update interval.
     */
    int m_updateInterval;

    /**
     * @brief Timestamp of last update.
     */
    qint64 m_lastTimestamp;
};

#endif // LOCATIONREADER_H
