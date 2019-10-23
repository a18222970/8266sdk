/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>    //版权
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

#include "ets_sys.h"
#include "osapi.h"
#include "ip_addr.h"
#include "espconn.h"
#include "mem.h"
#include "user_config.h"

#include "user_interface.h"
#include "driver/uart.h"
#include "modules/wangLuo.h"



#define DEVICE_TYPE 		"gh_9e2cff3dfa51" //wechat public number
#define DEVICE_ID 			"122475" //model ID

#define DEFAULT_LAN_PORT 	12476
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
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}
/********************   内存布局 不要改动  结束          *********/


/*定义软定时器*/
os_timer_t tcpServer;

void ICACHE_FLASH_ATTR server_recon1(void* arg, sint8 error) {
	os_printf("tcp server connect tcp client error %d\r\n", error);
}
void ICACHE_FLASH_ATTR server_recv1(void* arg, char* pdata, unsigned short len) {
	os_printf("tcp client receive tcp server data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n", len, pdata);
}
void ICACHE_FLASH_ATTR server_sent1(void* arg) {
	os_printf("send data succeed！\r");

}
void ICACHE_FLASH_ATTR server_discon1(void* arg) {
	os_printf("conect diable! \r");
}
void ICACHE_FLASH_ATTR server_listen1(void* arg) //注册 TCP 连接成功建立后的回调函数
{
	struct espconn* pespconn = arg;
	espconn_regist_recvcb(pespconn, server_recv1); //接收
	espconn_regist_sentcb(pespconn, server_sent1); //发送
	espconn_regist_disconcb(pespconn, server_discon1); //断开
	os_printf("a0003\n");
}

void ICACHE_FLASH_ATTR
tCP_server1(void)
{
	os_printf("a0002");
	os_printf("lianJieGeShu : %d\n", wifi_softap_get_station_num());
	struct espconn user_tcp_espconn;  //结构体espconn命名为变量user_tcp_espconn
	uint32 server_ip = ipaddr_addr(TCP_SERVER_IP);
	/* "."一般情况下读作"的” “->”一般读作"指向的结构体的" */
	user_tcp_espconn.proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp)); //分配空间
	user_tcp_espconn.type = ESPCONN_TCP; //设置类型为TCP协议
	os_memcpy(user_tcp_espconn.proto.tcp->remote_ip, TCP_SERVER_IP, 4);//设置要连接的Server IP地址
	user_tcp_espconn.proto.tcp->remote_port = 6666;//设置要连接的Server 端口号
	user_tcp_espconn.proto.tcp->local_port = 9266; //本地端口

	//注册连接成功回调函数和重新连接回调函数
	espconn_regist_connectcb(&user_tcp_espconn, server_listen1); //注册 TCP 连接成功建立后的回调函数
	espconn_regist_reconcb(&user_tcp_espconn, server_recon1); //注册 TCP 连接发生异常断开时的回调函数，可以在回调函数中进行重连
	espconn_connect(&user_tcp_espconn); //创建 TCP 
	
}

/*wifi连接_客户端模式_连接路由器*/
void ICACHE_FLASH_ATTR
wiFi_lianJie1(void)
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
	tCP_server1();

	/*系统崩溃时，打印出错信息*/
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
	//	os_timer_disarm(&tcpServer);
	//	os_timer_setfn(&tcpServer, (os_timer_t*)tCP_server, NULL);
	//	os_timer_arm(&tcpServer, 1000, true);

}

void ICACHE_FLASH_ATTR
abcasd(uint8_t status)
{
	os_printf("wifi_lianjie");
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
	
	//wiFi_lianJie1();
	lian_jie_AP(STA_SSID, STA_PASS, abcasd);
	//lian_jie_AP1(STA_SSID, STA_PASS);
	//cs01();
	os_printf("\n\nHello World0003! KO\n\n");
}
