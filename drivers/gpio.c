/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     30802       the first version
 */
//#define EN1_ON_OFF_PIN GET_PIN(A, 11) //定义输出� �
//#define EN2_ON_OFF_PIN GET_PIN(A, 12) //定义输出� �
#include <rtthread.h>
#include <board.h>
#include <channel.h>
#include <lcd.h>
#include <gpio.h>


void gpio_init()
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();


    gpio_init_struct.Pin = GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_2|GPIO_PIN_13|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_0;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高�   */
    HAL_GPIO_Init(GPIOB , &gpio_init_struct);       /* 初始化LED0引脚 */

    gpio_init_struct.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_11|GPIO_PIN_15;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高�   */
    HAL_GPIO_Init(GPIOA , &gpio_init_struct);       /* 初始化LED0引脚 */

    gpio_init_struct.Pin = GPIO_PIN_2;                   /* LED0引脚 */
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);       /* 初始化LED0引脚 */
    gpio_init_struct.Pin = GPIO_PIN_3;                   /* LED0引脚 */
    HAL_GPIO_Init(GPIOB , &gpio_init_struct);       /* 初始化LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pin = GPIO_PIN_9|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);       /* 初始化LED0引脚 */
    gpio_init_struct.Pin = GPIO_PIN_13|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);       /* 初始化LED0引脚 */


    gpio_init_struct.Pin = GPIO_PIN_4|GPIO_PIN_5;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高�   */
    HAL_GPIO_Init(GPIOB , &gpio_init_struct);       /* 初始化LED0引脚 */

    Light1_ON;
    Light2_ON;
    Light3_ON;
    Light4_ON;
    Complete1_ON;
    Complete2_ON;
    Complete3_ON;
    Complete4_ON;
    Alarm1_OFF;
    Alarm2_OFF;
    Alarm3_OFF;
    Alarm4_OFF;
    NC1_OFF;
    NC2_OFF;
    FAN1_OFF;
    FAN2_OFF;
    FAN3_OFF;
    FAN4_OFF;
}
