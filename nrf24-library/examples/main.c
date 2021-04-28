/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nrf24lo1.h"
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
Radio_typedef radio1;
Data_Pipe_typedef pipe0;

uint8_t tx_addr[] = {0x01,0x02,0x03};
TRX_Status stat;
char *msg = "hello";
 Data_Pipe_typedef pipe1;
 // note that the address width of all the pipe will be same for tx and rx so you have to mess up with the init function

 uint8_t pipe1_addr[] ={0x03,0x04,0x05};

 Data_Pipe_typedef pipe5;
 uint8_t pipe5_addr[] ={0x02};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void radio_init(void);
void radio_rx_init(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

// first initiallize the radio
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
 radio_init();
Radio_start_transmit(&radio1, tx_addr);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // this is the function which is used to transmit the payload with no ack 
    // the Interrupt is fired once the radio data transfer is complete 

	  Radio_transmit((uint8_t *)msg, strlen(msg));
    if(Radio_status() == radio_err)
    {
      // there is some problem with the communication

      Error_Handler();
      

    }
    else if(Radio_status() == radio_data_sent){
      // transmission succesfull
    }
    /* 
     now there is another function which is used to transmit data with ACK 
     the IRQ is fired only when either ACK is recieved or Maximum retries failed 
     so you can implemnt this function like this 
    */
    
    Radio_transmit_ACK((uint8_t *) msg, strlen(msg));
    if(Radio_status() == radio_data_sent)
    {
      // ACK is recieved and payload is removed from the FIFO
    }
    else if (Radio_status() == radio_max_ret_occur)
    {
      /* you can use a function to resend the payload 

     Radio_resend();
     or you can also count retries by 
     uint8_t retries = Radio_count_retries();

     */
    }
    else if (Radio_status() == radio_ack_have_payload)
    {
      // the recieved ACK have payload you can read through the following command 

      char *recv_ack_msg;
      uint8_t data_len = Radio_data_len();
      recv_ack_msg = malloc(data_len * sizeof(char));
      memset(recv_ack_msg, 0, data_len);
      if( Radio_get_data((uint8_t *)recv_ack_msg) != radio_ok)
      Error_Handler();
      free(recv_ack_msg);
      }

    /*
    now after transmitting you can also start for listiening but make sure you have data pipe configured and set 
    radio_rx_init();

    Radio_start_listening(&radio1);

    you can count here for a timer if the timer value exceed 
    if(timer >= elapsed peroid )
    {
      Radio_stop_listiening(&radio1);
       // you can also turn off the power by 
       Radio_Power_off();

    }

    */
   if(Radio_status() == radio_data_recv_pipe0)
   {
     // that menas pipe0 got some data and be ready to read by SPI
     // you can read the payload size and the entire payload like this 

       char *recv_pipe1_msg;
      uint8_t pipe1_len = Radio_data_len();
      recv_pipe1_msg = malloc(pipe1_len * sizeof(char));
      memset(recv_pipe1_msg, 0, pipe1_len);
      if(radio_ok != Radio_get_data((uint8_t *)recv_pipe1_msg))
         Error_Handler();
      free(recv_pipe1_msg);
      }

  
   
   // you can also write an ACK payload for a data pipe like this 
   // make sure your ACK payload dont exceed 32bytes 
   // you can write a max of 3 ACK payloads and once the ACK payload is transferd the payload is removed from the TX FIFO
   // there is a counter which count upto 3 and after return an error to restrict further tx ack payload 

   
   char *msg_ack_p1 = "ALright";
   Radio_write_ACK_Pay((uint8_t *)msg_ack_p1, strlen(msg_ack_p1), Data_Pipe1);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  
  /* USER CODE END 3 */
}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(chip_enable_GPIO_Port, chip_enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(chip_select_GPIO_Port, chip_select_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : IRQ_pin_Pin */
  GPIO_InitStruct.Pin = IRQ_pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IRQ_pin_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : chip_enable_Pin chip_select_Pin */
  GPIO_InitStruct.Pin = chip_enable_Pin|chip_select_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */
void radio_init(void)
{


//now defines the parameter of the radio 
radio1.Init.RX_DR_IRQ = enable;
radio1.Init.TX_DS_IRQ = enable;
radio1.Init.MAX_RT_IRQ = enable;
radio1.Init.EN_Crc = enable;
radio1.Init.CRC_Encoding = CRC_1_byte;
radio1.Data_rate = _250_Kbps_;
radio1.Length = _3_bytes;
radio1.Power = VeryHigh;
radio1.RF_ch = 10;
radio1.ARETR.EN_DA= disable;
radio1.ARETR.auto_re_count = ONE_RET;
radio1.ARETR.auto_re_delay = W_250us;


if(Radio_init(&radio1) != radio_ok)
{
  Error_Handler();
}

}

void radio_rx_init(void)
{
  // setiings for pipe1
  pipe1.EN_AUTO_ACK =enable;
  pipe1.EN_Dpl = enable;
  pipe1.pipe_no = Data_Pipe0;

// settings for pipe5
  pipe5.EN_AUTO_ACK =enable;
  pipe5.EN_Dpl = enable;
  pipe5.pipe_no = Data_Pipe5;

  // now this function will create a RX pipe with the following settings 
  Radio_open_pipe(pipe1_addr, &pipe1);
  Radio_open_pipe(pipe5_addr, &pipe5);

/*
 you can close a pipe if you want to by the function Radio_close_pipe(&pipe1);

*/

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	Radio_IRQ_handler(&radio1);

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
