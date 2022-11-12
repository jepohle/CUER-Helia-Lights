/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Brake(void);
void BrakeR(void);
void FogF(void);
void FogFR(void);
void FogR(void);
void FogRR(void);
void Day(void);
void DayR(void);
void FullIOReset(void);
void heartbeat(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOF
#define FRONTHIGH_Pin GPIO_PIN_0
#define FRONTHIGH_GPIO_Port GPIOA
#define FRONTDAY_Pin GPIO_PIN_1
#define FRONTDAY_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define BRAKE_Pin GPIO_PIN_0
#define BRAKE_GPIO_Port GPIOB
#define REARFOG_Pin GPIO_PIN_1
#define REARFOG_GPIO_Port GPIOB
#define FRONTFOG_Pin GPIO_PIN_8
#define FRONTFOG_GPIO_Port GPIOA
#define RINDIC_Pin GPIO_PIN_9
#define RINDIC_GPIO_Port GPIOA
#define LINDIC_Pin GPIO_PIN_10
#define LINDIC_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define FRONTFULL_Pin GPIO_PIN_3
#define FRONTFULL_GPIO_Port GPIOB
#define SAFELIGHT_Pin GPIO_PIN_4
#define SAFELIGHT_GPIO_Port GPIOB
#define HORN_Pin GPIO_PIN_5
#define HORN_GPIO_Port GPIOB
#define REVERSE_Pin GPIO_PIN_6
#define REVERSE_GPIO_Port GPIOB
#define REARPOS_Pin GPIO_PIN_7
#define REARPOS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
