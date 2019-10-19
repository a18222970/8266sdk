/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __USER_LIGHT_ADJ_H__
#define __USER_LIGHT_ADJ_H__
/*pwm.h: function and macro definition of PWM API , driver level   PWM API的功能和宏定义，驱动级 */
/*user_light.h: user interface for light setting, user level   用户界面为灯光设置，用户级*/
/*user_light_adj: API for color changing and lighting effects, user level  用于颜色变化和照明效果的API，用户级*/




/*save RGB params to flash when calling light_set_aim   调用light_set_aim时将RGB参数保存到flash中*/
#define SAVE_LIGHT_PARAM  0  //set to 0: do not save color params  设置为0:不保存颜色参数

/*check current consumption and limit the total current for LED driver IC  检查电流消耗和限制总电流为LED驱动器IC*/
/*NOTE: YOU SHOULD REPLACE WIHT THE LIMIT CURRENT OF YOUR OWN APPLICATION  注意:你应该用你自己的应用程序的限制电流来代替*/
#define LIGHT_CURRENT_LIMIT  0 //set to 0: do not limit total current  设置为0:不要限制总电流
#if LIGHT_CURRENT_LIMIT
#define LIGHT_TOTAL_CURRENT_MAX  (450*1000) //450000/1000 MA AT MOST
#define LIGHT_CURRENT_MARGIN  (80*1000) //80000/1000 MA CURRENT RAISES WHILE TEMPERATURE INCREASING
#define LIGHT_CURRENT_MARGIN_L2  (110*1000) //110000/1000 MA 
#define LIGHT_CURRENT_MARGIN_L3  (140*1000) //140000/1000 MA 
#endif


/*set target duty for PWM channels, change each channel duty gradually   设定PWM通道的目标任务，逐步改变每个通道的任务*/
void  light_set_aim(uint32 r,uint32 g,uint32 b,uint32 cw,uint32 ww,uint32 period);//'white' channel is not used in default demo
void light_set_aim_r(uint32 r);
void light_set_aim_g(uint32 g);
void light_set_aim_b(uint32 b);
void light_set_aim_cw(uint32 cw);
void light_set_aim_ww(uint32 ww);

#endif

