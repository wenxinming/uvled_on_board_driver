/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-24     30802       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <channel.h>
#include <lcd.h>
rt_uint16_t voltage;
//#define SDO_PIN GET_PIN(B, 13) //定义输出开关
#define SDO1 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14)
#define SDO1_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,1)
#define SDO1_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,0)

#define SDO HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)
#define SDO_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,1)
#define SDO_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,0)

//#define SCLK_PIN GET_PIN(B, 14) //定义输出开关
#define SCLK1_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,1)
#define SCLK1_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,0)

#define SCLK_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,1)
#define SCLK_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,0)
float Temp1,Temp2,Temp3,Temp4;//通道1温度 通道2温度
rt_uint8_t read_temp_running;//照射中温度更新
rt_uint8_t read_temp_watch;//温度设置中刷新
rt_thread_t read_temp;
extern void ftoa(float num,char * buff);
void SDOR(rt_uint8_t num)//num =0 12通道 =1 34通道
{
   //rt_pin_mode(SDO_PIN, PIN_MODE_INPUT);

    GPIO_InitTypeDef gpio_init_struct;
    if(num)
    {
        gpio_init_struct.Pin = GPIO_PIN_14;                   /* LED0òy?? */
    }else {
        gpio_init_struct.Pin = GPIO_PIN_8;
    }
    gpio_init_struct.Mode = GPIO_MODE_INPUT;            /* í?íìê?3? */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* é?à- */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    if(num)
    {
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);       /* 3?ê??ˉLED0òy?? */
    }else {
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);       /* 3?ê??ˉLED0òy?? */
    }
}

void SDOW(rt_uint8_t num)
{
    //rt_pin_mode(SDO_PIN, PIN_MODE_OUTPUT);
    GPIO_InitTypeDef gpio_init_struct;
    if(num)
    {
        gpio_init_struct.Pin = GPIO_PIN_14;                   /* LED0òy?? */
    }else {
        gpio_init_struct.Pin = GPIO_PIN_8;
    }
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    /* í?íìê?3? */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* é?à- */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    if(num)
    {
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);       /* 3?ê??ˉLED0òy?? */
    }else {
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);       /* 3?ê??ˉLED0òy?? */
    }
}
//配置CS1237芯片
void Con_CS1238(unsigned char ch,rt_uint8_t num)
{
    unsigned char i;
    unsigned char dat=0;
    unsigned char count_i=0;//溢出计时器

    if(ch == 0)              //选择通道A
        dat = 0x40;//0x40;// 0100 1000
    else if(ch == 1)         //选择通道B
        dat = 0x41;//0x41;// 0100 1000
    if(num==0)
    {
        SCLK_L;//SCK_0;//时钟拉低
    }else {
        SCLK1_L;//SCK_0;//时钟拉低
    }
    rt_thread_mdelay(10);
    SDOR(num);
    if(num==0)
    {
        while(SDO)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237全部拉低为0才算都准备好
        {
            rt_thread_mdelay(5);
            count_i++;
            if(count_i > 150)
            {
                SCLK_H;//SCK_1;
                SDOW(num);
                SDO_H;//DAT_1;
                return;//超时，则直接退出程序
            }
        }
    }else {
        while(SDO1)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237全部拉低为0才算都准备好
        {
            rt_thread_mdelay(5);
            count_i++;
            if(count_i > 150)
            {
                SCLK1_H;//SCK_1;
                SDOW(num);
                SDO1_H;//DAT_1;
                return;//超时，则直接退出程序
            }
        }
    }
    for(i=0;i<29;i++)// 1 - 26
    {
//      One_CLK;
        if(num==0)
        {
            SCLK_H;//SCK_1;
        }else {
            SCLK1_H;//SCK_1;
        }
        rt_hw_us_delay(5);
        if(num==0)
        {
            SCLK_L;//;SCK_0;
        }else {
            SCLK1_L;//;SCK_0;
        }
        rt_hw_us_delay(5);
    }
    SDOW(num);
    if(num==0)
    {
        SCLK_H;rt_hw_us_delay(5);SDO_H;SCLK_L;rt_hw_us_delay(5);//30
        SCLK_H;rt_hw_us_delay(5);SDO_H;SCLK_L;rt_hw_us_delay(5);//31
        SCLK_H;rt_hw_us_delay(5);SDO_L;SCLK_L;rt_hw_us_delay(5);//32
        SCLK_H;rt_hw_us_delay(5);SDO_L;SCLK_L;rt_hw_us_delay(5);//33
        SCLK_H;rt_hw_us_delay(5);SDO_H;SCLK_L;rt_hw_us_delay(5);//34
        SCLK_H;rt_hw_us_delay(5);SDO_L;SCLK_L;rt_hw_us_delay(5);//35
        SCLK_H;rt_hw_us_delay(5);SDO_H;SCLK_L;rt_hw_us_delay(5);//36
        SCLK_H;rt_hw_us_delay(5);SCLK_L;
    }else {
        SCLK1_H;rt_hw_us_delay(5);SDO1_H;SCLK1_L;rt_hw_us_delay(5);//30
        SCLK1_H;rt_hw_us_delay(5);SDO1_H;SCLK1_L;rt_hw_us_delay(5);//31
        SCLK1_H;rt_hw_us_delay(5);SDO1_L;SCLK1_L;rt_hw_us_delay(5);//32
        SCLK1_H;rt_hw_us_delay(5);SDO1_L;SCLK1_L;rt_hw_us_delay(5);//33
        SCLK1_H;rt_hw_us_delay(5);SDO1_H;SCLK1_L;rt_hw_us_delay(5);//34
        SCLK1_H;rt_hw_us_delay(5);SDO1_L;SCLK1_L;rt_hw_us_delay(5);//35
        SCLK1_H;rt_hw_us_delay(5);SDO1_H;SCLK1_L;rt_hw_us_delay(5);//36
        SCLK1_H;rt_hw_us_delay(5);SCLK1_L;
    }
    rt_hw_us_delay(5);//37     写入了0x65
    if(num==0)
    {
        for(i=0;i<8;i++)// 38 - 45个脉冲了，写8位数据
        {
            SCLK_H;
            if(dat&0x80)
                SDO_H;
            else
                SDO_L;
            dat <<= 1;
            SCLK_L;
        }
        SDOR(num);
        SCLK_H;rt_hw_us_delay(5);SCLK_L;rt_hw_us_delay(5);//46个脉冲拉高数据引脚
    }else {
        for(i=0;i<8;i++)// 38 - 45个脉冲了，写8位数据
        {
            SCLK1_H;
            if(dat&0x80)
                SDO1_H;
            else
                SDO1_L;
            dat <<= 1;
            SCLK1_L;
        }
        SDOR(num);
        SCLK1_H;rt_hw_us_delay(5);SCLK1_L;rt_hw_us_delay(5);//46个脉冲拉高数据引脚
    }
}

