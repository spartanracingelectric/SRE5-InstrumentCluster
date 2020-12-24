#ifndef I2C_H_
#define I2C_H_

#include <util/twi.h>

void error(); /* Error handling. Currently undefined */
void twi_init(void); /* Initialize I2C and its bitrate */
void twi_start(uint8_t i2c_address); /* Start I2C transmission */
void twi_write(uint8_t data); /* Write a byte of data through I2C */
void twi_stop(void); /* Stop I2C transmission */

#endif /* I2C_H_ */