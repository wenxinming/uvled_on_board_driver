/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-17     30802       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <uart.h>
#include <channel.h>
#include <temperature.h>
#include <gpio.h>
#include <lcd_string.h>
rt_uint8_t now_channel;//��ǰ������ͨ��
rt_thread_t lcd_receive_tid;//lcd�����߳�
rt_mq_t lcd_command_mq,lcd_command_send_mq;//����lcd������Ϣ����
rt_thread_t lcdcommand,lcdsend;
rt_uint8_t on_shine_page;
//rt_err_t sl;
extern void shine_entry(void *parameter);//lcd����ִ��

// ��ղ�������
rt_uint32_t password_clear=891227;//��������
rt_uint32_t receive_password_clear;//��������ս�������
// �ָ�������������
//rt_uint32_t password_reset=891227;//��������
//rt_uint32_t receive_password_reset;//�������ָ�������������
// ��������(�ָ�������������, ��������)
rt_uint32_t password=123456;//��������
rt_uint32_t receive_password;//��������������

rt_uint8_t select_channel;
rt_uint8_t current_show;
typedef struct
{
    rt_uint8_t send_uart[24];
    rt_uint8_t len;
} lcd_uart_send;
lcd_uart_send lcdsendbuf;
rt_uint8_t sendbuffer[24];
int Myatoi(const char *str)         //�ַ���ת�������ֵĺ���
{
    int i = 1;
    int tmp = 0;

    //assert(str!=NULL);             //����str����Ϊ��ָ��
    while(*str=='-'||*str=='+'||*str==' ')      //����whileѭ����ȥ�������ַ�
    {
        if(*str=='-')
        {
            i = -i;
        }
        str++;
    }
    while(isdigit(*str))     //ʵ��һ����һ���ַ���ת���������������������ַ���ǰ����
    {
        tmp = tmp*10+(*str-'0');
        str++;
    }

    return tmp*i;
}
void lcd_com_thread_entry(void *parameter)
{
    uart2msg msg;
    rt_uint8_t i,j;
    rt_size_t rx_len;
    rt_uint8_t buf[30];
    while(1)
    {
        rt_memset(&msg, 0, sizeof(msg));
        if(rt_mq_recv(uart2_rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER)==RT_EOK)
        {
            rx_len = msg.len;
            if((rx_len>6)&&(msg.send_uart[0]==0x5A)&&(msg.send_uart[1]==0xA5))//���ȴ���10 ��������
            {
                //ee b1 01 02 03 04 05 06 ee b1 01 02 03 04 05 06 ee b1 01 02 03 04
                i=0;
                while(1)
                {
                    j = 0;
                    rt_memset(&buf, 0, sizeof(buf));
                    for(i=i;i<rx_len;i++)
                    {
                        buf[j] = msg.send_uart[i];
                        j++;
                        if((msg.send_uart[i+1]==0x5A)&&(msg.send_uart[i+2]==0xA5))
                        {
                            break;
                        }
                    }
                    i++;
                    //msg2.len = j;
                    rt_mq_send(lcd_command_mq, &buf, j);
                    if(i>=rx_len)
                    {
                        break;
                    }
                }
            }
        }
    }
}
void updatarunbutton(rt_uint8_t button_state,rt_uint16_t buttonID,rt_uint8_t page)//����������水ť
{
    lcd_uart_send buff;
    rt_memset(&buff, 0, sizeof(buff));
    buff.send_uart[0]=0x5a;
    buff.send_uart[1]=0xa5;
    buff.send_uart[2]=0x05;//���ݳ���
    buff.send_uart[3]=0x82;
    buff.send_uart[4]=buttonID>>8;
    buff.send_uart[5]=buttonID;
    buff.send_uart[6]=0;
    buff.send_uart[7]=button_state;
    buff.len = 8;
    //HAL_UART_Transmit(&huart2, send, len, HAL_MAX_DELAY);
    rt_mq_send(lcd_command_send_mq, &buff, sizeof(buff));

}
void UpdataLcdDataU8(rt_uint16_t page,rt_uint8_t channel,rt_uint16_t num)//
{
    //page������ַ
    lcd_uart_send buff;
    rt_uint8_t len,i;
    rt_memset(&buff, 0, sizeof(buff));
    buff.send_uart[0]=0x5a;
    buff.send_uart[1]=0xa5;
    buff.send_uart[2]=0x05;//���ݳ���
    buff.send_uart[3]=0x82;
    buff.send_uart[4]=page>>8;
    buff.send_uart[5]=page;
    buff.send_uart[6]=num>>8;
    buff.send_uart[7]=num;
    buff.len = 8;
    //HAL_UART_Transmit(&huart2, send, len, HAL_MAX_DELAY);
    rt_mq_send(lcd_command_send_mq, &buff, sizeof(buff));
}

