/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>  //版权
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


#include "espconn.h"
#include "os_type.h"
#include "mem.h"
***************************************************/
#include "modules/wangLuo.h"
#include "user_config.h"

#include "c_types.h"
#include "user_interface.h"
#include "osapi.h"
#include "modules/debug.h"
#include "os_type.h"
#include "espconn.h"


static ETSTimer WiFiLinker;  //重新构建定时器
wifi_callback wifiCb = NULL;

static uint8_t wifiStatus = STATION_IDLE, lastWifiStatus = STATION_IDLE;
//wifi连接定时器回调
static void ICACHE_FLASH_ATTR
wifi_check_ip(void* arg)
{
	//使用结构体（Struct）来保存不同类型的数据,struct 结构体变量名.成员名;
	struct ip_info ipConfig;

	os_timer_disarm(&WiFiLinker);
	//查询8266的IP地址
	wifi_get_ip_info(STATION_IF, &ipConfig);
	//wifi_station_get_connect_status() 查询 ESP8266 Wi-Fi Station 接⼝连接 AP 的状态。
	wifiStatus = wifi_station_get_connect_status();
	//STATION_GOT_IP//获得IP
	if (wifiStatus == STATION_GOT_IP && ipConfig.ip.addr != 0)
	{
		os_timer_setfn(&WiFiLinker, (os_timer_func_t*)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 2000, 0);
	}
	else
	{
		if (wifi_station_get_connect_status() == STATION_WRONG_PASSWORD)//错误的密码
		{

			INFO("STATION_WRONG_PASSWORD\r\n");
			wifi_station_connect(); //连接wifi
		}
		else if (wifi_station_get_connect_status() == STATION_NO_AP_FOUND)////没有发现热点AP
		{
			INFO("STATION_NO_AP_FOUND\r\n");
			wifi_station_connect(); //连接wifi
		}
		else if (wifi_station_get_connect_status() == STATION_CONNECT_FAIL)//连接失败
		{
			INFO("STATION_CONNECT_FAIL\r\n");
			wifi_station_connect(); //连接wifi
		}
		else
		{
			INFO("STATION_IDLE\r\n");//闲置
		}

		os_timer_setfn(&WiFiLinker, (os_timer_func_t*)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 500, 0);
	}
	if (wifiStatus != lastWifiStatus) {
		lastWifiStatus = wifiStatus;
		if (wifiCb)
			wifiCb(wifiStatus);
	}
}

/*************************************************
Function: lian_jie_AP // 函数名称
Description: wifi客户端模式连接路由器 // 函数功能、性能等的描述
Calls:// 被本函数调用的函数清单
    wifi_set_opmode_current(): 设置wifi工作模式，不保存到flash;
	                           NULL_MODE       0x00  关闭wifi功能
                               STATION_MODE    0x01  客户端模式_连接路由器
                               SOFTAP_MODE     0x02  AP模式_热点
                               STATIONAP_MODE  0x03  混合模式
    os_memset():在一段内存块中填充某个给定值，当前的作用是
	           //用长度为sizeof(struct station_config)字节的0填充到地址为wifi_station_config的内存中，
			   //就是初始化wifi_station_config的值。
	           内存块指针，,填充值，填充大小
	os_sprintf():这个函数就是把第三部分的数据，按照第二部分格式化字符的格式，把第三部分的数据进行”格式化“，
	             //然后在把格式化后的数据类型，赋值给第一个参数。
    wifi_station_set_config_current():设置wifi_station接口配置，不保存到flash.
	                                  wifi_station_config 构建体
    wifi_station_set_auto_connect(): 设置是否上电连接已记录的AP(路由)，默认自动连接。
    wifi_station_connect(): 连接路由器.
Called By: // 调用本函数的函数清单
    user_main.c: user_init(void)
Input: // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
	ssid：wifi名称；
	pass: wifi密码；
	hui_diao: 回调函数名
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
void ICACHE_FLASH_ATTR
lian_jie_AP(uint8_t* ssid, uint8_t* pass, wifi_callback hui_diao) //uint8_t 需要c_types.h
{
	struct station_config wifi_station_config; //station_config  需要user_interface.h
	INFO("WIFI_INIT\r\n");
	wifi_set_opmode_current(STATION_MODE);     //需要user_interface.h
	wifiCb = hui_diao;
	os_memset(&wifi_station_config, 0, sizeof(struct station_config)); //需要osapi.h
	os_sprintf(wifi_station_config.ssid, "%s" , ssid);
	os_sprintf(wifi_station_config.password, "%s", pass);
	wifi_station_set_config_current(&wifi_station_config);
	os_timer_disarm(&WiFiLinker);//关闭定时器
	os_timer_setfn(&WiFiLinker, (os_timer_func_t*)wifi_check_ip, NULL);// wifi_check_i 回调函数
	os_timer_arm(&WiFiLinker, 1000, 0);
	wifi_station_set_auto_connect(ZI_DONG_LIAN_JIE);
	wifi_station_connect();
}

