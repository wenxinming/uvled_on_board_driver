/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-18     30802       the first version
 */
#ifndef DRIVERS_CHANNEL_H_
#define DRIVERS_CHANNEL_H_
extern struct channel channel1;
extern struct channel channel2;
extern struct channel channel3;
extern struct channel channel4;
extern struct channel_control control_mode;
extern rt_uint8_t channel_sync_mode;//同步模式 = 0异步模式 = 1同步模式
#define c1base_addr 0x807f800
#define c2base_addr 0x807f800+150
#define c3base_addr 0x807f800+300
#define c4base_addr 0x807f800+450

#define channel1_power_addr c1base_addr+0
#define channel1_time_addr c1base_addr+2
#define channel1_delay_addr c1base_addr+4
#define channel1_alarm_addr c1base_addr+6
#define channel1_timeh_addr c1base_addr+8
#define channel1_timem_addr c1base_addr+10
#define channel1_ligthm_addr c1base_addr+12
#define channel1_control_addr c1base_addr+14
#define channel1_cycle_addr c1base_addr+16
#define channel1_enable_addr c1base_addr+18
#define channel1_multistage_time_addr c1base_addr+20
#define channel1_multistage_power_addr c1base_addr+60
#define channel1_current_addr c1base_addr+100
#define channel1_temperature_addr c1base_addr+102


#define channel2_power_addr c2base_addr+0
#define channel2_time_addr c2base_addr+2
#define channel2_delay_addr c2base_addr+4
#define channel2_alarm_addr c2base_addr+6
#define channel2_timeh_addr c2base_addr+8
#define channel2_timem_addr c2base_addr+10
#define channel2_ligthm_addr c2base_addr+12
#define channel2_control_addr c2base_addr+14
#define channel2_cycle_addr c2base_addr+16
#define channel2_enable_addr c2base_addr+18
#define channel2_multistage_time_addr c2base_addr+20
#define channel2_multistage_power_addr c2base_addr+60
#define channel2_current_addr c2base_addr+100
#define channel2_temperature_addr c2base_addr+102

#define channel3_power_addr c3base_addr+0
#define channel3_time_addr c3base_addr+2
#define channel3_delay_addr c3base_addr+4
#define channel3_alarm_addr c3base_addr+6
#define channel3_timeh_addr c3base_addr+8
#define channel3_timem_addr c3base_addr+10
#define channel3_ligthm_addr c3base_addr+12
#define channel3_control_addr c3base_addr+14
#define channel3_cycle_addr c3base_addr+16
#define channel3_enable_addr c3base_addr+18
#define channel3_multistage_time_addr c3base_addr+20
#define channel3_multistage_power_addr c3base_addr+60
#define channel3_current_addr c3base_addr+100
#define channel3_temperature_addr c3base_addr+102

#define channel4_power_addr c4base_addr+0
#define channel4_time_addr c4base_addr+2
#define channel4_delay_addr c4base_addr+4
#define channel4_alarm_addr c4base_addr+6
#define channel4_timeh_addr c4base_addr+8
#define channel4_timem_addr c4base_addr+10
#define channel4_ligthm_addr c4base_addr+12
#define channel4_control_addr c4base_addr+14
#define channel4_cycle_addr c4base_addr+16
#define channel4_enable_addr c4base_addr+18
#define channel4_multistage_time_addr c4base_addr+20
#define channel4_multistage_power_addr c4base_addr+60
#define channel4_current_addr c4base_addr+100
#define channel4_temperature_addr c4base_addr+102
#define sync_mode_addr c4base_addr+104
struct channel_control
{
    rt_uint8_t control_mode;//模式 0手动 1自动
    rt_uint8_t light_mode;//0固定 1阶梯
    rt_uint8_t cycle;//多段循环次数
};
struct channel
{
    rt_uint8_t power;//功率
    rt_uint16_t time;//时间
    rt_uint8_t delay_time;//延时关闭时间
    rt_uint8_t alarm_temperature;//报警温度
    rt_uint8_t multistage_time[20];//多段时间
    rt_uint8_t multistage_power[20];//多段功率
    rt_uint16_t time_hour;//运行小时
    rt_uint8_t time_minutes;//运行分钟
    rt_uint8_t time_seconds;
    rt_uint8_t status;//0关闭 1开启
    rt_uint32_t now_time_s;//本次计时单位秒
    rt_uint8_t control_mode;//模式 0手动 1自动
    rt_uint8_t light_mode;//0固定 1阶梯
    rt_uint8_t cycle;//多段循环次数
    rt_uint8_t Enable;//通道使能
    rt_uint16_t set_current;//设置电流
    rt_uint16_t set_temperature;//设置电流
};
#endif /* DRIVERS_CHANNEL_H_ */
