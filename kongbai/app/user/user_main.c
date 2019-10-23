
/**C���� main.c���������һc�ļ��еĺ���
 * ���main.c��Ҫ����b.c�е�void led_gpio_config��������������Ҫ�½�һ��b.h����b.h�н�����b.c��������������
 * ����b.c���к���void led_gpio_config()������b.h���������£�void led_gpio_config(��������û�в�����void)��
 * Ȼ����main.c�� include"b.h" ����
 */
/**  #include
 * #include �������Ԥ��������ָ��ͷ�ļ������ݲ��뵽Ԥ�������������Ӧλ��
 * �����Ҫ������׼��ͷ�ļ�����ʵ�ְ汾���ṩ��ͷ�ļ���Ӧ��ʹ��#include <�ļ���>
 * �����Ҫ������Գ�����������Դ�ļ�����Ӧ��ʹ��#include "�ļ���"
 * ������������˫����ָ�����ļ���"�ļ���"����Ԥ������ͨ�������ڵ�ǰĿ¼��Ѱ�ң�Ҳ���ǰ����ó�������Դ�ļ���Ŀ¼
 * ����������ʹ�ü�����ָ�����ļ���<�ļ���>����Ԥ������ͨ�������ض���ϵͳ·��������
 */
#include "ets_sys.h"
#include "osapi.h" //����1��Ҫ��ͷ�ļ�
#include "ip_addr.h"
#include "espconn.h"//TCP������Ҫ��ͷ�ļ�
#include "mem.h" //ϵͳ������Ҫ��ͷ�ļ�


#include "user_interface.h"//WIFI������Ҫ��ͷ�ļ�
#include "smartconfig.h" //��������
#include "airkiss.h" //����
#include "modules/wifi.h"
#include "user_config.h" //�����ļ�
#include "modules/tcpServerClient.h"
#include "modules/udp.h"
#include "cJSON.h"


//#include "gpio.h"//GPIO�ڲ���
#include "modules/gpio_test.h"

//���ö�ʱ������00000000000011111
os_timer_t led_01_timer;

/**  #define
 * #define ����һ����ʶ������ʾһ�����������ص��ǣ�����ı�ʶ����ռ�ڴ棬ֻ��һ����ʱ�ķ��ţ�Ԥ�����������žͲ������ˡ�
 * #define  ��ʶ��  ����   //ע��, ���û�зֺ�
 * #define �� #include һ����Ҳ���ԡ�#����ͷ�ġ������ԡ�#����ͷ�ľ�ΪԤ����ָ��
 * #define�ֳƺ궨�壬��ʶ��Ϊ������ĺ�������ƺ�,��������ʾ����һ����������������ʾ����һ�����������Ը�������ֵ���������ܸ�������ֵ
 * ������ʾ�ĳ������������֡��ַ����ַ��������ʽ��������õ�������
 */
#define DEVICE_TYPE 		"gh_9e2cff3dfa51" //wechat public number  ΢�Ź�������
#define DEVICE_ID 			"122475" //model ID  ģ��ID

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
/*   Ԥ�����ַ�ֲ�����
 *  0x6A000=434176=106
 * 0x101000=1052672�ֽ�=257����
 * 0x3fb000=4173824=1019
 * 0x3fc000=4177920=1020
 * 0x3fd000=4182016=1021
 *******************************/
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
 *
 * ���Ϊ���������
 */
static const partition_item_t at_partition_table[] = {
		/*Ҫ����ϵͳ�ռ�����                                                                                                      ����ĵ�ַ                                                                                                                                           ������ڴ�ռ��С*/
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};



void Led_cmd(bool status){
	if (status == true){
		GPIO_OUTPUT_SET(GPIO_ID_PIN(12),0);
	}else{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(12),1);
	}
}

void Led_Task_Run(void){

	static bool status = false;
      if ( status == true ) {
	        status = false;
	    } else  {
	        status = true;
	    }
     Led_cmd( status );
}

