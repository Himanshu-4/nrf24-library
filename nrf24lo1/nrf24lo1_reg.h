#ifndef _nrf24lo1_reg_H_
#define _nrf24lo1_reg_H_



#define CONFIG 0x00

// bit fields of the register


/*
   mask interrupt caused by RX_DR (recieve data ready) 
     set-> Interrupt not reflected to IRQ pin
     RESET -> Interrupt reflected to IRQ pin
*/
#define  MASK_RX_DR 6
/*  
     mask interrupt caused by transmitter data sent 
     set-> Interrupt not reflected to IRQ pin
     RESET -> Interrupt reflected to IRQ pin
*/
#define  MASK_TX_DS 5
/*
    MASK interrupt caused by maximum retransmission
     set-> Interrupt not reflected to IRQ pin
     RESET -> Interrupt reflected to IRQ pin
*/
#define MASK_MAX_RT 4

/*
   Enable CRC . force high if one of the bit in EN_AA high
*/
#define EN_CRC 3
/*
   CRC encoding scheme 
   SET-> 2bytes CRC
   RESET -> 1byte CRC
*/
#define CRCO 2
/*
   Power conttrol bit
    SET -> POWER_UP
    RESET -> POWER_DOWN
*/
#define PWR_UP 1
/* 
   RX , TX control 
   SET-> set the radio in RX mode
   RESET -> set the radio in Tx mode
*/
#define PRIM_RX 0



/////////////////////////////////////////////////////
/* 
   Enable auto acknowledgement function 
*/

#define EN_AA 0x01

// bit fields
/* 
  enable auto ack for data pipe 5
*/
#define ENAA_P5 5
/* 
  enable auto ack for data pipe 4
*/
#define ENAA_P4 4
/* 
  enable auto ack for data pipe 3
*/
#define ENAA_P3 3
/* 
  enable auto ack for data pipe 2
*/
#define ENAA_P2 2
/* 
  enable auto ack for data pipe 1
*/
#define ENAA_P1 1
/* 
  enable auto ack for data pipe 0
*/
#define ENAA_P0 0

////////////////////////////////////////////////////
/*
  enabled RX addresses 
*/
#define EN_RXADDR 0x02

//bit fields
/*
  enable data pipe 5
*/
#define ERX_P5 5
/*
  enable data pipe 4
*/
#define ERX_P4 4
/*
  enable data pipe 3
*/
#define ERX_P3 3
/*
  enable data pipe 2
*/
#define ERX_P2 2
/*
  enable data pipe 1
*/
#define ERX_P1 1
/*
  enable data pipe 0
*/
#define ERX_P0 0

////////////////////////////////////////////////////////////

/* 
    setup of address widths
     common for all data pipe
     LSBute are used if address width is below 5 bytes  
     reset value five bytes 
*/
#define SETUP_AW 0x03

//bit field
#define three_bytes 0x01
#define four_bytes 0x02
#define five_bytes 0x03

///////////////////////////////////////////////////////////////////
/*
  setup for automatic retransmission 
*/
#define SETUP_RETR 0x04



/*
  RF channel selection 
  RF frequency = 2.4 + RF_CH
*/
#define RF_CH 0x05

///////////////////////////////////////////////////

/*
   RF_SEtup 
*/
#define RF_SETUP 0x06

// bit field
/*
  enables continous carrier transmission when SET 
*/
#define CONT_WAVE 7
/*
  selsction of low speed data rate 
  set RF data rate to 250Kbps
*/
#define RF_DR_LOW 5

/* 
  force PLL lock signal only used in test 
*/
#define PLL_LOCK 4

/*
  selection between the high speed data rate
  this bit is dont care if RF_DR_LOW is set
  SET -> 2Mbps data rate
  RESET -> 1MBps data rate
*/
#define RF_DR_HIGH 3
/*
  RF_PWR set the output power in TX mode
*/
#define _18dbm (0x00<<1) // very low power
#define _12dbm (0x01<<1) // LOW power
#define _6dbm  (0x02<<1)  // HIGH power
#define _0dbm  (0x03<<1)  // very High power

//////////////////////////////////////////////////
/*
  status of the various fields 
*/
#define STATUS 0x07

//bit field

