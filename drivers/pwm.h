/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     30802       the first version
 */
#ifndef DRIVERS_PWM_H_
#define DRIVERS_PWM_H_


/******************************************************************************************/
/* 通用定时器 定义 */

/* TIMX 中断定义
 * 默认是针对TIM2~TIM5, TIM12~TIM17.
 * 注意: 通过修改这4个宏定义,可以支持TIM1~TIM17任意一个定时器.
 */

#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */


/*********************************以下是通用定时器PWM输出实验相关宏定义*************************************/

/* TIMX PWM输出定义
 * 这里输出的PWM控制LED0(RED)的亮度 (需要一根杜邦线连接 PA0 和 PA8 !!!)
 * 默认是针对TIM2~TIM5
 * 注意:
 * 1, 通过修改这几个宏定义,可以支持TIM1~TIM8任意一个定时器,任意一个IO口输出PWM
 * 1, TIM2_CH1通过PA0输出PWM, 但是我们的LED0接在PA8上, 所以需要用杜邦线链接 PA0 和 PA8, 从而控制LED0亮度
 */
#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOC
#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_6
#define GTIM_TIMX_PWM_CHY_GPIO_PIN2          GPIO_PIN_7
#define GTIM_TIMX_PWM_CHY_GPIO_PIN3          GPIO_PIN_8
#define GTIM_TIMX_PWM_CHY_GPIO_PIN4          GPIO_PIN_9
#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define GTIM_TIMX_PWM                       TIM8
#define OutputCH1                   TIM_CHANNEL_1                                /* 通道Y,  1<= Y <=4 */
#define OutputCH2                   TIM_CHANNEL_2                                /* 通道Y,  1<= Y <=4 */
#define OutputCH3                   TIM_CHANNEL_3                                /* 通道Y,  1<= Y <=4 */
#define OutputCH4                   TIM_CHANNEL_4                                /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_PWM_CHY_CCRX              GTIM_TIMX_PWM->CCR1                          /* 通道Y的输出比较寄存器 */
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)   /* TIM2 时钟使能 */


/******************************************************************************************/

void gtim_timx_int_init(uint16_t arr, uint16_t psc);        /* 通用定时器 定时中断初始化函数 */
void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc);    /* 通用定时器 PWM初始化函数 */
extern void ch4_output_off();
extern void ch1_output_off();
extern void ch2_output_off();
extern void ch3_output_off();
extern void ch1_output_on();
extern void ch2_output_on();
extern void ch3_output_on();
extern void ch4_output_on();
extern void set_ch1_output(rt_uint8_t num);
extern void set_ch2_output(rt_uint8_t num);
extern void set_ch3_output(rt_uint8_t num);
extern void set_ch4_output(rt_uint8_t num);
extern void set_ch1_output2(rt_uint16_t num);
#endif /* DRIVERS_PWM_H_ */
