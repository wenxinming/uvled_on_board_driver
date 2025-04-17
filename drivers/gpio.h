/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     30802       the first version
 */
#ifndef DRIVERS_GPIO_H_
#define DRIVERS_GPIO_H_


#define PIN_HIGH 1
#define PIN_LOW 0

#define Light1_ON HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6, PIN_HIGH)
#define Light1_OFF HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6, PIN_LOW)
#define Light2_ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, PIN_HIGH)
#define Light2_OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, PIN_LOW)
#define Light3_ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, PIN_HIGH)
#define Light3_OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, PIN_LOW)
#define Light4_ON HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8, PIN_HIGH)
#define Light4_OFF HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8, PIN_LOW)

#define Complete1_ON HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, PIN_HIGH)
#define Complete1_OFF HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, PIN_LOW)
#define Complete2_ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2, PIN_HIGH)
#define Complete2_OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2, PIN_LOW)
#define Complete3_ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, PIN_HIGH)
#define Complete3_OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, PIN_LOW)
#define Complete4_ON HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, PIN_HIGH)
#define Complete4_OFF HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, PIN_LOW)

#define Alarm1_ON HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, PIN_LOW)
#define Alarm1_OFF HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, PIN_HIGH)
#define Alarm2_ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10, PIN_LOW)
#define Alarm2_OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10, PIN_HIGH)
#define Alarm3_ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, PIN_LOW)
#define Alarm3_OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, PIN_HIGH)
#define Alarm4_ON HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15, PIN_LOW)
#define Alarm4_OFF HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15, PIN_HIGH)


#define Read_CheckLed1 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)
#define Read_CheckLed2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)
#define Read_CheckLed3 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)
#define Read_CheckLed4 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)

#define Read_LedOn1 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)
#define Read_LedOn2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)
#define Read_LedOn3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)
#define Read_LedOn4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)

#define Read_NC1 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)
#define Read_NC2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)

#define NC1_ON HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, PIN_HIGH)
#define NC1_OFF HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, PIN_LOW)
#define NC2_ON HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2, PIN_HIGH)
#define NC2_OFF HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2, PIN_LOW)

#define Led_ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, PIN_LOW)
#define Led_OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, PIN_HIGH)

#endif /* DRIVERS_GPIO_H_ */