//读取ADC数据，返回的是一个有符号数据
unsigned long Read_CS1238(rt_uint8_t num)
{
    volatile unsigned char i;
    volatile unsigned long dat=0;//读取到的数据
    volatile unsigned char count_i=0;//溢出计时器
    if(num==0)
    {
        SDO_H;//端口锁存1，51必备
        SCLK_L;//时钟拉低
    }else {
        SDO1_H;//端口锁存1，51必备
        SCLK1_L;//时钟拉低
    }
    SDOR(num);
    rt_thread_mdelay(10);
    if(num==0)
    {
        while(SDO)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237拉低为0才算都准备好
        {
            rt_thread_mdelay(2);
            count_i++;
            if(count_i > 150)
            {
                SCLK_H;
                SDO_H;
                return 0;//超时，则直接退出程序
            }
        }
    }else {
        while(SDO1)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237拉低为0才算都准备好
        {
            rt_thread_mdelay(2);
            count_i++;
            if(count_i > 150)
            {
                SCLK1_H;
                SDO1_H;
                return 0;//超时，则直接退出程序
            }
        }
    }
    if(num==0)
    {
        SDO_H;//端口锁存1，51必备
    }else {
        SDO1_H;//端口锁存1，51必备
    }
    dat=0;
    SDOR(num);
    if(num==0)
    {
        for(i=0;i<24;i++)//获取24位有效转换
        {
            SCLK_H;
            rt_hw_us_delay(1);
            dat <<= 1;
            if(SDO)
                dat ++;
            SCLK_L;
            rt_hw_us_delay(1);
        }
    }else {
        for(i=0;i<24;i++)//获取24位有效转换
        {
            SCLK1_H;
            rt_hw_us_delay(1);
            dat <<= 1;
            if(SDO1)
                dat ++;
            SCLK1_L;
            rt_hw_us_delay(1);
        }
    }
    if(num==0)
    {
        for(i=0;i<3;i++)//一共输入27个脉冲
        {
            SCLK_H;rt_hw_us_delay(10);;SCLK_L;rt_hw_us_delay(10);;//46个脉冲拉高数据引脚
        }
        SDOW(num);
        SDO_H;
    }else {
        for(i=0;i<3;i++)//一共输入27个脉冲
        {
            SCLK1_H;rt_hw_us_delay(10);;SCLK1_L;rt_hw_us_delay(10);;//46个脉冲拉高数据引脚
        }
        SDOW(num);
        SDO1_H;
    }
    if((dat&0x800000) == 0x800000)  //最高位为1，表示输入为负值
    {
        dat = ~dat;
        dat&=0xFFFFFF;
    }
    else
    {

    }

    return dat;
}
float read_temperature(rt_uint8_t ch,rt_uint8_t num)
{
    float tempsensor;
    Con_CS1238(ch,num);
    rt_thread_mdelay(10);
    tempsensor=(float)Read_CS1238(num)/8388607.0*1.25/ 0.00042865f;
    if(tempsensor>100)
    {
        tempsensor=0;
    }
    return tempsensor;
}
void read_temp_entry(void *parameter)//温度刷新
{
    rt_uint8_t turn[10];
    rt_memset(&turn, 0, 10);
    while(1)
    {
        Temp1 = read_temperature(0,0);
        Temp2 = read_temperature(1,0);
        Temp3 = read_temperature(0,1);
        Temp4 = read_temperature(1,1);
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
        rt_thread_mdelay(1000);
    }
}
void temperature_init()
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOC_CLK_ENABLE();
    SDOR(0);
    SDOR(1);
    gpio_init_struct.Pin = GPIO_PIN_15;                   /* LED0òy?? */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* í?íìê?3? */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* é?à- */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);       /* 3?ê??ˉLED0òy?? */
    SCLK1_H;//时钟拉低
    gpio_init_struct.Pin = GPIO_PIN_9;                   /* LED0òy?? */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* í?íìê?3? */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* é?à- */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);       /* 3?ê??ˉLED0òy?? */
    SCLK_H;//时钟拉低
    read_temp = rt_thread_create("read_temp", read_temp_entry, RT_NULL, 512, 20, 10);
    rt_thread_startup(read_temp);//启动线程
}