void updata_control_mode()//���¿���ģʽ�˵�
{
    //chanel1
    if(channel1.control_mode==0)//�̶�ģʽ
    {
        updatarunbutton(0,ctrl_mode_page_c1_ctrlmode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c1_ctrlmode,0);//����ͨ��1����ģʽ
    }
    if(channel1.light_mode==0)//���Ʒ�ʽ
    {
        updatarunbutton(0,ctrl_mode_page_c1_lightmode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c1_lightmode,0);//����ͨ��1����ģʽ
    }
    if(channel1.Enable==0)//ͨ��ʹ��
    {
        updatarunbutton(0,ctrl_mode_page_c1_enable,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c1_enable,0);//����ͨ��1����ģʽ
    }
    //channel2
    if(channel2.control_mode==0)//�̶�ģʽ
    {
        updatarunbutton(0,ctrl_mode_page_c2_ctrlmode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c2_ctrlmode,0);//����ͨ��1����ģʽ
    }
    if(channel2.light_mode==0)//���Ʒ�ʽ
    {
        updatarunbutton(0,ctrl_mode_page_c2_lightmode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c2_lightmode,0);//����ͨ��1����ģʽ
    }
    if(channel2.Enable==0)//ͨ��ʹ��
    {
        updatarunbutton(0,ctrl_mode_page_c2_enable,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c2_enable,0);//����ͨ��1����ģʽ
    }
    //channel3
    if(channel3.control_mode==0)//�̶�ģʽ
    {
        updatarunbutton(0,ctrl_mode_page_c3_ctrlmode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c3_ctrlmode,0);//����ͨ��1����ģʽ
    }
    if(channel3.light_mode==0)//���Ʒ�ʽ
    {
        updatarunbutton(0,ctrl_mode_page_c3_lightmode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c3_lightmode,0);//����ͨ��1����ģʽ
    }
    if(channel3.Enable==0)//ͨ��ʹ��
    {
        updatarunbutton(0,ctrl_mode_page_c3_enable,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c3_enable,0);//����ͨ��1����ģʽ
    }
    //channel4
    if(channel4.control_mode==0)//�̶�ģʽ
    {
        updatarunbutton(0,ctrl_mode_page_c4_ctrlmode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c4_ctrlmode,0);//����ͨ��1����ģʽ
    }
    if(channel4.light_mode==0)//���Ʒ�ʽ
    {
        updatarunbutton(0,ctrl_mode_page_c4_lightmode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c4_lightmode,0);//����ͨ��1����ģʽ
    }
    if(channel4.Enable==0)//ͨ��ʹ��
    {
        updatarunbutton(0,ctrl_mode_page_c4_enable,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_c4_enable,0);//����ͨ��1����ģʽ
    }

    if(channel_sync_mode==0)//ͨ��ʹ��
    {
        updatarunbutton(0,ctrl_mode_page_sync_mode,0);//����ͨ��1����ģʽ
    }else {
        updatarunbutton(1,ctrl_mode_page_sync_mode,0);//����ͨ��1����ģʽ
    }
}
void UpdataShinec1Screen()//���¼�ؽ���
{
    UpdataLcdDataU8(monitor_page_power_c1,2,channel1.power);//���¹���
    UpdataLcdDataU8(monitor_page_current_c1,2,0);//���µ���
    if(channel1.control_mode==0)//�ֶ�
    {
        UpdataLcdDataU8(monitor_page_time_c1,2,0);//��������ʱ��
    }else {//�Զ�
        if(channel1.light_mode==0)//�̶�
        {
            UpdataLcdDataU8(monitor_page_time_c1,2,channel1.time);//��������ʱ��
        }else {//����
            UpdataLcdDataU8(monitor_page_time_c1,2,channel1.time);//��������ʱ��
        }
    }
}
void UpdataShinec2Screen()//���¼�ؽ���
{
    UpdataLcdDataU8(monitor_page_power_c2,2,channel2.power);//���¹���
    UpdataLcdDataU8(monitor_page_current_c2,2,0);//���µ���
    if(channel2.control_mode==0)//�ֶ�
    {
        UpdataLcdDataU8(monitor_page_time_c2,2,0);//��������ʱ��
    }else {//�Զ�
        if(channel2.light_mode==0)//�̶�
        {
            UpdataLcdDataU8(monitor_page_time_c2,2,channel2.time);//��������ʱ��
        }else {//����
            UpdataLcdDataU8(monitor_page_time_c2,2,channel2.time);//��������ʱ��
        }
    }
}
void UpdataShinec3Screen()//���¼�ؽ���
{
    UpdataLcdDataU8(monitor_page_power_c3,2,channel3.power);//���¹���
    UpdataLcdDataU8(monitor_page_current_c3,2,0);//���µ���
    if(channel3.control_mode==0)//�ֶ�
    {
        UpdataLcdDataU8(monitor_page_time_c3,2,0);//��������ʱ��
    }else {//�Զ�
        if(channel3.light_mode==0)//�̶�
        {
            UpdataLcdDataU8(monitor_page_time_c3,2,channel3.time);//��������ʱ��
        }else {//����
            UpdataLcdDataU8(monitor_page_time_c3,2,channel3.time);//��������ʱ��
        }
    }
}
void UpdataShinec4Screen()//���¼�ؽ���
{
    UpdataLcdDataU8(monitor_page_power_c4,2,channel4.power);//���¹���
    UpdataLcdDataU8(monitor_page_current_c4,2,0);//���µ���
    if(channel4.control_mode==0)//�ֶ�
    {
        UpdataLcdDataU8(monitor_page_time_c4,2,0);//��������ʱ��
    }else {//�Զ�
        if(channel4.light_mode==0)//�̶�
        {
            UpdataLcdDataU8(monitor_page_time_c4,2,channel4.time);//��������ʱ��
        }else {//����
            UpdataLcdDataU8(monitor_page_time_c4,2,channel4.time);//��������ʱ��
        }
    }
}
void UpdataShineScreen()//�������䷽ʽ����
{
    UpdataLcdDataU8(2,1,channel1.power);//ͨ��1����
    UpdataLcdDataU8(2,3,channel2.power);//ͨ��2����
    UpdataLcdDataU8(2,2,channel1.time);//ͨ��1ʱ��
    UpdataLcdDataU8(2,4,channel2.time);//ͨ��2ʱ��
    UpdataLcdDataU8(2,5,channel1.time_hour);//ͨ��1������ʱ��Сʱ
    UpdataLcdDataU8(2,7,channel2.time_hour);//ͨ��2������ʱ��Сʱ
    UpdataLcdDataU8(2,6,channel1.time_minutes);//ͨ��1������ʱ�����
    UpdataLcdDataU8(2,8,channel2.time_minutes);//ͨ��2������ʱ�����
    UpdataLcdDataU8(2,13,channel1.delay_time);//ͨ��1�ر���ʱ
    UpdataLcdDataU8(2,20,channel2.delay_time);//ͨ��2�ر���ʱ
}
void UpdataSetTemp()//���������¶�
{
    UpdataLcdDataU8(3,1,channel1.alarm_temperature);
    UpdataLcdDataU8(3,3,channel2.alarm_temperature);
}
void updata_parameter()//�������
{
    UpdataLcdDataU8(parameter_page_c1_power,2,channel1.power);//ͨ��1����
    UpdataLcdDataU8(parameter_page_c2_power,6,channel2.power);//ͨ��2����
    UpdataLcdDataU8(parameter_page_c3_power,6,channel3.power);//ͨ��2����
    UpdataLcdDataU8(parameter_page_c4_power,6,channel4.power);//ͨ��2����

    UpdataLcdDataU8(parameter_page_c1_time,2,channel1.time);//ͨ��1����
    UpdataLcdDataU8(parameter_page_c2_time,6,channel2.time);//ͨ��2����
    UpdataLcdDataU8(parameter_page_c3_time,6,channel3.time);//ͨ��2����
    UpdataLcdDataU8(parameter_page_c4_time,6,channel4.time);//ͨ��2����

    UpdataLcdDataU8(parameter_page_c1_delay,2,channel1.delay_time);//ͨ��1����
    UpdataLcdDataU8(parameter_page_c2_delay,6,channel2.delay_time);//ͨ��2����
    UpdataLcdDataU8(parameter_page_c3_delay,6,channel3.delay_time);//ͨ��2����
    UpdataLcdDataU8(parameter_page_c4_delay,6,channel4.delay_time);//ͨ��2����

    UpdataLcdDataU8(parameter_page_c1_current,2,channel1.alarm_temperature);//ͨ��1����
    UpdataLcdDataU8(parameter_page_c2_current,6,channel2.alarm_temperature);//ͨ��2����
    UpdataLcdDataU8(parameter_page_c3_current,6,channel3.alarm_temperature);//ͨ��2����
    UpdataLcdDataU8(parameter_page_c4_current,6,channel4.alarm_temperature);//ͨ��2����
}
void view_parameters()//�鿴��������
{
    UpdataLcdDataU8(show_parameter_c1_power,2,channel1.power);//ͨ��1����
    UpdataLcdDataU8(show_parameter_c1_time,2,channel1.time);//ͨ��1����ʱ��
    UpdataLcdDataU8(show_parameter_c1_current,2,channel1.alarm_temperature);//ͨ��1���õ���
    UpdataLcdDataU8(show_parameter_c1_alltime_h,2,channel1.time_hour);//ͨ��1���õ���
    UpdataLcdDataU8(show_parameter_c1_alltime_m,2,channel1.time_minutes);//ͨ��1���õ���

    UpdataLcdDataU8(show_parameter_c2_power,2,channel2.power);//ͨ��1����
    UpdataLcdDataU8(show_parameter_c2_time,2,channel2.time);//ͨ��1����ʱ��
    UpdataLcdDataU8(show_parameter_c2_current,2,channel2.alarm_temperature);//ͨ��1���õ���
    UpdataLcdDataU8(show_parameter_c2_alltime_h,2,channel2.time_hour);//ͨ��1���õ���
    UpdataLcdDataU8(show_parameter_c2_alltime_m,2,channel2.time_minutes);//ͨ��1���õ���

    UpdataLcdDataU8(show_parameter_c3_power,2,channel3.power);//ͨ��1����
    UpdataLcdDataU8(show_parameter_c3_time,2,channel3.time);//ͨ��1����ʱ��
    UpdataLcdDataU8(show_parameter_c3_current,2,channel3.alarm_temperature);//ͨ��1���õ���
    UpdataLcdDataU8(show_parameter_c3_alltime_h,2,channel3.time_hour);//ͨ��1���õ���
    UpdataLcdDataU8(show_parameter_c3_alltime_m,2,channel3.time_minutes);//ͨ��1���õ���

    UpdataLcdDataU8(show_parameter_c4_power,2,channel4.power);//ͨ��1����
    UpdataLcdDataU8(show_parameter_c4_time,2,channel4.time);//ͨ��1����ʱ��
    UpdataLcdDataU8(show_parameter_c4_current,2,channel4.alarm_temperature);//ͨ��1���õ���
    UpdataLcdDataU8(show_parameter_c4_alltime_h,2,channel4.time_hour);//ͨ��1���õ���
    UpdataLcdDataU8(show_parameter_c4_alltime_m,2,channel4.time_minutes);//ͨ��1���õ���
}
void updata_multistage_c1()//���¶������
{
    rt_uint8_t i;
    //ͨ��1
    for(i=0;i<20;i++)
    {
        UpdataLcdDataU8(c1_multistage_set1+i*4,0,channel1.multistage_power[i]);
    }
    for(i=0;i<20;i++)
    {
        UpdataLcdDataU8(c1_multistage_time_set1+i*4,0,channel1.multistage_time[i]);
    }
    UpdataLcdDataU8(c1_multistage_cycle_set,0x20,channel1.cycle);
}
void updata_multistage_c2()//���¶������
{
    rt_uint8_t i;
    //ͨ��1
    for(i=0;i<20;i++)
    {
        UpdataLcdDataU8(c2_multistage_set1+i*4,0,channel2.multistage_power[i]);
    }
    for(i=0;i<20;i++)
    {
        UpdataLcdDataU8(c2_multistage_time_set1+i*4,0,channel2.multistage_time[i]);
    }
    UpdataLcdDataU8(c2_multistage_cycle_set,0x20,channel2.cycle);
}
void updata_multistage_c3()//���¶������
{
    rt_uint8_t i;
    //ͨ��1
    for(i=0;i<20;i++)
    {
        UpdataLcdDataU8(c3_multistage_set1+i*4,0,channel3.multistage_power[i]);
    }
    for(i=0;i<20;i++)
    {
        UpdataLcdDataU8(c3_multistage_time_set1+i*4,0,channel3.multistage_time[i]);
    }
    UpdataLcdDataU8(c3_multistage_cycle_set,0x20,channel3.cycle);
}
void updata_multistage_c4()//���¶������
{
    rt_uint8_t i;
    //ͨ��1
    for(i=0;i<20;i++)
    {
        UpdataLcdDataU8(c4_multistage_set1+i*4,0,channel4.multistage_power[i]);
    }
    for(i=0;i<20;i++)
    {
        UpdataLcdDataU8(c4_multistage_time_set1+i*4,0,channel4.multistage_time[i]);
    }
    UpdataLcdDataU8(c4_multistage_cycle_set,0x20,channel4.cycle);
}
void UpdataSetCurrent()//���µ�������
{
    UpdataLcdDataU8(parameter_current_c1,0x20,channel1.set_current);
    UpdataLcdDataU8(parameter_current_c2,0x20,channel2.set_current);
    UpdataLcdDataU8(parameter_current_c3,0x20,channel3.set_current);
    UpdataLcdDataU8(parameter_current_c4,0x20,channel4.set_current);
}
void switch_show(rt_int8_t page)//�л�����
{
    lcd_uart_send buff;
    rt_uint8_t len;
    rt_memset(&buff, 0, sizeof(buff));
    buff.send_uart[0]=0x5A;
    buff.send_uart[1]=0xA5;
    buff.send_uart[2]=0x07;
    buff.send_uart[3]=0x82;
    buff.send_uart[4]=0X00;
    buff.send_uart[5]=0x84;
    buff.send_uart[6]=0x5A;
    buff.send_uart[7]=0x01;
    buff.send_uart[8]=0x00;
    buff.send_uart[9]=page;
    buff.len = 10;
    //HAL_UART_Transmit(&huart2, send, len, HAL_MAX_DELAY);
    rt_mq_send(lcd_command_send_mq, &buff, sizeof(buff));
}
void UpdataLcdString(rt_uint16_t page,rt_uint8_t channel,rt_uint8_t s[])//����Һ�����ַ���
{
    //page������ַ
    lcd_uart_send buff;
    rt_uint8_t len,i;
    rt_memset(&buff, 0, sizeof(buff));
    buff.send_uart[0]=0x5a;
    buff.send_uart[1]=0xa5;
    buff.send_uart[2]=0x00;//���ݳ���
    buff.send_uart[3]=0x82;
    buff.send_uart[4]=page>>8;
    buff.send_uart[5]=page;
    len=strlen(s);
    for(i=0;i<len;i++)
    {
        buff.send_uart[6+i]=s[i];
    }
    buff.len = 6+i;
    buff.send_uart[2]=3+len;//���ݳ���
    //HAL_UART_Transmit(&huart2, send, len, HAL_MAX_DELAY);
    rt_mq_send(lcd_command_send_mq, &buff, sizeof(buff));
}