uint16 cs1 = 0;
static void ICACHE_FLASH_ATTR
//��ʱ�ص�����
blue_led_timer_toggle1(void){
		 /**��ʱ��
		 * ����ͬһ����ʱ����os_time_aem��os_timer_arm_us�����ظ����ã���������os_time_disarmȡ����ʱ����
		 * ��ʱ���ص�os_timer_setfn����Ҫ�У����ұ����ڶ�ʱ��δʹ�ܵ�����µ��ã�
		 * �� os_timer_arm ��os_timer_arm_us ֮ǰ���� os_timer_disarm ֮��
		 *os_timer_disarm ȡ����ʱ��
		 *os_timer_setfn ��ʱ���ص�
		 *os_timer_arm   ʹ�ܺ��뼶��ʱ��
		 *os_timer_arm_us ʹ��΢���ʱ��
		 *system_timer_reinit ���³�ʼ����ʱ������ʹ��΢���ʱ��ʱ���á�
		 */
	os_timer_disarm(&led_01_timer);//ȡ����ʱ��
	uint32 status = GPIO_INPUT_GET(GPIO_ID_PIN(2));//��ȡ���ƹܽ�״̬
	GPIO_OUTPUT_SET(GPIO_ID_PIN(2),!status);//ȡ��ʵ�����ƹܽŵ�ƽ��ת���Ӷ�ʵ���������
	//os_printf("\n\nchenxin! KO0001 %d gg\n\n",cs1);
	if(status == 1){
		os_printf("\nchenxin! KO0001 %d gg\n",cs1);
		cs1++;
	}

	os_timer_arm(&led_01_timer, 1000, true);//ʹ�ܶ�ʱ��
}

//���
#if   TCP_CLIENT
/*******************  struct  *****************************************************
 * ��C�����У�����ʹ�ýṹ�壨Struct�������һ�鲻ͬ���͵�����
 * �ṹ����һ�ּ��ϣ�����������˶�����������飬���ǵ����Ϳ�����ͬ��Ҳ���Բ�ͬ
    struct stu{
    char *name;  //����
    int num;  //ѧ��
    int age;  //����
    char group;  //����ѧϰС��
    float score;  //�ɼ�
    };
    ��Ȼ�ṹ����һ���������ͣ���ô�Ϳ������������������
    struct stu stu1;����struct stu stu1, stu2;
    stu1�����Ա�ʾstu
    stu ����һ����ģ�塱����������ı�����������ͬ������
    Ҳ�����ڶ���ṹ���ͬʱ����ṹ�����
    struct stu{
    char *name;  //����
    int num;  //ѧ��
    int age;  //����
    }stu1;
 ************************************************************************************/
struct espconn tcp_client;
//���������ж�
#elif TCP_SERVER
struct espconn *tcp_server;
#elif UDP_TEST
struct espconn udp_test;
//����
#endif

uint8  messages_send_buffer[50];
uint16 messages_send_count = 0;

//���붨ʱ����os_timer_t ��ʱ���ṹ
os_timer_t wifistate_checktimer;
os_timer_t send_data_timer;

//û�з���ֵ�ĺ�����������Ϊ void
void ICACHE_FLASH_ATTR
//����ԭ�͵������������б��еĹؼ��� void ��ʾ�ú���û�в���
//tcp����
TCP_Send_data(void){

#if   TCP_CLIENT
	//os_sprintf ��һ���ַ����滻����һ���ַ���
	os_sprintf(messages_send_buffer,"hi this is ESP8266 TCP client![%d]\r\n",messages_send_count);//23
	//strlen ��һ������������������ָ���ַ��� str �ĳ��ȣ��������������ַ����� null �ַ���
	tcp_client_send_data(&tcp_client,messages_send_buffer,strlen(messages_send_buffer));
#elif TCP_SERVER
	os_sprintf(messages_send_buffer,"hi this is ESP8266 TCP server![%d]\r\n",messages_send_count);
	tcp_server_send_data(tcp_server,messages_send_buffer,strlen(messages_send_buffer));
#endif
	messages_send_count++;
}

