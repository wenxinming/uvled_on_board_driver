/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-01-24     ThinkPad       the first version
 */
#ifndef DRIVERS_LCD_STRING_H_
#define DRIVERS_LCD_STRING_H_

#define led1_open_strup UpdataLcdString(0x1010, 17, "开启照射")
#define led1_close_strup UpdataLcdString(0x1010, 17, "关闭照射")
#define led2_open_strup UpdataLcdString(0x1020, 17, "开启照射")
#define led2_close_strup UpdataLcdString(0x1020, 17, "关闭照射")
#define led3_open_strup UpdataLcdString(0x1030, 17, "开启照射")
#define led3_close_strup UpdataLcdString(0x1030, 17, "关闭照射")
#define led4_open_strup UpdataLcdString(0x1040, 17, "开启照射")
#define led4_close_strup UpdataLcdString(0x1040, 17, "关闭照射")

#define led1_power_strup UpdataLcdDataU8(0x5020,2,channel1.power)
#define led2_power_strup UpdataLcdDataU8(0x105C,2,channel2.power)
#define led3_power_strup UpdataLcdDataU8(0x1068,2,channel3.power)
#define led4_power_strup UpdataLcdDataU8(0x1074,2,channel4.power)

#define led1_now_time_strup UpdataLcdDataU8(0x1054,8,channel1.now_time_s)//通道1时间
#define led2_now_time_strup UpdataLcdDataU8(0x1060,8,channel2.now_time_s)//通道1时间
#define led3_now_time_strup UpdataLcdDataU8(0x106C,8,channel3.now_time_s)//通道1时间
#define led4_now_time_strup UpdataLcdDataU8(0x1078,8,channel4.now_time_s)//通道1时间

#define led1_current_alarm_strup UpdataLcdString(0x1010, 17, "电流异常")
#define led2_current_alarm_strup UpdataLcdString(0x1020, 17, "电流异常")
#define led3_current_alarm_strup UpdataLcdString(0x1030, 17, "电流异常")
#define led4_current_alarm_strup UpdataLcdString(0x1040, 17, "电流异常")

#define led1_checkled_alarm_strup UpdataLcdString(0x1010, 17, "接入异常")
#define led2_checkled_alarm_strup UpdataLcdString(0x1020, 17, "接入异常")
#define led3_checkled_alarm_strup UpdataLcdString(0x1030, 17, "接入异常")
#define led4_checkled_alarm_strup UpdataLcdString(0x1040, 17, "接入异常")

#define led1_temperature_alarm_strup UpdataLcdString(0x1010, 17, "温度报警")
#define led2_temperature_alarm_strup UpdataLcdString(0x1020, 17, "温度报警")
#define led3_temperature_alarm_strup UpdataLcdString(0x1030, 17, "温度报警")
#define led4_temperature_alarm_strup UpdataLcdString(0x1040, 17, "温度报警")

#define led1_button_off_strup updatarunbutton(0,0x1080,7)//开灯按钮off
#define led2_button_off_strup updatarunbutton(0,0x1082,7)
#define led3_button_off_strup updatarunbutton(0,0x1084,7)
#define led4_button_off_strup updatarunbutton(0,0x1086,7)

#define led1_button_on_strup updatarunbutton(1,0x1080,7)//开灯按钮on
#define led2_button_on_strup updatarunbutton(1,0x1082,7)
#define led3_button_on_strup updatarunbutton(1,0x1084,7)
#define led4_button_on_strup updatarunbutton(1,0x1086,7)

#define led1_remaining_time_strup UpdataLcdDataU8(0x1054,8,channel1.time-channel1.now_time_s)//通道1自动模式剩余时间
#define led2_remaining_time_strup UpdataLcdDataU8(0x1060,8,channel2.time-channel2.now_time_s)//通道1自动模式剩余时间
#define led3_remaining_time_strup UpdataLcdDataU8(0x106C,8,channel3.time-channel3.now_time_s)//通道1自动模式剩余时间
#define led4_remaining_time_strup UpdataLcdDataU8(0x1078,8,channel4.time-channel4.now_time_s)//通道1自动模式剩余时间

//控制模式菜单
#define ctrl_mode_page_c1_ctrlmode 0x108A
#define ctrl_mode_page_c1_lightmode 0x1098
#define ctrl_mode_page_c1_enable 0x10A0

