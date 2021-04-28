#include "spi.h"
#include <stdint.h>
#include<inttypes.h>
#include  "main.h"

extern SPI_HandleTypeDef hspi1;



static void write_pin(uint8_t OUT)
{

	HAL_GPIO_WritePin(chip_select_GPIO_Port, chip_select_Pin, OUT);
}
static void write_ce_pin(uint8_t state)
{
	HAL_GPIO_WritePin(chip_enable_GPIO_Port, chip_enable_Pin, state);
}

void turn_on_radio()
{
    write_ce_pin(HIGH);

}
void turn_off_radio()
{
    write_ce_pin(LOW);
}


uint8_t spi_read(uint8_t type)
{
    uint8_t data = ZERO_byte;
    write_pin(LOW);
    HAL_SPI_Transmit(&hspi1, &type, one_byte, timeout);
    HAL_SPI_Receive(&hspi1, &data, one_byte, timeout);
    write_pin(HIGH);
    return data;
}
void read_data(uint8_t type, uint8_t *data , uint8_t size_data)
{
  
    write_pin(LOW);
    HAL_SPI_Transmit(&hspi1, &type, one_byte, timeout);
    HAL_SPI_Receive(&hspi1, data, size_data, timeout);
    write_pin(HIGH);

}

void spi_write(uint8_t type, uint8_t *data, uint8_t data_size)
{
    write_pin(LOW);
    HAL_SPI_Transmit(&hspi1, &type, one_byte, timeout);
    HAL_SPI_Transmit(&hspi1, data, data_size, timeout);
    write_pin(HIGH);
}

void spi_write_cmd(uint8_t type)
{
    write_pin(LOW);
    HAL_SPI_Transmit(&hspi1, &type, one_byte, timeout);
    write_pin(HIGH);
}


