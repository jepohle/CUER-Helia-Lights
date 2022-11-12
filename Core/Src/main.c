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
CAN_RxHeaderTypeDef rxHeader;
CAN_TxHeaderTypeDef txHeader;
uint8_t rxData[8] = {0,0,0,0,0,0,0,0};      //[Brake, FogF, FogR, Day, IndicR, IndicL, Hazard, Safe]
uint8_t txData[8];
uint32_t canRxMailbox;
uint32_t canTxMailbox;
uint32_t RXID;
uint32_t INSERTHERE; //This is a random variable that needs changing of both type and content. Only a dummy as the 0 state for the cyclic switching lights
//CAN ID for lights
const int LIGHTS_ID = 0x730;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

  //initialise CAN filters
  CAN_FilterTypeDef canfil;
  canfil.FilterActivation = CAN_FILTER_ENABLE;
  canfil.FilterBank = 0;
  canfil.FilterMode = CAN_FILTERMODE_IDMASK;
  canfil.FilterFIFOAssignment = CAN_RX_FIFO0;
  canfil.FilterIdHigh = 0;
  canfil.FilterIdLow = 0;
  canfil.FilterMaskIdHigh = 0;
  canfil.FilterMaskIdLow = 0;
  canfil.FilterScale = CAN_FILTERSCALE_32BIT;
  canfil.FilterActivation = ENABLE;
  canfil.SlaveStartFilterBank = 14;
  HAL_CAN_ConfigFilter(&hcan, &canfil);

  //setup CAN header for transmitting messages
  txHeader.DLC = 2; // Number of bytes to be transmitted max- 8
  txHeader.IDE = CAN_ID_STD;
  txHeader.RTR = CAN_RTR_DATA;
  txHeader.StdId = 0x303;
  txData[0] = 50;
  txData[1] = 0xAA;

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
  HAL_UART_Receive_IT(&huart2, rxData, 8);
  HAL_CAN_Start(&hcan);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {  
    if(HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &canTxMailbox)!=HAL_OK){
      Error_Handler();
    }
    HAL_Delay(1000);
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
  FullIOReset();                       //Reset all lights for reassignment
  if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK){
    Error_Handler();
  }
  // else{
  //   if(){

  //   }
  //   else if(){

  //   }
  //   else if(){

  //   }
  //   else if(){

  //   }
  //   else if(){

  //   }
  //   else if(){

  //   }
  //   else if(){

  //   }
  }
//repeat above for UART interrupts for bench testing without proper CAN system and knowledge of data and addresses
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    FullIOReset();                       //Reset all pinstates for reasignment, should happen fast enough to not cause any major flashing of lighs
    if(rxData[0] == 1){
      Brake();
    }
    if(rxData[1] == 1){
      FogF();
    }
    if(rxData[2] == 1){
      FogR();
    }
    if(rxData[3] == 1){
      Day();
    }
    if(rxData[4] == 1||rxData[5] == 1||rxData[6] == 1||rxData[7] == 1){
      HAL_TIM_Base_Start_IT(&htim2);
    }
  HAL_UART_Receive_IT(&huart2, rxData, 8);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
      if(rxData[4] == 1||rxData[6] == 1){
        HAL_GPIO_TogglePin(RINDIC_GPIO_Port, RINDIC_Pin);
      } 
      if(rxData[5] == 1||rxData[6] == 1){
        HAL_GPIO_TogglePin(LINDIC_GPIO_Port, LINDIC_Pin);
      }
      if(rxData[7] == 1){
        HAL_GPIO_TogglePin(SAFELIGHT_GPIO_Port, SAFELIGHT_Pin);
      }
}

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



void FullIOReset(){
  DayR();
  FogRR();
  FogFR();
  BrakeR();
  HAL_TIM_Base_Stop_IT(&htim2);
}

void heartbeat(){
  if (HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &canTxMailbox) != HAL_OK){
    Error_Handler();
  }
  HAL_Delay(250);
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
