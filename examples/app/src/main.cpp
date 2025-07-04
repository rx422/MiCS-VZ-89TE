#include <Arduino.h>
#include "MiCS-VZ-89TE.h"
#include "VZ89TE_wrapper.h"

#define BAUDRATE			115200
#define REQUEST_RECURRENCE	1000

MiCS_VZ_89TE_t sensor;

void setup(void)
{
	MiCS_VZ_89TE_error_t error;

	Serial.begin(BAUDRATE);

	i2c_begin();

	error = MiCS_VZ_89TE_init(&sensor, I2C_OUTPUT, MICS_VZ_89TE_I2C_ADDR_DEFAULT, read_data);
	if (error != SENSOR_OK)
	{
		Serial.printf("Error initializing sensor: %d\n", error);
		while (true);		
	}
	else
	{
		printf("MiCS VZ-89TE Sensor Initialized\n");
	}

	error = MiCS_VZ_89TE_request_revision(&sensor);
	if (error != SENSOR_OK)
	{
		printf("Error requesting sensor revision: %d\n", error);
	}
	else
	{
		printf("%d-%d-%d %c %d\n", get_year(&sensor), get_month(&sensor), get_day(&sensor), get_rev(&sensor), get_crc(&sensor));
	}

	error = MiCS_VZ_89TE_request_r0(&sensor);
	if (error != SENSOR_OK)
	{
		printf("Error requesting R0 calibration: %d\n", error);
	}
	else
	{
		printf("R0 Calibration: %d Ohms\n", get_r0_calibration(&sensor));
	}
}

void loop(void)
{
	static uint32_t next_request = 0;
	static MiCS_VZ_89TE_error_t error;

	if (millis() < next_request)
	{
		return;
	}

	error = MiCS_VZ_89TE_cyclic_call(&sensor);
	next_request = millis() + REQUEST_RECURRENCE;

	if (error != SENSOR_OK)
	{
		printf("Error reading sensor data: %d\n", error);
		return;
	}

	if (get_crc(&sensor) != compute_crc(sensor.buffer, VZ89TE_CRC))
	{
		printf("CRC mismatch! Expected: %d, Computed: %d\n", get_crc(&sensor), compute_crc(sensor.buffer, VZ89TE_CRC));
		return;
	}

	printf("VOC: %d ppb, CO2: %d ppm\n", (int)get_VOC(&sensor), (int)get_CO2(&sensor));
	printf("Raw data: %d Ohms\n", get_raw_resistor_value(&sensor));
}