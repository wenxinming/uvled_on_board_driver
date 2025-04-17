/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-01-21     ThinkPad       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <channel.h>
#include <lcd.h>
#include <gpio.h>
rt_thread_t led;
void read_led_entry(void *parameter)//温度刷新
{
    while(1)
    {
        Led_ON;
        rt_thread_mdelay(1000);
        Led_OFF;
        rt_thread_mdelay(1000);
    }
}
void led_init()
{
    led = rt_thread_create("led", read_led_entry, RT_NULL, 512, 20, 10);
    rt_thread_startup(led);//启动线程
}
