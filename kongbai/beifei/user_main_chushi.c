/*
 * user_main_chushi.c
 *
 *  Created on: 2019年10月3日
 *      Author: Administrator
 */



/**  #include
 * #include 命令告诉预处理器将指定头文件的内容插入到预处理器命令的相应位置
 * 如果需要包含标准库头文件或者实现版本所提供的头文件，应该使用#include <文件名>
 * 如果需要包含针对程序所开发的源文件，则应该使用#include "文件名"
 * 对于命令中用双引号指定的文件（"文件名"），预处理器通常首先在当前目录下寻找，也就是包含该程序其他源文件的目录
 * 对于命令中使用尖括号指定的文件（<文件名>），预处理器通常会在特定的系统路径下搜索
 */
#include "ets_sys.h"
#include "osapi.h"
#include "ip_addr.h"
#include "espconn.h"
#include "mem.h"


#include "user_interface.h"
#include "smartconfig.h"
#include "airkiss.h"

/**  #define
 * #define 定义一个标识符来表示一个常量。其特点是：定义的标识符不占内存，只是一个临时的符号，预编译后这个符号就不存在了。
 * #define  标识符  常量   //注意, 最后没有分号
 * #define 和 #include 一样，也是以“#”开头的。凡是以“#”开头的均为预处理指令
 * #define又称宏定义，标识符为所定义的宏名，简称宏,变量名表示的是一个变量，但宏名表示的是一个常量。可以给变量赋值，但绝不能给常量赋值
 * 宏所表示的常量可以是数字、字符、字符串、表达式。其中最常用的是数字
 */
#define DEVICE_TYPE 		"gh_9e2cff3dfa51" //wechat public number
#define DEVICE_ID 			"122475" //model ID

#define DEFAULT_LAN_PORT 	12476
//条件判断
#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
//#error是使编译器产生一个错误，在你的程序中上一句#if判断为真，于是编译器就执行了#error这句，导致编译报错
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
//其他条件判断
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
//判断结束
#else
#error "The flash map is not supported"

/**  #endif
 * #ifndef x //先测试x是否被宏定义过
 * #define x //程序段1 //如果x没有被宏定义过，定义x，并编译程序段1
 * #endif  程序段2 //如果x已经定义过了则编译程序段2的语句，“忽视”程序段1。
 *
 */
#endif

/** static const
 * static在函数内的时候，表明这个变量在函数的生命周期结束之后也不会被释放
 * static在函数外的时候，表明这个变量的作用域只在该.c文件里，不能作用于整个工程
 * const规定了一个变量在它初始化值之后，值不能再改变，也就是只读。
 */
static const partition_item_t at_partition_table[] = {
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};

void ICACHE_FLASH_ATTR
user_pre_init(void)
{
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}


void ICACHE_FLASH_ATTR
user_init(void)
{
    os_printf("SDK version:%s\n", system_get_sdk_version());
    os_printf("\n\nHello World! KO\n\n");
    os_printf("\n\nchenxin! KO01\n\n");
//	smartconfig_set_type(SC_TYPE_AIRKISS); //SC_TYPE_ESPTOUCH,SC_TYPE_AIRKISS,SC_TYPE_ESPTOUCH_AIRKISS
//    wifi_set_opmode(STATION_MODE);
//    smartconfig_start(smartconfig_done);
}
