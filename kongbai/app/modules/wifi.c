/*
 * wifi.c
 *
 *  Created on: Dec 04, 2018
 *      Author: imliubo
 */
#include "modules/wifi.h"

#include "modules/config.h"
#include "debug.h"
#include "user_config.h"
#include "user_interface.h"
#include "osapi.h"
#include "espconn.h"
#include "os_type.h"
#include "mem.h"

static ETSTimer WiFiLinker;
WifiCallback wifiCb = NULL;  // WifiCallback wifiCb 使用这个新类型定义了变量wifiCb
static uint8_t wifiStatus = STATION_IDLE, lastWifiStatus = STATION_IDLE;
//wifi连接定时器回调
static void ICACHE_FLASH_ATTR
wifi_check_ip(void *arg)
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
		os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 2000, 0);
	}
	else
	{
		if(wifi_station_get_connect_status() == STATION_WRONG_PASSWORD)//错误的密码
		{

			INFO("STATION_WRONG_PASSWORD\r\n");
			wifi_station_connect(); //连接wifi
		}
		else if(wifi_station_get_connect_status() == STATION_NO_AP_FOUND)////没有发现热点AP
		{
			INFO("STATION_NO_AP_FOUND\r\n");
			wifi_station_connect(); //连接wifi
		}
		else if(wifi_station_get_connect_status() == STATION_CONNECT_FAIL)//连接失败
		{
			INFO("STATION_CONNECT_FAIL\r\n");
			wifi_station_connect(); //连接wifi
		}
		else
		{
			INFO("STATION_IDLE\r\n");//闲置
		}

		os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 500, 0);
	}
	if(wifiStatus != lastWifiStatus){
		lastWifiStatus = wifiStatus;
		if(wifiCb)
			wifiCb(wifiStatus);
	}
}

//wifi连接
void ICACHE_FLASH_ATTR
WIFI_Connect(uint8_t* ssid, uint8_t* pass, WifiCallback cb)
{
	struct station_config stationConf;

	INFO("WIFI_INIT\r\n");
	//设置wifi工作模式，不保存
	wifi_set_opmode_current(STATION_MODE); //STATION_MODE = 0x01

	//wifi_station_set_auto_connect(FALSE);  //设置设备上电是否自动连接wifi，默认自动连接。
	wifiCb = cb;

	/**os_memset 就是C语音memset函数，作用是在一段内存块中填充某个给定的值
	 * memset 是对较大的数组或结构体进行清零初始化的最快方法，因为它是直接对内存进行操作的。
	 * 将指针变量stationConf 所指向的内存单元用一个“整数” 0 替换
	 * memset 一般使用“0”初始化内存单元，而且通常是给数组或结构体进行初始化
	 * memset 函数的第三个参数 的值一般用 sizeof()  获取
	 */
	//os_memset 封装C语音函数，在一段内存块中填充某个给定值，初始化stationConf
	//os_memset(内存块指针, 填充值, 填充大小（占用多少内存块）)
	//sizeof 计算数据字节长度
	os_memset(&stationConf, 0, sizeof(struct station_config)); 

	//变参函数，用ssid的值替换stationConf中ssid的值，   不确定
	//os_sprintf 它是将格式化的字符串打印到stationConf.ssid中。
	//os_sprintf 这个函数就是把第三部分的数据，按照第二部分格式化字符的格式，把第三部分的数据进行”格式化“，然后在把格式化后的数据类型，存储到字符串的缓存区间里去。
	os_sprintf(stationConf.ssid, "%s", ssid);
	os_sprintf(stationConf.password, "%s", pass);
	/*****  wifi_station_set_config_current  *******
	*如果在user_init中调用，8266会在初始化完后自动连接AP，无需在调用 wifi_station_connect
	****************************************************/
	//3.5.8 设置wifi 连接模式的配置参数，但不保存
	wifi_station_set_config_current(&stationConf);

	os_timer_disarm(&WiFiLinker);//关闭定时器
	os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);// wifi_check_i 回调函数
	os_timer_arm(&WiFiLinker, 1000, 0);

	//设置是否上电自动连接已记录的wifi,默认自动连接
	wifi_station_set_auto_connect(TRUE);
	//连接wifi
	wifi_station_connect();
}

