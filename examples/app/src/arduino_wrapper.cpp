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

#include "VZ89TE_wrapper.h"

void i2c_begin(void)
{
    Wire.begin();
}

void read_data(MiCS_VZ_89TE_t *sensor, MiCS_VZ_89TE_request_t *request)
{
    Wire.beginTransmission(sensor->address);
	for (uint8_t i = 0; i < request->SIZE; i++)
	{
		Wire.write(request->buffer[i]);
	}
    Wire.endTransmission();

    delay(2);

    Wire.requestFrom(sensor->address, request->RESPONSE_SIZE);
    for (uint8_t i = 0; i < request->RESPONSE_SIZE; i++)
    {
        sensor->buffer[i] = Wire.read();
    }
}