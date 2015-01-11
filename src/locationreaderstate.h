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

#ifndef LOCATIONREADERSTATE_H
#define LOCATIONREADERSTATE_H

/**
 * @brief State of location reader.
 */
enum LocationReaderState {
    /**
     * Start of position processing.
     */
    STATE_BEGINNING = 0,

    /**
     * Skip the first event, it contains last known location on the device,
     * which may be weeks in the past.
     */
    STATE_FIRST_EVENT,

    /**
     * At least two events were received, difference may be computed.
     */
    STATE_MULTIPLE_EVENTS
};

#endif // LOCATIONREADERSTATE_H
