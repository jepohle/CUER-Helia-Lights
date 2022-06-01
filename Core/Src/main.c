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
uint8_t rxData[8] = {0,0,0,0,0,0,0,0};
uint8_t txData[8] = {0,0,0,0,0,0,0,0};
CAN_FilterTypeDef canfil;
uint32_t canRxMailbox;
uint32_t canTxMailbox;
uint32_t RXID;
uint32_t INSERTHERE; //This is a random variable that needs changing of both type and content. Only a dummy as the 0 state for the cyclic switching lights
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


  //initialise CAN filters
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


  //setup CAN header for transmitting messages
  txHeader.DLC = 8; // Number of bites to be transmitted max- 8
  txHeader.IDE = CAN_ID_STD;
  txHeader.RTR = CAN_RTR_DATA;
  txHeader.StdId = 0x030;
  txHeader.ExtId = 0x02;
  txHeader.TransmitGlobalTime = DISABLE;


  //initialize CAN bus
  HAL_CAN_ConfigFilter(&hcan, &canfil);
  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  if(HAL_CAN_GetRxMessage(&hcan, 0, &rxHeader, rxData) != HAL_OK){
    Error_Handler();
  }
}

void RBlinker(){
while(1){
  HAL_GPIO_TogglePin(RINDIC_GPIO_Port, RINDIC_Pin);
  HAL_Delay(750);
  if(rxData == INSERTHERE){
    HAL_GPIO_WritePin(RINDIC_GPIO_Port, RINDIC_Pin, RESET);
    return;
  }
}
}

void LBlinker(){
while(1){
  HAL_GPIO_TogglePin(LINDIC_GPIO_Port, LINDIC_Pin);
  HAL_Delay(750);
  if(rxData == INSERTHERE){
    HAL_GPIO_WritePin(LINDIC_GPIO_Port, LINDIC_Pin, RESET);
    return;
  }
}
}

void Brake(){
HAL_GPIO_WritePin(BRAKE_GPIO_Port, BRAKE_Pin, SET);
return;
}

void BrakeR(){
HAL_GPIO_WritePin(BRAKE_GPIO_Port, BRAKE_Pin, RESET);
return;
}

void FogF(){
HAL_GPIO_WritePin(FRONTFOG_GPIO_Port, FRONTFOG_Pin, SET);
return;
}

void FogFR(){
HAL_GPIO_WritePin(FRONTFOG_GPIO_Port, FRONTFOG_Pin, RESET);
return;
}

void FogR(){
HAL_GPIO_WritePin(REARFOG_GPIO_Port, REARFOG_Pin, SET);
return;
}


void FogRR(){
HAL_GPIO_WritePin(REARFOG_GPIO_Port, REARFOG_Pin, RESET);
return;
}

void Day(){
HAL_GPIO_WritePin(FROTDAY_GPIO_Port, FROTDAY_Pin, SET);
return;
}


void DayR(){
HAL_GPIO_WritePin(FROTDAY_GPIO_Port, FROTDAY_Pin, RESET);
return;
}

void safe(){
while(1){
  HAL_GPIO_TogglePin(SAFELIGHT_GPIO_Port, SAFELIGHT_Pin);
  HAL_Delay(750);
  if(rxData == INSERTHERE){
    HAL_GPIO_WritePin(SAFELIGHT_GPIO_Port, SAFELIGHT_Pin, RESET);
    return;
  }
}
}

void Hazards(){
while(1){
  HAL_GPIO_TogglePin(RINDIC_GPIO_Port, RINDIC_Pin);
  HAL_GPIO_TogglePin(LINDIC_GPIO_Port, LINDIC_Pin);
  HAL_Delay(750);
  if(rxData == INSERTHERE){
    HAL_GPIO_WritePin(RINDIC_GPIO_Port, RINDIC_Pin, RESET);
    HAL_GPIO_WritePin(LINDIC_GPIO_Port, LINDIC_Pin, RESET);
    return;
  }
}
}

void heartbeat(void){
  if (HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &canTxMailbox) != HAL_OK){
    Error_Handler();
  }
  HAL_Delay(250);
  return;
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
