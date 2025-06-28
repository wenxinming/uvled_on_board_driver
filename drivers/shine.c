/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-18     30802       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <uart.h>
#include <channel.h>
#include <temperature.h>
#include <gpio.h>
#include <lcd_string.h>
#include <current.h>
#include <pwm.h>
rt_thread_t c1,c2,c3,c4;
extern rt_uint8_t on_shine_page;
extern rt_uint16_t set_pwm1,set_pwm2,set_pwm3,set_pwm4;
rt_thread_t l_off1, l_off2, l_off3, l_off4;

extern rt_int16_t c1_set,c2_set,c3_set,c4_set;


void light_fan_delay_off1(void *parameter)
{
    rt_thread_mdelay(channel1.delay_time*1000);
    FAN1_OFF;
}
void light_fan_delay_off2(void *parameter)
{
    rt_thread_mdelay(channel2.delay_time*1000);
    FAN2_OFF;
}
void light_fan_delay_off3(void *parameter)
{
    rt_thread_mdelay(channel3.delay_time*1000);
    FAN3_OFF;
}
void light_fan_delay_off4(void *parameter)
{
    rt_thread_mdelay(channel4.delay_time*1000);
    FAN4_OFF;
}

void FanOFF1()
{
    l_off1 = rt_thread_create("lightoff1", light_fan_delay_off1, RT_NULL, 1024, 30, 100);
    if(l_off1 != RT_NULL)
    rt_thread_startup(l_off1);//启动线程
}

