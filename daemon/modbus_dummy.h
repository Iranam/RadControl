#ifndef MODBUS_DUMMY_H
#include<stdint.h>
typedef int modbus_t;
#endif
modbus_t*modbus_new_rtu(const char *device,int baud,char parity,int data_bit,int stop_bit);
int modbus_connect(modbus_t*ctx);
void modbus_close(modbus_t *ctx);
void modbus_free(modbus_t *ctx);
#undef print_name
int modbus_set_slave(modbus_t *ctx, int slave);
int modbus_read_registers(modbus_t*ctx,int addr,int nb,uint16_t *dest);
int modbus_write_register(modbus_t *ctx, int addr, int value);
int modbus_write_registers(modbus_t *ctx, int addr, int value,uint16_t *src);
float modbus_get_float(const uint16_t *src);
void modbus_set_response_timeout(modbus_t*ctx,struct timeval*timeout);
