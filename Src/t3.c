
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "t3.h"


/* defined the LED1 pin: PE6 */

#define LED1_PIN    GET_PIN(B, 1)
/*在使用软件定时器时,有时会干扰到ymode_m升级,使用硬件定时器不会,定时设置为10ms,
设置为1ms周期会影响串口
打开网络功能也会影响MCU的处理速度*/
#define LED_TIME_CYCLE                     200  // ms  1500
#define LED_TIME_OUTPUT                    5   // ms   50

static struct rt_timer timer1;  /* 定时器1的控制块 */

//extern rt_uint32_t led_time;

void led1_set_level(led_level_def level)
{
    if(level == LED_ON){
      rt_pin_write(LED1_PIN,PIN_LOW); //GPIO_ResetBits(GPIOF, GPIO_Pin_9);
    }
    else{
      rt_pin_write(LED1_PIN,PIN_HIGH); //GPIO_SetBits(GPIOF, GPIO_Pin_9);
    }
}

// number 闪烁次数
void led_show(led_para_def *p_led_para, uint8_t number)
{
  p_led_para->next_number = number;
}

// 调用频率小于 10 ms
void led_set_handle(led_para_def *p_led_para, uint32_t time)
{
  // 设置 LED
    if(((uint16_t)(time - p_led_para->set_last_time)) >= p_led_para->set_time_cycle)
    {
        p_led_para->set_last_time = time;

        if(p_led_para->curr_number)
        {
            p_led_para->led_set(((p_led_para->curr_number & 1) == 1) ? LED_OFF : LED_ON);
            p_led_para->curr_number--;

            if(p_led_para->curr_number == 0)
            {
               p_led_para->set_time_cycle = (uint16_t)-1;// 下一个周期不再进入
            }
        }
    }
  // 更新当前参数（1.5 s 更新一次）
  if(((uint16_t)(time - p_led_para->last_time_show_cycle)) >= LED_TIME_CYCLE)
  {                                 // 2000- 0                          //1500
        p_led_para->last_time_show_cycle = time;

        if(!p_led_para->curr_number)
        {
           uint8_t number = p_led_para->next_number; // 获取当前指示次数

          if(number < LED_TIME_CYCLE / LED_TIME_OUTPUT / 2)  // 限制闪烁次数
          {
            p_led_para->curr_number    = number * 2 - 1;
            p_led_para->set_last_time  = time;
            p_led_para->set_time_cycle = LED_TIME_OUTPUT;
            p_led_para->led_set(LED_ON);
          }
        }
   }
}
rt_uint32_t led_time;  //led状态指示灯 计时变量

/* 定时器1超时函数 */
static void timeout1(void* parameter)
{
 led_time++;//led 指示灯计时变量
}

//线程t3  --------------------------------------------------
void t3_thread_entry(void* parameter)
{



    /* 初始化定时器 */
  rt_timer_init(&timer1, "timer1",                /* 定时器名字是 timer1 */
                          timeout1,                /* 超时时回调的处理函数 */
                          RT_NULL,                 /* 超时函数的入口参数 */
                          10,                      /* 定时长度，以OS Tick为单位，即1个OS Tick  1ms */
                          RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */

    rt_timer_start(&timer1); /* 启动定时器 */


    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);

    led_para_def  led1_para ={
                                .set_time_cycle  =  (uint16_t)-1,
                                .curr_number     =  0,
                                .led_set         =  led1_set_level,
                             };

    led_show(&led1_para, 2); //设置闪烁次数
    while (1)
    {
      rt_thread_delay(10);
      led_set_handle(&led1_para, led_time);
    }
}
