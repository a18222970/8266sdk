/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>   
File name:user_main.c 
Author: cx           
Version: 1.0        
Date: 2019.10.27
Description: DS18B20温度,dht11温湿度读取;
Function List: 
1. hw_test_timer_cb(void) 读取,串口打印DS18B20温度;dht11 温湿度
*************************************************/

#include "ets_sys.h"
#include "osapi.h"
#include "ip_addr.h"
#include "espconn.h"
#include "mem.h"
#include "user_config.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "modules/ds18b20.h"


/*定义软定时器*/
static ETSTimer os_timer;  //重新构建定时器TCP_timer

void ICACHE_FLASH_ATTR 
hw_test_timer_cb(void)
{
	os_printf("ds18b20 wen_du : %d \n\n", (int)(Ds18b20ReadTemp() * 0.0625 + 0.005));
	os_printf("- - - - - - - - - - - - - - - - - - - - \r\n");
}

/*程序入口*/
void ICACHE_FLASH_ATTR
user_init(void)
{
	uart_init(115200, 115200);
    os_printf("ESP_SDK version:%s\n", system_get_sdk_version());
	Ds18b20Init(); //Ds18b20温度读取

	/** 关闭该定时器 */
	os_timer_disarm(&os_timer);
	/** 配置该定时器回调函数 */
	os_timer_setfn(&os_timer, (ETSTimerFunc *) (hw_test_timer_cb), NULL);
	/** 启动该定时器 */
	os_timer_arm(&os_timer, 6000, true);
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
