#include"modbus_dummy.h"
#include<iostream>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define print_name std::clog<<"MODBUS_DUMMY:"<<__func__<<std::endl;
modbus_t*modbus_new_rtu(const char *device,int baud,char parity,int data_bit,int stop_bit){
  print_name
  return new modbus_t;
}
int modbus_connect(modbus_t*ctx){
  print_name
  return 0;//success
}
void modbus_close(modbus_t *ctx){
  print_name
}
void modbus_free(modbus_t *ctx){
  print_name
  delete ctx;
}
#undef print_name
int modbus_set_slave(modbus_t *ctx, int slave){
  *ctx=slave;
  return 0;
}
int modbus_read_registers(modbus_t*ctx,int addr,int nb,uint16_t *dest){
  static int count=0;
  if(*ctx==2)return -1;//simulate malfunctioning detector
  count=(count+1)%1000;
  if(addr==3&&nb==1){
    dest[0]=(*ctx>10)?20:100;
    return nb;
  }
  if(addr==104&&nb==4){
    dest[0]=count+(*ctx)*1000;
    dest[1]=0;
    union{float f;uint16_t b[2];};
    f=8+count%5;
    dest[2]=b[0];
    dest[3]=b[1];
    return nb;
  }
  if(addr==38&&nb==7){
    dest[0]=count+(*ctx)*1000;
    union{float f;uint16_t b[2];};
    f=300+((*ctx)-20)*50+count%10;
    dest[5]=b[0];
    dest[6]=b[1];
    return nb;
  }
  if(addr==37&&nb==1){
    //data_ready
    dest[0]=(*ctx!=20)?1<<6:0;
    return nb;
  }
  for(int i=0;i<nb;++i){
    dest[i]=1+i+1000**ctx;
  }
  return nb;
}
int modbus_write_register(modbus_t *ctx, int addr, int value){
  return 1;
}
int modbus_write_registers(modbus_t *ctx, int addr, int value,uint16_t *src){
  return 1;
}
float modbus_get_float(const uint16_t *src){
  union{float ret;uint16_t buf[2];};
  buf[0]=src[0];
  buf[1]=src[1];
  return ret;
}
void modbus_set_response_timeout(modbus_t*ctx,struct timeval*timeout){
  return;
}
#pragma GCC diagnostic pop
