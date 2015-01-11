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

class LocationReader : public QObject
{
    Q_OBJECT

public:
    explicit LocationReader(QObject *parent = 0);
    virtual ~LocationReader();

signals:
    // void positionChanged();

public slots:
    void enableUpdates(bool enable);

private slots:
    void error(QGeoPositionInfoSource::Error positioningError);
    void updateTimeout();
    void positionUpdated(const QGeoPositionInfo &info);

private:
    QGeoPositionInfoSource* positionSource;
};

#endif // LOCATIONREADER_H
