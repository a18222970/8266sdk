/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>   //版权
File name: wangLuo.c // 文件名
Author: cx           // 作者
Version: 1.0        //版本
Date: 2019.10.23    //完成日期
Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
// 或函数的接口，输出值、取值范围、含义及参数间的控
// 制、顺序、独立或依赖等关系
Others: // 其它内容的说明
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
1. Date:
Author:
Modification:
2. ...
*************************************************/
/************************************************
使用#ifndef、#define、#endif组合宏定义的作用是：防止头文件的重复包含和编译
#ifndef            与#ifdef相反，判断某个宏是否未被定义
#define            定义一个预处理宏
#endif             #if, #ifdef, #ifndef这些条件命令的结束标志.
#undef            取消宏的定义
#ifdef              判断某个宏是否被定义，若已定义，执行随后的语句
#if                   编译预处理中的条件命令，相当于C语法中的if语句
#elif                若#if, #ifdef, #ifndef或前面的#elif条件不满足，则执行#elif之后的语句，相当于C语法中的else-if
#else              与#if, #ifdef, #ifndef对应, 若这些条件不满足，则执行#else之后的语句，相当于C语法中的else
defined         　与#if, #elif配合使用，判断某个宏是否被定义
***************************************************/

#ifndef USER_WIFI_H_
#define USER_WIFI_H_

/*载入类型头文件*/
#include "os_type.h"

/***************************
typedef void (*WifiCallback)(uint8_t);
定义了一个指向函数的指针WifiCallback，其返回值 void 类型，参数是后面的(uint8_t).
uint8_t pr = "6";
WifiCallback f = pr;  /*定义变量f并赋值pr
f(6);        /*执行，相当于pr(6)
**************************************/
typedef void (*wifi_callback)(uint8_t);

/*************************************************
Function: lian_jie_AP // 函数名称
Description: wifi客户端模式连接路由器 // 函数功能、性能等的描述
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
	user_main.c: user_init(void)
Input: // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    ssid：wifi名称；
	pass: wifi密码；
	hui_diao: 回调函数名.注意：此回调在user_init(void)开始执行
	          //时就会调用执行，在wifi连接成功后又会调用执行。
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
void ICACHE_FLASH_ATTR
lian_jie_AP(uint8_t* ssid, uint8_t* pass, wifi_callback hui_diao);










#endif
