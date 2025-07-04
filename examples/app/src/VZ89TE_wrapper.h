/**
 * MiCS-VZ-89TE – Indoor Air Quality Sensor Driver
 * Copyright (C) 2025  rx422 <ad.birnaz@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef VZ89TE_WRAPPER_H
#define VZ89TE_WRAPPER_H

#include <Arduino.h>
#include <Wire.h>
#include "MiCS-VZ-89TE.h"

void i2c_begin(void);
void read_data(MiCS_VZ_89TE_t *sensor, MiCS_VZ_89TE_request_t *request);

#endif
