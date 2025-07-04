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

#ifndef MICS_VZ_89TE_H
#define MICS_VZ_89TE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MICS_VZ_89TE_I2C_ADDR_DEFAULT	0x70	// Default I2C address for VZ-89TE
#define MICS_VZ_89TE_I2C_ADDR_MIN		0x70
#define MICS_VZ_89TE_I2C_ADDR_MAX		0x77

#define VZ89TE_INCOMING_OFFSET			13.0	// Offset for incoming data
#define VZ89TE_VOC_OFFSET				0.0		// Offset for VOC calculation
#define VZ89TE_CO2_OFFSET				400		// Offset for CO2 calculation
#define VZ89TE_CONSTANT_VOC				1000.0	// Constant for VOC calculation
#define VZ89TE_CONSTANT_CO2				1600.0	// Constant for CO2 calculation
#define VZ89TE_DIVISOR					229.0	// Divisor for calculation
#define VZ89TE_COMPUTED_VOC				(VZ89TE_CONSTANT_VOC / VZ89TE_DIVISOR) + VZ89TE_VOC_OFFSET // Computed VOC value
#define VZ89TE_COMPUTED_CO2				(VZ89TE_CONSTANT_CO2 / VZ89TE_DIVISOR) + VZ89TE_CO2_OFFSET // Computed CO2 value

#define MICS_VZ_89TE_I2C_BUFFER_SIZE	7		// Size of the buffer for I2C communication

typedef enum 
{
	SENSOR_OK = 0,					// no error
	SENSOR_NULL_POINTER,			// null pointer passed to function
	SENSOR_READ_CALLBACK_NULL,		// read callback function pointer is NULL
	SENSOR_ERROR_I2C_ADDR,			// I2C address out of range
	SENSOR_ERROR_TYPE,				// invalid output type specified
	SENSOR_ERROR_CRC,				// CRC error in received data
} MiCS_VZ_89TE_error_t;

typedef enum
{
	I2C_OUTPUT = 0u,
	PWM_OUTPUT
} VZ89TE_output_t;

typedef enum
{
    VZ89TE_CMD_SETPPMCO2    = 0x08,		// send ppmCO2 value for recalibration
    VZ89TE_CMD_RESERVED_09  = 0x09,		// reserved
    VZ89TE_CMD_RESERVED_0A  = 0x0A,		// reserved
    VZ89TE_CMD_RESERVED_0B  = 0x0B,		// reserved
    VZ89TE_CMD_GET_STATUS   = 0x0C,		// read status (6 bytes + CRC)
    VZ89TE_CMD_GET_REVISION = 0x0D,		// read revision (6 bytes + CRC)
    VZ89TE_CMD_RESERVED_0E  = 0x0E,		// reserved (overwrites calibration—don’t use)
    VZ89TE_CMD_RESERVED_0F  = 0x0F,		// reserved (overwrites calibration—don’t use)
    VZ89TE_CMD_GET_R0       = 0x10		// read R0 calibration value (6 bytes + CRC)
} VZ89TE_command_t;

typedef enum
{
    VZ89TE_VOC_LEVEL   = 0,		// Byte-1: VOC signal level
    VZ89TE_CO2_LEVEL   = 1,		// Byte-2: CO₂-equivalent level
    VZ89TE_RAW_MSB     = 2,		// Byte-3: Raw sensor value MSB
    VZ89TE_RAW_MID     = 3,		// Byte-4: Raw sensor value middle byte
    VZ89TE_RAW_LSB     = 4,		// Byte-5: Raw sensor value LSB
    VZ89TE_ERROR       = 5,		// Byte-6: Error status byte
    VZ89TE_CRC         = 6		// Byte-7: CRC byte
} VZ89TE_status_byte_index_t;

