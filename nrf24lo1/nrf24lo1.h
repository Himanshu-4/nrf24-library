#ifndef _nrf24lo1_H_
#define _nrf24lo1_H_

#include <inttypes.h>




#define one_byte 1

#define CRC_1_byte 0
#define CRC_2_byte 1

#define enable 1
#define disable 0



#define TX 0
#define RX 1

///////// defining SPI Command of nrf24lo1+////////////////////////////
#define read_reg(reg) ((uint8_t)(reg))
#define write_reg(reg) ((uint8_t)(0x20 | reg))

//// for reading and writing payload payload /////

// Read RX PAYLOAD 1 to 32 byte LSByte First


// write TX payload 1 to 32 byte LSByte First
#define W_TX_PAYLOAD 0xA0

// Flush TX FIFO
#define FLUSH_TX 0xE1

// FLush RX FIFO  should not be done when Radio is waiting for the ACK
#define FLUSH_RX 0xE2

// REUSE TX Payload (last transmitted) TX payload reuse is active until FLUSH TX or W_TX_payload is executed
#define REUSE_TX_PAYLOAD 0xE3

// read RX payload width  for the top
#define R_RX_PAYLOAD_WID 0x60

/*
   used in RX mode  write payload to be transmitted together with an ACK Packet on pipe PPP 
   maximum three ACK packet payload can be pending  packet with same PPP are handle using 
   first in first out principle 
  */
#define write_ACK_payload(pipe) ((uint8_t)(0xA8 | pipe))

// used in TX mode Disable AUTO_ACK on this specific packet
#define W_TX_PAYLOAD_NO_ACK 0xB0

// no operation can be used to read the status register
#define STATUS_READ 0xFF


#define set(REG, BIT) (REG |= 1<<BIT)

#define clear(REG, BIT) (REG &= ~ (1 << BIT))


#define read(REG, BIT) ((REG>>BIT) & 0x01)

#ifndef ZERO_byte
#define ZERO_byte ((uint8_t)0x00)
#endif


//delay defined from end of transmission to start of next transmission
#define W_250us   0x00 
#define W_500us   0x01 
#define W_750us   0x02 
#define W_1000us  0x03 
#define W_1250us  0x04 
#define W_1500us  0x05 
#define W_1750us  0x06 
#define W_2000us  0x07 
#define W_2250us  0x08 
#define W_2500us  0x09 
#define W_2750us  0xA 
#define W_3000us  0xB 
#define W_3250us  0xc 
#define W_3500us  0xd 
#define W_3750us  0xe 
#define W_4000us  0xf 

// automatic retransmit count
#define NO_RET 0x00
#define ONE_RET 0x01
#define TWO_RET 0x02
#define THREE_RET 0x03
#define FOUR_RET 0x04
#define FIVE_RET 0x05
#define SIX_RET 0x06
#define SEVEN_RET 0x07
#define EIGHT_RET 0x08
#define NINE_RET 0x09
#define TEN_RET 0xA
#define ELEVEN_RET 0xB
#define TWELVE_RET 0xC
#define THIRTEEN_RET 0xD
#define FOURTEEN_RET 0xE
#define FIFTEEN_RET 0xF

typedef enum _radio_status
{

   radio_ok,
   radio_busy,
   radio_free,
   radio_error
}radio_status;
typedef enum _transmission_status
{
  radio_err,
   radio_data_sent,
   radio_data_recv_pipe0,
   radio_data_recv_pipe1,
   radio_data_recv_pipe2,
   radio_data_recv_pipe3,
   radio_data_recv_pipe4,
   radio_data_recv_pipe5,
   radio_max_ret_occur,
   radio_ack_have_payload,
   radio_TX_FIFO_full
}TRX_Status;

typedef enum _Radio_status
{
  Data_sent = 1,
  Data_recv_pipe0,
  Data_recv_pipe1,
  Data_recv_pipe2,
  Data_recv_pipe3,
  Data_recv_pipe4,
  Data_recv_pipe5,
  MAX_ret_occur,
  Data_sent_ACK_payl,
  TX_FIFO_Full,
  clear,
  error

} rad_status;


typedef enum _Radio_data_rate
{
  _250_Kbps_ = 0,
  _1_Mbps_,
  _2_Mbps_
} Radio_data_rate;

typedef enum _Radio_TX_Power
{
  VeryLow = 1,
  Low,
  High,
  VeryHigh
} Radio_TX_Power;
typedef enum _Radio_address_length
{
  _3_bytes = 3,
  _4_bytes,
  _5_bytes

} Radio_address_length;
typedef enum _Radio_data_pipe
{
  Data_Pipe0 = 0,
  Data_Pipe1,
  Data_Pipe2,
  Data_Pipe3,
  Data_Pipe4,
  Data_Pipe5
} Radio_data_pipe;


typedef struct _Radio_init_typedef
{
  // initialize the radio config parameter
  uint8_t RX_DR_IRQ : 1;
  uint8_t TX_DS_IRQ : 1;
  uint8_t MAX_RT_IRQ : 1;
  uint8_t EN_Crc : 1;
  uint8_t CRC_Encoding : 1;
 
} Radio_init_typedef;

typedef struct _Auto_retransmission
{
  uint8_t EN_DA;
  uint8_t auto_re_delay;
  uint8_t auto_re_count;

} Auto_retransmit_typedef;



