/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-18     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "rtthread.h"
#include "board.h"

#define  APP_VERSION   "v1.0.0"

#include <t3.h>
//static struct rt_thread t1_thread; //线程控制块
static struct rt_thread t3_thread; //线程控制块

ALIGN (RT_ALIGN_SIZE)
//static rt_uint8_t rt_t1_thread_stack[1024];//线程栈
static rt_uint8_t rt_t3_thread_stack[1024];//线程栈

/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(B, 1)

int main(void)
{
    MX_GPIO_Init();
    MX_USART1_UART_Init();
 //   int count = 1;

    rt_kprintf("rt_thread版本是:%s\n",APP_VERSION);

    rt_thread_init(&t3_thread, //线程控制块
                   "t3",       //线程名字,在shell里面可以看到
                    t3_thread_entry,//线程入口函数
                    RT_NULL,//线程入口函数参数
                    &rt_t3_thread_stack[0],//线程栈起始地址
                    sizeof(rt_t3_thread_stack),//线程栈大小
                    23,//线程的优先级
                    20);//线程的时间片

    rt_thread_startup(&t3_thread);//启动线程

//    while (count++)
//    {
//        LOG_D("Hello RT-Thread!");
//        rt_thread_mdelay(1000);
//        rt_kprintf("count= %d \n",count);
//    }
    return RT_EOK;
}