#if UDP_TEST
void ICACHE_FLASH_ATTR
UDP_Send_Messages(void){
	os_sprintf(messages_send_buffer,"hi this is ESP8266 UDP send messages test![%d]\r\n",messages_send_count);
	udp_send_data(&udp_test,messages_send_buffer,strlen(messages_send_buffer));
	messages_send_count++;
}
#endif

//TCP�Զ�������Ϣ�ص�
void ICACHE_FLASH_ATTR
WifiStatus_Check(void){
	uint8 wifiStatus;

//	uint8 *macaddr;

	wifiStatus = wifi_station_get_connect_status(); //8266wifi�ͻ��˽ӿ�״̬
	if (wifiStatus == STATION_GOT_IP) {
		os_printf("WiFi connection is successful!\r\n");//17
		os_timer_disarm(&wifistate_checktimer);//ȡ����ʱ��
		struct ip_info local_ip;
		wifi_get_ip_info(STATION_IF,&local_ip);  //��ѯ�豸IP

//		wifi_get_macaddr(STATION_IF,macaddr);
//		os_sprintf("MAC version:%s\n",MAC2STR(macaddr));

#if   TCP_CLIENT
		tcp_client_init(&tcp_client,TCP_SERVER_IP,&local_ip.ip,TCP_SERVER_PORT);
		os_timer_disarm(&send_data_timer);//ȡ����ʱ��
		os_timer_setfn(&send_data_timer, (os_timer_func_t *) TCP_Send_data,NULL);//��ʱ���ص�����
		os_timer_arm(&send_data_timer, 40000, true);//ʹ�ܺ��뼶��ʱ��,1000���� = 1��
#elif TCP_SERVER
		tcp_server_init(tcp_server,TCP_LOCAL_PORT);
		//tcp_client_init1(&tcp_server,TCP_SERVER_IP,&local_ip.ip,TCP_LOCAL_PORT);
		//ȡ����ʱ��
		os_timer_disarm(&send_data_timer);
		//��ʱ���ص�����
		os_timer_setfn(&send_data_timer, (os_timer_func_t *) TCP_Send_data,NULL);
		//ʹ�ܺ��뼶��ʱ��
		os_timer_arm(&send_data_timer, 2000, true);
#elif UDP_TEST
		udp_init(&udp_test,UDP_REMOTE_IP,&local_ip.ip,UDP_REMOTE_PORT);
		os_timer_disarm(&send_data_timer);
		os_timer_setfn(&send_data_timer, (os_timer_func_t *)UDP_Send_Messages,NULL);
		os_timer_arm(&send_data_timer, 2000, true);
#endif

	}else{
		os_printf("WiFi connection failed!\r\n"); //7,15
	}
}

