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