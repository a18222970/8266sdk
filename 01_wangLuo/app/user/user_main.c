/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>   
File name: wangLuo.c 
Author: cx           
Version: 1.0        
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

#include "ets_sys.h"
#include "osapi.h"
#include "ip_addr.h"
#include "espconn.h"
#include "mem.h"
#include "user_config.h"

#include "user_interface.h"
#include "driver/uart.h"
#include "modules/wangLuo.h"
#include "../../include/osapi.h"

/*定义软定时器*/
static ETSTimer TCP_timer;  //重新构建定时器TCP_timer

/*判断是什么服务*/
#if   TCP_CLIENT
struct espconn tcp_client; //客户端模式
#elif TCP_SERVER
struct espconn tcp_server; //服务端模式
//结束
#endif

/*wifi连接_客户端模式_连接路由器*/
void ICACHE_FLASH_ATTR
wiFi_lianJie(void)
{
	/*NULL_MODE       0x00  关闭wifi功能
	  STATION_MODE    0x01  客户端模式_连接路由器
	  SOFTAP_MODE     0x02  AP模式_热点
	  STATIONAP_MODE  0x03  混合模式  */
	wifi_set_opmode(STATION_MODE);  //设置wifi工作模式并保存
	char ssid[32] = "cccxxx";
	char password[64] = "chenxin1234567890";

	struct station_config stationConf;
	stationConf.bssid_set = 0;                     //不需要检查ap的mac地址
	os_memcpy(&stationConf.ssid, ssid, 32);        //os_memcpy 内存拷贝填充。
	os_memcpy(&stationConf.password, password, 64);
	//wifi_station_set_config(&stationConf);       //设置Wi-Fi Station参数，保存到flash
	wifi_station_set_config_current(&stationConf);//设置Wi-Fi Station参数，不保存到flash
	wifi_station_connect();
	os_printf("duankou :OK\n");

	/***********系统崩溃显示乱码时，打印出错信息***************
	 * 打印错误信息，
	 * reset reason: 6  外部系统复位
	 * 				 0 电源正常启动
	 * 				 1 硬件看门狗复位
	 *               2 异常复位，gpio状态不变
	 *               3 软件看门狗复位，gpio状态不变
	 *               4 软件重启，system_restart, gpio状态不变
	 *               5 从沉睡中醒来
	 *********************************************************/
	struct rst_info* rtc_info = system_get_rst_info();
	os_printf("reset reason: %x\n", rtc_info->reason);
	if (rtc_info->reason == REASON_WDT_RST ||
		rtc_info->reason == REASON_EXCEPTION_RST ||
		rtc_info->reason == REASON_SOFT_WDT_RST)
	{
		if (rtc_info->reason == REASON_EXCEPTION_RST)
		{
			os_printf("Fatal exception (%d):\n", rtc_info->exccause);
		}
		//打印上次崩溃的地址，多用于崩溃时乱码调试
		os_printf("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x,excvaddr=0x%08x, depc=0x%08x\n",
			rtc_info->epc1, rtc_info->epc2, rtc_info->epc3, rtc_info->excvaddr, rtc_info->depc);
	}
}