void ICACHE_FLASH_ATTR
server_recon(void *arg,sint8 error){
	os_printf("tcp server connect tcp client error %d\r\n",error);
}
void ICACHE_FLASH_ATTR server_recv(void* arg, char* pdata, unsigned short len) {
	os_printf("length: %d \r\ndata: %s\r\n", len, pdata);
}
void ICACHE_FLASH_ATTR server_sent(void* arg) {
	os_printf("send data succeed��\r");
	
}
void ICACHE_FLASH_ATTR server_discon(void* arg) {
	os_printf("conect diable! \r");
}
void ICACHE_FLASH_ATTR server_listen(void* arg) //ע�� TCP ���ӳɹ�������Ļص�����
{
	struct espconn* pespconn = arg;
	espconn_regist_recvcb(pespconn, server_recv); //����
	espconn_regist_sentcb(pespconn, server_sent); //����
	espconn_regist_disconcb(pespconn, server_discon); //�Ͽ�
}
void Inter213_InitTCP(uint32_t Local_port) {

	//����ռ�
	struct espconn *user_tcp_espconn;  //�ṹ��espconn��ָ�����user_tcp_espconn
	/* "."һ������¶���"�ġ� ��->��һ�����"ָ��Ľṹ���" */
	user_tcp_espconn->proto.tcp = (esp_tcp *) os_zalloc(sizeof(esp_tcp)); //����ռ�
		user_tcp_espconn->type = ESPCONN_TCP; //��������ΪTCPЭ��

		user_tcp_espconn->proto.tcp->local_port = Local_port; //���ض˿�
		os_printf("Fatal exception gggggggggggggggggggggggg\n");
		
		//ע�����ӳɹ��ص��������������ӻص�����
		espconn_regist_connectcb(user_tcp_espconn, server_listen); //ע�� TCP ���ӳɹ�������Ļص�����
		espconn_regist_reconcb(user_tcp_espconn, server_recon); //ע�� TCP ���ӷ����쳣�Ͽ�ʱ�Ļص������������ڻص������н�������
		espconn_accept(user_tcp_espconn); //���� TCP server����������
		espconn_regist_time(user_tcp_espconn, 80, 0); //���ó�ʱ�Ͽ�ʱ�� ��λ���룬���ֵ��7200 ��
		os_printf("Fatal exception vvvvvvvvvvvvvvvvvvvvvvvvv\n");
		//�����ʱʱ������Ϊ 0��ESP8266 TCP server ��ʼ�ղ���Ͽ��Ѿ�������ͨ�ŵ� TCP client������������ʹ�á�
}

void Inter213_InitTCP2(uint32_t Local_port) {
	struct espconn user_tcp_espconn;  //�ṹ��espconn����Ϊ����user_tcp_espconn
	/* "."һ������¶���"�ġ� ��->��һ�����"ָ��Ľṹ���" */
	user_tcp_espconn.proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp)); //����ռ�
	user_tcp_espconn.type = ESPCONN_TCP; //��������ΪTCPЭ��
	user_tcp_espconn.proto.tcp->local_port = Local_port; //���ض˿�

	//ע�����ӳɹ��ص��������������ӻص�����
	espconn_regist_connectcb(&user_tcp_espconn, server_listen); //ע�� TCP ���ӳɹ�������Ļص�����
	espconn_regist_reconcb(&user_tcp_espconn, server_recon); //ע�� TCP ���ӷ����쳣�Ͽ�ʱ�Ļص������������ڻص������н�������
	espconn_accept(&user_tcp_espconn); //���� TCP server����������
	espconn_regist_time(&user_tcp_espconn, 180, 0); //���ó�ʱ�Ͽ�ʱ�� ��λ���룬���ֵ��720 ��
	//�����ʱʱ������Ϊ 0��ESP8266 TCP server ��ʼ�ղ���Ͽ��Ѿ�������ͨ�ŵ� TCP client������������ʹ�á�

}

void ICACHE_FLASH_ATTR
tcp_server_init1(struct espconn* espconn, int local_port) {
	//struct espconn *tcp_server;
	os_printf("tcp server waiting tcp client connect!\r\n");
	/* "."һ������¶���"�ġ� ��->��һ�����"ָ��Ľṹ���" */
	espconn->proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;  //espconn->type   �ȼ��� *(espconn).type �� ->���ṹ�壬�࣬��ͬ��ȣ�ȡ��Ա�����
	espconn->proto.tcp->local_port = local_port;//���ñ��ؼ����Ķ˿ںţ��ȴ�Client����

	espconn_regist_connectcb(espconn, server_listen);//ע��Server�����ص�����
	espconn_regist_reconcb(espconn, server_recon);//ע������������ӻص�����
	espconn_accept(espconn);//����Server,��ʼ����
	espconn_regist_time(espconn, 360, 0);//��ʱ�Ͽ�����ʱ��
}