void FanOFF2()
{
    l_off2 = rt_thread_create("lightoff2", light_fan_delay_off2, RT_NULL, 1024, 30, 100);
    if(l_off2 != RT_NULL)
    rt_thread_startup(l_off2);//启动线程
}
void FanOFF3()
{
    l_off3 = rt_thread_create("lightoff3", light_fan_delay_off3, RT_NULL, 1024, 30, 100);
    if(l_off3 != RT_NULL)
    rt_thread_startup(l_off3);//启动线程
}
void FanOFF4()
{
    l_off4 = rt_thread_create("lightoff4", light_fan_delay_off4, RT_NULL, 1024, 30, 100);
    if(l_off4 != RT_NULL)
    rt_thread_startup(l_off4);//启动线程
}
void FanON1()
{
    if(l_off1!=RT_NULL)
        rt_thread_delete(l_off1);
    FAN1_ON;
}
void FanON2()
{
    if(l_off2!=RT_NULL)
        rt_thread_delete(l_off2);
    FAN2_ON;
}
void FanON3()
{
    if(l_off3!=RT_NULL)
        rt_thread_delete(l_off3);
    FAN3_ON;
}
void FanON4()
{
    if(l_off4!=RT_NULL)
        rt_thread_delete(l_off4);
    FAN4_ON;
}
void channel1_manual()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t time_temp;
    Alarm1_OFF;
    NC1_OFF;
    Light1_OFF;
    last_state1 = Read_LedOn1;
    Complete1_OFF;
    channel1.now_time_s = 0;
    Clear_string1;
    led1_open_strup;
    led1_power_strup;
    set_ch1_output(channel1.power);//设置PWM占空比
    ch1_output_on();//使能PWM输出
    Light1_ON;
    led1_button_on_strup;
    time_temp = 0;
    while(1)
    {
        //UpdataLcdDataU8(monitor_page_current_c1,0,current1);
        if(Read_LedOn1 != last_state1)//状态变化
        {
            if(Read_LedOn1 == 0)//低电平
            {

            }else {
                        if(Read_LedOn1 == 1)//低电平
                        {
                            rt_thread_mdelay(200);
                            if(Read_LedOn1 == 1)//低电平
                            {
                                channel1.status = 0;
                            }
                        }
                   }
            last_state1 = Read_LedOn1;
        }
        if(channel1.status == 1)//channel open
        {
            if(Temp1>=channel1.alarm_temperature)//温度报警
            {
                Alarm1_ON;
                NC1_ON;
                channel1.status = 0;
                alarm_status=1;
                led1_temperature_alarm_strup;
            }
            if(channel1.now_time_s>=3)//检查电流
            {
              if(Read_CheckLed1==1)//报警
              {
                Alarm1_ON;
                NC1_ON;
                alarm_status=1;
                channel1.status = 0;
                led1_checkled_alarm_strup;
              }
              if(current1<(set_pwm1/1000.0*1500-100))//比较电流
              {
                  Alarm1_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel1.status = 0;
                  led1_current_alarm_strup;
              }
            }
            time_temp++;
            if(time_temp>=10)
            {
                time_temp = 0;
                channel1.now_time_s++;
                led1_now_time_strup;
            }
        }else {
                if(alarm_status)//如果发声报警
                {

                }else {
                    Clear_string1;
                    led1_close_strup;
                }
                //EN1_OFF;
                set_ch1_output(0);//设置PWM占空比
                ch1_output_off();//使能PWM输出
                Light1_OFF;
                all_time1 = (channel1.time_hour*60*60)+(channel1.time_minutes*60)+channel1.time_seconds;
                all_time1 += channel1.now_time_s;
                channel1.time_hour = all_time1/3600;
                channel1.time_minutes = (all_time1%3600)/60;
                channel1.time_seconds = (all_time1%3600)%60;
                led1_button_off_strup;
                save();
                break;
        }
        rt_thread_mdelay(100);
    }
}
void channel2_manual()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t time_temp;
    Alarm2_OFF;
    NC1_OFF;
    Light2_OFF;
    last_state1 = Read_LedOn2;
    Complete2_OFF;
    channel2.now_time_s = 0;
    led2_open_strup;
    led2_power_strup;
    set_ch2_output(channel2.power);//设置PWM占空比
    ch2_output_on();//使能PWM输出
    Light2_ON;
    led2_button_on_strup;
    time_temp = 0;
    while(1)
    {
        UpdataLcdDataU8(monitor_page_current_c2,0,current2);
        if(Read_LedOn2 != last_state1)//状态变化
        {
            if(Read_LedOn2 == 0)//低电平
            {

            }else {
                        if(Read_LedOn2 == 1)//低电平
                        {
                            rt_thread_mdelay(200);
                            if(Read_LedOn2 == 1)//低电平
                            {
                                channel2.status = 0;
                            }
                        }
                   }
            last_state1 = Read_LedOn2;
        }

        if(channel2.status == 1)//channel open
        {
            if(Temp2>=channel2.alarm_temperature)//温度报警
            {
                Alarm2_ON;
                NC1_ON;
                alarm_status=1;
                channel2.status = 0;
                led2_temperature_alarm_strup;
            }
            if(channel2.now_time_s>=3)//检查电流
            {
              if(Read_CheckLed2==1)//报警
              {
                Alarm2_ON;
                NC1_ON;
                alarm_status=1;
                channel2.status = 0;
                led2_checkled_alarm_strup;
              }
              if(current2<(set_pwm2/1000.0*1500-100))//比较电流
              {
                  Alarm2_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel2.status = 0;
                  led2_current_alarm_strup;
              }
            }
            time_temp++;
            if(time_temp>=10)
            {
                time_temp = 0;
                channel2.now_time_s++;
                led2_now_time_strup;
            }
        }else {
                if(alarm_status)//如果发声报警
                {

                }else {
                    led2_close_strup;
                }
                //EN1_OFF;
                set_ch2_output(0);//设置PWM占空比
                ch2_output_off();//使能PWM输出
                Light2_OFF;
                all_time1 = (channel2.time_hour*60*60)+(channel2.time_minutes*60)+channel2.time_seconds;
                all_time1 += channel2.now_time_s;
                channel2.time_hour = all_time1/3600;
                channel2.time_minutes = (all_time1%3600)/60;
                channel2.time_seconds = (all_time1%3600)%60;
                led2_button_off_strup;
                save();
                break;
        }
        rt_thread_mdelay(100);
    }
}
void channel3_manual()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t time_temp;
    Alarm3_OFF;
    NC1_OFF;
    Light3_OFF;
    last_state1 = Read_LedOn3;
    Complete3_OFF;
    channel3.now_time_s = 0;
    led3_open_strup;
    led3_power_strup;
    set_ch3_output(channel3.power);//设置PWM占空比
    ch3_output_on();//使能PWM输出
    Light3_ON;
    led3_button_on_strup;
    time_temp = 0;
    while(1)
    {
        //UpdataLcdDataU8(monitor_page_current_c3,0,current3);

        if(Read_LedOn3 != last_state1)//状态变化
        {
            if(Read_LedOn3 == 0)//低电平
            {

            }else {
                        if(Read_LedOn3 == 1)//低电平
                        {
                            rt_thread_mdelay(200);
                            if(Read_LedOn3 == 1)//低电平
                            {
                                channel3.status = 0;
                            }
                        }
                   }
            last_state1 = Read_LedOn3;
        }

        if(channel3.status == 1)//channel open
        {
            if(Temp3>=channel3.alarm_temperature)//温度报警
            {
                Alarm3_ON;
                NC1_ON;
                alarm_status=1;
                channel3.status = 0;
                led3_temperature_alarm_strup;
            }
            if(channel3.now_time_s>=3)//检查电流
            {
              if(Read_CheckLed3==1)//报警
              {
                Alarm3_ON;
                NC1_ON;
                alarm_status=1;
                channel3.status = 0;
                led3_checkled_alarm_strup;
              }
              if(current3<(set_pwm3/1000.0*1500-100))//比较电流
              {
                  Alarm3_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel3.status = 0;
                  led3_current_alarm_strup;
              }
            }
            time_temp++;
            if(time_temp>=10)
            {
                time_temp = 0;
                channel3.now_time_s++;
                led3_now_time_strup;
            }
        }else {
                if(alarm_status)//如果发声报警
                {

                }else {
                    led3_close_strup;
                }
                //EN1_OFF;
                set_ch3_output(0);//设置PWM占空比
                ch3_output_off();//使能PWM输出
                Light3_OFF;
                all_time1 = (channel3.time_hour*60*60)+(channel3.time_minutes*60)+channel3.time_seconds;
                all_time1 += channel3.now_time_s;
                channel3.time_hour = all_time1/3600;
                channel3.time_minutes = (all_time1%3600)/60;
                channel3.time_seconds = (all_time1%3600)%60;
                led3_button_off_strup;
                save();
                break;
        }
        rt_thread_mdelay(100);
    }
}
void channel4_manual()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t time_temp;
    Alarm4_OFF;
    NC1_OFF;
    Light4_OFF;
    last_state1 = Read_LedOn4;
    Complete4_OFF;
    channel4.now_time_s = 0;
    led4_open_strup;
    led4_power_strup;
    set_ch4_output(channel4.power);//设置PWM占空比
    ch4_output_on();//使能PWM输出
    Light4_ON;
    led4_button_on_strup;
    time_temp = 0;
    while(1)
    {
        //UpdataLcdDataU8(monitor_page_current_c4,0,current4);

        if(Read_LedOn4 != last_state1)//状态变化
        {
            if(Read_LedOn4 == 0)//低电平
            {

            }else {
                        if(Read_LedOn4 == 1)//低电平
                        {
                            rt_thread_mdelay(200);
                            if(Read_LedOn4 == 1)//低电平
                            {
                                channel4.status = 0;
                            }
                        }
                   }
            last_state1 = Read_LedOn4;
        }

        if(channel4.status == 1)//channel open
        {
            if(Temp4>=channel4.alarm_temperature)//温度报警
            {
                Alarm4_ON;
                NC1_ON;
                alarm_status=1;
                channel4.status = 0;
                led4_temperature_alarm_strup;
            }
            if(channel4.now_time_s>=3)//检查电流
            {
              if(Read_CheckLed4==1)//报警
              {
                Alarm4_ON;
                NC1_ON;
                alarm_status=1;
                channel4.status = 0;
                led4_checkled_alarm_strup;
              }
              if(current4<(set_pwm4/1000.0*1500-100))//比较电流
              {
                  Alarm4_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel4.status = 0;
                  led4_current_alarm_strup;
              }
            }
            time_temp++;
            if(time_temp>=10)
            {
                time_temp = 0;
                channel4.now_time_s++;
                led4_now_time_strup;
            }

        }else {
                if(alarm_status)//如果发声报警
                {

                }else {
                    led4_close_strup;
                }
                //EN1_OFF;
                set_ch4_output(0);//设置PWM占空比
                ch4_output_off();//使能PWM输出
                Light4_OFF;
                all_time1 = (channel4.time_hour*60*60)+(channel4.time_minutes*60)+channel4.time_seconds;
                all_time1 += channel4.now_time_s;
                channel4.time_hour = all_time1/3600;
                channel4.time_minutes = (all_time1%3600)/60;
                channel4.time_seconds = (all_time1%3600)%60;
                led4_button_off_strup;
                save();
                break;
        }
        rt_thread_mdelay(100);
    }
}

