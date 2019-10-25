/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>   
File name: wangLuo.c 
Author: cx           
Version: 1.0        
Date: 2019.10.23    //�������
Description: // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
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

/*������ʱ��*/
static ETSTimer TCP_timer;  //���¹�����ʱ��TCP_timer

/*�ж���ʲô����*/
#if   TCP_CLIENT
struct espconn tcp_client; //�ͻ���ģʽ
#elif TCP_SERVER
struct espconn tcp_server; //�����ģʽ
//����
#endif

/*wifi����_�ͻ���ģʽ_����·����*/
void ICACHE_FLASH_ATTR
wiFi_lianJie(void)
{
	/*NULL_MODE       0x00  �ر�wifi����
	  STATION_MODE    0x01  �ͻ���ģʽ_����·����
	  SOFTAP_MODE     0x02  APģʽ_�ȵ�
	  STATIONAP_MODE  0x03  ���ģʽ  */
	wifi_set_opmode(STATION_MODE);  //����wifi����ģʽ������
	char ssid[32] = "cccxxx";
	char password[64] = "chenxin1234567890";

	struct station_config stationConf;
	stationConf.bssid_set = 0;                     //����Ҫ���ap��mac��ַ
	os_memcpy(&stationConf.ssid, ssid, 32);        //os_memcpy �ڴ濽����䡣
	os_memcpy(&stationConf.password, password, 64);
	//wifi_station_set_config(&stationConf);       //����Wi-Fi Station���������浽flash
	wifi_station_set_config_current(&stationConf);//����Wi-Fi Station�����������浽flash
	wifi_station_connect();
	os_printf("duankou :OK\n");

	/***********ϵͳ������ʾ����ʱ����ӡ������Ϣ***************
	 * ��ӡ������Ϣ��
	 * reset reason: 6  �ⲿϵͳ��λ
	 * 				 0 ��Դ��������
	 * 				 1 Ӳ�����Ź���λ
	 *               2 �쳣��λ��gpio״̬����
	 *               3 ������Ź���λ��gpio״̬����
	 *               4 ���������system_restart, gpio״̬����
	 *               5 �ӳ�˯������
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
		//��ӡ�ϴα����ĵ�ַ�������ڱ���ʱ�������
		os_printf("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x,excvaddr=0x%08x, depc=0x%08x\n",
			rtc_info->epc1, rtc_info->epc2, rtc_info->epc3, rtc_info->excvaddr, rtc_info->depc);
	}
}

/*************************************************
Function: wifi_AP 
Description: wifi������APģʽ
Calls:// �����������õĺ����嵥
	wifi_set_opmode_current(): ����wifi����ģʽ�������浽flash;
							   NULL_MODE       0x00  �ر�wifi����
							   STATION_MODE    0x01  �ͻ���ģʽ_����·����
							   SOFTAP_MODE     0x02  APģʽ_�ȵ�
							   STATIONAP_MODE  0x03  ���ģʽ
    wifi_set_opmode(): ����wifi����ģʽ�����浽flash;
		wifi_set_sleep_type(): ����˯��ģʽ
	wifi_station_set_auto_connect()�� �����ϵ��Ƿ�����·������Ĭ���Զ����ӡ�
	                                  //����ڷ�user_init(void)�е��ã������´�����ʱ��Ч
	                             1 �Զ�����
								 0 ���Զ�����
	os_strcpy():�ѵڶ����ַ������Ƶ���һ���ַ���������Ϊָ�룬ԭ������
			   //char *ets_strcpy(char *s1, const char *s2);
	wifi_softap_set_config(wifiAP_config�������ַ):����wifi_AP�ӿ����ã����浽flash.
	wifi_softap_set_config_current(wifiAP_config�������ַ):����wifi_AP�ӿ����ã������浽flash.	
Called By: // ���ñ������ĺ����嵥
	user_main.c: user_init(void)
*************************************************/
void ICACHE_FLASH_ATTR
wifi_AP(void)
{
	struct softap_config wifiAP_config;
	//wifi_set_opmode(STATIONAP_MODE);  //���ģʽ
	wifi_set_opmode_current(SOFTAP_MODE);//apģʽ
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

/*����AP���ִ�лص�����������ʱ��Ҳ���ٻ�ִ��һ��*/
void ICACHE_FLASH_ATTR
station_cnnect_callback(uint8_t status)
{
	//struct espconn tcp_client;
	uint8 wifi_zhuang_tai;
	wifi_zhuang_tai = wifi_station_get_connect_status();
	if (wifi_zhuang_tai == STATION_GOT_IP)
	{
		struct ip_info station_ip;
		wifi_get_ip_info(STATION_IF, &station_ip);//��ȡIP
		//os_timer_disarm(&tcp_timer); //ȡ����ʱ��
		os_printf("tcpIP  %d\n", station_ip.ip);
        #if TCP_CLIENT
		tcp_client_init(&tcp_client, TCP_SERVER_IP, &station_ip.ip, TCP_SERVER_PORT);
        #elif TCP_SERVER
		//tcp_server_init1(&tcp_server, &station_ip.ip, LOCAL_PORT);
		tcp_server_init(&tcp_server, LOCAL_PORT);
        #endif
		os_printf("wifi_chenggong_cnnect");
		//os_printf("IP Address :"IPSTR"\r\n", IP2STR(ipaddr));//��ӡIP
	}
}

/*�������*/
void ICACHE_FLASH_ATTR
user_init(void)
{
	uart_init(115200, 115200);
	os_delay_us(50);
    os_printf("ESP_SDK version:%s\n", system_get_sdk_version());
	//���оƬID(Ψһ),%d ���ʮ��������
	os_printf("ID version:%d\n", system_get_chip_id());
	
	//wiFi_lianJie();
	lian_jie_AP(STA_SSID, STA_PASS, station_cnnect_callback);
	//wifi_AP();
	os_printf("\n\nHello World0003! KO\n\n");
}

/********************   �ڴ沼�� ��Ҫ�Ķ�  ��ʼ          *********/
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
/*   Ԥ�����ַ�ֲ�����
 * 0x6A000=434176=106
 * 0x101000=1052672�ֽ�=257����
 * 0x3fb000=4173824=1019
 * 0x3fc000=4177920=1020
 * 0x3fd000=4182016=1021
 *******************************/
#else
#error "The flash map is not supported"
#endif

static const partition_item_t at_partition_table[] = {
	/*Ҫ����ϵͳ�ռ�����                                ����ĵ�ַ                                            ������ڴ�ռ��С*/
	{ SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
	{ SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
	{ SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
	{ SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
	{ SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
	{ SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};
/*ע�������*/
void ICACHE_FLASH_ATTR user_pre_init(void)
{
	if (!system_partition_table_regist(at_partition_table, sizeof(at_partition_table) / sizeof(at_partition_table[0]), SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while (1);
	}
}
/********************   �ڴ沼�� ��Ҫ�Ķ�  ����          *********/