#define ctrl_mode_page_c2_ctrlmode 0x108E
#define ctrl_mode_page_c2_lightmode 0x109A
#define ctrl_mode_page_c2_enable 0x10A2

#define ctrl_mode_page_c3_ctrlmode 0x1092
#define ctrl_mode_page_c3_lightmode 0x109C
#define ctrl_mode_page_c3_enable 0x10A4

#define ctrl_mode_page_c4_ctrlmode 0x1096
#define ctrl_mode_page_c4_lightmode 0x109E
#define ctrl_mode_page_c4_enable 0x10A6

//查看参数界面
#define show_parameter_c1_power 0x2600
#define show_parameter_c1_time 0x2610
#define show_parameter_c1_current 0x2620
#define show_parameter_c1_alltime_h 0x2630
#define show_parameter_c1_alltime_m 0x2634

#define show_parameter_c2_power 0x2604
#define show_parameter_c2_time 0x2614
#define show_parameter_c2_current 0x2624
#define show_parameter_c2_alltime 0x2640
#define show_parameter_c2_alltime_h 0x2638
#define show_parameter_c2_alltime_m 0x263C

#define show_parameter_c3_power 0x2608
#define show_parameter_c3_time 0x2618
#define show_parameter_c3_current 0x2628
#define show_parameter_c3_alltime_h 0x2640
#define show_parameter_c3_alltime_m 0x2644

#define show_parameter_c4_power 0x260C
#define show_parameter_c4_time 0x261C
#define show_parameter_c4_current 0x262C
#define show_parameter_c4_alltime_h 0x2648
#define show_parameter_c4_alltime_m 0x264C

//参数设置界面
#define parameter_page_c1_power 0x3000
#define parameter_page_c1_time 0x3004
#define parameter_page_c1_delay 0x3008
#define parameter_page_c1_current 0x300C

#define parameter_page_c2_power 0x3010
#define parameter_page_c2_time 0x3014
#define parameter_page_c2_delay 0x3018
#define parameter_page_c2_current 0x301C

#define parameter_page_c3_power 0x3020
#define parameter_page_c3_time 0x3024
#define parameter_page_c3_delay 0x3028
#define parameter_page_c3_current 0x302C

#define parameter_page_c4_power 0x3030
#define parameter_page_c4_time 0x3034
#define parameter_page_c4_delay 0x3038
#define parameter_page_c4_current 0x303C

#define c1_multistage_set1  0x3100
#define c1_multistage_set2  0x3100+4
#define c1_multistage_set3  0x3100+8
#define c1_multistage_set4  0x3100+12
#define c1_multistage_set5  0x3100+16
#define c1_multistage_set6  0x3100+20
#define c1_multistage_set7  0x3100+24
#define c1_multistage_set8  0x3100+28
#define c1_multistage_set9  0x3100+32
#define c1_multistage_set10  0x3100+36
#define c1_multistage_set11  0x3100+40
#define c1_multistage_set12  0x3100+44
#define c1_multistage_set13  0x3100+48
#define c1_multistage_set14  0x3100+52
#define c1_multistage_set15  0x3100+56
#define c1_multistage_set16  0x3100+60
#define c1_multistage_set17  0x3100+64
#define c1_multistage_set18  0x3100+68
#define c1_multistage_set19  0x3100+72
#define c1_multistage_set20  0x3100+76

#define c1_multistage_time_set1  0x3150
#define c1_multistage_time_set2  0x3150+4
#define c1_multistage_time_set3  0x3150+8
#define c1_multistage_time_set4  0x3150+12
#define c1_multistage_time_set5  0x3150+16
#define c1_multistage_time_set6  0x3150+20
#define c1_multistage_time_set7  0x3150+24
#define c1_multistage_time_set8  0x3150+28
#define c1_multistage_time_set9  0x3150+32
#define c1_multistage_time_set10  0x3150+36
#define c1_multistage_time_set11  0x3150+40
#define c1_multistage_time_set12  0x3150+44
#define c1_multistage_time_set13  0x3150+48
#define c1_multistage_time_set14  0x3150+52
#define c1_multistage_time_set15  0x3150+56
#define c1_multistage_time_set16  0x3150+60
#define c1_multistage_time_set17  0x3150+64
#define c1_multistage_time_set18  0x3150+68
#define c1_multistage_time_set19  0x3150+72
#define c1_multistage_time_set20  0x3150+76
#define c1_multistage_cycle_set  0x31A0
#define c1_multistage_return_key  0x1D

