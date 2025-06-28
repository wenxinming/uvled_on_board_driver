/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-01-20     30802       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <channel.h>
#include <stdio.h>
#include <math.h>
#include <lcd_string.h>
#include <lcd.h>
#include <pwm.h>
rt_thread_t read_current;
rt_thread_t current_set;
float adc1,adc2,adc3,adc4;
rt_uint16_t current1,current2,current3,current4;
rt_uint8_t read_current_watch;
rt_uint16_t tempadc;
extern rt_uint16_t adc_temp[2000];
rt_int16_t currentset1,currentset2,currentset3,currentset4;
rt_int16_t c1_set,c2_set,c3_set,c4_set;
extern rt_uint16_t set_pwm1,set_pwm2,set_pwm3,set_pwm4;
float calculate_rms(rt_uint16_t *signal, rt_uint16_t length)
{
    float sum_of_squares = 0.0;
    for (int i = 0; i < length; i++)
    {
        sum_of_squares += signal[i] * signal[i];
    }
    return sqrt(sum_of_squares / length);
}
float calculate_mean(rt_uint16_t *signal, rt_uint16_t length)
{
    float sum_of_squares = 0.0;
    for (int i = 0; i < length; i++)
    {
        sum_of_squares += signal[i];
    }
    return (sum_of_squares / length);
}

void read_current_entry(void *parameter)//温度刷新
{
    //40毫欧 50倍放大
    rt_uint8_t turn[10];
    rt_uint16_t i=0;
    rt_uint16_t time;
    float tempadc;
    rt_memset(&turn, 0, 10);
    currentset1 = 0;
    time = 0;
    while(1)
    {
       // adc1 = adc_get_result_average(0, 1);    /* 获取ADC通道的转换值，10次取平均 */
        //adc2 = adc_get_result_average(1, 1);    /* 获取ADC通道的转换值，10次取平均 */
        //adc3 = adc_get_result_average(2, 1);    /* 获取ADC通道的转换值，10次取平均 */
       // adc4 = adc_get_result_average(3, 1);    /* 获取ADC通道的转换值，10次取平均 */
       // current1 = adc1*3.3/4096.0/50.0/0.04*1000.0;
       // current2 = adc2*3.3/4096.0/50.0/0.04*1000.0;
       // current3 = adc3*3.3/4096.0/50.0/0.04*1000.0;
       // current4 = adc4*3.3/4096.0/50.0/0.04*1000.0;
        adc_get_result2(0);
        adc1 = calculate_mean(adc_temp,2000);
        current1 = adc1*3.3/4096.0/50.0/0.04*1000.0;
        tempadc = 1.0;
        if(current1<1000)
        {
            tempadc = 0.99;
        }
        if(current1<600)
        {
            tempadc = 0.98;
        }
        current1 *= tempadc;

        adc_get_result2(1);
        adc2 = calculate_mean(adc_temp,2000);
        current2 = adc2*3.3/4096.0/50.0/0.04*1000.0;
        tempadc = 1.0;
        if(current2<1000)
        {
            tempadc = 0.99;
        }
        if(current2<600)
        {
            tempadc = 0.98;
        }
        current2 *= tempadc;


        adc_get_result2(4);
        adc3 = calculate_mean(adc_temp,2000);
        current3 = adc3*3.3/4096.0/50.0/0.04*1000.0;
        tempadc = 1.0;
        if(current3<1000)
        {
            tempadc = 0.99;
        }
        if(current3<600)
        {
            tempadc = 0.98;
        }
        current3 *= tempadc;

        adc_get_result2(5);
        adc4 = calculate_mean(adc_temp,2000);
        current4 = adc4*3.3/4096.0/50.0/0.04*1000.0;
        tempadc = 1.0;
        if(current4<1000)
        {
            tempadc = 0.99;
        }
        if(current4<600)
        {
            tempadc = 0.98;
        }
        current4 *= tempadc;
        if(time>=20)
        {
            time=0;
            UpdataLcdDataU8(show_current_c1,0,current1);//更新电流
            UpdataLcdDataU8(show_current_c2,0,current2);//更新电流
            UpdataLcdDataU8(show_current_c3,0,current3);//更新电流
            UpdataLcdDataU8(show_current_c4,0,current4);//更新电流
        }

        if((set_pwm1!=0)&&(channel1.status == 1)&&(c1_set<70)&&channel1.control_mode==0)
        {
            if(abs(current1-c1_set)>5)
            {
                if(current1<c1_set)
                {
                    set_ch1_output2(currentset1+set_pwm1);//设置PWM占空比
                    currentset1 += 5;
                }else {
                    set_ch1_output2(set_pwm1+currentset1);//设置PWM占空比
                    currentset1 -= 5;
                }
            }
        }else {
            currentset1 = 0;
        }

        if((set_pwm2!=0)&&(channel2.status == 1)&&(c2_set<70)&&channel2.control_mode==0)
        {
            if(abs(current2-c2_set)>5)
            {
                if(current2<c2_set)
                {
                    set_ch2_output2(currentset2+set_pwm2);//设置PWM占空比
                    currentset2 += 5;
                }else {
                    set_ch2_output2(set_pwm2+currentset2);//设置PWM占空比
                    currentset2 -= 5;
                }
            }
        }else {
            currentset2 = 0;
        }

        if((set_pwm3!=0)&&(channel3.status == 1)&&(c3_set<70)&&channel3.control_mode==0)
        {
            if(abs(current3-c3_set)>5)
            {
                if(current3<c3_set)
                {
                    set_ch3_output2(currentset3+set_pwm3);//设置PWM占空比
                    currentset3 += 5;
                }else {
                    set_ch3_output2(set_pwm3+currentset3);//设置PWM占空比
                    currentset3 -= 5;
                }
            }
        }else {
            currentset3 = 0;
        }

        if((set_pwm4!=0)&&(channel4.status == 1)&&(c4_set<70)&&channel4.control_mode==0)
        {
            if(abs(current4-c4_set)>5)
            {
                if(current4<c4_set)
                {
                    set_ch4_output2(currentset4+set_pwm4);//设置PWM占空比
                    currentset4 += 5;
                }else {
                    set_ch4_output2(set_pwm4+currentset4);//设置PWM占空比
                    currentset4 -= 5;
                }
            }
        }else {
            currentset4 = 0;
        }
        /*
        if(read_temp_watch)
        {
            ftoa(Temp1,turn);
            UpdataLcdString(3,2,turn);
            ftoa(Temp2,turn);
            UpdataLcdString(3,4,turn);
        }
        {
            //UpdataLcdDataFloat(7,3,Temp1);
            //UpdataLcdDataFloat(7,14,Temp2);
            ftoa(Temp1,turn);
            UpdataLcdString(7,3,turn);
            ftoa(Temp2,turn);
            UpdataLcdString(7,14,turn);
        }*/
        rt_thread_mdelay(50);
        time++;
    }
}

void current_init()
{
    read_current = rt_thread_create("read_current", read_current_entry, RT_NULL, 512, 20, 10);
    rt_thread_startup(read_current);//启动线程
}
