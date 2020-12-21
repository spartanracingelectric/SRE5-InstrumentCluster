#ifndef I2C_H_
#define I2C_H_

#include <util/twi.h>

void error();
void twi_init(void);
void twi_start(uint8_t i2c_address);
void twi_write(uint8_t data);
void twi_stop(void);

#endif /* I2C_H_ */