void channel1_auto()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t time_temp=0;
    Alarm1_OFF;
    NC1_OFF;
    Light1_OFF;
    last_state1 = Read_LedOn1;
    Complete1_OFF;
    channel1.now_time_s = 0;
    Clear_string1;
    led1_open_strup;
    led1_power_strup;
    set_ch1_output(channel1.power);//设置PWM占空比
    ch1_output_on();//使能PWM输出
    Light1_ON;
    led1_button_on_strup;
    while(1)
    {
        if(Read_LedOn1 != last_state1)//状态变化
        {
            if(Read_LedOn1 == 0)//低电平
            {
                if(channel1.status == 1)
                {
                    channel1.now_time_s = 0;
                }else {
                    channel1.status = 1;
                    channel1.now_time_s = 0;
                }
            }else {
                   }
            last_state1 = Read_LedOn1;
        }
        //UpdataLcdDataU8(monitor_page_current_c1,0,current1);//更新电流
        if(channel1.status == 1)//通道1开启
        {
            /*
            if(Temp1>=channel1.alarm_temperature)//温度报警
            {
                //BEEP_ON;
                Alarm1_ON;
                channel1.status = 0;
            }*/
            if(channel1.now_time_s>=3)//检查电流
           {
                if(Temp1>=channel1.alarm_temperature)//温度报警
                {
                    Alarm1_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel1.status = 0;
                    led1_temperature_alarm_strup;
                }
                if(channel1.now_time_s>=3)//检查电流
                {
                  if(Read_CheckLed1==1)//报警
                  {
                    Alarm1_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel1.status = 0;
                    led1_checkled_alarm_strup;
                  }
                  if(current1<(set_pwm1/1000.0*1500-100))//比较电流
                  {
                      Alarm1_ON;
                      NC1_ON;
                      alarm_status=1;
                      channel1.status = 0;
                      led1_current_alarm_strup;
                  }
                }
           }
            time_temp++;
            if(time_temp>=10)
            {
                time_temp = 0;
                channel1.now_time_s++;
                led1_remaining_time_strup;
            }
            if(channel1.now_time_s>=channel1.time)
            {
                channel1.status = 0;
                Complete1_ON;
            }
        }else {
                if(alarm_status)//如果发声报警
                {

                }else {
                    Clear_string1;
                    led1_close_strup;
                }
                //EN1_OFF;
                set_ch1_output(0);//设置PWM占空比
                ch1_output_off();//使能PWM输出
                Light1_OFF;
                all_time1 = (channel1.time_hour*60*60)+(channel1.time_minutes*60)+channel1.time_seconds;
                all_time1 += channel1.now_time_s;
                channel1.time_hour = all_time1/3600;
                channel1.time_minutes = (all_time1%3600)/60;
                channel1.time_seconds = (all_time1%3600)%60;
                led1_button_off_strup;
                save();
                break;
        }
        rt_thread_mdelay(100);
    }
    //rt_thread_delete(shine);
}