//TCP server�Զ�������Ϣ�ص�
void ICACHE_FLASH_ATTR
WifiStatus_Check_server(void) {
	struct softap_config apConfig;
		wifi_set_opmode(0x02);    //����ΪAPģʽ�����浽 flash
		apConfig.ssid_len = 10;						//����ssid����
		os_strcpy(apConfig.ssid, "xuhongWifi");	    //����ssid����
		os_strcpy(apConfig.password, "12345678");	//��������
		apConfig.authmode = 3;                      //���ü���ģʽ
		apConfig.beacon_interval = 100;            //�ű���ʱ��100 ~ 60000 ms
		apConfig.channel = 1;                      //ͨ����1 ~ 13
		apConfig.max_connection = 4;               //���������
		apConfig.ssid_hidden = 0;                  //����SSID

		wifi_softap_set_config(&apConfig);		//���� WiFi soft-AP �ӿ����ã������浽 flash
	    Inter213_InitTCP(8266);
		//tcp_server_init1(tcp_server, 8266);

	uint8 wifiStatus;
	/*************************��ʼ***************************************
	 * ��ӡ������Ϣ��
	 * reset reason: 6  �ⲿϵͳ��λ
	 * 				 0 ��Դ��������
	 * 				 1 Ӳ�����Ź���λ
	 *               2 �쳣��λ��gpio״̬����
	 *               3 ������Ź���λ��gpio״̬����
	 *               4 ���������system_restart, gpio״̬����
	 *               5 �ӳ�˯������
	 */
	struct rst_info* rtc_info = system_get_rst_info();
	os_printf("reset reason: %x\n", rtc_info->reason);
	if (rtc_info->reason == REASON_WDT_RST ||
		rtc_info->reason == REASON_EXCEPTION_RST ||
		rtc_info->reason == REASON_SOFT_WDT_RST) {
		if (rtc_info->reason == REASON_EXCEPTION_RST) {
			os_printf("Fatal exception (%d):\n", rtc_info->exccause);
		}
		//��ӡ�ϴα����ĵ�ַ�������ڱ���ʱ�������
		os_printf("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x,excvaddr=0x%08x, depc=0x%08x\n",
			rtc_info->epc1, rtc_info->epc2, rtc_info->epc3, rtc_info->excvaddr, rtc_info->depc);
	}
	/****************************����*********************************************/
		//os_timer_disarm(&wifistate_checktimer);//ȡ����ʱ��
		//struct ip_info local_ip;
		//wifi_get_ip_info(SOFTAP_IF, &local_ip);  //��ѯ�豸APģʽIP

//		wifi_get_macaddr(STATION_IF,macaddr);
//		os_sprintf("MAC version:%s\n",MAC2STR(macaddr));
/**
#if   TCP_CLIENT
		tcp_client_init(&tcp_client, TCP_SERVER_IP, &local_ip.ip, TCP_SERVER_PORT);
		os_timer_disarm(&send_data_timer);//ȡ����ʱ��
		os_timer_setfn(&send_data_timer, (os_timer_func_t*)TCP_Send_data, NULL);//��ʱ���ص�����
		os_timer_arm(&send_data_timer, 40000, true);//ʹ�ܺ��뼶��ʱ��,1000���� = 1��
#elif TCP_SERVER
		tcp_server_init(tcp_server, TCP_LOCAL_PORT);
		//tcp_client_init1(&tcp_server,TCP_SERVER_IP,&local_ip.ip,TCP_LOCAL_PORT);
		//ȡ����ʱ��
		os_timer_disarm(&send_data_timer);
		//��ʱ���ص�����
		os_timer_setfn(&send_data_timer, (os_timer_func_t*)TCP_Send_data, NULL);
		//ʹ�ܺ��뼶��ʱ��
		os_timer_arm(&send_data_timer, 3000, true);
#elif UDP_TEST
		udp_init(&udp_test, UDP_REMOTE_IP, &local_ip.ip, UDP_REMOTE_PORT);
		os_timer_disarm(&send_data_timer);
		os_timer_setfn(&send_data_timer, (os_timer_func_t*)UDP_Send_Messages, NULL);
		os_timer_arm(&send_data_timer, 2000, true);
#endif
*/
}