#define c2_multistage_set1  0x3200
#define c2_multistage_set2  0x3200+4
#define c2_multistage_set3  0x3200+8
#define c2_multistage_set4  0x3200+12
#define c2_multistage_set5  0x3200+16
#define c2_multistage_set6  0x3200+20
#define c2_multistage_set7  0x3200+24
#define c2_multistage_set8  0x3200+28
#define c2_multistage_set9  0x3200+32
#define c2_multistage_set10  0x3200+36
#define c2_multistage_set11  0x3200+40
#define c2_multistage_set12  0x3200+44
#define c2_multistage_set13  0x3200+48
#define c2_multistage_set14  0x3200+52
#define c2_multistage_set15  0x3200+56
#define c2_multistage_set16  0x3200+60
#define c2_multistage_set17  0x3200+64
#define c2_multistage_set18  0x3200+68
#define c2_multistage_set19  0x3200+72
#define c2_multistage_set20  0x3200+76

#define c2_multistage_time_set1  0x3250
#define c2_multistage_time_set2  0x3250+4
#define c2_multistage_time_set3  0x3250+8
#define c2_multistage_time_set4  0x3250+12
#define c2_multistage_time_set5  0x3250+16
#define c2_multistage_time_set6  0x3250+20
#define c2_multistage_time_set7  0x3250+24
#define c2_multistage_time_set8  0x3250+28
#define c2_multistage_time_set9  0x3250+32
#define c2_multistage_time_set10  0x3250+36
#define c2_multistage_time_set11  0x3250+40
#define c2_multistage_time_set12  0x3250+44
#define c2_multistage_time_set13  0x3250+48
#define c2_multistage_time_set14  0x3250+52
#define c2_multistage_time_set15  0x3250+56
#define c2_multistage_time_set16  0x3250+60
#define c2_multistage_time_set17  0x3250+64
#define c2_multistage_time_set18  0x3250+68
#define c2_multistage_time_set19  0x3250+72
#define c2_multistage_time_set20  0x3250+76
#define c2_multistage_cycle_set  0x32A0
#define c2_multistage_return_key  0x1E

#define c3_multistage_set1  0x3300
#define c3_multistage_set2  0x3300+4
#define c3_multistage_set3  0x3300+8
#define c3_multistage_set4  0x3300+12
#define c3_multistage_set5  0x3300+16
#define c3_multistage_set6  0x3300+20
#define c3_multistage_set7  0x3300+24
#define c3_multistage_set8  0x3300+28
#define c3_multistage_set9  0x3300+32
#define c3_multistage_set10  0x3300+36
#define c3_multistage_set11  0x3300+40
#define c3_multistage_set12  0x3300+44
#define c3_multistage_set13  0x3300+48
#define c3_multistage_set14  0x3300+52
#define c3_multistage_set15  0x3300+56
#define c3_multistage_set16  0x3300+60
#define c3_multistage_set17  0x3300+64
#define c3_multistage_set18  0x3300+68
#define c3_multistage_set19  0x3300+72
#define c3_multistage_set20  0x3300+76

#define c3_multistage_time_set1  0x3350
#define c3_multistage_time_set2  0x3350+4
#define c3_multistage_time_set3  0x3350+8
#define c3_multistage_time_set4  0x3350+12
#define c3_multistage_time_set5  0x3350+16
#define c3_multistage_time_set6  0x3350+20
#define c3_multistage_time_set7  0x3350+24
#define c3_multistage_time_set8  0x3350+28
#define c3_multistage_time_set9  0x3350+32
#define c3_multistage_time_set10  0x3350+36
#define c3_multistage_time_set11  0x3350+40
#define c3_multistage_time_set12  0x3350+44
#define c3_multistage_time_set13  0x3350+48
#define c3_multistage_time_set14  0x3350+52
#define c3_multistage_time_set15  0x3350+56
#define c3_multistage_time_set16  0x3350+60
#define c3_multistage_time_set17  0x3350+64
#define c3_multistage_time_set18  0x3350+68
#define c3_multistage_time_set19  0x3350+72
#define c3_multistage_time_set20  0x3350+76
#define c3_multistage_cycle_set  0x33A0
#define c3_multistage_return_key  0x1F

