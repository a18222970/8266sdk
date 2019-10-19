/*
 * user_main_chushi.c
 *
 *  Created on: 2019��10��3��
 *      Author: Administrator
 */



/**  #include
 * #include �������Ԥ��������ָ��ͷ�ļ������ݲ��뵽Ԥ�������������Ӧλ��
 * �����Ҫ������׼��ͷ�ļ�����ʵ�ְ汾���ṩ��ͷ�ļ���Ӧ��ʹ��#include <�ļ���>
 * �����Ҫ������Գ�����������Դ�ļ�����Ӧ��ʹ��#include "�ļ���"
 * ������������˫����ָ�����ļ���"�ļ���"����Ԥ������ͨ�������ڵ�ǰĿ¼��Ѱ�ң�Ҳ���ǰ����ó�������Դ�ļ���Ŀ¼
 * ����������ʹ�ü�����ָ�����ļ���<�ļ���>����Ԥ������ͨ�������ض���ϵͳ·��������
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
 * #define ����һ����ʶ������ʾһ�����������ص��ǣ�����ı�ʶ����ռ�ڴ棬ֻ��һ����ʱ�ķ��ţ�Ԥ�����������žͲ������ˡ�
 * #define  ��ʶ��  ����   //ע��, ���û�зֺ�
 * #define �� #include һ����Ҳ���ԡ�#����ͷ�ġ������ԡ�#����ͷ�ľ�ΪԤ����ָ��
 * #define�ֳƺ궨�壬��ʶ��Ϊ������ĺ�������ƺ�,��������ʾ����һ����������������ʾ����һ�����������Ը�������ֵ���������ܸ�������ֵ
 * ������ʾ�ĳ������������֡��ַ����ַ��������ʽ��������õ�������
 */
#define DEVICE_TYPE 		"gh_9e2cff3dfa51" //wechat public number
#define DEVICE_ID 			"122475" //model ID

#define DEFAULT_LAN_PORT 	12476
//�����ж�
#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
//#error��ʹ����������һ����������ĳ�������һ��#if�ж�Ϊ�棬���Ǳ�������ִ����#error��䣬���±��뱨��
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
//���������ж�
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
//�жϽ���
#else
#error "The flash map is not supported"

/**  #endif
 * #ifndef x //�Ȳ���x�Ƿ񱻺궨���
 * #define x //�����1 //���xû�б��궨���������x������������1
 * #endif  �����2 //���x�Ѿ�����������������2����䣬�����ӡ������1��
 *
 */
#endif

/** static const
 * static�ں����ڵ�ʱ�򣬱�����������ں������������ڽ���֮��Ҳ���ᱻ�ͷ�
 * static�ں������ʱ�򣬱������������������ֻ�ڸ�.c�ļ��������������������
 * const�涨��һ������������ʼ��ֵ֮��ֵ�����ٸı䣬Ҳ����ֻ����
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