void channel2_auto()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t time_temp=0;
    Alarm2_OFF;
    NC1_OFF;
    Light2_OFF;
    last_state1 = Read_LedOn2;
    Complete2_OFF;
    channel2.now_time_s = 0;
    led2_open_strup;
    led2_power_strup;
    set_ch2_output(channel2.power);//设置PWM占空比
    ch2_output_on();//使能PWM输出
    Light2_ON;
    led2_button_on_strup;
    while(1)
    {
        if(Read_LedOn2 != last_state1)//状态变化
        {
            if(Read_LedOn2 == 0)//低电平
            {
                if(channel2.status == 1)
                {
                    channel2.now_time_s = 0;
                }else {
                    channel2.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn2;
        }
        //UpdataLcdDataU8(monitor_page_current_c2,0,current2);//更新电流
        if(channel2.status == 1)//通道1开启
        {
            /*
            if(Temp1>=channel1.alarm_temperature)//温度报警
            {
                //BEEP_ON;
                Alarm1_ON;
                channel1.status = 0;
            }*/
            if(channel2.now_time_s>=3)//检查电流
           {
                if(Temp2>=channel2.alarm_temperature)//温度报警
                {
                    Alarm2_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel2.status = 0;
                    led2_temperature_alarm_strup;
                }
                if(channel2.now_time_s>=3)//检查电流
                {
                  if(Read_CheckLed2==1)//报警
                  {
                    Alarm2_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel2.status = 0;
                    led2_checkled_alarm_strup;
                  }
                  if(current2<(set_pwm2/1000.0*1500-100))//比较电流
                  {
                      Alarm2_ON;
                      NC1_ON;
                      alarm_status=1;
                      channel2.status = 0;
                      led2_current_alarm_strup;
                  }
                }
           }
            time_temp++;
            if(time_temp>=10)
            {
                time_temp = 0;
                channel2.now_time_s++;
                led2_remaining_time_strup;
            }

            if(channel2.now_time_s>=channel2.time)
            {
                channel2.status = 0;
                Complete2_ON;
            }
        }else {
                if(alarm_status)//如果发声报警
                {

                }else {
                    led2_close_strup;
                }
                //EN1_OFF;
                set_ch2_output(0);//设置PWM占空比
                ch2_output_off();//使能PWM输出
                Light2_OFF;
                all_time1 = (channel2.time_hour*60*60)+(channel2.time_minutes*60)+channel2.time_seconds;
                all_time1 += channel2.now_time_s;
                channel2.time_hour = all_time1/3600;
                channel2.time_minutes = (all_time1%3600)/60;
                channel2.time_seconds = (all_time1%3600)%60;
                led2_button_off_strup;
                save();
                break;
        }
        rt_thread_mdelay(100);
    }
    //rt_thread_delete(shine);
}

void channel3_auto()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t time_temp=0;
    Alarm3_OFF;
    NC1_OFF;
    Light3_OFF;
    last_state1 = Read_LedOn3;
    Complete3_OFF;
    channel3.now_time_s = 0;
    led3_open_strup;
    led3_power_strup;
    set_ch3_output(channel3.power);//设置PWM占空比
    ch3_output_on();//使能PWM输出
    Light3_ON;
    led3_button_on_strup;
    while(1)
    {
        if(Read_LedOn3 != last_state1)//状态变化
        {
            if(Read_LedOn3 == 0)//低电平
            {
                if(channel3.status == 1)
                {
                    channel3.now_time_s = 0;
                }else {
                    channel3.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn3;
        }
        //UpdataLcdDataU8(monitor_page_current_c3,0,current3);//更新电流
        if(channel3.status == 1)//通道1开启
        {
            /*
            if(Temp1>=channel1.alarm_temperature)//温度报警
            {
                //BEEP_ON;
                Alarm1_ON;
                channel1.status = 0;
            }*/
            if(channel3.now_time_s>=3)//检查电流
           {
                if(Temp3>=channel3.alarm_temperature)//温度报警
                {
                    Alarm3_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel3.status = 0;
                    led3_temperature_alarm_strup;
                }
                if(channel3.now_time_s>=3)//检查电流
                {
                  if(Read_CheckLed3==1)//报警
                  {
                    Alarm3_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel3.status = 0;
                    led3_checkled_alarm_strup;
                  }
                  if(current3<(set_pwm3/1000.0*1500-100))//比较电流
                  {
                      Alarm3_ON;
                      NC1_ON;
                      alarm_status=1;
                      channel3.status = 0;
                      led3_current_alarm_strup;
                  }
                }
           }
            time_temp++;
            if(time_temp>=10)
            {
                time_temp = 0;
                channel3.now_time_s++;
                led3_remaining_time_strup;
            }

            if(channel3.now_time_s>=channel3.time)
            {
                channel3.status = 0;
                Complete3_ON;
            }

        }else {
                if(alarm_status)//如果发声报警
                {

                }else {
                    led3_close_strup;
                }
                //EN1_OFF;
                set_ch3_output(0);//设置PWM占空比
                ch3_output_off();//使能PWM输出
                Light3_OFF;
                all_time1 = (channel3.time_hour*60*60)+(channel3.time_minutes*60)+channel3.time_seconds;
                all_time1 += channel3.now_time_s;
                channel3.time_hour = all_time1/3600;
                channel3.time_minutes = (all_time1%3600)/60;
                channel3.time_seconds = (all_time1%3600)%60;
                led3_button_off_strup;
                save();
                break;
        }
        rt_thread_mdelay(100);
    }
    //rt_thread_delete(shine);
}

void channel4_auto()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t time_temp=0;
    Alarm4_OFF;
    NC1_OFF;
    Light4_OFF;
    last_state1 = Read_LedOn4;
    Complete4_OFF;
    channel4.now_time_s = 0;
    led4_open_strup;
    led4_power_strup;
    set_ch4_output(channel4.power);//设置PWM占空比
    ch4_output_on();//使能PWM输出
    Light4_ON;
    led4_button_on_strup;
    while(1)
    {
        if(Read_LedOn4 != last_state1)//状态变化
        {
            if(Read_LedOn4 == 0)//低电平
            {
                if(channel4.status == 1)
                {
                    channel4.now_time_s = 0;
                }else {
                    channel4.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn4;
        }
        //UpdataLcdDataU8(monitor_page_current_c4,0,current4);//更新电流
        if(channel4.status == 1)//通道1开启
        {
            /*
            if(Temp1>=channel1.alarm_temperature)//温度报警
            {
                //BEEP_ON;
                Alarm1_ON;
                channel1.status = 0;
            }*/
            if(channel4.now_time_s>=3)//检查电流
           {
                if(Temp4>=channel4.alarm_temperature)//温度报警
                {
                    Alarm4_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel4.status = 0;
                    led4_temperature_alarm_strup;
                }
                if(channel4.now_time_s>=3)//检查电流
                {
                  if(Read_CheckLed4==1)//报警
                  {
                    Alarm4_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel4.status = 0;
                    led4_checkled_alarm_strup;
                  }
                  if(current4<(set_pwm4/1000.0*1500-100))//比较电流
                  {
                      Alarm4_ON;
                      NC1_ON;
                      alarm_status=1;
                      channel4.status = 0;
                      led4_current_alarm_strup;
                  }
                }
           }
            time_temp++;
            if(time_temp>=10)
            {
                time_temp = 0;
                channel4.now_time_s++;
                led4_remaining_time_strup;
            }
            if(channel4.now_time_s>=channel4.time)
            {
                channel4.status = 0;
                Complete4_ON;
            }
        }else {
                if(alarm_status)//如果发声报警
                {

                }else {
                    led4_close_strup;
                }
                //EN1_OFF;
                set_ch4_output(0);//设置PWM占空比
                ch4_output_off();//使能PWM输出
                Light4_OFF;
                all_time1 = (channel4.time_hour*60*60)+(channel4.time_minutes*60)+channel4.time_seconds;
                all_time1 += channel4.now_time_s;
                channel4.time_hour = all_time1/3600;
                channel4.time_minutes = (all_time1%3600)/60;
                channel4.time_seconds = (all_time1%3600)%60;
                led4_button_off_strup;
                save();
                break;
        }
        rt_thread_mdelay(100);
    }
    //rt_thread_delete(shine);
}

void channel1_multistage()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t cycle1,large_cycle1;
    Alarm1_OFF;
    NC1_OFF;
    Light1_OFF;
    last_state1 = Read_LedOn1;
    Complete1_OFF;
    channel1.now_time_s = 0;
    Clear_string1;
    led1_open_strup;
    led1_power_strup;
    Light1_ON;
    led1_button_on_strup;
    UpdataLcdDataU8(monitor_page_power_c1,2,channel1.multistage_power[0]);//更新功率
    cycle1 = 0;
    large_cycle1 =0;
    set_ch1_output(channel1.multistage_power[cycle1]);//设置PWM占空比
    ch1_output_on();//使能PWM输出
    while(1)
    {
      if(Read_LedOn1 != last_state1)//状态变化
      {
          if(Read_LedOn1 == 0)//低电平
          {
              if(channel1.status == 1)
              {
                  channel1.now_time_s = 0;
                  cycle1 = 0;

                  set_ch1_output(channel1.multistage_power[cycle1]);//设置PWM占空比
                   ch1_output_on();//使能PWM输出
                   UpdataLcdDataU8(monitor_page_power_c1,2,channel1.multistage_power[cycle1]);//更新功率
              }else {
                  channel1.status = 1;
              }
          }else {
                 }
          last_state1 = Read_LedOn1;
      }
      UpdataLcdDataU8(monitor_page_current_c1,0,current1);//更新电流
      if(channel1.status == 1)//通道1开启
      {
         if(channel1.now_time_s>=3)//检查电流
         {
              if(Temp1>=channel1.alarm_temperature)//温度报警
              {
                  Alarm1_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel1.status = 0;
                  led1_temperature_alarm_strup;
              }
              if(channel1.now_time_s>=3)//检查电流
              {
                if(Read_CheckLed1==1)//报警
                {
                  Alarm1_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel1.status = 0;
                  led1_checkled_alarm_strup;
                }
                if(current1<(set_pwm1/1000.0*1500-100))//比较电流
                {
                    Alarm1_ON;

                    alarm_status=1;
                    channel1.status = 0;
                    led1_current_alarm_strup;
                }
              }
         }
          channel1.now_time_s++;
          if(channel1.now_time_s>=channel1.multistage_time[cycle1])
          {
             cycle1++;
             all_time1 = (channel1.time_hour*60*60)+(channel1.time_minutes*60)+channel1.time_seconds;
             all_time1 += channel1.now_time_s;
             channel1.time_hour = all_time1/3600;
             channel1.time_minutes = (all_time1%3600)/60;
             channel1.time_seconds = (all_time1%3600)%60;
             channel1.now_time_s=0;
             if(cycle1>19)
             {
               large_cycle1++;
               if(large_cycle1>=channel1.cycle)
                {
                    channel1.status = 0;
                }
                cycle1=0;
             }
             set_ch1_output(channel1.multistage_power[cycle1]);//设置PWM占空比
             ch1_output_on();//使能PWM输出
             UpdataLcdDataU8(monitor_page_power_c1,2,channel1.multistage_power[cycle1]);//更新功率
          }
          UpdataLcdDataU8(led1_time_strup,8,channel1.multistage_time[cycle1]-channel1.now_time_s);//通道1自动模式剩余时间;
      }else {
              if(alarm_status)//如果发声报警
              {

              }else {
                  Clear_string1;
                  led1_close_strup;
              }
              //EN1_OFF;
              set_ch1_output(0);//设置PWM占空比
              ch1_output_off();//使能PWM输出
              Light1_OFF;
              all_time1 = (channel1.time_hour*60*60)+(channel1.time_minutes*60)+channel1.time_seconds;
              all_time1 += channel1.now_time_s;
              channel1.time_hour = all_time1/3600;
              channel1.time_minutes = (all_time1%3600)/60;
              channel1.time_seconds = (all_time1%3600)%60;
              led1_button_off_strup;
              save();
              break;
      }
      rt_thread_mdelay(1000);
    }
}

void channel2_multistage()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t cycle1,large_cycle1;
    Alarm2_OFF;
    NC1_OFF;
    Light2_OFF;
    last_state1 = Read_LedOn2;
    Complete2_OFF;
    channel2.now_time_s = 0;
    led2_open_strup;
    led2_power_strup;
    Light2_ON;
    led2_button_on_strup;
    UpdataLcdDataU8(monitor_page_power_c2 ,2,channel2.multistage_power[0]);//更新功率
    cycle1 = 0;
    large_cycle1 =0;
    set_ch2_output(channel2.multistage_power[cycle1]);//设置PWM占空比
    ch2_output_on();//使能PWM输出
    while(1)
    {
      if(Read_LedOn2 != last_state1)//状态变化
      {
          if(Read_LedOn2 == 0)//低电平
          {
              if(channel2.status == 1)
              {
                  channel2.now_time_s = 0;
                  cycle1 = 0;
                  set_ch2_output(channel2.multistage_power[cycle1]);//设置PWM占空比
                   ch2_output_on();//使能PWM输出
                   UpdataLcdDataU8(monitor_page_power_c2 ,2,channel2.multistage_power[cycle1]);//更新功率
              }else {
                  channel2.status = 1;
              }
          }else {
                 }
          last_state1 = Read_LedOn2;
      }
      UpdataLcdDataU8(monitor_page_current_c2,0,current2);//更新电流
      if(channel2.status == 1)//通道1开启
      {
         if(channel2.now_time_s>=3)//检查电流
         {
              if(Temp2>=channel2.alarm_temperature)//温度报警
              {
                  Alarm2_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel2.status = 0;
                  led2_temperature_alarm_strup;
              }
              if(channel2.now_time_s>=3)//检查电流
              {
                if(Read_CheckLed2==1)//报警
                {
                  Alarm2_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel2.status = 0;
                  led2_checkled_alarm_strup;
                }
                if(current2<(set_pwm2/1000.0*1500-100))//比较电流
                {
                    Alarm2_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel2.status = 0;
                    led2_current_alarm_strup;
                }
              }
         }
          channel2.now_time_s++;
          if(channel2.now_time_s>=channel2.multistage_time[cycle1])
          {
             cycle1++;
             all_time1 = (channel2.time_hour*60*60)+(channel2.time_minutes*60)+channel2.time_seconds;
             all_time1 += channel2.now_time_s;
             channel2.time_hour = all_time1/3600;
             channel2.time_minutes = (all_time1%3600)/60;
             channel2.time_seconds = (all_time1%3600)%60;
             channel2.now_time_s=0;
             if(cycle1>19)
             {
               large_cycle1++;
               if(large_cycle1>=channel2.cycle)
                {
                    channel2.status = 0;
                }
                cycle1=0;
             }
             set_ch2_output(channel2.multistage_power[cycle1]);//设置PWM占空比
             ch2_output_on();//使能PWM输出
             UpdataLcdDataU8(monitor_page_power_c2 ,2,channel2.multistage_power[cycle1]);//更新功率
          }
          UpdataLcdDataU8(led2_time_strup,8,channel2.multistage_time[cycle1]-channel2.now_time_s);//通道1自动模式剩余时间;
      }else {
              if(alarm_status)//如果发声报警
              {

              }else {
                  led2_close_strup;
              }
              //EN1_OFF;
              set_ch2_output(0);//设置PWM占空比
              ch2_output_off();//使能PWM输出
              Light2_OFF;
              all_time1 = (channel2.time_hour*60*60)+(channel2.time_minutes*60)+channel2.time_seconds;
              all_time1 += channel2.now_time_s;
              channel2.time_hour = all_time1/3600;
              channel2.time_minutes = (all_time1%3600)/60;
              channel2.time_seconds = (all_time1%3600)%60;
              led2_button_off_strup;
              save();
              break;
      }
      rt_thread_mdelay(1000);
    }
}
void channel3_multistage()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t cycle1,large_cycle1;
    Alarm3_OFF;
    NC1_OFF;
    Light3_OFF;
    last_state1 = Read_LedOn3;
    Complete3_OFF;
    channel3.now_time_s = 0;
    led3_open_strup;
    led3_power_strup;
    Light3_ON;
    led3_button_on_strup;
    UpdataLcdDataU8(monitor_page_power_c3,2,channel3.multistage_power[0]);//更新功率
    cycle1 = 0;
    large_cycle1 =0;
    set_ch3_output(channel3.multistage_power[cycle1]);//设置PWM占空比
    ch3_output_on();//使能PWM输出
    while(1)
    {
      if(Read_LedOn3 != last_state1)//状态变化
      {
          if(Read_LedOn3 == 0)//低电平
          {
              if(channel3.status == 1)
              {
                  channel3.now_time_s = 0;
                 cycle1 = 0;
                 set_ch3_output(channel3.multistage_power[cycle1]);//设置PWM占空比
                  ch3_output_on();//使能PWM输出
                  UpdataLcdDataU8(monitor_page_power_c3 ,2,channel3.multistage_power[cycle1]);//更新功率
              }else {
                  channel3.status = 1;
              }
          }else {
                 }
          last_state1 = Read_LedOn3;
      }
      UpdataLcdDataU8(monitor_page_current_c3,0,current3);//更新电流
      if(channel3.status == 1)//通道1开启
      {
         if(channel3.now_time_s>=3)//检查电流
         {
              if(Temp3>=channel3.alarm_temperature)//温度报警
              {
                  Alarm3_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel3.status = 0;
                  led3_temperature_alarm_strup;
              }
              if(channel3.now_time_s>=3)//检查电流
              {
                if(Read_CheckLed3==1)//报警
                {
                  Alarm3_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel3.status = 0;
                  led3_checkled_alarm_strup;
                }
                if(current3<(set_pwm3/1000.0*1500-100))//比较电流
                {
                    Alarm3_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel3.status = 0;
                    led3_current_alarm_strup;
                }
              }
         }
          channel3.now_time_s++;
          if(channel3.now_time_s>=channel3.multistage_time[cycle1])
          {
             cycle1++;
             all_time1 = (channel3.time_hour*60*60)+(channel3.time_minutes*60)+channel3.time_seconds;
             all_time1 += channel3.now_time_s;
             channel3.time_hour = all_time1/3600;
             channel3.time_minutes = (all_time1%3600)/60;
             channel3.time_seconds = (all_time1%3600)%60;
             channel3.now_time_s=0;
             if(cycle1>19)
             {
               large_cycle1++;
               if(large_cycle1>=channel3.cycle)
                {
                    channel3.status = 0;
                }
                cycle1=0;
             }
             set_ch3_output(channel3.multistage_power[cycle1]);//设置PWM占空比
             ch3_output_on();//使能PWM输出
             UpdataLcdDataU8(monitor_page_power_c3 ,2,channel3.multistage_power[cycle1]);//更新功率
          }
          UpdataLcdDataU8(led3_time_strup,8,channel3.multistage_time[cycle1]-channel3.now_time_s);//通道1自动模式剩余时间;
      }else {
              if(alarm_status)//如果发声报警
              {

              }else {
                  led3_close_strup;
              }
              //EN1_OFF;
              set_ch3_output(0);//设置PWM占空比
              ch3_output_off();//使能PWM输出
              Light3_OFF;
              all_time1 = (channel3.time_hour*60*60)+(channel3.time_minutes*60)+channel3.time_seconds;
              all_time1 += channel3.now_time_s;
              channel3.time_hour = all_time1/3600;
              channel3.time_minutes = (all_time1%3600)/60;
              channel3.time_seconds = (all_time1%3600)%60;
              led3_button_off_strup;
              save();
              break;
      }
      rt_thread_mdelay(1000);
    }
}

