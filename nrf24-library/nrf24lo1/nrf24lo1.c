#include "nrf24lo1_reg.h"
#include "nrf24lo1.h"

#include "spi.h"

static volatile uint8_t Flag = RESET;
static volatile uint8_t number = 0;
static volatile uint8_t mode = TX;
static volatile rad_status status;


radio_status Radio_init(Radio_typedef *radio)
{
    // assignment of the pin for the radio

    uint8_t config_value = ZERO_byte;
    if (!(radio->Init.TX_DS_IRQ))
        set(config_value, MASK_TX_DS);
    if (!(radio->Init.RX_DR_IRQ))
        set(config_value, MASK_RX_DR);
    if (!(radio->Init.MAX_RT_IRQ))
        set(config_value, MASK_MAX_RT);
    if (radio->Init.EN_Crc)
    {
        set(config_value, EN_CRC);
        if (radio->Init.CRC_Encoding == CRC_2_byte)
            set(config_value, CRCO);
    }
    set(config_value, PWR_UP);
    set(config_value, PRIM_RX);

    spi_write(write_reg(CONFIG), &config_value, one_byte);

    // config the data pipe for the reciever
    uint8_t radio_addr_len = ZERO_byte;
    if (radio->Length == _3_bytes)
        radio_addr_len |= three_bytes;
    else if (radio_addr_len == _4_bytes)
        radio_addr_len |= four_bytes;
    else if (radio->Length == _5_bytes)
        radio_addr_len |= five_bytes;
    spi_write(write_reg(SETUP_AW), &radio_addr_len, one_byte);
    // set the channel for the radio
    uint8_t channel = radio->RF_ch;

    spi_write(write_reg(RF_CH), &channel, one_byte);

    // set the RF settings of the radio
    uint8_t Rf_setup = ZERO_byte;
    if (radio->Data_rate == _250_Kbps_)
    {
        set(Rf_setup, RF_DR_LOW);
        clear(Rf_setup, RF_DR_HIGH);
    }
    else if (radio->Data_rate == _2_Mbps_)
    {
        set(Rf_setup, RF_DR_HIGH);
        clear(Rf_setup, RF_DR_LOW);
    }
    else if (radio->Data_rate == _1_Mbps_)
    {
        clear(Rf_setup, RF_DR_LOW);
        clear(Rf_setup, RF_DR_HIGH);
    }
    if (radio->Power == VeryLow)
        Rf_setup |= _18dbm;
    else if (radio->Power == Low)
        Rf_setup |= _12dbm;
    else if (radio->Power == High)
        Rf_setup |= _6dbm;
    else if (radio->Power == VeryHigh)
        Rf_setup |= _0dbm;

    spi_write(write_reg(RF_SETUP), &Rf_setup, one_byte);

    // set the features of the Radio
    uint8_t feature = ZERO_byte;

        set(feature, EN_DPL);
       set(feature, EN_ACK_PAY);
       set(feature, EN_DYN_ACK);

    spi_write(write_reg(FEATURE), &feature, one_byte);

    // setup for the retransmission for the radio
    if (radio->ARETR.EN_DA == ENABLE)
    {
        uint8_t aret = ((radio->ARETR.auto_re_delay) << 4 | (radio->ARETR.auto_re_count));
        spi_write(write_reg(SETUP_RETR), &aret, one_byte);
    }

    // we have to flush the RX and TX  FIFO so that we getting the bare chip

    spi_write_cmd(FLUSH_RX);
    spi_write_cmd(FLUSH_TX);

    uint8_t fifo_status = spi_read(read_reg(FIFO_STATUS));
    if (read(fifo_status, TX_EMPTY) && read(fifo_status, RX_EMPTY))
    {
        // the TX and RX FIFO must be empty at startup
        return radio_ok;
    }

    return radio_error;
}

// for opening the writing pipe for the radio
radio_status Radio_start_transmit(Radio_typedef *radio,  uint8_t *address)
{
    if (Flag == SET)
        return radio_busy;
    // set the TX addr
    uint8_t addr_len = radio->Length;
    spi_write(write_reg(TX_ADDR), address, addr_len);
   // to recieve the ACK and the ACK payloads we have to enebale the data pipe 0
    spi_write(write_reg(RX_ADDR_P0), address, addr_len);
     // set the address width of the RX line to 32 byte 
     uint8_t len =32;
     spi_write(write_reg(RX_PW_P0),&len, one_byte);
     
    
    return radio_ok;
}

