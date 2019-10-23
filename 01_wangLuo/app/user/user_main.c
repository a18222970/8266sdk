/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>    //��Ȩ
File name: wangLuo.c // �ļ���
Author: cx           // ����
Version: 1.0        //�汾
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



#define DEVICE_TYPE 		"gh_9e2cff3dfa51" //wechat public number
#define DEVICE_ID 			"122475" //model ID

#define DEFAULT_LAN_PORT 	12476
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
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}
/********************   �ڴ沼�� ��Ҫ�Ķ�  ����          *********/


/*������ʱ��*/
os_timer_t tcpServer;

void ICACHE_FLASH_ATTR server_recon1(void* arg, sint8 error) {
	os_printf("tcp server connect tcp client error %d\r\n", error);
}
void ICACHE_FLASH_ATTR server_recv1(void* arg, char* pdata, unsigned short len) {
	os_printf("tcp client receive tcp server data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n", len, pdata);
}
void ICACHE_FLASH_ATTR server_sent1(void* arg) {
	os_printf("send data succeed��\r");

}
void ICACHE_FLASH_ATTR server_discon1(void* arg) {
	os_printf("conect diable! \r");
}
void ICACHE_FLASH_ATTR server_listen1(void* arg) //ע�� TCP ���ӳɹ�������Ļص�����
{
	struct espconn* pespconn = arg;
	espconn_regist_recvcb(pespconn, server_recv1); //����
	espconn_regist_sentcb(pespconn, server_sent1); //����
	espconn_regist_disconcb(pespconn, server_discon1); //�Ͽ�
	os_printf("a0003\n");
}

void ICACHE_FLASH_ATTR
tCP_server1(void)
{
	os_printf("a0002");
	os_printf("lianJieGeShu : %d\n", wifi_softap_get_station_num());
	struct espconn user_tcp_espconn;  //�ṹ��espconn����Ϊ����user_tcp_espconn
	uint32 server_ip = ipaddr_addr(TCP_SERVER_IP);
	/* "."һ������¶���"�ġ� ��->��һ�����"ָ��Ľṹ���" */
	user_tcp_espconn.proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp)); //����ռ�
	user_tcp_espconn.type = ESPCONN_TCP; //��������ΪTCPЭ��
	os_memcpy(user_tcp_espconn.proto.tcp->remote_ip, TCP_SERVER_IP, 4);//����Ҫ���ӵ�Server IP��ַ
	user_tcp_espconn.proto.tcp->remote_port = 6666;//����Ҫ���ӵ�Server �˿ں�
	user_tcp_espconn.proto.tcp->local_port = 9266; //���ض˿�

	//ע�����ӳɹ��ص��������������ӻص�����
	espconn_regist_connectcb(&user_tcp_espconn, server_listen1); //ע�� TCP ���ӳɹ�������Ļص�����
	espconn_regist_reconcb(&user_tcp_espconn, server_recon1); //ע�� TCP ���ӷ����쳣�Ͽ�ʱ�Ļص������������ڻص������н�������
	espconn_connect(&user_tcp_espconn); //���� TCP 
	
}

/*wifi����_�ͻ���ģʽ_����·����*/
void ICACHE_FLASH_ATTR
wiFi_lianJie1(void)
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
	tCP_server1();

	/*ϵͳ����ʱ����ӡ������Ϣ*/
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
	//	os_timer_disarm(&tcpServer);
	//	os_timer_setfn(&tcpServer, (os_timer_t*)tCP_server, NULL);
	//	os_timer_arm(&tcpServer, 1000, true);

}

void ICACHE_FLASH_ATTR
abcasd(uint8_t status)
{
	os_printf("wifi_lianjie");
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
	
	//wiFi_lianJie1();
	lian_jie_AP(STA_SSID, STA_PASS, abcasd);
	//lian_jie_AP1(STA_SSID, STA_PASS);
	//cs01();
	os_printf("\n\nHello World0003! KO\n\n");
}
