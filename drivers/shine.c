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

void channel1_manual()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    Alarm1_OFF;
    Light1_OFF;
    last_state1 = Read_LedOn1;
    Complete1_OFF;
    channel1.now_time_s = 0;
    led1_open_strup;
    led1_power_strup;
    set_ch1_output(channel1.power);//����PWMռ�ձ�
    ch1_output_on();//ʹ��PWM���
    Light1_ON;
    led1_button_on_strup;
    while(1)
    {
        UpdataLcdDataU8(monitor_page_current_c1,0,current1);
        if(Read_LedOn1 != last_state1)//״̬�仯
        {
            if(Read_LedOn1 == 0)//�͵�ƽ
            {
                if(channel1.status == 1)
                {
                    channel1.status = 0;
                }else {
                    channel1.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn1;
        }
        if(channel1.status == 1)//channel open
        {
            if(Temp1>=channel1.alarm_temperature)//�¶ȱ���
            {
                Alarm1_ON;
                channel1.status = 0;
            }
            if(channel1.now_time_s>=3)//������
            {
              if(Read_CheckLed1==1)//����
              {
                Alarm1_ON;
                alarm_status=1;
                channel1.status = 0;
                led1_checkled_alarm_strup;
              }
              if(current1<(set_pwm1/1000.0*1500-100))//�Ƚϵ���
              {
                  Alarm1_ON;
                  alarm_status=1;
                  channel1.status = 0;
                  led1_current_alarm_strup;
              }
            }
            channel1.now_time_s++;
            led1_now_time_strup;
        }else {
                if(alarm_status)//�����������
                {

                }else {
                    led1_close_strup;
                }
                //EN1_OFF;
                set_ch1_output(0);//����PWMռ�ձ�
                ch1_output_off();//ʹ��PWM���
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
void channel2_manual()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    Alarm2_OFF;
    Light2_OFF;
    last_state1 = Read_LedOn2;
    Complete2_OFF;
    channel2.now_time_s = 0;
    led2_open_strup;
    led2_power_strup;
    set_ch2_output(channel2.power);//����PWMռ�ձ�
    ch2_output_on();//ʹ��PWM���
    Light2_ON;
    led2_button_on_strup;
    while(1)
    {
        UpdataLcdDataU8(monitor_page_current_c2,0,current2);
        if(Read_LedOn2 != last_state1)//״̬�仯
        {
            if(Read_LedOn2 == 0)//�͵�ƽ
            {
                if(channel2.status == 1)
                {
                    channel2.status = 0;
                }else {
                    channel2.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn2;
        }
        if(channel2.status == 1)//channel open
        {
            if(Temp1>=channel2.alarm_temperature)//�¶ȱ���
            {
                Alarm2_ON;
                channel2.status = 0;
            }
            if(channel2.now_time_s>=3)//������
            {
              if(Read_CheckLed2==1)//����
              {
                Alarm2_ON;
                alarm_status=1;
                channel2.status = 0;
                led2_checkled_alarm_strup;
              }
              if(current2<(set_pwm2/1000.0*1500-100))//�Ƚϵ���
              {
                  Alarm2_ON;
                  alarm_status=1;
                  channel2.status = 0;
                  led2_current_alarm_strup;
              }
            }
            channel2.now_time_s++;
            led2_now_time_strup;
        }else {
                if(alarm_status)//�����������
                {

                }else {
                    led2_close_strup;
                }
                //EN1_OFF;
                set_ch2_output(0);//����PWMռ�ձ�
                ch2_output_off();//ʹ��PWM���
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
void channel3_manual()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    Alarm3_OFF;
    Light3_OFF;
    last_state1 = Read_LedOn3;
    Complete3_OFF;
    channel3.now_time_s = 0;
    led3_open_strup;
    led3_power_strup;
    set_ch3_output(channel3.power);//����PWMռ�ձ�
    ch3_output_on();//ʹ��PWM���
    Light3_ON;
    led3_button_on_strup;
    while(1)
    {
        UpdataLcdDataU8(monitor_page_current_c3,0,current3);
        if(Read_LedOn3 != last_state1)//״̬�仯
        {
            if(Read_LedOn3 == 0)//�͵�ƽ
            {
                if(channel3.status == 1)
                {
                    channel3.status = 0;
                }else {
                    channel3.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn3;
        }
        if(channel3.status == 1)//channel open
        {
            if(Temp1>=channel3.alarm_temperature)//�¶ȱ���
            {
                Alarm3_ON;
                channel3.status = 0;
            }
            if(channel3.now_time_s>=3)//������
            {
              if(Read_CheckLed3==1)//����
              {
                Alarm3_ON;
                alarm_status=1;
                channel3.status = 0;
                led3_checkled_alarm_strup;
              }
              if(current3<(set_pwm3/1000.0*1500-100))//�Ƚϵ���
              {
                  Alarm3_ON;
                  alarm_status=1;
                  channel3.status = 0;
                  led3_current_alarm_strup;
              }
            }
            channel3.now_time_s++;
            led3_now_time_strup;
        }else {
                if(alarm_status)//�����������
                {

                }else {
                    led3_close_strup;
                }
                //EN1_OFF;
                set_ch3_output(0);//����PWMռ�ձ�
                ch3_output_off();//ʹ��PWM���
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
void channel4_manual()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    Alarm4_OFF;
    Light4_OFF;
    last_state1 = Read_LedOn4;
    Complete4_OFF;
    channel4.now_time_s = 0;
    led4_open_strup;
    led4_power_strup;
    set_ch4_output(channel4.power);//����PWMռ�ձ�
    ch4_output_on();//ʹ��PWM���
    Light4_ON;
    led4_button_on_strup;
    while(1)
    {
        UpdataLcdDataU8(monitor_page_current_c4,0,current4);
        if(Read_LedOn4 != last_state1)//״̬�仯
        {
            if(Read_LedOn4 == 0)//�͵�ƽ
            {
                if(channel4.status == 1)
                {
                    channel4.status = 0;
                }else {
                    channel4.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn4;
        }
        if(channel4.status == 1)//channel open
        {
            if(Temp1>=channel4.alarm_temperature)//�¶ȱ���
            {
                Alarm4_ON;
                channel4.status = 0;
            }
            if(channel4.now_time_s>=3)//������
            {
              if(Read_CheckLed4==1)//����
              {
                Alarm4_ON;
                alarm_status=1;
                channel4.status = 0;
                led4_checkled_alarm_strup;
              }
              if(current4<(set_pwm4/1000.0*1500-100))//�Ƚϵ���
              {
                  Alarm4_ON;
                  alarm_status=1;
                  channel4.status = 0;
                  led4_current_alarm_strup;
              }
            }
            channel4.now_time_s++;
            led4_now_time_strup;
        }else {
                if(alarm_status)//�����������
                {

                }else {
                    led4_close_strup;
                }
                //EN1_OFF;
                set_ch4_output(0);//����PWMռ�ձ�
                ch4_output_off();//ʹ��PWM���
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
void channel1_auto()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    Alarm1_OFF;
    Light1_OFF;
    last_state1 = Read_LedOn1;
    Complete1_OFF;
    channel1.now_time_s = 0;
    led1_open_strup;
    led1_power_strup;
    set_ch1_output(channel1.power);//����PWMռ�ձ�
    ch1_output_on();//ʹ��PWM���
    Light1_ON;
    led1_button_on_strup;
    while(1)
    {
        if(Read_LedOn1 != last_state1)//״̬�仯
        {
            if(Read_LedOn1 == 0)//�͵�ƽ
            {
                if(channel1.status == 1)
                {
                    channel1.status = 0;
                }else {
                    channel1.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn1;
        }
        UpdataLcdDataU8(monitor_page_current_c1,0,current1);//���µ���
        if(channel1.status == 1)//ͨ��1����
        {
            /*
            if(Temp1>=channel1.alarm_temperature)//�¶ȱ���
            {
                //BEEP_ON;
                Alarm1_ON;
                channel1.status = 0;
            }*/
            if(channel1.now_time_s>=3)//������
           {
                if(Temp1>=channel1.alarm_temperature)//�¶ȱ���
                {
                    Alarm1_ON;
                    channel1.status = 0;
                }
                if(channel1.now_time_s>=3)//������
                {
                  if(Read_CheckLed1==1)//����
                  {
                    Alarm1_ON;
                    alarm_status=1;
                    channel1.status = 0;
                    led1_checkled_alarm_strup;
                  }
                  if(current1<(set_pwm1/1000.0*1500-100))//�Ƚϵ���
                  {
                      Alarm1_ON;
                      alarm_status=1;
                      channel1.status = 0;
                      led1_current_alarm_strup;
                  }
                }
           }
            channel1.now_time_s++;
            if(channel1.now_time_s>=channel1.time)
            {
                channel1.status = 0;
                Complete1_ON;
            }
            led1_remaining_time_strup;
        }else {
                if(alarm_status)//�����������
                {

                }else {
                    led1_close_strup;
                }
                //EN1_OFF;
                set_ch1_output(0);//����PWMռ�ձ�
                ch1_output_off();//ʹ��PWM���
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
    //rt_thread_delete(shine);
}
void channel2_auto()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    Alarm2_OFF;
    Light2_OFF;
    last_state1 = Read_LedOn2;
    Complete2_OFF;
    channel2.now_time_s = 0;
    led2_open_strup;
    led2_power_strup;
    set_ch2_output(channel2.power);//����PWMռ�ձ�
    ch2_output_on();//ʹ��PWM���
    Light2_ON;
    led2_button_on_strup;
    while(1)
    {
        if(Read_LedOn2 != last_state1)//״̬�仯
        {
            if(Read_LedOn2 == 0)//�͵�ƽ
            {
                if(channel2.status == 1)
                {
                    channel2.status = 0;
                }else {
                    channel2.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn2;
        }
        UpdataLcdDataU8(monitor_page_current_c2,0,current2);//���µ���
        if(channel2.status == 1)//ͨ��1����
        {
            /*
            if(Temp1>=channel1.alarm_temperature)//�¶ȱ���
            {
                //BEEP_ON;
                Alarm1_ON;
                channel1.status = 0;
            }*/
            if(channel2.now_time_s>=3)//������
           {
                if(Temp1>=channel2.alarm_temperature)//�¶ȱ���
                {
                    Alarm2_ON;
                    channel2.status = 0;
                }
                if(channel2.now_time_s>=3)//������
                {
                  if(Read_CheckLed2==1)//����
                  {
                    Alarm2_ON;
                    alarm_status=1;
                    channel2.status = 0;
                    led2_checkled_alarm_strup;
                  }
                  if(current2<(set_pwm2/1000.0*1500-100))//�Ƚϵ���
                  {
                      Alarm2_ON;
                      alarm_status=1;
                      channel2.status = 0;
                      led2_current_alarm_strup;
                  }
                }
           }
            channel2.now_time_s++;
            if(channel2.now_time_s>=channel2.time)
            {
                channel2.status = 0;
                Complete2_ON;
            }
            led2_remaining_time_strup;
        }else {
                if(alarm_status)//�����������
                {

                }else {
                    led2_close_strup;
                }
                //EN1_OFF;
                set_ch2_output(0);//����PWMռ�ձ�
                ch2_output_off();//ʹ��PWM���
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
    //rt_thread_delete(shine);
}
void channel3_auto()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    Alarm3_OFF;
    Light3_OFF;
    last_state1 = Read_LedOn3;
    Complete3_OFF;
    channel3.now_time_s = 0;
    led3_open_strup;
    led3_power_strup;
    set_ch3_output(channel3.power);//����PWMռ�ձ�
    ch3_output_on();//ʹ��PWM���
    Light3_ON;
    led3_button_on_strup;
    while(1)
    {
        if(Read_LedOn3 != last_state1)//״̬�仯
        {
            if(Read_LedOn3 == 0)//�͵�ƽ
            {
                if(channel3.status == 1)
                {
                    channel3.status = 0;
                }else {
                    channel3.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn3;
        }
        UpdataLcdDataU8(monitor_page_current_c3,0,current3);//���µ���
        if(channel3.status == 1)//ͨ��1����
        {
            /*
            if(Temp1>=channel1.alarm_temperature)//�¶ȱ���
            {
                //BEEP_ON;
                Alarm1_ON;
                channel1.status = 0;
            }*/
            if(channel3.now_time_s>=3)//������
           {
                if(Temp1>=channel3.alarm_temperature)//�¶ȱ���
                {
                    Alarm3_ON;
                    channel3.status = 0;
                }
                if(channel3.now_time_s>=3)//������
                {
                  if(Read_CheckLed3==1)//����
                  {
                    Alarm3_ON;
                    alarm_status=1;
                    channel3.status = 0;
                    led3_checkled_alarm_strup;
                  }
                  if(current3<(set_pwm3/1000.0*1500-100))//�Ƚϵ���
                  {
                      Alarm3_ON;
                      alarm_status=1;
                      channel3.status = 0;
                      led3_current_alarm_strup;
                  }
                }
           }
            channel3.now_time_s++;
            if(channel3.now_time_s>=channel3.time)
            {
                channel3.status = 0;
                Complete3_ON;
            }
            led3_remaining_time_strup;
        }else {
                if(alarm_status)//�����������
                {

                }else {
                    led3_close_strup;
                }
                //EN1_OFF;
                set_ch3_output(0);//����PWMռ�ձ�
                ch3_output_off();//ʹ��PWM���
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
    //rt_thread_delete(shine);
}
void channel4_auto()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    Alarm4_OFF;
    Light4_OFF;
    last_state1 = Read_LedOn4;
    Complete4_OFF;
    channel4.now_time_s = 0;
    led4_open_strup;
    led4_power_strup;
    set_ch4_output(channel4.power);//����PWMռ�ձ�
    ch4_output_on();//ʹ��PWM���
    Light4_ON;
    led4_button_on_strup;
    while(1)
    {
        if(Read_LedOn4 != last_state1)//״̬�仯
        {
            if(Read_LedOn4 == 0)//�͵�ƽ
            {
                if(channel4.status == 1)
                {
                    channel4.status = 0;
                }else {
                    channel4.status = 1;
                }
            }else {
                   }
            last_state1 = Read_LedOn4;
        }
        UpdataLcdDataU8(monitor_page_current_c4,0,current4);//���µ���
        if(channel4.status == 1)//ͨ��1����
        {
            /*
            if(Temp1>=channel1.alarm_temperature)//�¶ȱ���
            {
                //BEEP_ON;
                Alarm1_ON;
                channel1.status = 0;
            }*/
            if(channel4.now_time_s>=3)//������
           {
                if(Temp1>=channel4.alarm_temperature)//�¶ȱ���
                {
                    Alarm4_ON;
                    channel4.status = 0;
                }
                if(channel4.now_time_s>=3)//������
                {
                  if(Read_CheckLed4==1)//����
                  {
                    Alarm4_ON;
                    alarm_status=1;
                    channel4.status = 0;
                    led4_checkled_alarm_strup;
                  }
                  if(current4<(set_pwm4/1000.0*1500-100))//�Ƚϵ���
                  {
                      Alarm4_ON;
                      alarm_status=1;
                      channel4.status = 0;
                      led4_current_alarm_strup;
                  }
                }
           }
            channel4.now_time_s++;
            if(channel4.now_time_s>=channel4.time)
            {
                channel4.status = 0;
                Complete4_ON;
            }
            led4_remaining_time_strup;
        }else {
                if(alarm_status)//�����������
                {

                }else {
                    led4_close_strup;
                }
                //EN1_OFF;
                set_ch4_output(0);//����PWMռ�ձ�
                ch4_output_off();//ʹ��PWM���
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
    //rt_thread_delete(shine);
}
void channel1_multistage()
{
    rt_uint32_t all_time1;
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    rt_uint8_t cycle1,large_cycle1;
    Alarm1_OFF;
    Light1_OFF;
    last_state1 = Read_LedOn1;
    Complete1_OFF;
    channel1.now_time_s = 0;
    led1_open_strup;
    led1_power_strup;
    Light1_ON;
    led1_button_on_strup;
    UpdataLcdDataU8(monitor_page_power_c1,2,channel1.multistage_power[0]);//���¹���
    cycle1 = 0;
    large_cycle1 =0;
    while(1)
    {
      if(Read_LedOn1 != last_state1)//״̬�仯
      {
          if(Read_LedOn1 == 0)//�͵�ƽ
          {
              if(channel1.status == 1)
              {
                  channel1.status = 0;
              }else {
                  channel1.status = 1;
              }
          }else {
                 }
          last_state1 = Read_LedOn1;
      }
      UpdataLcdDataU8(monitor_page_current_c1,0,current1);//���µ���
      if(channel1.status == 1)//ͨ��1����
      {
         if(channel1.now_time_s>=3)//������
         {
              if(Temp1>=channel1.alarm_temperature)//�¶ȱ���
              {
                  Alarm1_ON;
                  channel1.status = 0;
              }
              if(channel1.now_time_s>=3)//������
              {
                if(Read_CheckLed1==1)//����
                {
                  Alarm1_ON;
                  alarm_status=1;
                  channel1.status = 0;
                  led1_checkled_alarm_strup;
                }
                if(current1<(set_pwm1/1000.0*1500-20))//�Ƚϵ���
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
             set_ch1_output(channel1.multistage_power[cycle1]);//����PWMռ�ձ�
             ch1_output_on();//ʹ��PWM���
             UpdataLcdDataU8(monitor_page_power_c1,2,channel1.multistage_power[cycle1]);//���¹���
          }
          UpdataLcdDataU8(led1_time_strup,8,channel1.multistage_time[cycle1]-channel1.now_time_s);//ͨ��1�Զ�ģʽʣ��ʱ��;
      }else {
              if(alarm_status)//�����������
              {

              }else {
                  led1_close_strup;
              }
              //EN1_OFF;
              set_ch1_output(0);//����PWMռ�ձ�
              ch1_output_off();//ʹ��PWM���
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
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    rt_uint8_t cycle1,large_cycle1;
    Alarm2_OFF;
    Light2_OFF;
    last_state1 = Read_LedOn2;
    Complete2_OFF;
    channel2.now_time_s = 0;
    led2_open_strup;
    led2_power_strup;
    Light2_ON;
    led2_button_on_strup;
    UpdataLcdDataU8(monitor_page_power_c2 ,2,channel2.multistage_power[0]);//���¹���
    cycle1 = 0;
    large_cycle1 =0;
    while(1)
    {
      if(Read_LedOn2 != last_state1)//״̬�仯
      {
          if(Read_LedOn2 == 0)//�͵�ƽ
          {
              if(channel2.status == 1)
              {
                  channel2.status = 0;
              }else {
                  channel2.status = 1;
              }
          }else {
                 }
          last_state1 = Read_LedOn2;
      }
      UpdataLcdDataU8(monitor_page_current_c2,0,current2);//���µ���
      if(channel2.status == 1)//ͨ��1����
      {
         if(channel2.now_time_s>=3)//������
         {
              if(Temp1>=channel2.alarm_temperature)//�¶ȱ���
              {
                  Alarm2_ON;
                  channel2.status = 0;
              }
              if(channel2.now_time_s>=3)//������
              {
                if(Read_CheckLed2==1)//����
                {
                  Alarm2_ON;
                  alarm_status=1;
                  channel2.status = 0;
                  led2_checkled_alarm_strup;
                }
                if(current2<(set_pwm2/1000.0*1500-20))//�Ƚϵ���
                {
                    Alarm2_ON;
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
             set_ch2_output(channel2.multistage_power[cycle1]);//����PWMռ�ձ�
             ch2_output_on();//ʹ��PWM���
             UpdataLcdDataU8(monitor_page_power_c2 ,2,channel2.multistage_power[cycle1]);//���¹���
          }
          UpdataLcdDataU8(led2_time_strup,8,channel2.multistage_time[cycle1]-channel2.now_time_s);//ͨ��1�Զ�ģʽʣ��ʱ��;
      }else {
              if(alarm_status)//�����������
              {

              }else {
                  led2_close_strup;
              }
              //EN1_OFF;
              set_ch2_output(0);//����PWMռ�ձ�
              ch2_output_off();//ʹ��PWM���
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
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    rt_uint8_t cycle1,large_cycle1;
    Alarm3_OFF;
    Light3_OFF;
    last_state1 = Read_LedOn3;
    Complete3_OFF;
    channel3.now_time_s = 0;
    led3_open_strup;
    led3_power_strup;
    Light3_ON;
    led3_button_on_strup;
    UpdataLcdDataU8(monitor_page_power_c3,2,channel3.multistage_power[0]);//���¹���
    cycle1 = 0;
    large_cycle1 =0;
    while(1)
    {
      if(Read_LedOn3 != last_state1)//״̬�仯
      {
          if(Read_LedOn3 == 0)//�͵�ƽ
          {
              if(channel3.status == 1)
              {
                  channel3.status = 0;
              }else {
                  channel3.status = 1;
              }
          }else {
                 }
          last_state1 = Read_LedOn3;
      }
      UpdataLcdDataU8(monitor_page_current_c3,0,current3);//���µ���
      if(channel3.status == 1)//ͨ��1����
      {
         if(channel3.now_time_s>=3)//������
         {
              if(Temp1>=channel3.alarm_temperature)//�¶ȱ���
              {
                  Alarm3_ON;
                  channel3.status = 0;
              }
              if(channel3.now_time_s>=3)//������
              {
                if(Read_CheckLed3==1)//����
                {
                  Alarm3_ON;
                  alarm_status=1;
                  channel3.status = 0;
                  led3_checkled_alarm_strup;
                }
                if(current3<(set_pwm3/1000.0*1500-20))//�Ƚϵ���
                {
                    Alarm3_ON;
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
             set_ch3_output(channel3.multistage_power[cycle1]);//����PWMռ�ձ�
             ch3_output_on();//ʹ��PWM���
             UpdataLcdDataU8(monitor_page_power_c3 ,2,channel3.multistage_power[cycle1]);//���¹���
          }
          UpdataLcdDataU8(led3_time_strup,8,channel3.multistage_time[cycle1]-channel3.now_time_s);//ͨ��1�Զ�ģʽʣ��ʱ��;
      }else {
              if(alarm_status)//�����������
              {

              }else {
                  led3_close_strup;
              }
              //EN1_OFF;
              set_ch3_output(0);//����PWMռ�ձ�
              ch3_output_off();//ʹ��PWM���
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
    rt_uint8_t last_state1;//�ⲿ����IO״̬
    rt_uint8_t alarm_status=0;
    rt_uint8_t cycle1,large_cycle1;
    Alarm4_OFF;
    Light4_OFF;
    last_state1 = Read_LedOn4;
    Complete4_OFF;
    channel4.now_time_s = 0;
    led4_open_strup;
    led4_power_strup;
    Light4_ON;
    led4_button_on_strup;
    UpdataLcdDataU8(monitor_page_power_c4,2,channel4.multistage_power[0]);//���¹���
    cycle1 = 0;
    large_cycle1 =0;
    while(1)
    {
      if(Read_LedOn4 != last_state1)//״̬�仯
      {
          if(Read_LedOn4 == 0)//�͵�ƽ
          {
              if(channel4.status == 1)
              {
                  channel4.status = 0;
              }else {
                  channel4.status = 1;
              }
          }else {
                 }
          last_state1 = Read_LedOn4;
      }
      UpdataLcdDataU8(monitor_page_current_c4,0,current4);//���µ���
      if(channel4.status == 1)//ͨ��1����
      {
         if(channel4.now_time_s>=3)//������
         {
              if(Temp1>=channel4.alarm_temperature)//�¶ȱ���
              {
                  Alarm4_ON;
                  channel4.status = 0;
              }
              if(channel4.now_time_s>=3)//������
              {
                if(Read_CheckLed4==1)//����
                {
                  Alarm4_ON;
                  alarm_status=1;
                  channel4.status = 0;
                  led4_checkled_alarm_strup;
                }
                if(current4<(set_pwm4/1000.0*1500-20))//�Ƚϵ���
                {
                    Alarm4_ON;
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
             set_ch4_output(channel4.multistage_power[cycle1]);//����PWMռ�ձ�
             ch4_output_on();//ʹ��PWM���
             UpdataLcdDataU8(monitor_page_power_c4,2,channel3.multistage_power[cycle1]);//���¹���
          }
          UpdataLcdDataU8(led4_time_strup,8,channel4.multistage_time[cycle1]-channel4.now_time_s);//ͨ��1�Զ�ģʽʣ��ʱ��;
      }else {
              if(alarm_status)//�����������
              {

              }else {
                  led4_close_strup;
              }
              //EN1_OFF;
              set_ch4_output(0);//����PWMռ�ձ�
              ch4_output_off();//ʹ��PWM���
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
void shine_channel1(void *parameter)//manual mode
{
    rt_uint8_t old_status;
    old_status = channel1.status;
    //���¼�ؽ�������
    led1_close_strup;
    UpdataLcdDataU8(0x5020,2,channel1.power);//���¹���
    UpdataLcdDataU8(0x1054,2,0);//��������ʱ��
    UpdataLcdDataU8(0x1058,2,0);//���µ���
    updatarunbutton(0,0x1080,0);//���°�ť״̬
    if(channel1.control_mode==0)//�ֶ�
    {
        UpdataLcdDataU8(0x1054,2,0);//��������ʱ��
    }else {//�Զ�
        if(channel1.light_mode==0)//�̶�
        {
            UpdataLcdDataU8(0x1054,2,channel1.time);//��������ʱ��
        }else {//����
            UpdataLcdDataU8(0x1054,2,channel1.time);//��������ʱ��
        }
    }
    while(1)
    {
        if (old_status!=channel1.status)//���״̬�б仯j
        {
            if(channel1.status==1)//����
            {
                if(channel1.light_mode==0)//�ֶ�
                {
                    channel1_manual();
                    UpdataLcdDataU8(monitor_page_current_c1,0,0);
                }else {//�Զ�
                    if(channel1.control_mode==0)//�̶�
                    {
                        channel1_auto();
                        UpdataLcdDataU8(monitor_page_current_c1,0,0);
                    }else {//����
                        channel1_multistage();
                        UpdataLcdDataU8(monitor_page_current_c1,0,0);
                    }
                }
            }
            old_status = channel1.status;
        }
        rt_thread_mdelay(1000);
    }
}
void shine_channel2(void *parameter)//manual mode
{
    rt_uint8_t old_status;
    old_status = channel2.status;
    //���¼�ؽ�������
    led2_close_strup;
    UpdataLcdDataU8(monitor_page_power_c2 ,2,channel2.power);//���¹���
    UpdataLcdDataU8(monitor_page_time_c2 ,2,0);//��������ʱ��
    UpdataLcdDataU8(monitor_page_current_c2 ,2,0);//���µ���
    led2_button_off_strup;
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
    while(1)
    {
        if (old_status!=channel2.status)//���״̬�б仯j
        {
            if(channel2.status==1)//����
            {
                if(channel2.light_mode==0)//�ֶ�
                {
                    channel2_manual();
                    UpdataLcdDataU8(monitor_page_current_c2,0,0);
                }else {//�Զ�
                    if(channel2.control_mode==0)//�̶�
                    {
                        channel2_auto();
                        UpdataLcdDataU8(monitor_page_current_c2,0,0);
                    }else {//����
                        channel2_multistage();
                        UpdataLcdDataU8(monitor_page_current_c2,0,0);
                    }
                }
            }
            old_status = channel2.status;
        }
        rt_thread_mdelay(1000);
    }
}
void shine_channel3(void *parameter)//manual mode
{
    rt_uint8_t old_status;
    old_status = channel3.status;
    //���¼�ؽ�������
    led3_close_strup;
    UpdataLcdDataU8(monitor_page_power_c3 ,2,channel3.power);//���¹���
    UpdataLcdDataU8(monitor_page_time_c3 ,2,0);//��������ʱ��
    UpdataLcdDataU8(monitor_page_current_c3 ,2,0);//���µ���
    led3_button_off_strup;
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
    while(1)
    {
        if (old_status!=channel3.status)//���״̬�б仯j
        {
            if(channel3.status==1)//����
            {
                if(channel3.light_mode==0)//�ֶ�
                {
                    channel3_manual();
                    UpdataLcdDataU8(monitor_page_current_c3,0,0);
                }else {//�Զ�
                    if(channel3.control_mode==0)//�̶�
                    {
                        channel3_auto();
                        UpdataLcdDataU8(monitor_page_current_c3,0,0);
                    }else {//����
                        channel3_multistage();
                        UpdataLcdDataU8(monitor_page_current_c3,0,0);
                    }
                }
            }
            old_status = channel3.status;
        }
        rt_thread_mdelay(1000);
    }
}
void shine_channel4(void *parameter)//manual mode
{
    rt_uint8_t old_status;
    old_status = channel4.status;
    //���¼�ؽ�������
    led4_close_strup;
    UpdataLcdDataU8(monitor_page_power_c4 ,2,channel4.power);//���¹���
    UpdataLcdDataU8(monitor_page_time_c4 ,2,0);//��������ʱ��
    UpdataLcdDataU8(monitor_page_current_c4 ,2,0);//���µ���
    led4_button_off_strup;
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
    while(1)
    {
        if (old_status!=channel4.status)//���״̬�б仯j
        {
            if(channel4.status==1)//����
            {
                if(channel4.light_mode==0)//�ֶ�
                {
                    channel4_manual();
                    UpdataLcdDataU8(monitor_page_current_c4,0,0);
                }else {//�Զ�
                    if(channel4.control_mode==0)//�̶�
                    {
                        channel4_auto();
                        UpdataLcdDataU8(monitor_page_current_c4,0,0);
                    }else {//����
                        channel4_multistage();
                        UpdataLcdDataU8(monitor_page_current_c4,0,0);
                    }
                }
            }
            old_status = channel4.status;
        }
        rt_thread_mdelay(1000);
    }
}
void shine_init()//���� ��ʼ��
{
    c1 = rt_thread_create("shine1", shine_channel1, RT_NULL, 1024, 20, 10);
    rt_thread_startup(c1);//�����߳�
    c2 = rt_thread_create("shine2", shine_channel2, RT_NULL, 1024, 20, 10);
    rt_thread_startup(c2);//�����߳�
    c3 = rt_thread_create("shine3", shine_channel3, RT_NULL, 1024, 20, 10);
    rt_thread_startup(c3);//�����߳�
    c4 = rt_thread_create("shine4", shine_channel4, RT_NULL, 1024, 20, 10);
    rt_thread_startup(c4);//�����߳�*/
    /*
    UpdataLcdString(7,17,"");//�ַ�����ʼ��
    UpdataLcdString(7,19,"");//�ַ�����ʼ��
    updatarunbutton(0,1,7);//ͨ��1��ť
    updatarunbutton(0,0x10,7);//ͨ��1��ť
    UpdataLcdDataU8(7,8,0);//ͨ��1ʱ��
    UpdataLcdDataU8(7,12,0);//ͨ��2ʱ��
    on_shine_page = 1;
    if(control_mode.control_mode == 0)//�ֶ�ģʽ
    {
        //UpdataLcdDataU8(7,8,0);//ͨ��1ʱ��
        //UpdataLcdDataU8(7,12,0);//ͨ��2ʱ��
        shine = rt_thread_create("shine", shine_entry_manual, RT_NULL, 1024, 20, 10);
        rt_thread_startup(shine);//�����߳�
    }else {//�Զ�ģʽ

        if(control_mode.light_mode == 0)//�̶�����
        {
            shine = rt_thread_create("shine", shine_entry_auto, RT_NULL, 1024, 20, 10);
            rt_thread_startup(shine);//�����߳�
        }else {//��������
            shine = rt_thread_create("shine", shine_entry_multistage, RT_NULL, 1024, 20, 10);
            rt_thread_startup(shine);//�����߳�
        }
        //UpdataLcdDataU8(7,8,channel1.time);//ͨ��1ʱ��
        //UpdataLcdDataU8(7,12,channel1.time);//ͨ��2ʱ��
    }*/
}
