/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


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

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
CAN_RxHeaderTypeDef rxHeader;
CAN_TxHeaderTypeDef txHeader;
uint8_t rxData[8];      //[Brake, FogF, FogR, Day, RearPos, Reverse, FrontFull, FrontHigh, Horn, IndicR, IndicL, Hazard, Safe], first eight bits in first data byte, followed by last four in second data byte
uint8_t txData[8];      //could be used for status or heartbeat messages to driver controls board, could be left blank otherwise
uint32_t canRxMailbox;
uint32_t canTxMailbox;
uint32_t RXID;
int UART_Start_Flag = 1; //Flag to be checked in the while loop for reactivation of UART receive
int i = 0;
//CAN ID for lights
const int LIGHTS_ID = 0x730;
int RxFlag = 0;
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
  
  //initialise CAN filters
  CAN_FilterTypeDef canfil;
  canfil.FilterMaskIdHigh = 0;
  canfil.FilterMaskIdLow = 0;
  canfil.FilterIdHigh = 0;
  canfil.FilterIdLow = 0;
  canfil.FilterBank = 1;
  canfil.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  canfil.FilterMode = CAN_FILTERMODE_IDMASK;
  canfil.FilterScale = CAN_FILTERSCALE_16BIT;
  canfil.FilterActivation = ENABLE;
  HAL_CAN_ConfigFilter(&hcan, &canfil);

  //setup CAN header for transmitting messages
  txHeader.DLC = 2; // Number of bytes to be transmitted max- 8
  txHeader.IDE = CAN_ID_STD;
  txHeader.RTR = CAN_RTR_DATA;
  txHeader.StdId = 0x400;
  txData[0] = 0x3d;
  txData[1] = 0x4e;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // HAL_UART_Receive_IT(&huart2, rxData, 2);
  FullIOReset();
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
  HAL_CAN_Start(&hcan);
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {  
    // if(HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &canTxMailbox)!=HAL_OK){
    //   Error_Handler();
    // }
    // HAL_Delay(1000);
    // if(UART_Start_Flag == 1){
    //   UART_Start_Flag = 0;
    //   HAL_UART_Receive_IT(&huart2, rxData, 2);
    // }
    for(i=0;i<=5;i++){
    heartbeat();
    }
    txHeader.DLC = 1;
    txHeader.StdId = 0x34F;
    txData[1]=0;
    txData[0]=0xff;
    if(HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &canTxMailbox) != HAL_OK){
      Error_Handler();
    }
    txHeader.StdId = 0x400;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
  if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK){
    Error_Handler();
  }
  if(HAL_CAN_AddTxMessage(hcan, &txHeader, rxData, &canTxMailbox) != HAL_OK){
    Error_Handler();
  }
}
void LightsRefresh(uint8_t rxData[]){
  FullIOReset();
  if((rxData[0] & 1) != 0){           //[Brake, FogF, FogR, Day, RearPos, Reverse, FrontFull, FrontHigh] [Horn, IndicR, IndicL, Hazard, Safe]
    Brake();
  }
  if((rxData[0] & 2) != 0){
    FogF();
  }
  if((rxData[0] & 4) != 0){
    FogR();
  }
  if((rxData[0] & 8) != 0){
    Day();
  }
  if((rxData[0] & 16) != 0){
    RearPos();
  }
  if((rxData[0] & 32) != 0){
    Reverse();
  }
  if((rxData[0] & 64) != 0){
    FrontFull();
  }
  if((rxData[0] & 128) != 0){
    FrontHigh();
  }
  if((rxData[1] & 1) != 0){
    Horn();
  }
  if((rxData[1] & 2) != 0||(rxData[1] & 4) != 0||(rxData[1] & 16) != 0||(rxData[1] & 32) != 0){
    HAL_TIM_Base_Start_IT(&htim2);
  }
  }
