#ifndef VZ89TE_WRAPPER_H
#define VZ89TE_WRAPPER_H

#include <Arduino.h>
#include <Wire.h>
#include "MiCS-VZ-89TE.h"

void i2c_begin(void);
void read_data(MiCS_VZ_89TE_t *sensor, MiCS_VZ_89TE_request_t *request);

#endif
