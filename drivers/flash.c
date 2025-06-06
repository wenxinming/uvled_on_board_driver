/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-21     30802       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <uart.h>
#include <channel.h>
void save()
{
    uint8_t i;
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef FlashSet;
    FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
    FlashSet.PageAddress = 0x807f800;
    FlashSet.NbPages = 1;
    uint32_t PageError = 0;
    HAL_FLASHEx_Erase(&FlashSet, &PageError);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_power_addr, channel1.power);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_time_addr, channel1.time);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_delay_addr, channel1.delay_time);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_alarm_addr, channel1.alarm_temperature);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_timeh_addr, channel1.time_hour);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_timem_addr, channel1.time_minutes);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_ligthm_addr, channel1.light_mode);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_control_addr, channel1.control_mode);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_cycle_addr, channel1.cycle);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_enable_addr, channel1.Enable);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_current_addr, channel1.set_current);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel1_temperature_addr, channel1.set_temperature);
    for(i=0;i<20;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,channel1_multistage_time_addr+i*2,channel1.multistage_time[i]);
    }
    for(i=0;i<20;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,channel1_multistage_power_addr+i*2,channel1.multistage_power[i]);
    }

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_power_addr, channel2.power);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_time_addr, channel2.time);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_delay_addr, channel2.delay_time);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_alarm_addr, channel2.alarm_temperature);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_timeh_addr, channel2.time_hour);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_timem_addr, channel2.time_minutes);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_ligthm_addr, channel2.light_mode);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_control_addr, channel2.control_mode);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_cycle_addr, channel2.cycle);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_enable_addr, channel2.Enable);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_current_addr, channel2.set_current);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel2_temperature_addr, channel2.set_temperature);
    for(i=0;i<20;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,channel2_multistage_time_addr+i*2,channel2.multistage_time[i]);
    }
    for(i=0;i<20;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,channel2_multistage_power_addr+i*2,channel2.multistage_power[i]);
    }

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_power_addr, channel3.power);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_time_addr, channel3.time);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_delay_addr, channel3.delay_time);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_alarm_addr, channel3.alarm_temperature);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_timeh_addr, channel3.time_hour);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_timem_addr, channel3.time_minutes);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_ligthm_addr, channel3.light_mode);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_control_addr, channel3.control_mode);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_cycle_addr, channel3.cycle);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_enable_addr, channel3.Enable);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_current_addr, channel3.set_current);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel3_temperature_addr, channel3.set_temperature);
    for(i=0;i<20;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,channel3_multistage_time_addr+i*2,channel3.multistage_time[i]);
    }
    for(i=0;i<20;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,channel3_multistage_power_addr+i*2,channel3.multistage_power[i]);
    }


    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_power_addr, channel4.power);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_time_addr, channel4.time);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_delay_addr, channel4.delay_time);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_alarm_addr, channel4.alarm_temperature);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_timeh_addr, channel4.time_hour);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_timem_addr, channel4.time_minutes);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_ligthm_addr, channel4.light_mode);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_control_addr, channel4.control_mode);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_cycle_addr, channel4.cycle);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_enable_addr, channel4.Enable);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_current_addr, channel4.set_current);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, channel4_temperature_addr, channel4.set_temperature);
    for(i=0;i<20;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,channel4_multistage_time_addr+i*2,channel4.multistage_time[i]);
    }
    for(i=0;i<20;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,channel4_multistage_power_addr+i*2,channel4.multistage_power[i]);
    }
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, sync_mode_addr, channel_sync_mode);
    HAL_FLASH_Lock();
}
