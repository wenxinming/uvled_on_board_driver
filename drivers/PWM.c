/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     30802       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <channel.h>
#include <stdio.h>
#include <pwm.h>
#include <temperature.h>
TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* 定时器x句柄 */
rt_uint16_t set_pwm1,set_pwm2,set_pwm3,set_pwm4;
#define PWM_Period 500000
/**
 * @brief       通用定时器TIMX 通道Y PWM输出 初始化函数（使用PWM模式1）
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy  = {0};                          /* 定时器PWM输出配置 */

    g_timx_pwm_chy_handle.Instance = GTIM_TIMX_PWM;                     /* 定时器 */
    g_timx_pwm_chy_handle.Init.Prescaler = psc;                         /* 定时器分频 */
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 递增计数模式 */
    g_timx_pwm_chy_handle.Init.Period = arr;                            /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);                           /* 初始化PWM */

    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                           /* 模式选择PWM1 */
    timx_oc_pwm_chy.Pulse = arr / 2;                                    /* 设置比较值,此值用来确定占空比 */
                                                                        /* 这里默认设置比较值为自动重装载值的一半,即占空比为50% */
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                    /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, OutputCH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, OutputCH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, OutputCH3); /* 配置TIMx通道y */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, OutputCH4); /* 配置TIMx通道y */
    HAL_TIM_PWM_Stop(&g_timx_pwm_chy_handle, OutputCH1);       /* 开启对应PWM通道 */
    HAL_TIM_PWM_Stop(&g_timx_pwm_chy_handle, OutputCH2);       /* 开启对应PWM通道 */
    HAL_TIM_PWM_Stop(&g_timx_pwm_chy_handle, OutputCH3);       /* 开启对应PWM通道 */
    HAL_TIM_PWM_Stop(&g_timx_pwm_chy_handle, OutputCH4);       /* 开启对应PWM通道 */
}
/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_PWM_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_PWM)
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE();               /* 开启通道y的CPIO时钟 */
        GTIM_TIMX_PWM_CHY_CLK_ENABLE();
        gpio_init_struct.Pin = GTIM_TIMX_PWM_CHY_GPIO_PIN|GTIM_TIMX_PWM_CHY_GPIO_PIN2|GTIM_TIMX_PWM_CHY_GPIO_PIN3|GTIM_TIMX_PWM_CHY_GPIO_PIN4; /* 通道y的CPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;           /* 复用推完输出 */
        //gpio_init_struct.Pull = GPIO_PULLUP;               /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;     /* 高速 */
        HAL_GPIO_Init(GTIM_TIMX_PWM_CHY_GPIO_PORT, &gpio_init_struct);
    }
}
void pwm_init()
{
    //gtim_timx_pwm_chy_init(500 - 1, 64 - 1);//2KHZ
    gtim_timx_pwm_chy_init(1000 - 1, 73 - 1);//2KHZ
    /* 修改比较值控制占空比 */
    //__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 100);
    //__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, TIM_CHANNEL_2, 250);
}
void set_ch1_output(rt_uint8_t num)
{
    float temp;
    unsigned int set;
    if(num<2)
    num = 2;
    set = channel1.set_current;
    if(set>1400)
    set = 1400;
    temp = set/1500.0;
    //最大值92% 0-92
    if(set>1350)
    {
        set_pwm1 = num*10*temp*0.99;
    }else {
        set_pwm1 = num*10*temp*1.01;
    }
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, OutputCH1, set_pwm1);
}
void set_ch1_output2(rt_uint16_t num)
{
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, OutputCH1, num);
}
void set_ch2_output2(rt_uint16_t num)
{
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, OutputCH2, num);
}
void set_ch3_output2(rt_uint16_t num)
{
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, OutputCH3, num);
}
void set_ch4_output2(rt_uint16_t num)
{
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, OutputCH4, num);
}
void set_ch2_output(rt_uint8_t num)
{
    float temp;
    unsigned int set;
    if(num<2)
        num = 2;
    set = channel2.set_current;
    if(set>1400)
    set = 1400;
    temp = set/1500.0;
    //最大值92% 0-92
    if(set>1350)
    {
        set_pwm2 = num*10*temp*0.99;
    }else {
        set_pwm2 = num*10*temp*1.01;
    }
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, OutputCH2, set_pwm2);
}
void set_ch3_output(rt_uint8_t num)
{
    float temp;
    unsigned int set;
    if(num<2)
        num = 2;
    set = channel3.set_current;
    if(set>1400)
    set = 1400;
    temp = set/1500.0;
    //最大值92% 0-92
    if(set>1350)
    {
        set_pwm3 = num*10*temp*0.99;
    }else {
        set_pwm3 = num*10*temp*1.01;
    }
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, OutputCH3, set_pwm3);
}
void set_ch4_output(rt_uint8_t num)
{
    float temp;
    unsigned int set;
    if(num<2)
        num = 2;
    set = channel4.set_current;
    if(set>1400)
    set = 1400;
    temp = set/1500.0;
    //最大值92% 0-92
    if(set>1350)
    {
        set_pwm4 = num*10*temp*1.08;
    }else {
        set_pwm4 = num*10*temp*1.06;
    }
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, OutputCH4, set_pwm4);
}
void ch1_output_off()
{
    HAL_TIM_PWM_Stop(&g_timx_pwm_chy_handle, OutputCH1);       /* 开启对应PWM通道 */
}
void ch2_output_off()
{
    HAL_TIM_PWM_Stop(&g_timx_pwm_chy_handle, OutputCH2);       /* 开启对应PWM通道 */
}
void ch3_output_off()
{
    HAL_TIM_PWM_Stop(&g_timx_pwm_chy_handle, OutputCH3);       /* 开启对应PWM通道 */
}
void ch4_output_off()
{
    HAL_TIM_PWM_Stop(&g_timx_pwm_chy_handle, OutputCH4);       /* 开启对应PWM通道 */
}
void ch1_output_on()
{
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, OutputCH1);       /* 开启对应PWM通道 */
}
void ch2_output_on()
{
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, OutputCH2);       /* 开启对应PWM通道 */
}
void ch3_output_on()
{
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, OutputCH3);       /* 开启对应PWM通道 */
}
void ch4_output_on()
{
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, OutputCH4);       /* 开启对应PWM通道 */
}
