#include "hal_data.h"
/*
 *
 * I2C read and write subroutines
 *
 */
ssp_err_t g_int_i2c_bus_slave_addr_set( uint16_t const slave);
ssp_err_t g_int_i2c_bus_read(uint8_t * const p_src, uint32_t const bytes, bool const restart);
ssp_err_t g_int_i2c_bus_write(uint8_t * const p_src, uint32_t const bytes, bool const restart);

ssp_err_t g_ext_i2c_bus_slave_addr_set( uint16_t const slave);
ssp_err_t g_ext_i2c_bus_read(uint8_t * const p_src, uint32_t const bytes, bool const restart);
ssp_err_t g_ext_i2c_bus_write(uint8_t * const p_src, uint32_t const bytes, bool const restart);




