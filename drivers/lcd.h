/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-17     30802       the first version
 */
#ifndef DRIVERS_LCD_H_
#define DRIVERS_LCD_H_
extern rt_thread_t lcd_receive_tid;//lcd接收线程
extern void lcd_init();
extern void updatarunbutton(rt_uint8_t button_state,rt_uint16_t buttonID,rt_uint8_t page);//更新照射界面按钮
extern void UpdataLcdDataU8(rt_uint16_t page,rt_uint8_t channel,rt_uint16_t num);//
extern rt_mq_t lcd_command_mq;//创建lcd命令消息队列
extern rt_uint8_t current_show;
#endif /* DRIVERS_LCD_H_ */