typedef struct _Data_Pipe_typedef
{
  /* 
      this is the pipe no if you select a particular pipe 
      no the pipe will get automatically activated and
      configured based on your settings      
    */
  Radio_data_pipe pipe_no;
  /*
       this field is to enable the auto ack feature for that data pipe 
     */
  uint8_t EN_AUTO_ACK;
  /*
       this field is used in PRX mode actually it gives the no of data bytes 
       present in that PIPE  
    */
  uint8_t Data_length;
  /*
       this field is used to enable the dynamic payload length for that data pipe 
       the dynamic payload length must be enbled if you want to recieve ACK with payloads  
    */
  uint8_t EN_Dpl;

} Data_Pipe_typedef;

typedef struct _Radio_typedef
{

  Radio_init_typedef Init;
  Radio_address_length Length;
  Radio_TX_Power Power;
  Radio_data_rate Data_rate;
  Auto_retransmit_typedef ARETR;
  uint8_t RF_ch;
  uint8_t state;

} Radio_typedef;


/* #############################################################################################
        ###########################################################################################
        #################################################################################*/

// functions section

radio_status Radio_init(Radio_typedef *Radio);

/* 
  this function will start the radio to transmit a packet to a particular addr
    once  make sure to not use this function when the radio is used in RX mode 
    because the RX use this addr field to transmit an ACK packet 
   @param1 - radio_typedef 
   @param2 - address 
*/

radio_status Radio_start_transmit(Radio_typedef *radio,  uint8_t *address);

/*
count the number of packet that sent by the radio if PRX doesnt recieve it 
 @max value -15 the counter reset when a new packet is going to transmit 

*/
uint8_t  Radio_count_retries();

/*
  trasnmit the data packet to the radio if the size of data packet is large than 32 byte (max payload size) this function return an error 
 the IRQ is reflected when the data is completely transmitted 
 @param 1 = pointer to the data 
 @param2 = sizeof data
*/
radio_status Radio_transmit(uint8_t *data, uint8_t sizeof_data);
/*
  transmit the data but also request the PRX for an ACK
  the status of data sent is only happen when ACK is recieved 
  make sure that you dont use start listiening until you dont recieve the ACK 
  because the TX payload only get removes from the FIFO when ACK is recieved 
  @param1 = pointer to the data 
  @param2 = sizeof data

*/
radio_status Radio_transmit_ACK(uint8_t *data, uint8_t sizeof_data);


/*
   reuse the TX payload if max ret occurs 
   can be used to retransmit the data whenever max retries occur 

*/
radio_status Radio_resend();

////////////// for reception of the data

/*
   config the pipe  to RX for a particular address and assign the data to a particular pipe 
   make sure you only write one byte data address for the piepe no 2 to 5
   the upper bytes are configured from the pipe 1 
   @param1= Radio_typedef 
   @param2 = address pointer 
   @param3 = pipe_typedef 
   @retval = status 
*/

radio_status Radio_open_pipe( uint8_t *addr, Data_Pipe_typedef *pipeno);

/*
  disable the data pipe and vanish its address , data pipe 1 or 0 is enable by default 
  there shall be a data pipe enabled before calling the function start_listiening
  this function will set the radio to idle if it is in RX mode and after restart the Radio
  @param =  data_pipe struct 
*/
radio_status Radio_close_pipe(Data_Pipe_typedef *piepno);
/*
  start to  listen for the incoming traffic and handling auto packet validation 
  auto packet dissassembly 
  auto packet acknowledgement 
  @param1 =  radio start in PRX mode
  @retval = status 
*/
radio_status Radio_start_listiening(Radio_typedef *radio);

/*
  stop the listeing and radio is configured in the TX mode 
  all the pipe related content is not affected 
   you can use stop listening when you want to reconfigure your data pipes
   flush the RX FIFO 
   @param1 = Radio_typedef 
*/
radio_status Radio_stop_listiening(Radio_typedef *radio);

/*
  give the radio status
  that is 
  Data sent --> by PTX 
  data recieved_pipeX ---> by PRX and also give the data pipe no onto which data is recieved 
     it is programmer overhead to call the function get data in correct format so that data will be recieved 
     only on the correct pipe buffer  

  max_ret occured --> by PTX 
  recived ACK have payload ----> by PTX 
  error 
*/
TRX_Status Radio_status();
/*
  this function is used to get the data lenghth of the recieved data 
  if the dynamic payload lenght is enabled 
*/
uint8_t Radio_data_len();
/*
  this function will copy the data present in the FIFO to the input data ptr
  and returns the size of data 
  if dynamic payload length is disabled the size of data buff must be equal to the recieved data 
  it is programmer headache to handle the recived data correctly 

*/
radio_status Radio_get_data(uint8_t *data);
/*
  powerdown the radio to enter into sleep mode 
  in sleep mode the radio draw only 900na 

*/
void Radio_poweroff(void);
/*
  this function is used to write the payload to be sent with the ACK by the PRX 
  you have to give the data , size and the pipe no 
  this function only recieve payload for three pipes and once the ACK is transmitted the 
  the payload is removed from the fifo 
  this function must be called before listening for the data 
  this function  returns an error when you are going to write  more than 3 pipe payload 
*/
radio_status Radio_write_ACK_Pay(uint8_t *data , uint8_t data_len , Radio_data_pipe pipe_no);
/*
   this is the IRQ handler for the radio 
   this must be copied into the EXTI handler 
   because it will configure the radio
   give the radio status 

*/
void Radio_IRQ_handler(Radio_typedef *radio);
/*
this is a function which is called when an interrupt is fired you can implement this function in your main code
like get the data or status in the interrupt part even you can use this pointer to typecast to another pointer 
so that you can pass user defined data type to your project 
@todo  i am suppose to do include multiple void pointer as 
argument
*/
void IRQcallback(void *ptr);
/* 

  this function will output a constant carrier wave to 
  analyze the working range of the radio in different conditions 
  
*/
void Radio_carrier_out(Radio_typedef *radio);
#endif
