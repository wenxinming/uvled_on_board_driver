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
rt_thread_t read_current;
float adc1,adc2,adc3,adc4;
rt_uint16_t current1,current2,current3,current4;
rt_uint8_t read_current_watch;
rt_uint16_t tempadc;
extern rt_uint16_t adc_temp[2000];

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
    rt_memset(&turn, 0, 10);
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

        adc_get_result2(1);
        adc2 = calculate_mean(adc_temp,1000);
        current2 = adc2*3.3/4096.0/50.0/0.04*1000.0;

        adc_get_result2(4);
        adc3 = calculate_mean(adc_temp,1000);
        current3 = adc3*3.3/4096.0/50.0/0.04*1000.0;

        adc_get_result2(5);
        adc4 = calculate_mean(adc_temp,1000);
        current4 = adc4*3.3/4096.0/50.0/0.04*1000.0;
        /*
        if(read_temp_watch)
        {
            ftoa(Temp1,turn);
            UpdataLcdString(3,2,turn);
            ftoa(Temp2,turn);
            UpdataLcdString(3,4,turn);
        }
        if(read_temp_running)
        {
            //UpdataLcdDataFloat(7,3,Temp1);
            //UpdataLcdDataFloat(7,14,Temp2);
            ftoa(Temp1,turn);
            UpdataLcdString(7,3,turn);
            ftoa(Temp2,turn);
            UpdataLcdString(7,14,turn);
        }*/
       // rt_thread_mdelay(10);
    }
}

void current_init()
{
    read_current = rt_thread_create("read_current", read_current_entry, RT_NULL, 512, 20, 10);
    rt_thread_startup(read_current);//启动线程
}