/*************************************************
Function: wifi_AP 
Description: wifi工作在AP模式
Calls:// 被本函数调用的函数清单
	wifi_set_opmode_current(): 设置wifi工作模式，不保存到flash;
							   NULL_MODE       0x00  关闭wifi功能
							   STATION_MODE    0x01  客户端模式_连接路由器
							   SOFTAP_MODE     0x02  AP模式_热点
							   STATIONAP_MODE  0x03  混合模式
    wifi_set_opmode(): 设置wifi工作模式，保存到flash;
		wifi_set_sleep_type(): 设置睡眠模式
	wifi_station_set_auto_connect()： 设置上电是否连接路由器，默认自动连接。
	                                  //如果在非user_init(void)中调用，会在下次启动时生效
	                             1 自动连接
								 0 不自动连接
	os_strcpy():把第二个字符串复制到第一个字符串，参数为指针，原型声明
			   //char *ets_strcpy(char *s1, const char *s2);
	wifi_softap_set_config(wifiAP_config构建体地址):设置wifi_AP接口配置，保存到flash.
	wifi_softap_set_config_current(wifiAP_config构建体地址):设置wifi_AP接口配置，不保存到flash.	
Called By: // 调用本函数的函数清单
	user_main.c: user_init(void)
*************************************************/
void ICACHE_FLASH_ATTR
wifi_AP(void)
{
	struct softap_config wifiAP_config;
	//wifi_set_opmode(STATIONAP_MODE);  //混合模式
	wifi_set_opmode_current(SOFTAP_MODE);//ap模式
	wifi_station_set_auto_connect(BU_LIAN_JIE);  
	wifi_set_sleep_type(NONE_SLEEP_T);                     
	wifiAP_config.ssid_len = 12;
	os_strcpy(wifiAP_config.ssid, "chenxin12345");
	os_strcpy(wifiAP_config.password, "12345678");
	wifiAP_config.authmode = 3;
	wifiAP_config.beacon_interval = 100;
	wifiAP_config.channel = 1;
	wifiAP_config.max_connection = 3;
	wifiAP_config.ssid_hidden = 0;
	//wifi_softap_set_config(&apConfig);
	wifi_softap_set_config_current(&wifiAP_config);

}

/*连接AP后的执行回调，在启动的时候也至少会执行一次*/
void ICACHE_FLASH_ATTR
station_cnnect_callback(uint8_t status)
{
	//struct espconn tcp_client;
	uint8 wifi_zhuang_tai;
	wifi_zhuang_tai = wifi_station_get_connect_status();
	if (wifi_zhuang_tai == STATION_GOT_IP)
	{
		struct ip_info station_ip;
		wifi_get_ip_info(STATION_IF, &station_ip);//获取IP
		//os_timer_disarm(&tcp_timer); //取消定时器
		os_printf("tcpIP  %d\n", station_ip.ip);
        #if TCP_CLIENT
		tcp_client_init(&tcp_client, TCP_SERVER_IP, &station_ip.ip, TCP_SERVER_PORT);
        #elif TCP_SERVER
		//tcp_server_init1(&tcp_server, &station_ip.ip, LOCAL_PORT);
		tcp_server_init(&tcp_server, LOCAL_PORT);
        #endif
		os_printf("wifi_chenggong_cnnect");
		//os_printf("IP Address :"IPSTR"\r\n", IP2STR(ipaddr));//打印IP
	}
}

/*程序入口*/
void ICACHE_FLASH_ATTR
user_init(void)
{
	uart_init(115200, 115200);
	os_delay_us(50);
    os_printf("ESP_SDK version:%s\n", system_get_sdk_version());
	//输出芯片ID(唯一),%d 输出十进制整数
	os_printf("ID version:%d\n", system_get_chip_id());
	
	//wiFi_lianJie();
	lian_jie_AP(STA_SSID, STA_PASS, station_cnnect_callback);
	//wifi_AP();
	os_printf("\n\nHello World0003! KO\n\n");
}

/********************   内存布局 不要改动  开始          *********/
#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
#elif (SPI_FLASH_SIZE_MAP == 3)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 4)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000   
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
/*   预定义地址分布参数
 * 0x6A000=434176=106
 * 0x101000=1052672字节=257扇区
 * 0x3fb000=4173824=1019
 * 0x3fc000=4177920=1020
 * 0x3fd000=4182016=1021
 *******************************/
#else
#error "The flash map is not supported"
#endif

static const partition_item_t at_partition_table[] = {
	/*要分配系统空间名字                                分配的地址                                            分配的内存空间大小*/
	{ SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
	{ SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
	{ SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
	{ SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
	{ SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
	{ SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};
/*注册分区表*/
void ICACHE_FLASH_ATTR user_pre_init(void)
{
	if (!system_partition_table_regist(at_partition_table, sizeof(at_partition_table) / sizeof(at_partition_table[0]), SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while (1);
	}
}
/********************   内存布局 不要改动  结束          *********/