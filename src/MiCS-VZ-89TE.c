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

#include "MiCS-VZ-89TE.h"

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_init(
	MiCS_VZ_89TE_t *sensor,
	VZ89TE_output_t output,
	uint8_t address,
	void (*read_data_i2c)(MiCS_VZ_89TE_t *, MiCS_VZ_89TE_request_t *))
{
	MiCS_VZ_89TE_error_t error = SENSOR_OK;

	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	if (MiCS_VZ_89TE_set_read_callback(sensor, read_data_i2c) != SENSOR_OK)
	{
		return SENSOR_READ_CALLBACK_NULL;
	}

	if (MiCS_VZ_89TE_set_output_type(sensor, output) != SENSOR_OK)
	{
		return SENSOR_ERROR_TYPE;
	}

	if (sensor->output == I2C_OUTPUT)
	{
		error = MiCS_VZ_89TE_set_address(sensor, address);
	}

	if (sensor->output == PWM_OUTPUT)
	{
		error = MiCS_VZ_89TE_set_address(sensor, MICS_VZ_89TE_I2C_ADDR_DEFAULT);
	}

	sensor->co2 = 0;
	sensor->voc = 0;
	sensor->status = 0;

	for (uint8_t i = 0; i < MICS_VZ_89TE_I2C_BUFFER_SIZE; i++)
	{
		sensor->buffer[i] = 0;
	}

	return error;
}

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_set_read_callback(
	MiCS_VZ_89TE_t *sensor,
	void (*read_data_i2c)(MiCS_VZ_89TE_t *, MiCS_VZ_89TE_request_t *))
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	if (read_data_i2c == NULL)
	{
		return SENSOR_READ_CALLBACK_NULL;
	}

	sensor->read_data_i2c = read_data_i2c;

	return SENSOR_OK;
}

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_set_output_type(MiCS_VZ_89TE_t *sensor, VZ89TE_output_t output)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	if (output != I2C_OUTPUT && output != PWM_OUTPUT)
	{
		return SENSOR_ERROR_TYPE;
	}

	sensor->output = output;
	return SENSOR_OK;
}

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_set_address(MiCS_VZ_89TE_t *sensor, uint8_t address)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	if (address < MICS_VZ_89TE_I2C_ADDR_MIN || address > MICS_VZ_89TE_I2C_ADDR_MAX)
	{
		return SENSOR_ERROR_I2C_ADDR;
	}

	sensor->address = address;
	return SENSOR_OK;
}

void convert_sensor_data(MiCS_VZ_89TE_t *sensor)
{
	sensor->status = sensor->buffer[VZ89TE_ERROR];
	sensor->voc = (sensor->buffer[VZ89TE_VOC_LEVEL] - VZ89TE_INCOMING_OFFSET) * VZ89TE_COMPUTED_VOC; // ppb: 0 .. 1000
	sensor->co2 = (sensor->buffer[VZ89TE_CO2_LEVEL] - VZ89TE_INCOMING_OFFSET) * VZ89TE_COMPUTED_CO2; // ppm: 400 .. 2000
}

MiCS_VZ_89TE_request_t *request_data_pack(VZ89TE_command_t command)
{
	const uint8_t REQUEST_CRC_INDEX = (MICS_VZ_89TE_I2C_BUFFER_SIZE - 2);

	static MiCS_VZ_89TE_request_t request =
		{
			buffer : {0},
			SIZE : MICS_VZ_89TE_I2C_BUFFER_SIZE - 1,
			RESPONSE_SIZE : MICS_VZ_89TE_I2C_BUFFER_SIZE
		};

	request.buffer[0] = command;
	request.buffer[REQUEST_CRC_INDEX] = compute_crc(request.buffer, REQUEST_CRC_INDEX);

	return &request;
}

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_cyclic_call(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	if (sensor->read_data_i2c == NULL)
	{
		return SENSOR_READ_CALLBACK_NULL;
	}

	sensor->read_data_i2c(sensor, request_data_pack(VZ89TE_CMD_GET_STATUS));
	convert_sensor_data(sensor);

	return SENSOR_OK;
}

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_request_revision(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	sensor->read_data_i2c(sensor, request_data_pack(VZ89TE_CMD_GET_REVISION));

	return SENSOR_OK;
}

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_request_r0(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	sensor->read_data_i2c(sensor, request_data_pack(VZ89TE_CMD_GET_R0));

	return SENSOR_OK;
}

double get_CO2(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return sensor->co2;
}

double get_VOC(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return sensor->voc;
}

uint32_t get_raw_resistor_value(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	// The raw resistor value is stored in three bytes (MSB, MID, LSB) and scaled by 10 as per the datasheet.
	return (sensor->buffer[VZ89TE_RAW_MSB] << 16) |
		   (sensor->buffer[VZ89TE_RAW_MID] << 8) |
		   (sensor->buffer[VZ89TE_RAW_LSB]) * 10; // scale to Ohms
}

uint8_t get_status(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return sensor->status;
}

uint16_t get_r0_calibration(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return (sensor->buffer[VZ89TE_R0_MSB] & 0x3F) << 8 | sensor->buffer[VZ89TE_R0_LSB];
}

uint8_t get_year(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return sensor->buffer[VZ89TE_YEAR];
}

uint8_t get_month(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return sensor->buffer[VZ89TE_MONTH];
}

uint8_t get_day(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return sensor->buffer[VZ89TE_DAY];
}

char get_rev(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return sensor->buffer[VZ89TE_CHAR];
}

uint8_t get_crc(MiCS_VZ_89TE_t *sensor)
{
	if (sensor == NULL)
	{
		return SENSOR_NULL_POINTER;
	}

	return sensor->buffer[VZ89TE_CRC];
}

uint8_t compute_crc(uint8_t *buffer, const uint8_t BUFFER_SIZE)
{
	uint8_t crc = 0x00;
	uint16_t sum = 0x0000;

	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		sum += buffer[i];
	}

	crc = (uint8_t)sum; // low 8 bits
	crc += (sum >> 8);	// add high-byte as carry
	crc = 0xFF - crc;	// one’s-complement

	return crc;
}