/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-11     30802       the first version
 */
#include "stm32f1xx_hal.h"
#include <rtthread.h>
#include <string.h>
#include <uart.h>
#include <channel.h>
#define DBG_TAG "uart"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
rt_thread_t rs485_tid;//lcd接收线程
extern UART_HandleTypeDef huart2,huart1;
rt_uint8_t address = 1;//定义本机
#define REG_HOLDING_NREGS 250 //寄存器个数
unsigned char M_CRC[2];//定义数组
rt_uint8_t usart3sendbuf[300];
rt_uint8_t usRegHoldingBuf[300];//定义寄存器
extern struct channel_control control_mode;
extern rt_mq_t uart2_rx_mq,uart1_rx_mq;
extern float Temp1,Temp2;
void crc16(unsigned char *ptr,unsigned int len)//计算通信校验位
{
    unsigned long wcrc=0XFFFF;//预置16位crc寄存器，初值全部为1
    unsigned char temp;//定义中间变量
    int i=0,j=0;//定义计数
    for(i=0;i<len;i++)//循环计算每个数据
    {
        temp=*ptr&0X00FF;//将八位数据与crc寄存器亦或
        ptr++;//指针地址增加，指向下个数据
        wcrc^=temp;//将数据存入crc寄存器
        for(j=0;j<8;j++)//循环计算数据的
        {
            if(wcrc&0X0001)//判断右移出的是不是1，如果是1则与多项式进行异或。
            {
                wcrc>>=1;//先将数据右移一位
                wcrc^=0XA001;//与上面的多项式进行异或
            }
            else//如果不是1，则直接移出
            {
                wcrc>>=1;//直接移出
            }
        }
    }
    temp=wcrc;//crc的值
    M_CRC[0]=wcrc;//crc的低八位
    M_CRC[1]=wcrc>>8;//crc的高八位
}
void updatec1_modbus_register()//更新通道1modbus寄存器
{
    rt_uint16_t baseaddr = 0;
    rt_uint8_t i;
    usRegHoldingBuf[1+baseaddr] = channel1.power;
    usRegHoldingBuf[2+baseaddr] = channel1.time;
    usRegHoldingBuf[3+baseaddr] = channel1.delay_time;
    usRegHoldingBuf[4+baseaddr] = channel1.alarm_temperature;
    usRegHoldingBuf[5+baseaddr] = channel1.time_hour;
    usRegHoldingBuf[6+baseaddr] = channel1.time_minutes;
    usRegHoldingBuf[7+baseaddr] = channel1.status;
    usRegHoldingBuf[8+baseaddr] = channel1.control_mode;
    usRegHoldingBuf[9+baseaddr] = channel1.light_mode;
    usRegHoldingBuf[10+baseaddr] = channel1.Enable;
    usRegHoldingBuf[11+baseaddr] = channel1.set_current;
    for(i=0;i<20;i++)
    {
        usRegHoldingBuf[12+baseaddr+i] = channel1.multistage_time[i];
    }
    for(i=0;i<20;i++)
    {
        //usRegHoldingBuf[32+baseaddr+i] = channel1.power[i];
    }
    usRegHoldingBuf[52+baseaddr] = channel1.cycle;
}
void updatec2_modbus_register()//更新通道2modbus寄存器
{
    rt_uint16_t baseaddr = 60;
    rt_uint8_t i;
    usRegHoldingBuf[1+baseaddr] = channel2.power;
    usRegHoldingBuf[2+baseaddr] = channel2.time;
    usRegHoldingBuf[3+baseaddr] = channel2.delay_time;
    usRegHoldingBuf[4+baseaddr] = channel2.alarm_temperature;
    usRegHoldingBuf[5+baseaddr] = channel2.time_hour;
    usRegHoldingBuf[6+baseaddr] = channel2.time_minutes;
    usRegHoldingBuf[7+baseaddr] = channel2.status;
    usRegHoldingBuf[8+baseaddr] = channel2.control_mode;
    usRegHoldingBuf[9+baseaddr] = channel2.light_mode;
    usRegHoldingBuf[10+baseaddr] = channel2.Enable;
    usRegHoldingBuf[11+baseaddr] = channel2.set_current;
    for(i=0;i<20;i++)
    {
        usRegHoldingBuf[12+baseaddr+i] = channel2.multistage_time[i];
    }
    for(i=0;i<20;i++)
    {
        //usRegHoldingBuf[32+baseaddr+i] = channel2.power[i];
    }
    usRegHoldingBuf[52+baseaddr] = channel2.cycle;
}
void updatec3_modbus_register()//更新通道2modbus寄存器
{
    rt_uint16_t baseaddr = 120;
    rt_uint8_t i;
    usRegHoldingBuf[1+baseaddr] = channel3.power;
    usRegHoldingBuf[2+baseaddr] = channel3.time;
    usRegHoldingBuf[3+baseaddr] = channel3.delay_time;
    usRegHoldingBuf[4+baseaddr] = channel3.alarm_temperature;
    usRegHoldingBuf[5+baseaddr] = channel3.time_hour;
    usRegHoldingBuf[6+baseaddr] = channel3.time_minutes;
    usRegHoldingBuf[7+baseaddr] = channel3.status;
    usRegHoldingBuf[8+baseaddr] = channel3.control_mode;
    usRegHoldingBuf[9+baseaddr] = channel3.light_mode;
    usRegHoldingBuf[10+baseaddr] = channel3.Enable;
    usRegHoldingBuf[11+baseaddr] = channel3.set_current;
    for(i=0;i<20;i++)
    {
        usRegHoldingBuf[12+baseaddr+i] = channel3.multistage_time[i];
    }
    for(i=0;i<20;i++)
    {
        //usRegHoldingBuf[32+baseaddr+i] = channel3.power[i];
    }
    usRegHoldingBuf[52+baseaddr] = channel3.cycle;
}
void updatec4_modbus_register()//更新通道2modbus寄存器
{
    rt_uint16_t baseaddr = 180;
    rt_uint8_t i;
    usRegHoldingBuf[1+baseaddr] = channel4.power;
    usRegHoldingBuf[2+baseaddr] = channel4.time;
    usRegHoldingBuf[3+baseaddr] = channel4.delay_time;
    usRegHoldingBuf[4+baseaddr] = channel4.alarm_temperature;
    usRegHoldingBuf[5+baseaddr] = channel4.time_hour;
    usRegHoldingBuf[6+baseaddr] = channel4.time_minutes;
    usRegHoldingBuf[7+baseaddr] = channel4.status;
    usRegHoldingBuf[8+baseaddr] = channel4.control_mode;
    usRegHoldingBuf[9+baseaddr] = channel4.light_mode;
    usRegHoldingBuf[10+baseaddr] = channel4.Enable;
    usRegHoldingBuf[11+baseaddr] = channel4.set_current;
    for(i=0;i<20;i++)
    {
        usRegHoldingBuf[12+baseaddr+i] = channel4.multistage_time[i];
    }
    for(i=0;i<20;i++)
    {
        //usRegHoldingBuf[32+baseaddr+i] = channel4.power[i];
    }
    usRegHoldingBuf[52+baseaddr] = channel4.cycle;
}
void modbus_set_ch1(rt_uint16_t in_address,rt_uint16_t num)//mosbus设置 通道1
{
    switch(in_address)
    {
        case 1:
            channel1.power = num;
        break;
        case 2:
            channel1.time = num;
        break;
        case 3:
            channel1.delay_time = num;
        break;
        case 4:
            channel1.alarm_temperature = num;
        break;
        case 5:
            channel1.time_hour = num;
        break;
        case 6:
            channel1.time_minutes = num;
        break;
        case 7:
            channel1.Enable = num;
        break;
        case 8:
            channel1.control_mode = num;
        break;
        case 9:
            channel1.light_mode = num;
        break;
        case 10:
            channel1.Enable = num;
        break;
        case 11:
            channel1.set_current = num;
        break;
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            channel1.multistage_time[in_address-12] = num;
        break;
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
            channel1.multistage_power[in_address-32] = num;
        break;
        case 52:
            channel1.cycle = num;
        break;
        default:
        break;
    }

}
void modbus_set_ch2(rt_uint16_t in_address,rt_uint16_t num)//mosbus设置 通道1
{
    in_address -= 60;
    switch(in_address)
    {
        case 1:
            channel2.power = num;
        break;
        case 2:
            channel2.time = num;
        break;
        case 3:
            channel2.delay_time = num;
        break;
        case 4:
            channel2.alarm_temperature = num;
        break;
        case 5:
            channel2.time_hour = num;
        break;
        case 6:
            channel2.time_minutes = num;
        break;
        case 7:
            channel2.Enable = num;
        break;
        case 8:
            channel2.control_mode = num;
        break;
        case 9:
            channel2.light_mode = num;
        break;
        case 10:
            channel2.Enable = num;
        break;
        case 11:
            channel2.set_current = num;
        break;
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            channel2.multistage_time[in_address-12] = num;
        break;
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
            channel2.multistage_power[in_address-32] = num;
        break;
        case 52:
            channel2.cycle = num;
        break;
        default:
        break;
    }

}
void modbus_set_ch3(rt_uint16_t in_address,rt_uint16_t num)//mosbus设置 通道1
{
    in_address -= 120;
    switch(in_address)
    {
        case 1:
            channel3.power = num;
        break;
        case 2:
            channel3.time = num;
        break;
        case 3:
            channel3.delay_time = num;
        break;
        case 4:
            channel3.alarm_temperature = num;
        break;
        case 5:
            channel3.time_hour = num;
        break;
        case 6:
            channel3.time_minutes = num;
        break;
        case 7:
            channel3.Enable = num;
        break;
        case 8:
            channel3.control_mode = num;
        break;
        case 9:
            channel3.light_mode = num;
        break;
        case 10:
            channel3.Enable = num;
        break;
        case 11:
            channel3.set_current = num;
        break;
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            channel3.multistage_time[in_address-12] = num;
        break;
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
            channel3.multistage_power[in_address-32] = num;
        break;
        case 52:
            channel3.cycle = num;
        break;
        default:
        break;
    }

}
void modbus_set_ch4(rt_uint16_t in_address,rt_uint16_t num)//mosbus设置 通道1
{
    in_address -= 180;
    switch(in_address)
    {
        case 1:
            channel4.power = num;
        break;
        case 2:
            channel4.time = num;
        break;
        case 3:
            channel4.delay_time = num;
        break;
        case 4:
            channel4.alarm_temperature = num;
        break;
        case 5:
            channel4.time_hour = num;
        break;
        case 6:
            channel4.time_minutes = num;
        break;
        case 7:
            channel4.Enable = num;
        break;
        case 8:
            channel4.control_mode = num;
        break;
        case 9:
            channel4.light_mode = num;
        break;
        case 10:
            channel4.Enable = num;
        break;
        case 11:
            channel4.set_current = num;
        break;
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            channel4.multistage_time[in_address-12] = num;
        break;
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
            channel4.multistage_power[in_address-32] = num;
        break;
        case 52:
            channel4.cycle = num;
        break;
        default:
        break;
    }

}
void receive_modbus(rt_uint16_t in_address,rt_uint16_t num)//接收modbus
{
    if(in_address==0)
    {
        address = usRegHoldingBuf[0];
    }
    if(in_address>0&&in_address<=60)//ch1
    {
        modbus_set_ch1(in_address, num);
    }else if (in_address>60&&in_address<=120) {
        modbus_set_ch2(in_address, num);
    }else if (in_address>120&&in_address<=180) {
        modbus_set_ch3(in_address, num);
    }else if (in_address>180) {
        modbus_set_ch4(in_address, num);
    }
}
//modbus寄存器1-60 channel1 61-120 channel2 121-180 channel3 181-240 channel4
void rs485_thread_entry(void *parameter)
{
    rt_uint8_t usart1_buffer[64];
    rt_uint8_t i;
    while(1)
    {
        if(rt_mq_recv(uart1_rx_mq, &usart1_buffer, sizeof(usart1_buffer), RT_WAITING_FOREVER)==RT_EOK)
        {
            if((usart1_buffer[0]==address||usart1_buffer[0]==0)&&(usart1_buffer[1]==0x03||usart1_buffer[1]==0x06))//比较地址码
            {
                crc16(usart1_buffer,6);
                if(usart1_buffer[6]==M_CRC[0]&&usart1_buffer[7]==M_CRC[1])//比较校验码
                {
                    if((usart1_buffer[2]*256+usart1_buffer[3])<REG_HOLDING_NREGS)//比较寄存器地址不能超过寄存器数量
                    {
                        if(usart1_buffer[1]==0x03)//读
                        {
                            updatec1_modbus_register();
                            updatec2_modbus_register();
                            updatec3_modbus_register();
                            updatec4_modbus_register();

                            //data
                            usart3sendbuf[0]=usart1_buffer[0];//填充发送字节
                            usart3sendbuf[1]=usart1_buffer[1];//填充
                            usart3sendbuf[2]=usart1_buffer[5]*2;//填充
                            for(i=0;i<usart1_buffer[5];i++)//按协议回复数据
                            {
                                usart3sendbuf[3+i*2]=usRegHoldingBuf[usart1_buffer[3]+i]>>8;
                                usart3sendbuf[3+i*2+1]=usRegHoldingBuf[usart1_buffer[3]+i];
                            }
                            crc16(usart3sendbuf,usart1_buffer[5]*2+3);//计算CRC
                            usart3sendbuf[usart1_buffer[5]*2+3+0]=M_CRC[0];//填充CRC
                            usart3sendbuf[usart1_buffer[5]*2+3+1]=M_CRC[1];//填充CRC
                            HAL_UART_Transmit(&huart1, usart3sendbuf, usart1_buffer[5]*2+3+2, HAL_MAX_DELAY);
                        }else if(usart1_buffer[1]==0x06)//写入
                                    {
                                        usRegHoldingBuf[usart1_buffer[2]*256+usart1_buffer[3]]=usart1_buffer[4]*256+usart1_buffer[5];//提取写入位置
                                        memcpy(usart3sendbuf,usart1_buffer,8);//数据回复拷贝
                                        HAL_UART_Transmit(&huart1, usart3sendbuf, usart1_buffer[5]*2+3+2, HAL_MAX_DELAY);
                                        receive_modbus(usart1_buffer[2]*256+usart1_buffer[3],usart1_buffer[4]*256+usart1_buffer[5]);
                                    }
                    }
                }
            }
        }else {
            rt_thread_mdelay(100);
        }
    }
}
void rs485_init()
{
    rs485_tid = rt_thread_create("rs485_receive", rs485_thread_entry, RT_NULL, 1024, 15, 10);
    rt_thread_startup(rs485_tid);//启动线程
    MX_USART1_UART_Init();//LCD串口初始化
}