/* 
  data ready RX FIFO interrupt 
  when data arrives in RX FIFO
  write 1 to clear the bit 
*/
#define RX_DR 6
/*
 data sent TX FIFO interrupt  assert when packet transmitted on TX
 if AUTO_ACK is enabled the bit is sent only when ACK is recieved 
 write 1 to clear bit 
*/
#define TX_DS 5
/*
  Maximum number of TX retransmit interrupt 
  write 1 to clear the bit 
  if maximum retransmitted is asserted it must be cleared to enable further communications
*/
#define MAX_RT 4
/*
  read only parameter 
  data pipe number for the payload available for reading from RX FIFO
  000 -> data pipe 0
  001 -> data pipe 1
  010 -> data pipe 2
  011 -> data pipe 3
  100 -> data pipe 4
  101 -> data pipe 5

  110 not used 
  111 empty 

*/
#define DATA_PIPE0 0x0 << 1
#define DATA_PIPE1 0x1 << 1
#define DATA_PIPE2 0x2 << 1
#define DATA_PIPE3 0x3 << 1
#define DATA_PIPE4 0x4 << 1
#define DATA_PIPE5 0x5 << 1

/*
  TX FIFO full flag 
  SET -> TX FIFO full
  RESET -> Available locations in TX FIFO
*/
#define TX_FULL 0

/////////////////////////////////////////////////////
/*
  transmit observe register 
*/
#define OBSERVE_TX 0x08

/*
  read only parameter 
  counts lost packets 
  overflow protected 
  counter is reset by writing to RF_CH
*/

#define ONE_PACKET_LOST   (0x1 << 4)
#define TWO_PACKET_LOST   (0x2 << 4)
#define THREE_PACKET_LOST (0x3 << 4)
#define FOUR_PACKET_LOST  (0x4 << 4)
#define FIVE_PACKET_LOST  (0x5 << 4)
#define SIX_PACKET_LOST   (0x6 << 4)
#define SEVEN_PACKET_LOST (0x7 << 4)
#define EIGHT_PACKET_LOST (0x8 << 4)
#define NINE_PACKET_LOST  (0x9 << 4)
#define TEN_PACKET_LOST   (0xA << 4)
#define ELEVEN_PACKET_LOST (0xB << 4)
#define TWELVE_PACKET_LOST (0xC << 4)
#define THIRTEEN_PACKET_LOST (0xD << 4)
#define FOURTEEN_PACKET_LOST (0xE << 4)
#define FIFTEEN_PACKET_LOST (0xF << 4)

/*
  read only parameter 
  counter is RESET when transmission of a new packet starts 
  count retransmitted packet 
*/
#define ONE_PACKET_TRANSMITTED 0x1
#define TWO_PACKET_TRANSMITTED 0x2
#define THREE_PACKET_TRANSMITTED 0x3
#define FOUR_PACKET_TRANSMITTED 0x4
#define FIVE_PACKET_TRANSMITTED 0x5
#define SIX_PACKET_TRANSMITTED 0x6
#define SEVEN_PACKET_TRANSMITTED 0x7
#define EIGHT_PACKET_TRANSMITTED 0x8
#define NINE_PACKET_TRANSMITTED 0x9
#define TEN_PACKET_TRANSMITTED 0xA
#define ELEVEN_PACKET_TRANSMITTED 0xB
#define TWELVE_PACKET_TRANSMITTED 0xC
#define THIRTEEN_PACKET_TRANSMITTED 0xD
#define FOURTEEN_PACKET_TRANSMITTED 0xE
#define FIFTEEN_PACKET_TRANSMITTED 0xF

//////////////////////////////////////////////
/*
  recieved power detector . this is called carrier detect 
  SET -> recieved power is greater than -64dbm
  RESET -> recieved power is less than -64dbm
  read only param
*/
#define RPD 0x09
#define RPD_BIT 0

/////////////////////////////////////////////////////
/*
  recieved address data pipe 0 
  5 bytes maxmimum lenght (LSByte written first )
  write the no of bytes defined by SETUP_AW
*/
#define RX_ADDR_P0 0x0A
// recieved aaddress data pipe 1
#define RX_ADDR_P1 0x0B
/*
  recieved address data pipe 2 
  only LSB . MSByte are equal to RX_ADDR_P1
*/
#define RX_ADDR_P2 0x0C
/*
  recieved address data pipe 3 
  only LSB . MSByte are equal to RX_ADDR_P1
*/
#define RX_ADDR_P3 0x0D
/*
  recieved address data pipe 4
  only LSB . MSByte are equal to RX_ADDR_P1
*/
#define RX_ADDR_P4 0x0E
/*
  recieved address data pipe 5 
  only LSB . MSByte are equal to RX_ADDR_P1
*/
#define RX_ADDR_P5 0x0F