void lcd_command_entry(void *parameter)//lcd����ִ��
{
    rt_uint8_t temp[10];
    rt_uint8_t i;
    rt_uint8_t buff[64];
    rt_uint8_t last_state1,last_state2;
    i=0;
    last_state1 = Read_LedOn3;
    last_state2 = Read_LedOn4;
    select_channel = 0;
    current_show = 0;
    while(1)
    {
        rt_memset(&buff, 0, sizeof(buff));
        if(rt_mq_recv(lcd_command_mq, &buff, sizeof(buff), 200)==RT_EOK)
        {
            //rt_device_write(RS485_com, 0, &msg.send_uart, msg.len);
            switch(buff[4]*256+buff[5])
            {
                case 0x1000://����
                    switch(buff[8])//��ֵ
                    {
                        case 1://ͨ��1����
                            if(channel1.Enable)//ͨ��1ʹ��
                            {
                                if(channel1.status)
                                {
                                    channel1.status = 0;
                                    led1_button_off_strup;
                                }else {
                                    channel1.status = 1;
                                }
                            }
                        break;
                        case 2://ͨ��2����
                            if(channel2.Enable)//ͨ��1ʹ��
                            {
                                if(channel2.status)
                                {
                                    channel2.status = 0;
                                    led2_button_off_strup;
                                }else {
                                    channel2.status = 1;
                                }
                            }
                        break;
                        case 3://ͨ��3����
                            if(channel3.Enable)//ͨ��1ʹ��
                            {
                                if(channel3.status)
                                {
                                    channel3.status = 0;
                                    led3_button_off_strup;
                                }else {
                                    channel3.status = 1;
                                }
                            }
                        break;
                        case 4://ͨ��4����
                            if(channel4.Enable)//ͨ��1ʹ��
                            {
                                if(channel4.status)
                                {
                                    channel4.status = 0;
                                    led4_button_off_strup;
                                }else {
                                    channel4.status = 1;
                                }
                            }
                        break;
                        case 6://����ģʽ����
                            updata_control_mode();
                        break;
                        case 7://��������
                            updata_parameter();//�������
                        break;
                        case 8://�ָ�����
                            UpdataLcdString(reset_password_state, 1, "Input password:  ");
                            select_channel = 6;
                        break;
                        case 9://�鿴����
                            view_parameters();//�鿴��������
                        break;
                        case 0x0A://���ص���ؽ���
                            UpdataShinec1Screen();
                            UpdataShinec2Screen();
                            UpdataShinec3Screen();
                            UpdataShinec4Screen();
                        break;
                        case 0x0b://c1����ģʽ��ť
                            if(channel1.control_mode==0)
                            {
                                channel1.control_mode = 1;
                            }else {
                                channel1.control_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x0c://c1���Ʒ�ʽ��ť
                            if(channel1.light_mode==0)
                            {
                                channel1.light_mode = 1;
                            }else {
                                channel1.light_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x0d://c1ʹ�ܰ�ť
                            if(channel1.Enable==0)
                            {
                                channel1.Enable = 1;
                            }else {
                                channel1.Enable = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x0e://c2����ģʽ��ť
                            if(channel2.control_mode==0)
                            {
                                channel2.control_mode = 1;
                            }else {
                                channel2.control_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x0f://c2���Ʒ�ʽ��ť
                            if(channel2.light_mode==0)
                            {
                                channel2.light_mode = 1;
                            }else {
                                channel2.light_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x10://c2ʹ�ܰ�ť
                            if(channel2.Enable==0)
                            {
                                channel2.Enable = 1;
                            }else {
                                channel2.Enable = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x11://c3����ģʽ��ť
                            if(channel3.control_mode==0)
                            {
                                channel3.control_mode = 1;
                            }else {
                                channel3.control_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x12://c3���Ʒ�ʽ��ť
                            if(channel3.light_mode==0)
                            {
                                channel3.light_mode = 1;
                            }else {
                                channel3.light_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x13://c3ʹ�ܰ�ť
                            if(channel3.Enable==0)
                            {
                                channel3.Enable = 1;
                            }else {
                                channel3.Enable = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x14://c4����ģʽ��ť
                            if(channel4.control_mode==0)
                            {
                                channel4.control_mode = 1;
                            }else {
                                channel4.control_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x15://c4���Ʒ�ʽ��ť
                            if(channel4.light_mode==0)
                            {
                                channel4.light_mode = 1;
                            }else {
                                channel4.light_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x16://c4ʹ�ܰ�ť
                            if(channel4.Enable==0)
                            {
                                channel4.Enable = 1;
                            }else {
                                channel4.Enable = 0;
                            }
                            updata_control_mode();
                        break;
                        case 0x17://����ģʽ����
                            save();
                        break;
                        case 0x18://c1��������
                            updata_multistage_c1();//���¶������
                        break;
                        case 0x19://c2��������
                            updata_multistage_c2();//���¶������
                        break;
                        case 0x1A://c3��������
                            updata_multistage_c3();//���¶������
                        break;
                        case 0x1B://c4��������
                            updata_multistage_c4();//���¶������
                        break;
                        case 0x1C://�������÷���
                            save();
                        break;
                        case c1_multistage_return_key://c1������÷���
                        case c2_multistage_return_key://c2������÷���
                        case c3_multistage_return_key://c3������÷���
                        case c4_multistage_return_key://c4������÷���
                            save();
                        break;
                        case c1_clear_alltime_key:
                            select_channel = 1;
                            UpdataLcdString(clear_alltime_password_state, 1, "Input password:  ");
                        break;
                        case c2_clear_alltime_key:
                            select_channel = 2;
                            UpdataLcdString(clear_alltime_password_state, 1, "Input password:  ");
                        break;
                        case c3_clear_alltime_key:
                            select_channel = 3;
                            UpdataLcdString(clear_alltime_password_state, 1, "Input password:  ");
                        break;
                        case c4_clear_alltime_key:
                            select_channel = 4;
                            UpdataLcdString(clear_alltime_password_state, 1, "Input password:  ");
                        break;
                        case current_set_key:
                            select_channel = 5;
                            receive_password = 0; //�������
                            UpdataLcdString(reset_password_state, 1, "Input password:  ");
                        break;
                        case current_return_key:
                            current_show = 0;
                        break;
                        case clear_alltime_password_ok_key://��ռ�ʱ�������ȷ�ϰ���
                            if(password_clear==receive_password_clear)//������ȷ
                            {
                                switch_show(23);
                                receive_password_clear = 0;//�������
                                switch (select_channel)
                                {
                                    case 1:
                                        channel1.time_hour = 0;
                                        channel1.time_minutes = 0;
                                        channel1.time_seconds = 0;
                                        save();
                                    break;
                                    case 2:
                                        channel2.time_hour = 0;
                                        channel2.time_minutes = 0;
                                        channel2.time_seconds = 0;
                                        save();
                                    break;
                                    case 3:
                                        channel3.time_hour = 0;
                                        channel3.time_minutes = 0;
                                        channel3.time_seconds = 0;
                                        save();
                                    break;
                                    case 4:
                                        channel4.time_hour = 0;
                                        channel4.time_minutes = 0;
                                        channel4.time_seconds = 0;
                                        save();
                                    break;
                                    case 5:

                                    break;
                                    default:
                                    break;
                                }
                                view_parameters();//�鿴��������
                            }else {
                                UpdataLcdString(clear_alltime_password_state, 1, "Password error  ");
                            }
                        break;
                        case clear_alltime_password_cancel_key:
                            receive_password_clear = 0;//�������
                            view_parameters();//�鿴��������
                        break;
                        case reset_password_ok_key://�ָ���������
                            if(password==receive_password)//������ȷ
                            {
                                if(select_channel==5)
                                {
                                    switch_show(24);
                                    current_show = 1;
                                    UpdataSetCurrent();
                                }else {
                                    switch_show(4);
                                }
                            }else {
                                UpdataLcdString(clear_alltime_password_state, 1, "Password error  ");
                            }
                        break;
                        case reset_password_cancel_key://�ָ����� ����
                            receive_password = 0;
                        break;
                        case reset_ok://ȷ�ϻָ�����
                            switch_show(7);
                            restore_default();//�ָ�����
                            rt_thread_mdelay(2000);
                            switch_show(20);
                        break;
                        case reset_cancel://ȡ���ָ�����
                            receive_password =0;
                        break;
                        case sync_mode_key:
                            if(channel_sync_mode == 0)
                            {
                                channel_sync_mode = 1;
                            }else {
                                channel_sync_mode = 0;
                            }
                            updata_control_mode();
                        break;
                        default:
                        break;
                    }
                break;

                case parameter_page_c1_power://C1���书������
                    channel1.power = buff[8];
                break;
                case parameter_page_c1_time://C1����ʱ������
                    channel1.time = buff[8];
                break;
                case parameter_page_c1_delay://C1��ʱ������
                    channel1.delay_time = buff[8];
                break;
                case parameter_page_c1_current://C1��������
                    //channel1.set_current = buff[7]*256+buff[8];
                    channel1.alarm_temperature = buff[8];
                break;

                case parameter_page_c2_power://C2���书������
                    channel2.power = buff[8];
                break;
                case parameter_page_c2_time://C2����ʱ������
                    channel2.time = buff[8];
                break;
                case parameter_page_c2_delay://C2��ʱ������
                    channel2.delay_time = buff[8];
                break;
                case parameter_page_c2_current://C2��������
                    //channel2.set_current = buff[7]*256+buff[8];
                    channel2.alarm_temperature = buff[8];
                break;
                case parameter_current_c1:
                    channel1.set_current = buff[7]*256+buff[8];
                    if (channel1.set_current>1400) {
                        channel1.set_current = 1400;
                    }
                break;
                case parameter_current_c2:
                    channel2.set_current = buff[7]*256+buff[8];
                    if (channel2.set_current>1400) {
                        channel2.set_current = 1400;
                    }
                break;
                case parameter_current_c3:
                    channel3.set_current = buff[7]*256+buff[8];
                    if (channel3.set_current>1400) {
                        channel3.set_current = 1400;
                    }
                break;
                case parameter_current_c4:
                    channel4.set_current = buff[7]*256+buff[8];
                    if (channel4.set_current>1400) {
                        channel4.set_current = 1400;
                    }
                break;
                case parameter_page_c3_power://C3���书������
                    channel3.power = buff[8];
                break;
                case parameter_page_c3_time://C3����ʱ������
                    channel3.time = buff[8];
                break;
                case parameter_page_c3_delay://C3��ʱ������
                    channel3.delay_time = buff[8];
                break;
                case parameter_page_c3_current://C3��������
                    //channel3.set_current = buff[7]*256+buff[8];
                    channel3.alarm_temperature = buff[8];
                break;

                case parameter_page_c4_power://C4���书������
                    channel4.power = buff[8];
                break;
                case parameter_page_c4_time://C4����ʱ������
                    channel4.time = buff[8];
                break;
                case parameter_page_c4_delay://C4��ʱ������
                    channel4.delay_time = buff[8];
                break;
                case parameter_page_c4_current://C4�¶�����
                    //channel4.set_current = buff[7]*256+buff[8];
                    channel4.alarm_temperature = buff[8];
                break;
                //������� ͨ��1����
                case c1_multistage_set1:
                case c1_multistage_set2:
                case c1_multistage_set3:
                case c1_multistage_set4:
                case c1_multistage_set5:
                case c1_multistage_set6:
                case c1_multistage_set7:
                case c1_multistage_set8:
                case c1_multistage_set9:
                case c1_multistage_set10:
                case c1_multistage_set11:
                case c1_multistage_set12:
                case c1_multistage_set13:
                case c1_multistage_set14:
                case c1_multistage_set15:
                case c1_multistage_set16:
                case c1_multistage_set17:
                case c1_multistage_set18:
                case c1_multistage_set19:
                case c1_multistage_set20:
                    channel1.multistage_power[(buff[4]*256+buff[5]-c1_multistage_set1)/4] = buff[8];
                break;
                //������� ͨ��1ʱ��
                case c1_multistage_time_set1:
                case c1_multistage_time_set2:
                case c1_multistage_time_set3:
                case c1_multistage_time_set4:
                case c1_multistage_time_set5:
                case c1_multistage_time_set6:
                case c1_multistage_time_set7:
                case c1_multistage_time_set8:
                case c1_multistage_time_set9:
                case c1_multistage_time_set10:
                case c1_multistage_time_set11:
                case c1_multistage_time_set12:
                case c1_multistage_time_set13:
                case c1_multistage_time_set14:
                case c1_multistage_time_set15:
                case c1_multistage_time_set16:
                case c1_multistage_time_set17:
                case c1_multistage_time_set18:
                case c1_multistage_time_set19:
                case c1_multistage_time_set20:
                    channel1.multistage_time[(buff[4]*256+buff[5]-c1_multistage_time_set1)/4] = buff[7]*256+buff[8];
                break;
                case c1_multistage_cycle_set://ѭ������
                    channel1.cycle = buff[7]*256+buff[8];
                break;
                //������� ͨ��2����
                case c2_multistage_set1:
                case c2_multistage_set2:
                case c2_multistage_set3:
                case c2_multistage_set4:
                case c2_multistage_set5:
                case c2_multistage_set6:
                case c2_multistage_set7:
                case c2_multistage_set8:
                case c2_multistage_set9:
                case c2_multistage_set10:
                case c2_multistage_set11:
                case c2_multistage_set12:
                case c2_multistage_set13:
                case c2_multistage_set14:
                case c2_multistage_set15:
                case c2_multistage_set16:
                case c2_multistage_set17:
                case c2_multistage_set18:
                case c2_multistage_set19:
                case c2_multistage_set20:
                    channel2.multistage_power[(buff[4]*256+buff[5]-c2_multistage_set1)/4] = buff[8];
                break;
                //������� ͨ��2ʱ��
                case c2_multistage_time_set1:
                case c2_multistage_time_set2:
                case c2_multistage_time_set3:
                case c2_multistage_time_set4:
                case c2_multistage_time_set5:
                case c2_multistage_time_set6:
                case c2_multistage_time_set7:
                case c2_multistage_time_set8:
                case c2_multistage_time_set9:
                case c2_multistage_time_set10:
                case c2_multistage_time_set11:
                case c2_multistage_time_set12:
                case c2_multistage_time_set13:
                case c2_multistage_time_set14:
                case c2_multistage_time_set15:
                case c2_multistage_time_set16:
                case c2_multistage_time_set17:
                case c2_multistage_time_set18:
                case c2_multistage_time_set19:
                case c2_multistage_time_set20:
                    channel2.multistage_time[(buff[4]*256+buff[5]-c2_multistage_time_set1)/4] = buff[7]*256+buff[8];
                break;
                case c2_multistage_cycle_set://ѭ������
                    channel2.cycle = buff[7]*256+buff[8];
                break;
                //������� ͨ��3����
                case c3_multistage_set1:
                case c3_multistage_set2:
                case c3_multistage_set3:
                case c3_multistage_set4:
                case c3_multistage_set5:
                case c3_multistage_set6:
                case c3_multistage_set7:
                case c3_multistage_set8:
                case c3_multistage_set9:
                case c3_multistage_set10:
                case c3_multistage_set11:
                case c3_multistage_set12:
                case c3_multistage_set13:
                case c3_multistage_set14:
                case c3_multistage_set15:
                case c3_multistage_set16:
                case c3_multistage_set17:
                case c3_multistage_set18:
                case c3_multistage_set19:
                case c3_multistage_set20:
                    channel3.multistage_power[(buff[4]*256+buff[5]-c3_multistage_set1)/4] = buff[8];
                break;
                //������� ͨ��3ʱ��
                case c3_multistage_time_set1:
                case c3_multistage_time_set2:
                case c3_multistage_time_set3:
                case c3_multistage_time_set4:
                case c3_multistage_time_set5:
                case c3_multistage_time_set6:
                case c3_multistage_time_set7:
                case c3_multistage_time_set8:
                case c3_multistage_time_set9:
                case c3_multistage_time_set10:
                case c3_multistage_time_set11:
                case c3_multistage_time_set12:
                case c3_multistage_time_set13:
                case c3_multistage_time_set14:
                case c3_multistage_time_set15:
                case c3_multistage_time_set16:
                case c3_multistage_time_set17:
                case c3_multistage_time_set18:
                case c3_multistage_time_set19:
                case c3_multistage_time_set20:
                    channel3.multistage_time[(buff[4]*256+buff[5]-c3_multistage_time_set1)/4] = buff[7]*256+buff[8];
                break;
                case c3_multistage_cycle_set://ѭ������
                    channel3.cycle = buff[7]*256+buff[8];
                break;
                //������� ͨ��4����
                case c4_multistage_set1:
                case c4_multistage_set2:
                case c4_multistage_set3:
                case c4_multistage_set4:
                case c4_multistage_set5:
                case c4_multistage_set6:
                case c4_multistage_set7:
                case c4_multistage_set8:
                case c4_multistage_set9:
                case c4_multistage_set10:
                case c4_multistage_set11:
                case c4_multistage_set12:
                case c4_multistage_set13:
                case c4_multistage_set14:
                case c4_multistage_set15:
                case c4_multistage_set16:
                case c4_multistage_set17:
                case c4_multistage_set18:
                case c4_multistage_set19:
                case c4_multistage_set20:
                    channel4.multistage_power[(buff[4]*256+buff[5]-c4_multistage_set1)/4] = buff[8];
                break;
                //������� ͨ��1ʱ��
                case c4_multistage_time_set1:
                case c4_multistage_time_set2:
                case c4_multistage_time_set3:
                case c4_multistage_time_set4:
                case c4_multistage_time_set5:
                case c4_multistage_time_set6:
                case c4_multistage_time_set7:
                case c4_multistage_time_set8:
                case c4_multistage_time_set9:
                case c4_multistage_time_set10:
                case c4_multistage_time_set11:
                case c4_multistage_time_set12:
                case c4_multistage_time_set13:
                case c4_multistage_time_set14:
                case c4_multistage_time_set15:
                case c4_multistage_time_set16:
                case c4_multistage_time_set17:
                case c4_multistage_time_set18:
                case c4_multistage_time_set19:
                case c4_multistage_time_set20:
                    channel4.multistage_time[(buff[4]*256+buff[5]-c4_multistage_time_set1)/4] = buff[7]*256+buff[8];
                break;
                case c4_multistage_cycle_set://ѭ������
                    channel4.cycle = buff[7]*256+buff[8];
                break;
                case clear_alltime_password://�������ʱ������
                    receive_password_clear = (buff[7]<<24)+(buff[8]<<16)+(buff[9]<<8)+(buff[10]);
                    if(receive_password_clear == password_clear)//������ȷ
                    {
                        UpdataLcdString(clear_alltime_password_state, 1, "Password correct  ");
                    }else {
                        UpdataLcdString(clear_alltime_password_state, 1, "Password error    ");
                    }
                break;
                case reset_password://��������
                    receive_password = (buff[7]<<24)+(buff[8]<<16)+(buff[9]<<8)+(buff[10]);
                    if(receive_password == password)//������ȷ
                    {
                        UpdataLcdString(reset_password_state, 1, "Password correct  ");
                    }else {
                        UpdataLcdString(reset_password_state, 1, "Password error    ");
                    }
                break;
                case 5://�ָ�����
                    if(buff[6]==0x01)
                    {
                        switch_show(16);
                        channel1.alarm_temperature = 65;
                        channel2.alarm_temperature = 65;
                        channel1.power = 80;
                        channel2.power = 80;
                        channel1.time = 60;
                        channel2.time = 60;
                        control_mode.control_mode = 0;
                        //Channel2CtrlMode=0;
                        save();//��������
                        rt_thread_mdelay(1000);
                        switch_show(17);
                    }
                break;
                case 0x1d://��ת�ָ���������
                    if(buff[6]==0x01)
                    {
                        switch_show(4);
                    }
                break;
                case 0x06://���ͨ��1����ʱ��
                    if(buff[6]==0x01)//������ȷ
                    {
                        if(now_channel==0)//ͨ��1
                        {
                            channel1.time_hour = 0;
                            channel1.time_minutes = 0;
                            save();
                        }else {//ͨ��2
                            channel2.time_hour = 0;
                            channel2.time_minutes = 0;
                            save();
                        }
                    }
                break;
                case 7://�������
                    if(buff[6]==0x01)//ͨ��1����
                    {
                        if(buff[9]==0x01)//ͨ��1����
                        {
                            channel1.status = 1;//ͨ��1����
                            updatarunbutton(1,1,7);
                            /*
                            strat_led1 = 1;
                            updatarunbutton(1,1);
                            if(English)
                            {
                                UpdataLcdString(7,17,"Open the light");
                            }else
                                    {
                                        UpdataLcdString(7,17,"��������");
                                    }
                            rt_pwm_set(pwm_dev_c1, PWM_DEV_C1, PWM_Period, 0);//2000HZ
                            rt_pwm_enable(pwm_dev_c1, PWM_DEV_C1);*/
                        }else
                            {

                                //strat_led1 = 0;
                                updatarunbutton(0,1,7);
                                channel1.status = 0;//ͨ��1����
                            }
                    }
                    if(buff[6]==0x15)//����
                    {
                        read_temp_running = 0;
                        updatarunbutton(0,1,7);
                        updatarunbutton(1,0x10,7);
                    }


                    if(buff[6]==0x10)//ͨ��2����
                    {
                        if(buff[9]==0x01)//ͨ��1����
                        {
                            channel2.status = 1;//ͨ��1����
                            updatarunbutton(1,0x10,7);
                        }else
                            {
                                channel2.status = 0;//ͨ��1����
                                updatarunbutton(0,0x10,7);
                            }
                    }
                break;
                case 0x1f://ͨ��1�������
                    i = 0;
                    if(buff[6]!=0x2b)
                    {
                        for(i=0;i<10;i++)//
                        {
                            if(buff[8+i]==0x00)
                            {
                                break;
                            }
                            temp[i]=buff[8+i];
                        }
                    }
                    temp[i]='\0';
                    switch(buff[6])
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                            channel1.multistage_power[buff[6]-1]=Myatoi(temp);
                            save();
                        break;
                        case 11:
                        case 12:
                        case 13:
                        case 14:
                        case 15:
                        case 16:
                        case 17:
                        case 18:
                        case 19:
                        case 20:
                            channel1.multistage_time[buff[6]-11]=Myatoi(temp);
                            save();
                        break;
                        case 0x20:
                            control_mode.cycle=Myatoi(temp);
                            save();
                        break;
                        case 0x30:
                        case 0x31:
                        case 0x32:
                        case 0x33:
                        case 0x34:
                        case 0x35:
                        case 0x36:
                        case 0x37:
                        case 0x38:
                        case 0x39:
                            channel2.multistage_power[buff[6]-0x30]=Myatoi(temp);
                            save();
                        break;
                        case 0x3A:
                        case 0x3B:
                        case 0x3C:
                        case 0x3D:
                        case 0x3E:
                        case 0x3F:
                        case 0x40:
                        case 0x41:
                        case 0x42:
                        case 0x43:
                            channel2.multistage_time[buff[6]-0x3A]=Myatoi(temp);
                            save();
                        break;
                        default:
                        break;
                    }
                break;
                default:
                break;
            }
        }
    }
}
void lcd_command_send_entry(void *parameter)//lcd����ִ��
{
    while(1)
    {
        rt_memset(&lcdsendbuf, 0, sizeof(lcdsendbuf));
        if(rt_mq_recv(lcd_command_send_mq, &lcdsendbuf, sizeof(lcdsendbuf), RT_WAITING_FOREVER)==RT_EOK)
        {

            memcpy(sendbuffer,lcdsendbuf.send_uart,24);
            HAL_UART_Transmit(&huart2, lcdsendbuf.send_uart, lcdsendbuf.len, HAL_MAX_DELAY);
            //HAL_UART_Transmit_DMA(&huart2, &sendbuffer, lcdsendbuf.len);
        }
    }
}
void lcd_init()
{
    lcd_receive_tid = rt_thread_create("lcd_receive", lcd_com_thread_entry, RT_NULL, 1024, 15, 10);
    rt_thread_startup(lcd_receive_tid);//�����߳�
    lcd_command_mq = rt_mq_create("lcd_commandsend_mq", 30, 48, RT_IPC_FLAG_FIFO);
    lcd_command_send_mq = rt_mq_create("lcd_commandsend_mq", 32, 48, RT_IPC_FLAG_FIFO);
    lcdcommand = rt_thread_create("lcd_command", lcd_command_entry, RT_NULL, 1024, 20, 10);
    lcdsend = rt_thread_create("lcd_send", lcd_command_send_entry, RT_NULL, 1024, 20, 10);
    rt_thread_startup(lcdcommand);//�����߳�
    rt_thread_startup(lcdsend);//�����߳�
    MX_USART2_UART_Init();//LCD���ڳ�ʼ��
    switch_show(0);
}