// code for transmit radio
radio_status Radio_transmit(uint8_t *data, uint8_t data_size)
{
    // check if the radio is busy in other mode
    if (Flag == SET)
        return radio_busy;

    Flag = SET;
    mode = TX;
    // first check if radio is sleeping or not
    // now check for the config register
    uint8_t config_reg = spi_read(read_reg(CONFIG));
    if (read(config_reg, PRIM_RX))
        clear(config_reg, PRIM_RX);

    if (!read(config_reg, PWR_UP))
        set(config_reg, PWR_UP);

    spi_write(write_reg(CONFIG), &config_reg, one_byte);

    spi_write(W_TX_PAYLOAD_NO_ACK, data, data_size);
    turn_on_radio();
    // Flag will bw RESET in  the IRQ handler
    return radio_ok;
}

radio_status Radio_transmit_ACK(uint8_t *data, uint8_t data_size)
{
    // check if the radio is busy in other mode
    if (Flag == SET)
        return radio_busy;

    Flag = SET;
    mode = TX;
    // now check for the config register
    uint8_t config_reg = spi_read(read_reg(CONFIG));
    if (read(config_reg, PRIM_RX))
       clear(config_reg, PRIM_RX);
        // check if radio is sleeping or not
       
        if (!read(config_reg, PWR_UP))
            set(config_reg, PWR_UP);
        // radio is in RX mode
        // switch to TX mode
        
    spi_write(write_reg(CONFIG), &config_reg, one_byte);
     
  
   spi_write(W_TX_PAYLOAD, data, data_size);

    
    turn_on_radio();
    // this flag is reset if the ACK is recieved
    return radio_ok;
}
// retransmitting the data
radio_status Radio_resend()
{
    if (Flag == SET)
        return radio_busy;

    Flag = SET;
    mode = TX;
    // now check for the config register
    uint8_t config_reg = spi_read(read_reg(CONFIG));
    if (read(config_reg, PRIM_RX))
        clear(config_reg, PRIM_RX);
        // check if the radio is slepping or not
        if (!read(config_reg, PWR_UP))
            set(config_reg, PWR_UP);
        // radio is in RX mode
        // switch to TX mode
        
        spi_write(write_reg(CONFIG), &config_reg, one_byte);
    

    uint8_t fifo_s = spi_read(read_reg(FIFO_STATUS));
    if (read(fifo_s, TX_EMPTY))
        return radio_error;

    spi_write_cmd(REUSE_TX_PAYLOAD);
    turn_on_radio();
    return radio_ok;
}

// count the retries of packet
uint8_t Radio_count_retries()
{
    // read the retransmit packets
    return (0x0f & spi_read(read_reg(OBSERVE_TX)));
}

//////////////////////////////////////
/////////////////////////////////////////////
////////////// for recieving of data

