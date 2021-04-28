#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>
#include<inttypes.h>
#include  "main.h"

#define timeout 100000

#define one_byte 1
#if !defined (ZERO_byte)
#define ZERO_byte 0x00
#endif

#define R_RX_PAYLOAD 0x61

#define HIGH 1
#define LOW 0

/*
  turn on the radio 
  enable the radio
*/ 
void turn_on_radio();
/*
  turn off the radio
  disable the radio
*/
void turn_off_radio();

/*
   read the register value , payload width
   and status  so just give the type as STATUS , read_reg(reg), 
   RX_PAYLOAD_WID
*/
uint8_t spi_read(uint8_t type);
/*

this function can be used to write the value of register 
, payload , address of transmission

*/
void spi_write(uint8_t type, uint8_t *data, uint8_t data_size);
/*
  this will write the cmd to the radio
  there are three cmd you can write 
  FLUSH_TX, FLUSH_RX, RUSE_TX_PAYLOAD

*/
void spi_write_cmd(uint8_t type);
/*
  read the data from the RX FIFO 

*/
void read_data(uint8_t type, uint8_t *data, uint8_t size_data);


#endif