/**
 * Wi-Fi���ӻص�����
 */
void ICACHE_FLASH_ATTR
wifiConnectCb(uint8_t status){
	/*************************��ʼ***************************************
	 * ��ӡ������Ϣ��
	 * reset reason: 6  �ⲿϵͳ��λ
	 * 				 0 ��Դ��������
	 * 				 1 Ӳ�����Ź���λ
	 *               2 �쳣��λ��gpio״̬����
	 *               3 ������Ź���λ��gpio״̬����
	 *               4 ���������system_restart, gpio״̬����
	 *               5 �ӳ�˯������
	 */
	struct rst_info *rtc_info = system_get_rst_info();
		os_printf("reset reason: %x\n", rtc_info->reason);
		if (rtc_info->reason == REASON_WDT_RST ||
		rtc_info->reason == REASON_EXCEPTION_RST ||
		rtc_info->reason == REASON_SOFT_WDT_RST) {
		if (rtc_info->reason == REASON_EXCEPTION_RST) {
		os_printf("Fatal exception (%d):\n", rtc_info->exccause);
		}
		//��ӡ�ϴα����ĵ�ַ�������ڱ���ʱ�������
		os_printf("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x,excvaddr=0x%08x, depc=0x%08x\n",
				rtc_info->epc1, rtc_info->epc2, rtc_info->epc3, rtc_info->excvaddr, rtc_info->depc);
		}
	/****************************����*********************************************/

	/**��ʱ��
	 * ����ͬһ����ʱ����os_time_aem��os_timer_arm_us�����ظ����ã���������os_time_disarmȡ����ʱ����
	 * ��ʱ���ص�os_timer_setfn����Ҫ�У����ұ����ڶ�ʱ��δʹ�ܵ�����µ��ã�
	 * �� os_timer_arm ��os_timer_arm_us ֮ǰ���� os_timer_disarm ֮��
	 *os_timer_disarm ȡ����ʱ��
	 *os_timer_setfn ��ʱ���ص�
	 *os_timer_arm   ʹ�ܺ��뼶��ʱ��
	 *os_timer_arm_us ʹ��΢���ʱ��
	 *system_timer_reinit ���³�ʼ����ʱ������ʹ��΢���ʱ��ʱ���á�
	 */
	//ȡ����ʱ��
	os_timer_disarm(&wifistate_checktimer);
	//��ʱ���ص�����
	os_timer_setfn(&wifistate_checktimer, (os_timer_func_t *) WifiStatus_Check,NULL);
	//ʹ�ܺ��뼶��ʱ��
	os_timer_arm(&wifistate_checktimer, 2000, true);
}

//ICACHE_FLASH_ATTR  ��Ӧ���򽫴�ŵ�flash�У�������ʱ�ż��ص�cache���С�
//�����IRAM_ATTR��ĺ����������ϵ��ͼ��ص�iRAM
void ICACHE_FLASH_ATTR
user_pre_init(void)
{
	//system_partition_table_regist ע��partition table
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}

#define lec 2
#define sec 137  //������
#define chang_duc 1
#define sec_pianyi 0