radio_status Radio_open_pipe(uint8_t *addr, Data_Pipe_typedef *pipeno)
{

    if (Flag == SET)
        return radio_busy;
    // first we have to read the previous settings so that
    // we dont overwrite it by mistake
    uint8_t data_len = pipeno->Data_length;
    uint8_t en_auto_Ack = spi_read(read_reg(EN_AA));
    uint8_t en_rx_addr = spi_read(read_reg(EN_RXADDR));
    uint8_t en_DYpl = spi_read(read_reg(DYNPD));
    uint8_t rx_addr_wid = spi_read(read_reg(SETUP_AW));
    // code for opening a data pipe
    uint8_t pipe_no = pipeno->pipe_no;
    /* by default the data pipe 1 and 0 are enabled
      lets first enable the data pipe 
     */
    set(en_rx_addr, pipe_no);
    spi_write(write_reg(EN_RXADDR), &en_rx_addr, one_byte);
    /*
   lets first check if the EN_AUTO_ACK feature is enabled and disable 
   if it is enabled the nothing to do because all pipe 
   auto ack is enable by default 
   */
    if (pipeno->EN_AUTO_ACK == DISABLE)
    {
        clear(en_auto_Ack, pipe_no);
        spi_write(write_reg(EN_AA), &en_auto_Ack, one_byte);
    }
    else if (pipeno->EN_AUTO_ACK == ENABLE)
    {
        set(en_auto_Ack, pipe_no);
        spi_write(write_reg(EN_AA), &en_auto_Ack, one_byte);
    }
    // configure the dynamic payload lenght all pipe dyn pl lenght are disable by default

    if (pipeno->EN_Dpl == ENABLE)
    {
        set(en_DYpl, pipe_no);
        spi_write(write_reg(DYNPD), &en_DYpl, one_byte);
    }
    else if (pipeno->EN_Dpl == DISABLE)
    {
        clear(en_DYpl, pipe_no);
        spi_write(write_reg(DYNPD), &en_DYpl, one_byte);
        spi_write(write_reg((RX_PW_P0 + pipe_no)), &data_len, one_byte);
    }

    if (pipe_no == Data_Pipe0 || pipe_no == DATA_PIPE1)
    {
        if (pipe_no == DATA_PIPE0)
            spi_write(write_reg(RX_ADDR_P0), addr, rx_addr_wid);
        if (pipe_no == DATA_PIPE1)
            spi_write(write_reg(RX_ADDR_P1), addr, rx_addr_wid);
    }
    else
    {
        // i can add like previousley adding mechanism but lets use switch case for fun

        switch (pipe_no)
        {
        case DATA_PIPE2:
            spi_write(write_reg(RX_ADDR_P2), addr, one_byte);
            break;
        case DATA_PIPE3:
            spi_write(write_reg(RX_ADDR_P3), addr, one_byte);
            break;
        case DATA_PIPE4:
            spi_write(write_reg(RX_ADDR_P4), addr, one_byte);
            break;
        case DATA_PIPE5:
            spi_write(write_reg(RX_ADDR_P5), addr, one_byte);
            break;

        default:
            break;
        }
    }

    return radio_ok;
}
radio_status Radio_close_pipe(Data_Pipe_typedef *pipeno)
{
    // we have to check the flag because if the radio is in RX mode then we cannot accidently close a data pipe
    // it must not be safe
    if (Flag == SET)
        return radio_busy;
    uint8_t en_rx_addr = spi_read(read_reg(EN_RXADDR));
    clear(en_rx_addr, pipeno->pipe_no);
    spi_write(write_reg(EN_RXADDR), &en_rx_addr, one_byte);

    return radio_ok;
}

radio_status Radio_get_data(uint8_t *data)
{
    if (Flag == SET)
        return radio_busy;

    uint8_t data_len = spi_read(R_RX_PAYLOAD_WID);
    read_data(R_RX_PAYLOAD, data, data_len);

    // once the payload is read the data must be cleared from the
    // FIFO and then  if it not then there must be an error
    uint8_t fifo = spi_read(read_reg(FIFO_STATUS));
    if (read(fifo, RX_EMPTY))
        return radio_ok;
    else
        return radio_error;
}

uint8_t Radio_data_len()
{
    uint8_t fifo_status = spi_read(read_reg(FIFO_STATUS));
    if (read(fifo_status, RX_EMPTY))
        return 0;
    else if (read(fifo_status, RX_FULL))
        return 32;
    // return the RX payload width
    return spi_read(R_RX_PAYLOAD_WID);
}

radio_status Radio_start_listiening(Radio_typedef *radio)
{
    if (Flag == SET)
        return radio_busy;

    Flag = SET;
    mode = RX;
    // now radio is armed peoperly we are going to listien for incoming data

    uint8_t config = spi_read(read_reg(CONFIG));
    if (!read(config, PRIM_RX))
    {
        // if radio is not in RX mode then turn it into that
        set(config, PRIM_RX);
        spi_write(write_reg(CONFIG), &config, one_byte);
    }
    uint8_t fifo = spi_read(read_reg(FIFO_STATUS));
    if (!read(fifo, TX_EMPTY))
    {
        // if TX FIFO is not empty then we have to flush the tx fifo
        spi_write_cmd(FLUSH_TX);
    }
    if (!read(fifo, RX_EMPTY))
    {
        // if Rx FIFO is not empty we have to empty it
        spi_write_cmd(FLUSH_RX);
    }
    turn_on_radio();

    return radio_ok;
}

