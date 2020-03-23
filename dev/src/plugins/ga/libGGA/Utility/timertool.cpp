/**
 * @file timertool.h
 * @author Yvan Da Silva <yvan.m.silva AT gmail.com>
 * @author IICT Institute for Information and Communication<www.iict.ch>
 * @author HEIG-VD (Haute école d'inénierie et de gestion) <www.heig-vd.ch>
 * @date 06.2012
 * @section LICENSE
 *
 * This application is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @class TimerTool
 * @brief Compute elapsed time between to points in time.
 *
 */
#include "timertool.h"

/**
 * @brief TimerTool::getElapsedTime
 * Get time elapsed between two time values.
 *
 * @param startTime The time of the start
 * @param endTime The time of the end
 * @return The elapsed time between start and end times.
 */
qreal TimerTool::getElapsedTime(struct timeval &startTime, struct timeval &endTime){
    qreal elapsedTime;
    elapsedTime = (endTime.tv_sec - startTime.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (endTime.tv_usec - startTime.tv_usec) / 1000.0;   // us to ms
    return elapsedTime;
}