#define c4_multistage_set1  0x3400
#define c4_multistage_set2  0x3400+4
#define c4_multistage_set3  0x3400+8
#define c4_multistage_set4  0x3400+12
#define c4_multistage_set5  0x3400+16
#define c4_multistage_set6  0x3400+20
#define c4_multistage_set7  0x3400+24
#define c4_multistage_set8  0x3400+28
#define c4_multistage_set9  0x3400+32
#define c4_multistage_set10  0x3400+36
#define c4_multistage_set11  0x3400+40
#define c4_multistage_set12  0x3400+44
#define c4_multistage_set13  0x3400+48
#define c4_multistage_set14  0x3400+52
#define c4_multistage_set15  0x3400+56
#define c4_multistage_set16  0x3400+60
#define c4_multistage_set17  0x3400+64
#define c4_multistage_set18  0x3400+68
#define c4_multistage_set19  0x3400+72
#define c4_multistage_set20  0x3400+76

#define c4_multistage_time_set1  0x3450
#define c4_multistage_time_set2  0x3450+4
#define c4_multistage_time_set3  0x3450+8
#define c4_multistage_time_set4  0x3450+12
#define c4_multistage_time_set5  0x3450+16
#define c4_multistage_time_set6  0x3450+20
#define c4_multistage_time_set7  0x3450+24
#define c4_multistage_time_set8  0x3450+28
#define c4_multistage_time_set9  0x3450+32
#define c4_multistage_time_set10  0x3450+36
#define c4_multistage_time_set11  0x3450+40
#define c4_multistage_time_set12  0x3450+44
#define c4_multistage_time_set13  0x3450+48
#define c4_multistage_time_set14  0x3450+52
#define c4_multistage_time_set15  0x3450+56
#define c4_multistage_time_set16  0x3450+60
#define c4_multistage_time_set17  0x3450+64
#define c4_multistage_time_set18  0x3450+68
#define c4_multistage_time_set19  0x3450+72
#define c4_multistage_time_set20  0x3450+76
#define c4_multistage_cycle_set  0x34A0
#define c4_multistage_return_key  0x20

#define show_parameter_c1_temperature 0x34A4
#define show_parameter_c2_temperature 0x34A8
#define show_parameter_c3_temperature 0x34AC
#define show_parameter_c4_temperature 0x34B0

#define parameter_page_c1_temperature 0x34B4
#define parameter_page_c2_temperature 0x34B8
#define parameter_page_c3_temperature 0x34BC
#define parameter_page_c4_temperature 0x34C0

#define parameter_current_c1 0x34C4
#define parameter_current_c2 0x34C8
#define parameter_current_c3 0x34CC
#define parameter_current_c4 0x34D0

#define show_current_c1 0x34D4
#define show_current_c2 0x34D8
#define show_current_c3 0x34DC
#define show_current_c4 0x34E0
#define show_current_c4 0x34E0

#define  ctrl_mode_page_sync_mode 0x34c4

#define c1_clear_alltime_key  0x21
#define c2_clear_alltime_key  0x22
#define c3_clear_alltime_key  0x23
#define c4_clear_alltime_key  0x24

#define clear_alltime_password_ok_key  0x26
#define clear_alltime_password_cancel_key  0x27
#define clear_alltime_password  0x2650
#define clear_alltime_password_state  0x2660

#define reset_password_state  0x2670
#define reset_password  0x2680
#define reset_password_ok_key  0x28
#define reset_password_cancel_key  0x29
#define reset_ok 0x2A
#define reset_cancel 0x2B
#define sync_mode_key 0x2C
#define current_return_key 0x2D
#define current_set_key 0x2E

#define monitor_page_power_c1 0x5020
#define monitor_page_time_c1 0x1054
#define monitor_page_current_c1 0x1058

#define monitor_page_power_c2 0x105C
#define monitor_page_time_c2 0x1060
#define monitor_page_current_c2 0x1064

#define monitor_page_power_c3 0x1068
#define monitor_page_time_c3 0x106C
#define monitor_page_current_c3 0x1070

#define monitor_page_power_c4 0x1074
#define monitor_page_time_c4 0x1078
#define monitor_page_current_c4 0x107C


#define led1_time_strup 0x1054
#define led2_time_strup 0x1060
#define led3_time_strup 0x106C
#define led4_time_strup 0x1078
#endif /* DRIVERS_LCD_STRING_H_ */
