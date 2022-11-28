#ifndef __T3_H
#define __T3_H

#include <rtthread.h>

typedef enum
{
    LED_ON=1,
    LED_OFF
}led_level_def;

typedef struct
{
    rt_uint16_t   last_time_show_cycle;  // 状态查询时间
       uint16_t   set_time_cycle;        // 循环时间
       uint16_t   set_last_time;         // 上一次电平输出时间
        uint8_t   curr_number;           // 当前
        uint8_t   next_number;           // 下一个指示次数
         void     (*led_set)(led_level_def);
}led_para_def;


extern rt_uint32_t led_time;  //led状态指示灯 计时变量
extern void t3_thread_entry(void* parameter);

void t3_set_level(led_level_def level);
void led_show(led_para_def *p_led_para, uint8_t number);
void led_set_handle(led_para_def *p_led_para, uint32_t time);

#endif