void channel4_multistage()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//外部开灯IO状态
    rt_uint8_t alarm_status=0;
    rt_uint8_t cycle1,large_cycle1;
    Alarm4_OFF;
    NC1_OFF;
    Light4_OFF;
    last_state1 = Read_LedOn4;
    Complete4_OFF;
    channel4.now_time_s = 0;
    led4_open_strup;
    led4_power_strup;
    Light4_ON;
    led4_button_on_strup;
    UpdataLcdDataU8(monitor_page_power_c4,2,channel4.multistage_power[0]);//更新功率
    cycle1 = 0;
    large_cycle1 =0;
    set_ch4_output(channel4.multistage_power[cycle1]);//设置PWM占空比
    ch4_output_on();//使能PWM输出
    while(1)
    {
      if(Read_LedOn4 != last_state1)//状态变化
      {
          if(Read_LedOn4 == 0)//低电平
          {
              if(channel4.status == 1)
              {
                  channel4.now_time_s = 0;
                  cycle1 = 0;
                  set_ch4_output(channel4.multistage_power[cycle1]);//设置PWM占空比
                   ch4_output_on();//使能PWM输出
                   UpdataLcdDataU8(monitor_page_power_c4,2,channel3.multistage_power[cycle1]);//更新功率
              }else {
                  channel4.status = 1;
              }
          }else {
                 }
          last_state1 = Read_LedOn4;
      }
      UpdataLcdDataU8(monitor_page_current_c4,0,current4);//更新电流
      if(channel4.status == 1)//通道1开启
      {
         if(channel4.now_time_s>=3)//检查电流
         {
              if(Temp4>=channel4.alarm_temperature)//温度报警
              {
                  Alarm4_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel4.status = 0;
                  led4_temperature_alarm_strup;
              }
              if(channel4.now_time_s>=3)//检查电流
              {
                if(Read_CheckLed4==1)//报警
                {
                  Alarm4_ON;
                  NC1_ON;
                  alarm_status=1;
                  channel4.status = 0;
                  led4_checkled_alarm_strup;
                }
                if(current4<(set_pwm4/1000.0*1500-100))//比较电流
                {
                    Alarm4_ON;
                    NC1_ON;
                    alarm_status=1;
                    channel4.status = 0;
                    led4_current_alarm_strup;
                }
              }
         }
          channel4.now_time_s++;
          if(channel4.now_time_s>=channel4.multistage_time[cycle1])
          {
             cycle1++;
             all_time1 = (channel4.time_hour*60*60)+(channel4.time_minutes*60)+channel4.time_seconds;
             all_time1 += channel4.now_time_s;
             channel4.time_hour = all_time1/3600;
             channel4.time_minutes = (all_time1%3600)/60;
             channel4.time_seconds = (all_time1%3600)%60;
             channel4.now_time_s=0;
             if(cycle1>19)
             {
               large_cycle1++;
               if(large_cycle1>=channel4.cycle)
                {
                    channel4.status = 0;
                }
                cycle1=0;
             }
             set_ch4_output(channel4.multistage_power[cycle1]);//设置PWM占空比
             ch4_output_on();//使能PWM输出
             UpdataLcdDataU8(monitor_page_power_c4,2,channel3.multistage_power[cycle1]);//更新功率
          }
          UpdataLcdDataU8(led4_time_strup,8,channel4.multistage_time[cycle1]-channel4.now_time_s);//通道1自动模式剩余时间;
      }else {
              if(alarm_status)//如果发声报警
              {

              }else {
                  led4_close_strup;
              }
              //EN1_OFF;
              set_ch4_output(0);//设置PWM占空比
              ch4_output_off();//使能PWM输出
              Light4_OFF;
              all_time1 = (channel4.time_hour*60*60)+(channel4.time_minutes*60)+channel4.time_seconds;
              all_time1 += channel4.now_time_s;
              channel4.time_hour = all_time1/3600;
              channel4.time_minutes = (all_time1%3600)/60;
              channel4.time_seconds = (all_time1%3600)%60;
              led4_button_off_strup;
              save();
              break;
      }
      rt_thread_mdelay(1000);
    }
}