//////////////////////////////////////////////
/*
  transmit address . used for a PTX device only 
  LSBYTE is written first 
  set RX_ADDR_P0  equal to this address to handle automatic ACK 
  if this is a PTX device with enhanced ShockBurst enabled 
*/
#define TX_ADDR 0x10
/////////////////////////////////////////////////////////
/*
  no of bytes in RX payload in data pipe 0
  1 to 32 byte 
  1 -> 1byte 
  32 -> 32 byte 
*/
#define RX_PW_P0 0x11
/*
  no of bytes in RX payload in data pipe 1
  1 to 32 byte 
  1 -> 1byte 
  32 -> 32 byte 
*/
#define RX_PW_P1 0x12
/*
  no of bytes in RX payload in data pipe 2
  1 to 32 byte 
  1 -> 1byte 
  32 -> 32 byte 
*/
#define RX_PW_P2 0x13
/*
  no of bytes in RX payload in data pipe 3
  1 to 32 byte 
  1 -> 1byte 
  32 -> 32 byte 
*/
#define RX_PW_P3 0x14
/*
  no of bytes in RX payload in data pipe 4
  1 to 32 byte 
  1 -> 1byte 
  32 -> 32 byte 
*/
#define RX_PW_P4 0x15
/*
  no of bytes in RX payload in data pipe 5
  1 to 32 byte 
  1 -> 1byte 
  32 -> 32 byte 
*/
#define RX_PW_P5 0x16
//////////////////////////////////////////////////
/*
  FIFO Status register  
*/
#define FIFO_STATUS 0x17
/*
  read only parameter
  used for a PTX device 
  pulse the rfcc high for at least 10us to reuse the last transmitted payload 
  Tx payload reuse is active until W_TX_PAYLOAD or FLUSH TX is executed 
  TX_REUSE is set by SPI command REUSE_TX_PL
  reset by the SPI command W_TX_PAYLOAD or FLUSH_TX
*/
#define TX_REUSE 6
/*
  TX_FIFO full flag 
  SET -> FIFO_FULL
  RESET -> Available location
*/
#define TX_FULLL 5
/*
  TX_FIFO empty flag 
  SET -> TX_FIFO empty
  RESET -> data in FIFO
*/
#define TX_EMPTY 4
/*
  RX_FIFO full flag 
  SET -> FIFO_FULL
  RESET -> Available location
*/
#define RX_FULL 1
/*
  RX_FIFO empty flag 
  SET -> RX_FIFO empty
  RESET -> data in FIFO
*/
#define RX_EMPTY 0

//////////////////////////////////////////////////////////////////

/*
  enable dynamic payload lenght 
*/
#define DYNPD 0x1C
/*
  enable dynamic payload lenght data pipe 5
  (Require EN_DPL and EN_AA_P5)
*/
#define DPL_P5 5
/*
  enable dynamic payload lenght data pipe 4
  (Require EN_DPL and EN_AA_P4)
*/
#define DPL_P4 4
/*
  enable dynamic payload lenght data pipe 3
  (Require EN_DPL and EN_AA_P3)
*/
#define DPL_P3 3
/*
  enable dynamic payload lenght data pipe 2
  (Require EN_DPL and EN_AA_P2)
*/
#define DPL_P2 2
/*
  enable dynamic payload lenght data pipe 1
  (Require EN_DPL and EN_AA_P1)
*/
#define DPL_P1 1
/*
  enable dynamic payload lenght data pipe 0
  (Require EN_DPL and EN_AA_P0)
*/
#define DPL_P0 0
/////////////////////////////////////////////////////////////////
/*
  feature register 
*/
#define FEATURE 0x1D
/*
  ENable dynamic payload lenght 
*/
#define EN_DPL 2
/*
  enable payload with ACK 
*/
#define EN_ACK_PAY 1
/*
  enables the W_TX_PAYLOAD_NOACK command 
*/
#define EN_DYN_ACK 0

#endif