void ICACHE_FLASH_ATTR
user_init(void)
{
	/** wifi_set_opmode
		 * ���ò�����WIFIģ�鹤��ģʽ��Ĭ��APģʽ
		 * 0x01 ����ģʽ
		 * 0x02 �ȵ�APģʽ
		 * 0x03���ģʽ
		 */

    //���ò�������WIFIģ�鹤��ģʽ-���ģʽ
	//wifi_set_opmode_current(0x03);
	//���ò�����WIFIģ�鹤��ģʽ-�ͷ���ģʽ
	//wifi_set_opmode(0x01); 
	//���ò�������WIFIģ�鹤��ģʽ-�ͷ���ģʽ
	//wifi_set_opmode_current(0x01);
	//���ò�������WIFIģ�鹤��ģʽ-AP��ģʽ
	//wifi_set_opmode_current(0x02);

	//����AP���������ص�����wifiConnectCb
	//WIFI_Connect(STA_SSID, STA_PASS, wifiConnectCb);

	//���SDK�汾,%s ����ַ�
    os_printf("SDK version:%s\n", system_get_sdk_version());
    //���оƬID(Ψһ),%d ���ʮ��������
    os_printf("ID version:%d\n", system_get_chip_id());
    //�ڴ�ռ�������
    system_print_meminfo();

    wifi_station_set_auto_connect(1);  //��ֹ8266����·����
    wifi_set_sleep_type(NONE_SLEEP_T);                     //�ر�ʡ��ģʽ

    //��ʱ50us,���ֵ65535
    os_delay_us(50);
    //��ѯϵͳʣ����� heap ���ռ��С
    os_printf("KJDX version:%d\n", system_get_free_heap_size());
    //��ѯϵͳʱ��
	os_printf("CXSJ version:%d\n", system_get_time());
	
	//��ӡ��ǰ����Ķѿռ������ڴ��..
	//system_show_malloc();

    os_printf("\n\nHello World0001! KO\n\n");

    uint8 ser_ip[lec*4];
	uint32 value;  //typedef unsigned int        uint32;
	/********************ָ��**********************************
	&  ��ȡ�����ڴ��ַ
	*  ��ȡ�ڴ��ַ��ֵ
	   uint8* addr_case1�ȼ��� uint8 *addr_case1
	   uint8* addr_case1  char���͵�ָ�����  
	   &value  ����value���ڴ��ַ
	   (uint8*) ǿ������ת��ָ��

	**********************************************************/
	uint8* addr_case1 = (uint8*)&value;  //typedef unsigned char       uint8;
//	addr_case1[0] = 12;
//	addr_case1[1] = 14;
//	struct server_info { char server_ip[4]; int server_port; }ser2_info;
//	os_memcpy(ser2_info.server_ip, ser_ip, os_strlen(ser_ip));
//	ser2_info.server_port = 9999;
	//spi_flash_erase_sector(sec);
	//typedef unsigned int        uint32;
	//SpiFlashOpResult spi_flash_write(uint32 des_addr, uint32 *src_addr, uint32 size);
	                                          //����&��ȡ��ַ�����,*�Ǽ�������
	//spi_flash_write(sec*4*1024+sec_pianyi, (uint32 *)&addr_case1, sizeof(addr_case1));
//	spi_flash_write(sec*4*1024+sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
//	os_printf("@@@@@@FLASH write@@@@@:%02x%02x\r\n", addr_case1[0], addr_case1[1]);

	
	spi_flash_read(sec*4*1024+sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//��ȡflashֵ
	os_printf("@@@@@@FLASH000123 read@@@@@:%d %d\r\n", addr_case1[0], addr_case1[1]);
	
	WifiStatus_Check_server();

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);//����
    GPIO_OUTPUT_SET(GPIO_ID_PIN(2),1);//����GPIO2Ϊ�ߵ�ƽ���(��)
//    	os_timer_disarm(&led_01_timer);
//    	os_timer_setfn(&led_01_timer, (os_timer_func_t *) blue_led_timer_toggle1,NULL);//��ʱ�ص�����
//    	os_timer_arm(&led_01_timer, 1000, true);//����ʱ��Ϊ1s
    //gpio_test();
    //blue_led_timer_toggle1();

}