void sync_mode()
{
    if(channel_sync_mode==1)
    {
        channel1.status = 1;
        channel2.status = 1;
        channel3.status = 1;
        channel4.status = 1;
    }
}

void sync_mode_off()
{
    if(channel_sync_mode==1)
    {
        channel1.status = 0;
        channel2.status = 0;
        channel3.status = 0;
        channel4.status = 0;
    }
}

void updata_shine_page()
{
    UpdataLcdDataU8(0x5020,2,channel1.power);//更新功率
    UpdataLcdDataU8(0x1054,2,0);//更新照射时间
    UpdataLcdDataU8(0x1058,2,0);//更新电流
    updatarunbutton(0,0x1080,0);//更新按钮状态
    if(channel1.control_mode==0)//手动
    {
        UpdataLcdDataU8(0x1054,2,0);//更新照射时间
    }else {//自动
        if(channel1.light_mode==0)//固定
        {
            UpdataLcdDataU8(0x1054,2,channel1.time);//更新照射时间
        }else {//阶梯
            UpdataLcdDataU8(0x1054,2,channel1.time);//更新照射时间
        }
    }
    UpdataLcdDataU8(monitor_page_power_c2 ,2,channel2.power);//更新功率
    UpdataLcdDataU8(monitor_page_time_c2 ,2,0);//更新照射时间
    UpdataLcdDataU8(monitor_page_current_c2 ,2,0);//更新电流
    led2_button_off_strup;
    if(channel2.control_mode==0)//手动
    {
        UpdataLcdDataU8(monitor_page_time_c2,2,0);//更新照射时间
    }else {//自动
        if(channel2.light_mode==0)//固定
        {
            UpdataLcdDataU8(monitor_page_time_c2,2,channel2.time);//更新照射时间
        }else {//阶梯
            UpdataLcdDataU8(monitor_page_time_c2,2,channel2.time);//更新照射时间
        }
    }
    UpdataLcdDataU8(monitor_page_power_c3 ,2,channel3.power);//更新功率
    UpdataLcdDataU8(monitor_page_time_c3 ,2,0);//更新照射时间
    UpdataLcdDataU8(monitor_page_current_c3 ,2,0);//更新电流
    led3_button_off_strup;
    if(channel3.control_mode==0)//手动
    {
        UpdataLcdDataU8(monitor_page_time_c3,2,0);//更新照射时间
    }else {//自动
        if(channel3.light_mode==0)//固定
        {
            UpdataLcdDataU8(monitor_page_time_c3,2,channel3.time);//更新照射时间
        }else {//阶梯
            UpdataLcdDataU8(monitor_page_time_c3,2,channel3.time);//更新照射时间
        }
    }
    UpdataLcdDataU8(monitor_page_power_c4 ,2,channel4.power);//更新功率
    UpdataLcdDataU8(monitor_page_time_c4 ,2,0);//更新照射时间
    UpdataLcdDataU8(monitor_page_current_c4 ,2,0);//更新电流
    led4_button_off_strup;

    if(channel4.control_mode==0)//手动
    {
        UpdataLcdDataU8(monitor_page_time_c4,2,0);//更新照射时间
    }else {//自动
        if(channel4.light_mode==0)//固定
        {
            UpdataLcdDataU8(monitor_page_time_c4,2,channel4.time);//更新照射时间
        }else {//阶梯
            UpdataLcdDataU8(monitor_page_time_c4,2,channel4.time);//更新照射时间
        }
    }
}