radio_status Radio_stop_listiening(Radio_typedef *radio)
{
    turn_off_radio();
    return radio_free;

    // i have implemented a weak function you can define this weak function into your main code
}
void Radio_poweroff()
{
    turn_off_radio();
    // turn off the radio
    uint8_t config = spi_read(read_reg(CONFIG));
    set(config, PWR_UP);
    spi_write(write_reg(CONFIG), &config, one_byte);
}

radio_status Radio_write_ACK_Pay(uint8_t *data, uint8_t data_len, Radio_data_pipe pipe_no)
{
    if (Flag == SET)
        return radio_busy;
    if ((mode == TX) || (number > 3))
        return radio_error;

    // now write the payload and increment the number
    spi_write(write_ACK_payload(pipe_no), data, data_len);
    number++;
    return radio_ok;
}

TRX_Status Radio_status()
{
    TRX_Status s = 0;
    switch (status)
    {
    case Data_sent:
        s = radio_data_sent;
        break;
    case Data_recv_pipe0:
        s = radio_data_recv_pipe0;
        break;
    case Data_recv_pipe1:
        s = radio_data_recv_pipe1;
        break;
    case Data_recv_pipe2:
        s = radio_data_recv_pipe2;
        break;
    case Data_recv_pipe3:
        s = radio_data_recv_pipe3;
        break;
    case Data_recv_pipe4:
        s = radio_data_recv_pipe4;
        break;
    case Data_recv_pipe5:
        s = radio_data_recv_pipe5;
        break;
    case MAX_ret_occur:
        s = radio_max_ret_occur;
        break;
    case Data_sent_ACK_payl:
        s = radio_ack_have_payload;
        break;
    case TX_FIFO_Full:
        s = radio_TX_FIFO_full;
        break;

    default:
        s = radio_err;
        break;
    }
    status = clear;
    return s;
}

void Radio_IRQ_handler(Radio_typedef *radio)
{
    Flag = RESET;
    turn_off_radio();

    uint8_t stat = spi_read(STATUS);
    // check if the data is sent properly

    if (mode == TX)
    {

        if (read(stat, TX_DS) && read(stat, RX_DR))
        {
            // data is recieved but the ACK have a payload
            status = Data_sent_ACK_payl;
            // clear the status of this event
            set(stat, TX_DS);
            set(stat, RX_DR);
        }
        else if (read(stat, TX_DS))
        {
            status = Data_sent;

            // clear the status of the event
            set(stat, TX_DS);
        }
        // if max retreis occured
        else if (read(stat, MAX_RT))
        {
            status = MAX_ret_occur;

            // clear the event
            set(stat, MAX_RT);
        }
    }
    if (mode == RX)
    {
        if (read(stat, RX_DR))
        {
            // check which data pipe data is available in FIFO
            uint8_t pipe = ((stat >> 1) & 0x07);
            switch (pipe)
            {
            case 0:
                status = Data_recv_pipe0;
                break;
            case 1:
                status = Data_recv_pipe1;
                break;
            case 2:
                status = Data_recv_pipe2;
                break;
            case 3:
                status = Data_recv_pipe3;
                break;
            case 4:
                status = Data_recv_pipe4;
                break;
            case 5:
                status = Data_recv_pipe5;
                break;
            default:
                status = error;
                break;
            }

            // clear the event
            set(stat, RX_DR);
        }
        // decrement the no because the payload is removed onces the ack is transmitted
        if (read(stat, TX_DS))
        {
            // clear the status of the event
            set(stat, TX_DS);

            if (number <= 0)
                number = 0;
            else
                number--;
        }
    }
    // if the TX FIFO get full

    else if (read(stat, TX_FULL))
        status = TX_FIFO_Full;

    // to clear the event in the status register write the 1 to that event
    // this is neccesary because the IRQ pin will hang until the reset is cleared
    spi_write(write_reg(STATUS), &stat, one_byte);
    __weak void IRQcallback(void *ptr)
    {
        UNUSED(ptr);
        
    }
}
