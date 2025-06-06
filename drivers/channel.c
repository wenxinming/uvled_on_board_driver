/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-18     ThinkPad       the first version
 */
#include "stm32f1xx_hal.h"
#include <rtthread.h>
#include <string.h>
#include <uart.h>
#include <channel.h>
struct channel channel1;
struct channel channel2;
struct channel channel3;
struct channel channel4;
struct channel_control control_mode;
rt_uint8_t k;
rt_uint8_t channel_sync_mode;//同步模式 = 0异步模式 = 1同步模式
void channel_init()
{
    rt_uint8_t i;
    //flash最后一页地址0x807f800
    channel1.power = (*(unsigned int*)(channel1_power_addr));
    channel1.time = (*(unsigned int*)(channel1_time_addr));
    channel1.delay_time = (*(unsigned int*)(channel1_delay_addr));
    channel1.alarm_temperature = (*(unsigned int*)(channel1_alarm_addr));
    channel1.time_hour = (*(unsigned int*)(channel1_timeh_addr));
    channel1.time_minutes = (*(unsigned int*)(channel1_timem_addr));
    channel1.light_mode = (*(unsigned int*)(channel1_ligthm_addr));
    channel1.control_mode = (*(unsigned int*)(channel1_control_addr));
    channel1.cycle = (*(unsigned int*)(channel1_cycle_addr));
    channel1.Enable = (*(unsigned int*)(channel1_enable_addr));
    channel1.set_current = (*(unsigned int*)(channel1_current_addr));
    channel1.set_temperature = (*(unsigned int*)(channel1_temperature_addr));
    for(i=0;i<20;i++)
    {
        channel1.multistage_time[i] = (*(unsigned int*)(channel1_multistage_time_addr+i*2));
    }
    for(i=0;i<20;i++)
    {
        channel1.multistage_power[i] = (*(unsigned int*)(channel1_multistage_power_addr+i*2));
    }
    channel1.status = 0;
    channel1.now_time_s = 0;

    channel2.power = (*(unsigned int*)(channel2_power_addr));
    channel2.time = (*(unsigned int*)(channel2_time_addr));
    channel2.delay_time = (*(unsigned int*)(channel2_delay_addr));
    channel2.alarm_temperature = (*(unsigned int*)(channel2_alarm_addr));
    channel2.time_hour = (*(unsigned int*)(channel2_timeh_addr));
    channel2.time_minutes = (*(unsigned int*)(channel2_timem_addr));
    channel2.light_mode = (*(unsigned int*)(channel2_ligthm_addr));
    channel2.control_mode = (*(unsigned int*)(channel2_control_addr));
    channel2.cycle = (*(unsigned int*)(channel2_cycle_addr));
    channel2.Enable = (*(unsigned int*)(channel2_enable_addr));
    channel2.set_current = (*(unsigned int*)(channel2_current_addr));
    channel2.set_temperature = (*(unsigned int*)(channel2_temperature_addr));
    for(i=0;i<20;i++)
    {
        channel2.multistage_time[i] = (*(unsigned int*)(channel2_multistage_time_addr+i*2));
    }
    for(i=0;i<20;i++)
    {
        channel2.multistage_power[i] = (*(unsigned int*)(channel2_multistage_power_addr+i*2));
    }
    channel2.status = 0;
    channel2.now_time_s = 0;

    channel3.power = (*(unsigned int*)(channel3_power_addr));
    channel3.time = (*(unsigned int*)(channel3_time_addr));
    channel3.delay_time = (*(unsigned int*)(channel3_delay_addr));
    channel3.alarm_temperature = (*(unsigned int*)(channel3_alarm_addr));
    channel3.time_hour = (*(unsigned int*)(channel3_timeh_addr));
    channel3.time_minutes = (*(unsigned int*)(channel3_timem_addr));
    channel3.light_mode = (*(unsigned int*)(channel3_ligthm_addr));
    channel3.control_mode = (*(unsigned int*)(channel3_control_addr));
    channel3.cycle = (*(unsigned int*)(channel3_cycle_addr));
    channel3.Enable = (*(unsigned int*)(channel3_enable_addr));
    channel3.set_current = (*(unsigned int*)(channel3_current_addr));
    channel3.set_temperature = (*(unsigned int*)(channel3_temperature_addr));
    for(i=0;i<20;i++)
    {
        channel3.multistage_time[i] = (*(unsigned int*)(channel3_multistage_time_addr+i*2));
    }
    for(i=0;i<20;i++)
    {
        channel3.multistage_power[i] = (*(unsigned int*)(channel3_multistage_power_addr+i*2));
    }
    channel3.status = 0;
    channel3.now_time_s = 0;

    channel4.power = (*(unsigned int*)(channel4_power_addr));
    channel4.time = (*(unsigned int*)(channel4_time_addr));
    channel4.delay_time = (*(unsigned int*)(channel4_delay_addr));
    channel4.alarm_temperature = (*(unsigned int*)(channel4_alarm_addr));
    channel4.time_hour = (*(unsigned int*)(channel4_timeh_addr));
    channel4.time_minutes = (*(unsigned int*)(channel4_timem_addr));
    channel4.light_mode = (*(unsigned int*)(channel4_ligthm_addr));
    channel4.control_mode = (*(unsigned int*)(channel4_control_addr));
    channel4.cycle = (*(unsigned int*)(channel4_cycle_addr));
    channel4.Enable = (*(unsigned int*)(channel4_enable_addr));
    channel4.set_current = (*(unsigned int*)(channel4_current_addr));
    channel4.set_temperature = (*(unsigned int*)(channel4_temperature_addr));
    for(i=0;i<20;i++)
    {
        channel4.multistage_time[i] = (*(unsigned int*)(channel4_multistage_time_addr+i*2));
    }
    for(i=0;i<20;i++)
    {
        channel4.multistage_power[i] = (*(unsigned int*)(channel4_multistage_power_addr+i*2));
    }
    channel4.status = 0;
    channel4.now_time_s = 0;
    channel_sync_mode = (*(unsigned int*)(sync_mode_addr));
}
void restore_default()//恢复出厂
{
    rt_uint8_t i;
    channel1.power = 100;
    channel1.time = 10;
    channel1.delay_time = 3;
    channel1.alarm_temperature = 100;
    channel1.time_hour = 0;
    channel1.time_minutes = 0;
    channel1.light_mode = 0;
    channel1.control_mode = 0;
    channel1.cycle = 3;
    channel1.Enable = 1;

    channel1.set_current = 1400;
    channel1.set_temperature = 60;
    for(i=0;i<20;i++)
    {
        channel1.multistage_time[i] = 10;
    }
    for(i=0;i<20;i++)
    {
        channel1.multistage_power[i] = 50;
    }
    channel1.status = 0;
    channel1.now_time_s = 0;

    channel2.power = 100;
    channel2.time = 10;
    channel2.delay_time = 3;
    channel2.alarm_temperature = 100;
    channel2.time_hour = 0;
    channel2.time_minutes = 0;
    channel2.light_mode = 0;
    channel2.control_mode = 0;
    channel2.cycle = 3;
    channel2.Enable = 1;
    channel2.set_current = 1400;
    channel2.set_temperature = 60;
    for(i=0;i<20;i++)
    {
        channel2.multistage_time[i] = 10;
    }
    for(i=0;i<20;i++)
    {
        channel2.multistage_power[i] = 50;
    }
    channel2.status = 0;
    channel2.now_time_s = 0;

    channel3.power = 100;
    channel3.time = 10;
    channel3.delay_time = 3;
    channel3.alarm_temperature = 100;
    channel3.time_hour = 0;
    channel3.time_minutes = 0;
    channel3.light_mode = 0;
    channel3.control_mode = 0;
    channel3.cycle = 3;
    channel3.Enable = 1;
    channel3.set_current = 1400;
    channel3.set_temperature = 60;
    for(i=0;i<20;i++)
    {
        channel3.multistage_time[i] = 10;
    }
    for(i=0;i<20;i++)
    {
        channel3.multistage_power[i] = 50;
    }
    channel3.status = 0;
    channel3.now_time_s = 0;

    channel4.power = 100;
    channel4.time = 10;
    channel4.delay_time = 3;
    channel4.alarm_temperature = 100;
    channel4.time_hour = 0;
    channel4.time_minutes = 0;
    channel4.light_mode = 0;
    channel4.control_mode = 0;
    channel4.cycle = 3;
    channel4.Enable = 1;
    channel4.set_current = 1400;
    channel4.set_temperature = 60;
    for(i=0;i<20;i++)
    {
        channel4.multistage_time[i] = 10;
    }
    for(i=0;i<20;i++)
    {
        channel4.multistage_power[i] = 50;
    }
    channel4.status = 0;
    channel4.now_time_s = 0;
    channel_sync_mode = 0;
    save();
}