void shine_channel1(void *parameter)//manual mode
{
    rt_uint8_t old_status,last_state1;
    old_status = channel1.status;
    last_state1 = Read_LedOn1;
    //更新监控界面文字
    Clear_string1;
    led1_close_strup;
    UpdataLcdDataU8(0x5020,2,channel1.power);//更新功率
    UpdataLcdDataU8(0x1054,2,0);//更新照射时间
    UpdataLcdDataU8(0x1058,2,0);//更新电流
    updatarunbutton(0,0x1080,0);//更新按钮状态
    if(channel1.control_mode==0)//手动
    {
        UpdataLcdDataU8(0x1054,2,0);//更新照射时间
    }else {//自动
        if(channel1.light_mode==0)//固定
        {
            UpdataLcdDataU8(0x1054,2,channel1.time);//更新照射时间
        }else {//阶梯
            UpdataLcdDataU8(0x1054,2,channel1.time);//更新照射时间
        }
    }
    while(1)
    {
        if(Read_LedOn1 != last_state1)//状态变化
        {
            if(Read_LedOn1 == 0)//低电平
            {
                if(channel1.status == 1)
                {
                    channel1.status = 0;
                }else {
                    channel1.status = 1;
                    switch_show(0);
                }
            }else {
                   }
            last_state1 = Read_LedOn1;
        }
        if (old_status!=channel1.status)//如果状态有变化
        {
            if(channel1.status==1)//开灯
            {
                sync_mode();
                FanON1();
                if(channel1.light_mode==0)//手动
                {
                    c1_set = channel1.set_current;
                    channel1_manual();
                    //UpdataLcdDataU8(monitor_page_current_c1,0,0);
                }else {//自动
                    if(channel1.control_mode==0)//固定
                    {
                        c1_set = channel1.set_current;
                        channel1_auto();
                       // UpdataLcdDataU8(monitor_page_current_c1,0,0);
                    }else {//阶梯
                        c1_set = channel1.set_current;
                        channel1_multistage();
                        //UpdataLcdDataU8(monitor_page_current_c1,0,0);
                    }
                }

                FanOFF1();
                sync_mode_off();
            }
            old_status = channel1.status;
            last_state1 = Read_LedOn1;
        }
        rt_thread_mdelay(100);
    }
}

void shine_channel2(void *parameter)//manual mode
{
    rt_uint8_t old_status,last_state1;
    old_status = channel2.status;
    last_state1 = Read_LedOn2;
    //更新监控界面文字
    led2_close_strup;
    UpdataLcdDataU8(monitor_page_power_c2 ,2,channel2.power);//更新功率
    UpdataLcdDataU8(monitor_page_time_c2 ,2,0);//更新照射时间
    UpdataLcdDataU8(monitor_page_current_c2 ,2,0);//更新电流
    led2_button_off_strup;
    if(channel2.control_mode==0)//手动
    {
        UpdataLcdDataU8(monitor_page_time_c2,2,0);//更新照射时间
    }else {//自动
        if(channel2.light_mode==0)//固定
        {
            UpdataLcdDataU8(monitor_page_time_c2,2,channel2.time);//更新照射时间
        }else {//阶梯
            UpdataLcdDataU8(monitor_page_time_c2,2,channel2.time);//更新照射时间
        }
    }
    while(1)
    {
        if(Read_LedOn2 != last_state1)//状态变化
        {
            if(Read_LedOn2 == 0)//低电平
            {
                if(channel2.status == 1)
                {
                    channel2.status = 0;
                }else {
                    channel2.status = 1;
                    switch_show(0);
                }
            }else {
                   }
            last_state1 = Read_LedOn2;
        }
        if (old_status!=channel2.status)//如果状态有变化j
        {
            if(channel2.status==1)//开灯
            {
                sync_mode();
                FanON2();
                if(channel2.light_mode==0)//手动
                {
                    c2_set = channel2.set_current;
                    channel2_manual();
                    //UpdataLcdDataU8(monitor_page_current_c2,0,0);
                }else {//自动
                    if(channel2.control_mode==0)//固定
                    {
                        c2_set = channel2.set_current;
                        channel2_auto();
                        //UpdataLcdDataU8(monitor_page_current_c2,0,0);
                    }else {//阶梯
                        channel2_multistage();
                        //UpdataLcdDataU8(monitor_page_current_c2,0,0);
                    }
                }
                FanOFF2();
                sync_mode_off();
            }
            last_state1 = Read_LedOn2;
            old_status = channel2.status;
        }

        rt_thread_mdelay(100);
    }
}