typedef enum
{
    VZ89TE_YEAR        = 0,		// Byte-1: Year
    VZ89TE_MONTH       = 1,		// Byte-2: Month
    VZ89TE_DAY         = 2,		// Byte-3: Day
    VZ89TE_CHAR        = 3,		// Byte-4: ASCII code for a character
    VZ89TE_ZERO_1      = 4,		// Byte-5: Always 0
    VZ89TE_ZERO_2      = 5,		// Byte-6: Always 0
    VZ89TE_REV_CRC     = 6		// Byte-7: CRC
} VZ89TE_revision_byte_index_t;

typedef enum
{
    VZ89TE_R0_LSB      = 0,		// Byte-1: R0 low-order byte (LSB)
    VZ89TE_R0_MSB      = 1,		// Byte-2: R0 high-order byte (MSB)
    VZ89TE_R0_ZERO1    = 2,		// Byte-3: always 0
    VZ89TE_R0_ZERO2    = 3,		// Byte-4: always 0
    VZ89TE_R0_ZERO3    = 4,		// Byte-5: always 0
    VZ89TE_R0_ZERO4    = 5,		// Byte-6: always 0
    VZ89TE_R0_CRC      = 6		// Byte-7: CRC
} VZ89TE_r0_byte_index_t;

typedef struct
{
	uint8_t buffer[MICS_VZ_89TE_I2C_BUFFER_SIZE - 1]; // Buffer for I2C communication
	const uint8_t SIZE;
	const uint8_t RESPONSE_SIZE;
} MiCS_VZ_89TE_request_t;

typedef struct VZ89TE
{
	VZ89TE_output_t output;
	uint8_t address;
	double co2;
    double voc;
    uint8_t status;
	uint8_t buffer[MICS_VZ_89TE_I2C_BUFFER_SIZE]; // Buffer for I2C communication
	void (*read_data_i2c)(struct VZ89TE*, MiCS_VZ_89TE_request_t*); // Function pointer for reading data
} MiCS_VZ_89TE_t;

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_init(
				MiCS_VZ_89TE_t *sensor,
				VZ89TE_output_t output,
				uint8_t address,
				void (*read_data_i2c)(MiCS_VZ_89TE_t*, MiCS_VZ_89TE_request_t*)
			);

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_set_read_callback(
				MiCS_VZ_89TE_t *sensor,
				void (*read_data_i2c)(MiCS_VZ_89TE_t*, MiCS_VZ_89TE_request_t*)
			);

MiCS_VZ_89TE_error_t MiCS_VZ_89TE_set_address(MiCS_VZ_89TE_t *sensor, uint8_t address);
MiCS_VZ_89TE_error_t MiCS_VZ_89TE_set_output_type(MiCS_VZ_89TE_t *sensor, VZ89TE_output_t output);

MiCS_VZ_89TE_error_t  MiCS_VZ_89TE_cyclic_call(MiCS_VZ_89TE_t *sensor);
MiCS_VZ_89TE_error_t MiCS_VZ_89TE_request_revision(MiCS_VZ_89TE_t *sensor);
MiCS_VZ_89TE_error_t MiCS_VZ_89TE_request_r0(MiCS_VZ_89TE_t *sensor);

double get_CO2(MiCS_VZ_89TE_t *sensor);
double get_VOC(MiCS_VZ_89TE_t *sensor);
uint32_t get_raw_resistor_value(MiCS_VZ_89TE_t *sensor);
uint8_t get_status(MiCS_VZ_89TE_t *sensor);

uint16_t get_r0_calibration(MiCS_VZ_89TE_t *sensor);

uint8_t get_year(MiCS_VZ_89TE_t *sensor);
uint8_t get_month(MiCS_VZ_89TE_t *sensor);
uint8_t get_day(MiCS_VZ_89TE_t *sensor);
char get_rev(MiCS_VZ_89TE_t *sensor);
uint8_t get_crc(MiCS_VZ_89TE_t *sensor);

uint8_t compute_crc(uint8_t *buffer, const uint8_t BUFFER_SIZE);

#ifdef __cplusplus
}
#endif

#endif // MICS_VZ_89TE_H