//repeat above for UART interrupts for bench testing without proper CAN system and knowledge of data and addresses
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//     UART_Start_Flag = 0;
//     FullIOReset();                       //Reset all pinstates for reasignment, should happen fast enough to not cause any major flashing of lighs
//     if((rxData[0] & 1) != 0){           //[Brake, FogF, FogR, Day, RearPos, Reverse, FrontFull, FrontHigh] [Horn, IndicR, IndicL, Hazard, Safe]
//       Brake();
//     }
//     if((rxData[0] & 2) != 0){
//       FogF();
//     }
//     if((rxData[0] & 4) != 0){
//       FogR();
//     }
//     if((rxData[0] & 8) != 0){
//       Day();
//     }
//     if((rxData[0] & 16) != 0){
//       RearPos();
//     }
//     if((rxData[0] & 32) != 0){
//       Reverse();
//     }
//     if((rxData[0] & 64) != 0){
//       FrontFull();
//     }
//     if((rxData[0] & 128) != 0){
//       FrontHigh();
//     }
//     if((rxData[1] & 1) != 0){
//       Horn();
//     }
//     if((rxData[1] & 2) != 0||(rxData[1] & 4) != 0||(rxData[6] & 16) != 0||(rxData[7] & 32) != 0){
//       HAL_TIM_Base_Start_IT(&htim2);
//     }
//   UART_Start_Flag = 1;
// }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
      if((rxData[1] & 2) != 0||(rxData[1] & 8) != 0){
        HAL_GPIO_TogglePin(RINDIC_GPIO_Port, RINDIC_Pin);
      } 
      if((rxData[1] & 4) != 0||(rxData[1] & 8) != 0){
        HAL_GPIO_TogglePin(LINDIC_GPIO_Port, LINDIC_Pin);
      }
      if((rxData[1] & 16) != 0){
        HAL_GPIO_TogglePin(SAFELIGHT_GPIO_Port, SAFELIGHT_Pin);
      }
}


//function to set all of the GPIO pins individually and make other functions more readable
void Brake(){
HAL_GPIO_WritePin(BRAKE_GPIO_Port, BRAKE_Pin, SET);
}

void BrakeR(){
HAL_GPIO_WritePin(BRAKE_GPIO_Port, BRAKE_Pin, RESET);
}

void FogF(){
HAL_GPIO_WritePin(FRONTFOG_GPIO_Port, FRONTFOG_Pin, SET);
}

void FogFR(){
HAL_GPIO_WritePin(FRONTFOG_GPIO_Port, FRONTFOG_Pin, RESET);
}

void FogR(){
HAL_GPIO_WritePin(REARFOG_GPIO_Port, REARFOG_Pin, SET);
}

void FogRR(){
HAL_GPIO_WritePin(REARFOG_GPIO_Port, REARFOG_Pin, RESET);
}

void Day(){
HAL_GPIO_WritePin(FRONTDAY_GPIO_Port, FRONTDAY_Pin, SET);
}

void DayR(){
HAL_GPIO_WritePin(FRONTDAY_GPIO_Port, FRONTDAY_Pin, RESET);
}

void RearPos(){
  HAL_GPIO_WritePin(REARPOS_GPIO_Port, REARPOS_Pin, SET);
}

void RearPosR(){
  HAL_GPIO_WritePin(REARPOS_GPIO_Port, REARPOS_Pin, RESET);
}

void Horn(){
  HAL_GPIO_WritePin(HORN_GPIO_Port, HORN_Pin, SET);
}

void HornR(){
  HAL_GPIO_WritePin(HORN_GPIO_Port, HORN_Pin, RESET);
}

void Reverse(){
  HAL_GPIO_WritePin(REVERSE_GPIO_Port, REVERSE_Pin, SET);
}

void ReverseR(){
  HAL_GPIO_WritePin(REVERSE_GPIO_Port, REVERSE_Pin, RESET);
}

void FrontFull(){
  HAL_GPIO_WritePin(FRONTFULL_GPIO_Port, FRONTFULL_Pin, SET);
}

void FrontFullR(){
  HAL_GPIO_WritePin(FRONTFULL_GPIO_Port, FRONTFULL_Pin, RESET);
}

void FrontHigh(){
  HAL_GPIO_WritePin(FRONTHIGH_GPIO_Port, FRONTHIGH_Pin, SET);
}

void FrontHighR(){
  HAL_GPIO_WritePin(FRONTHIGH_GPIO_Port, FRONTHIGH_Pin, RESET);
}


void FullIOReset(){                       //function to fully reset all GPIO pins used for the lights
  DayR();
  FogRR();
  FogFR();
  BrakeR();
  FrontHighR();
  FrontFullR();
  ReverseR();
  HornR();
  RearPosR();
  HAL_TIM_Base_Stop_IT(&htim2);
  HAL_GPIO_WritePin(SAFELIGHT_GPIO_Port,SAFELIGHT_Pin, RESET);
  HAL_GPIO_WritePin(RINDIC_GPIO_Port, RINDIC_Pin, RESET);
  HAL_GPIO_WritePin(LINDIC_GPIO_Port, LINDIC_Pin, RESET);
}

void heartbeat(){
  if (HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &canTxMailbox) != HAL_OK){
    Error_Handler();
  }
  HAL_Delay(1000);
}

// void uartHeartbeat(){
//   if(HAL_UART_Transmit(&huart2, &txData[0], 1, 100) != HAL_OK){
//     Error_Handler();
//   }
//   HAL_Delay(250);
// }
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