void shine_channel3(void *parameter)//manual mode
{
    rt_uint8_t old_status,last_state1;
    old_status = channel3.status;
    last_state1 = Read_LedOn3;
    //更新监控界面文字
    led3_close_strup;
    UpdataLcdDataU8(monitor_page_power_c3 ,2,channel3.power);//更新功率
    UpdataLcdDataU8(monitor_page_time_c3 ,2,0);//更新照射时间
    UpdataLcdDataU8(monitor_page_current_c3 ,2,0);//更新电流
    led3_button_off_strup;
    if(channel3.control_mode==0)//手动
    {
        UpdataLcdDataU8(monitor_page_time_c3,2,0);//更新照射时间
    }else {//自动
        if(channel3.light_mode==0)//固定
        {
            UpdataLcdDataU8(monitor_page_time_c3,2,channel3.time);//更新照射时间
        }else {//阶梯
            UpdataLcdDataU8(monitor_page_time_c3,2,channel3.time);//更新照射时间
        }
    }
    while(1)
    {
        if(Read_LedOn3 != last_state1)//状态变化
        {
            if(Read_LedOn3 == 0)//低电平
            {
                if(channel3.status == 1)
                {
                    channel3.status = 0;

                }else {
                    channel3.status = 1;
                    switch_show(0);
                }
            }else {
                   }
            last_state1 = Read_LedOn3;
        }
        if (old_status!=channel3.status)//如果状态有变化j
        {
            if(channel3.status==1)//开灯
            {
                sync_mode();
                FanON3();
                if(channel3.light_mode==0)//手动
                {
                    c3_set = channel3.set_current;
                    channel3_manual();
                    //UpdataLcdDataU8(monitor_page_current_c3,0,0);
                }else {//自动
                    if(channel3.control_mode==0)//固定
                    {
                        c3_set = channel3.set_current;
                        channel3_auto();
                        //UpdataLcdDataU8(monitor_page_current_c3,0,0);
                    }else {//阶梯
                        channel3_multistage();
                        //UpdataLcdDataU8(monitor_page_current_c3,0,0);
                    }
                }
                FanOFF3();
                sync_mode_off();
            }
            old_status = channel3.status;
            last_state1 = Read_LedOn3;
        }
        rt_thread_mdelay(100);
    }
}

void shine_channel4(void *parameter)//manual mode
{
    rt_uint8_t old_status,last_state1;
    old_status = channel4.status;
    last_state1 = Read_LedOn4;
    //更新监控界面文字
    led4_close_strup;
    UpdataLcdDataU8(monitor_page_power_c4 ,2,channel4.power);//更新功率
    UpdataLcdDataU8(monitor_page_time_c4 ,2,0);//更新照射时间
    UpdataLcdDataU8(monitor_page_current_c4 ,2,0);//更新电流
    led4_button_off_strup;
    last_state1 = Read_LedOn4;
    if(channel4.control_mode==0)//手动
    {
        UpdataLcdDataU8(monitor_page_time_c4,2,0);//更新照射时间
    }else {//自动
        if(channel4.light_mode==0)//固定
        {
            UpdataLcdDataU8(monitor_page_time_c4,2,channel4.time);//更新照射时间
        }else {//阶梯
            UpdataLcdDataU8(monitor_page_time_c4,2,channel4.time);//更新照射时间
        }
    }
    while(1)
    {
        if(Read_LedOn4 != last_state1)//状态变化
        {
            if(Read_LedOn4 == 0)//低电平
            {
                if(channel4.status == 1)
                {
                    channel4.status = 0;
                }else {
                    channel4.status = 1;
                    switch_show(0);
                }
            }else {
                   }
            last_state1 = Read_LedOn4;
        }
        if (old_status!=channel4.status)//如果状态有变化j
        {
            if(channel4.status==1)//开灯
            {
                sync_mode();
                FanON4();
                if(channel4.light_mode==0)//手动
                {
                    c4_set = channel4.set_current;
                    channel4_manual();
                    //UpdataLcdDataU8(monitor_page_current_c4,0,0);
                }else {//自动
                    if(channel4.control_mode==0)//固定
                    {
                        c4_set = channel4.set_current;
                        channel4_auto();
                        //UpdataLcdDataU8(monitor_page_current_c4,0,0);
                    }else {//阶梯
                        channel4_multistage();
                        //UpdataLcdDataU8(monitor_page_current_c4,0,0);
                    }
                }
                FanOFF4();
                sync_mode_off();
            }
            old_status = channel4.status;
            last_state1 = Read_LedOn4;
        }
        rt_thread_mdelay(100);
    }
}

void shine_init()//照射 初始化
{
    l_off1 = 0;
    l_off2 = 0;
    l_off3 = 0;
    l_off4 = 0;
    c1 = rt_thread_create("shine1", shine_channel1, RT_NULL, 1024, 20, 10);
    rt_thread_startup(c1);//启动线程
    c2 = rt_thread_create("shine2", shine_channel2, RT_NULL, 1024, 20, 10);
    rt_thread_startup(c2);//启动线程
    c3 = rt_thread_create("shine3", shine_channel3, RT_NULL, 1024, 20, 10);
    rt_thread_startup(c3);//启动线程
    c4 = rt_thread_create("shine4", shine_channel4, RT_NULL, 1024, 20, 10);
    rt_thread_startup(c4);//启动线程*/
    /*
    UpdataLcdString(7,17,"");//字符串初始化
    UpdataLcdString(7,19,"");//字符串初始化
    updatarunbutton(0,1,7);//通道1按钮
    updatarunbutton(0,0x10,7);//通道1按钮
    UpdataLcdDataU8(7,8,0);//通道1时间
    UpdataLcdDataU8(7,12,0);//通道2时间
    on_shine_page = 1;
    if(control_mode.control_mode == 0)//手动模式
    {
        //UpdataLcdDataU8(7,8,0);//通道1时间
        //UpdataLcdDataU8(7,12,0);//通道2时间
        shine = rt_thread_create("shine", shine_entry_manual, RT_NULL, 1024, 20, 10);
        rt_thread_startup(shine);//启动线程
    }else {//自动模式

        if(control_mode.light_mode == 0)//固定照射
        {
            shine = rt_thread_create("shine", shine_entry_auto, RT_NULL, 1024, 20, 10);
            rt_thread_startup(shine);//启动线程
        }else {//阶梯照射
            shine = rt_thread_create("shine", shine_entry_multistage, RT_NULL, 1024, 20, 10);
            rt_thread_startup(shine);//启动线程
        }
        //UpdataLcdDataU8(7,8,channel1.time);//通道1时间
        //UpdataLcdDataU8(7,12,channel1.time);//通道2时间
    }*/
}
