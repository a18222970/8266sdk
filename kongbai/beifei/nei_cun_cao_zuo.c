
#include "modules/tcpServerClient.h"
//#include "modules/gpio_test.h"
#include "gpio.h"//GPIO�ڲ���


/****************************
 *   TCP CLIENT FUNCTIONS TCP�ͻ�������  *
 ****************************/

 /**********************************
  *  TCP CLIENT STATIC PROTOTYPES TCP�ͻ��˾�̬ԭ�� *
  **********************************/
static void tcp_client_sent_cb(void* arg);
static void tcp_client_recv_cb(void* arg, char* pdata, unsigned short length);
static void tcp_client_recon_cb(void* arg, sint8 error);
static void tcp_client_discon_cb(void* arg);
static void tcp_client_connect_cb(void* arg);

/**********************************
 *   TCP CLIENT STATIC VARIABLES TCP�ͻ��˾�̬���� *
 **********************************/

 /**********************************
  *   TCP CLIENT STATIC FUNCTIONS TCP�ͻ��˾�̬���� *
  **********************************/

  /**
   * TCP Client���ݷ��ͻص�����
   */
static void ICACHE_FLASH_ATTR
tcp_client_sent_cb(void* arg) {
	os_printf("tcp client send data successful\r\n");//20
}

/**
 * TCP Client���ݽ��ջص��������������⴦���յ�Server����������
 */

 //Flash��д�������
#define sec 137  //������
#define sec_pianyi 0
uint32 value;

/****************** struct �����ṹ�� *******************************************************************
struct du_xie_shu_ju_01 {
	uint8 *zhang_hao[32];  //*zhang_hao[32]��ָ������������ַ�����Ҫ��[]����Ҫ��4�ı�������ʾ�����ֽ�
	uint8 *mi_ma[64];
	uint32 id[4];
} shuju_zu_01 ;

struct du_xie_shu_ju_01 shuju_zu_02;//���ṹ��һ��������shuju_zu_02,һ�����ھ���ʹ�õ�ʱ��

struct �ؼ���
du_xie_shu_ju_01 �ṹ����
shuju_zu_01 �ṹ�����1
shuju_zu_02 �ṹ�����2
 �ṹ���Ա�Ļ�ȡ   �ṹ�������.��Ա��;

 ��ס��ָ���� = ��ֵ��������strcpy��ֵ
************************************* *******************************************************************/
struct du_xie_shu_ju_01 {
	uint8* zhang_hao[32];  //�ַ���
	uint8* mi_ma[64];      //�ַ���
	uint32 id[4];          //����
} shuju_zu_01;//���ṹ��һ��������shuju_zu_01���������ݸ�ֵ

struct du_xie_shu_ju_01 shuju_zu_02;//���ṹ��һ��������shuju_zu_02,һ�����ھ���ʹ�õ�ʱ��

static void ICACHE_FLASH_ATTR
tcp_client_recv_cb(void* arg, char* pdata, unsigned short len) {
	os_printf("tcp client receive tcp server data\r\n");//21
	os_printf("length: %d \r\ndata: %s\r\n", len, pdata);//22
	/********************ָ��**********************************
	&  ��ȡ�����ڴ��ַ
	*  ��ȡ�ڴ��ַ��ֵ
	   uint8* addr_case1�ȼ��� uint8 *addr_case1
	   uint8* addr_case1  char���͵�ָ�����
	   &value  ����value���ڴ��ַ
	   (uint8*) ǿ������ת��ָ��
	   ��ס��ָ���� = ��ֵ��������strcpy��ֵ

	**********************************************************/

	uint8* addr_case1 = (uint8*)&value;   //typedef unsigned char       uint8;
	uint32 sec200 = 200;  //������          typedef unsigned int        uint32;
	uint32 sec138 = 138;  //������
	uint32 sec140 = 140;  //������
	//strcmp�Ƚ��ַ���str1��str2�Ƿ���ͬ�������ͬ�򷵻�0
	if (strcmp(pdata, "a001") == 0) {
		os_printf("\nok  01\n");
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);//����
		GPIO_OUTPUT_SET(GPIO_ID_PIN(2), 0);//����GPIO2Ϊ�͵�ƽ���(��)
		//uint32 value;
		uint8* addr_case1 = (uint8*)&value;
		addr_case1[0] = 10;
		addr_case1[1] = 11;
		spi_flash_erase_sector(sec);
		spi_flash_write(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
		os_printf("@@@@@@FLASH write@@@@@:%02x%02x\r\n", addr_case1[0], addr_case1[1]);
	}
	//strcmp�Ƚ��ַ���str1��str2�Ƿ���ͬ�������ͬ�򷵻�0
	if (strcmp(pdata, "a002") == 0) {
		os_printf("\nok  02\n");
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);//����
		GPIO_OUTPUT_SET(GPIO_ID_PIN(2), 1);//����GPIO2Ϊ�ߵ�ƽ���(��)
		spi_flash_read(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//��ȡflashֵ
		os_printf("@@@@@@du_QU 000123 read@@@@@:%d%d\r\n", addr_case1[0], addr_case1[1]);
	}
	//strcmp�Ƚ��ַ���str1��str2�Ƿ���ͬ�������ͬ�򷵻�0
	if (strcmp(pdata, "a003") == 0) {
		os_printf("\nok  03\n");
		//uint32 value;
		uint8* addr_case1 = (uint8*)&value;
		addr_case1[0] = 152;
		addr_case1[1] = 99;
		spi_flash_erase_sector(sec);
		spi_flash_write(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
		os_printf("@@@@@@FLASH write@@@@@:%d%d\r\n", addr_case1[0], addr_case1[1]);
	}
	if (strcmp(pdata, "a004") == 0) {
		os_printf("\nok  04\n");
		//uint32 value;
		uint8* addr_case1 = (uint8*)&value;
		addr_case1[0] = 12;
		addr_case1[1] = 13;
		addr_case1[2] = 14;
		addr_case1[3] = 15;
		spi_flash_erase_sector(sec);
		spi_flash_write(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
		os_printf("@@@@@@FLASH write@@@@@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a005") == 0) {
		os_printf("\nok  05\n");
		spi_flash_read(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//��ȡflashֵ
		os_printf("@@@@@@du_QU 000123 read@@@@@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a006") == 0) {
		uint32 sec200 = 200;
		uint8* addr_case1 = (uint8*)&value;
		addr_case1[0] = 12;
		addr_case1[1] = 13;
		addr_case1[2] = 14;
		addr_case1[3] = 15;
		addr_case1[4] = 10;
		addr_case1[5] = 11;
		spi_flash_erase_sector(sec200);
		spi_flash_write(sec200 * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
		os_printf("@@@@@@FLASH write@@@@@:%02x%02x%02x%02x%02x%02x\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3], addr_case1[4], addr_case1[5]);
	}
	if (strcmp(pdata, "a007") == 0)
	{
		os_printf("\nok  07\n");
		spi_flash_read(sec200 * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//��ȡflashֵ
		os_printf("@@@@@@FLASH write@@@@@:%02x%02x%02x%02x%02x%02x\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3], addr_case1[4], addr_case1[5]);
		os_printf("@@@@@@FLASH write@@@@@:%d%d%d%d%d%d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3], addr_case1[4], addr_case1[5]);
	}
	if (strcmp(pdata, "a008") == 0) {
		os_printf("\nok  08\n");
		//uint32 value;
		uint8* addr_case1 = (uint8*)&value;
		addr_case1[0] = 111;
		addr_case1[1] = 125;
		addr_case1[2] = 88;
		addr_case1[3] = 226;
		spi_flash_erase_sector(sec138);
		spi_flash_write(sec138 * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
		os_printf("@@@@@@xie_ru@@@@@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a009") == 0) {
		os_printf("\nok  09\n");
		spi_flash_read(sec138 * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//��ȡflashֵ
		os_printf("@@@@@@du_QU 000123 read@@@@@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0010") == 0) {
		uint8* addr_case1 = (uint8*)&value;
		addr_case1[0] = 12;
		addr_case1[1] = 13;
		addr_case1[2] = 14;
		addr_case1[3] = 15;
		addr_case1[4] = 10;
		addr_case1[5] = 11;
		spi_flash_erase_sector(sec);
		spi_flash_write(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
		os_printf("@@@@@@FLASH write@@@@@:%02x%02x%02x%02x%02x%02x\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3], addr_case1[4], addr_case1[5]);
	}
	if (strcmp(pdata, "a0011") == 0)
	{
		os_printf("\nok  11\n");
		spi_flash_read(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//��ȡflashֵ
		os_printf("@@@@@@FLASH write@@@@@:%02x%02x%02x%02x%02x%02x\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3], addr_case1[4], addr_case1[5]);
		os_printf("@@@@@@FLASH write@@@@@:%d%d%d%d%d%d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3], addr_case1[4], addr_case1[5]);
	}
	if (strcmp(pdata, "a0012") == 0) {
		os_printf("\nok  012\n");
		//uint32 value;
		uint8* addr_case1 = (uint8*)&value;
		addr_case1[0] = 221;
		addr_case1[1] = 145;
		addr_case1[2] = 108;
		addr_case1[3] = 26;
		//spi_flash_erase_sector(0x8c);
		//spi_flash_write(sec138 * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
		system_param_save_with_protect(0x8C, (uint32*)addr_case1, sizeof(addr_case1));
		os_printf("@@@@@@xie_ru@@@@@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0013") == 0) {
		//uint32 value;
		uint8* addr_case1 = (uint8*)&value;
		addr_case1[0] = 223;
		addr_case1[1] = 95;
		addr_case1[2] = 108;
		addr_case1[3] = 26;
		system_param_save_with_protect(0x8C, (uint32*)addr_case1, sizeof(addr_case1));//����ģʽд��flashֵ
		os_printf("@@@@@@xie_ru@@@@@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0014") == 0) {
		system_param_load(0x8C, 0, (uint32*)addr_case1, sizeof(addr_case1));//����ģʽ��ȡflashֵ
		os_printf("@du_qu@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0015") == 0) {
		spi_flash_read(140 * 4096, (uint32*)addr_case1, sizeof(addr_case1));//��ȡflashֵ
		os_printf("@du_qu@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0016") == 0) {
		spi_flash_read(141 * 4096, (uint32*)addr_case1, sizeof(addr_case1));//��ȡflashֵ
		os_printf("@du_qu@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0017") == 0) {
		shuju_zu_01.zhang_hao[32] = "cccooxxx";   //�ṹ���Ա�Ļ�ȡ�͸�ֵ��ֻ���ں�������
		shuju_zu_01.mi_ma[64] = "cxshn1234er567890";
		shuju_zu_01.id[4] = system_get_chip_id();
		spi_flash_erase_sector(139);
		spi_flash_write(139 * 4096, (uint32*)&shuju_zu_01, sizeof(shuju_zu_01));//д��flash
		os_printf("@@@@@@xie_ru@@@@@:%s %s %d\r\n", shuju_zu_01.zhang_hao[32], shuju_zu_01.mi_ma[64], shuju_zu_01.id[4]);
	}
	if (strcmp(pdata, "a0018") == 0) {
		//struct du_xie_shu_ju_01 shuju_zu_02;//����һ���ṹ�������shuju_zu_02��һ�����ھ���ʹ�õ�λ�ã�����Ҫ��ε���Ӧ���ں�����������
		shuju_zu_02.zhang_hao[32] = "bbcxcxcx";   //�ṹ���Ա�Ļ�ȡ�͸�ֵ��ֻ���ں������ڣ�
		shuju_zu_02.mi_ma[64] = "ecxf125318090";
		shuju_zu_02.id[4] = system_get_chip_id();
		spi_flash_erase_sector(139);
		spi_flash_write(139 * 4096, (uint32*)&shuju_zu_02, sizeof(shuju_zu_02));//д��flash
		os_printf("@@@@@@xie_ru@@@@@:%s %s %d\r\n", shuju_zu_02.zhang_hao[32], shuju_zu_02.mi_ma[64], shuju_zu_02.id[4]);
	}
	if (strcmp(pdata, "a0019") == 0) {
		spi_flash_read(139 * 4096, (uint32*)&shuju_zu_01, sizeof(shuju_zu_01));//��ȡflashֵ
		os_printf("@du_qu@:%s %s %d\r\n", shuju_zu_01.zhang_hao[32], shuju_zu_01.mi_ma[64], shuju_zu_01.id[4]);
	}
	if (strcmp(pdata, "a0020") == 0) {
		//struct du_xie_shu_ju_01 shuju_zu_02;
		spi_flash_read(139 * 4096, (uint32*)&shuju_zu_02, sizeof(shuju_zu_02));//��ȡflashֵ
		os_printf("@du_qu@:%s %s %d\r\n", shuju_zu_02.zhang_hao[32], shuju_zu_02.mi_ma[64], shuju_zu_02.id[4]);
	}

	//TO DO

	/**
	 *process the receive data
	 */
}

/**
 * TCP Client�����ص������������ڴ˺������������Ӵ���
 */
static void ICACHE_FLASH_ATTR
tcp_client_recon_cb(void* arg, sint8 error) {
	os_printf("tcp client connect tcp server error %d\r\n", error);
}

/**
 * TCP Client�Ͽ����ӻص�����
 */
static void ICACHE_FLASH_ATTR
tcp_client_discon_cb(void* arg) {
	os_printf("tcp client disconnect tcp server successful\r\n");
}

/**
 * TCP Client���ӳɹ��ص������������������3������
 * espconn_regist_recvcb ���ݽ������
 * espconn_regist_sentcb tcp���ݷ������
 * espconn_regist_disconcb ���������Ͽ�
 */
static void ICACHE_FLASH_ATTR
tcp_client_connect_cb(void* arg) {
	struct espconn* pespconn = arg;

	os_printf("tcp client connect tcp server successful\r\n");//19
	//espconn_regist_recvcb ���ݽ������
	espconn_regist_recvcb(pespconn, tcp_client_recv_cb);//ע��������ݻص�����
	//espconn_regist_sentcb tcp���ݷ������
	espconn_regist_sentcb(pespconn, tcp_client_sent_cb);//ע�����ݷ�����ɻص�����
	//espconn_regist_disconcb ���������Ͽ�
	espconn_regist_disconcb(pespconn, tcp_client_discon_cb);//ע��Ͽ����ӻص�����

}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS TCP�ͻ���ȫ�ֺ��� *
 **********************************/
 /**
  * TCP Client��ʱ�������ݻص�����
  */
sint8 ICACHE_FLASH_ATTR
tcp_client_send_data(struct espconn* espconn, uint8* pdata, uint16 length) {
	if (espconn == NULL) {
		return ESPCONN_ARG;
	}
	espconn_send(espconn, pdata, length);
}

/**
 * TCP Client��ʼ������
 * @remote_ip    Ҫ���ӵ�TCP Server IP��ַ
 * @local_ip     ESP8266 IP��ַ
 * @remote_port  Ҫ���ӵ�TCP Server�˿ں�
 */
void ICACHE_FLASH_ATTR
tcp_client_init(struct espconn* espconn, uint8* remote_ip, struct ip_addr* local_ip, int remote_port) {

	uint32 server_ip = ipaddr_addr(remote_ip);

	os_printf("tcp client connect to tcp server\r\n");//18
	espconn->proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;

	os_memcpy(espconn->proto.tcp->remote_ip, &server_ip, 4);//����Ҫ���ӵ�Server IP��ַ
	espconn->proto.tcp->remote_port = remote_port;//����Ҫ���ӵ�Server �˿ں�
	os_memcpy(espconn->proto.tcp->local_ip, local_ip, 4);//���ñ���IP��ַ
	espconn->proto.tcp->local_port = TCP_LOCAL_PORT;//���ñ��ض˿ں�

	espconn_regist_connectcb(espconn, tcp_client_connect_cb);//ע�����ӳɹ��ص�����
	espconn_regist_reconcb(espconn, tcp_client_recon_cb);//ע������������ӻص�����

	espconn_connect(espconn);//Client����Server
}



/****************************
 *   TCP SERVER FUNCTIONS  TCP���������� *
 ****************************/
 /**********************************
  *  TCP SERVER STATIC PROTOTYPES TCP��������̬ԭ�� *
  **********************************/
static void tcp_server_sent_cb(void* arg);
static void tcp_server_recv_cb(void* arg, char* pdata, unsigned short length);
static void tcp_server_recon_cb(void* arg, sint8 error);
static void tcp_server_discon_cb(void* arg);
static void tcp_server_listen_cb(void* arg);

/**********************************
 *   TCP SERVER STATIC VARIABLES TCP��������̬���� *
 **********************************/


 /**********************************
  *   TCP server STATIC FUNCTIONS TCP��������̬���� *
  **********************************/

  /**
   * TCP Server���ݷ��ͻص�����
   */
static void ICACHE_FLASH_ATTR
tcp_server_sent_cb(void* arg) {
	os_printf("tcp server send data successful\r\n");

}

/**
 * TCP Server���ݽ��ջص��������������⴦���յ�Client����������
 */
static void ICACHE_FLASH_ATTR
tcp_server_recv_cb(void* arg, char* pdata, unsigned short len) {
	os_printf("tcp server receive tcp client data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n", len, pdata);

	//TO DO

	/**
	 *process the receive data
	 */
}

/**
 * TCP Server�����ص������������ڴ˺������������Ӵ���
 */
static void ICACHE_FLASH_ATTR
tcp_server_recon_cb(void* arg, sint8 error) {
	os_printf("tcp server connect tcp client error %d\r\n", error);
}

/**
 * TCP Server�Ͽ����ӻص�����
 */
static void ICACHE_FLASH_ATTR
tcp_server_discon_cb(void* arg) {
	os_printf("tcp server disconnect tcp client successful\r\n");
}

/**
 * TCP Server����Client���ӻص�����
 */
static void ICACHE_FLASH_ATTR
tcp_server_listen_cb(void* arg) {
	struct espconn* pespconn = arg;

	os_printf("tcp server have tcp client connect\r\n");
	espconn_regist_recvcb(pespconn, tcp_server_recv_cb);//ע���յ����ݻص�����
	espconn_regist_sentcb(pespconn, tcp_server_sent_cb);//ע�ᷢ�������ݻص�����
	espconn_regist_disconcb(pespconn, tcp_server_discon_cb);//ע��Ͽ����ӻص�����

}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS TCP�ͻ���ȫ�ֺ��� *
 **********************************/

 /**
  * TCP Server��ʱ�������ݻص�����
  */
sint8 ICACHE_FLASH_ATTR
tcp_server_send_data(struct espconn* espconn, uint8* pdata, uint16 length) {
	if (espconn == NULL) {
		return ESPCONN_ARG;
	}

	espconn_send(espconn, pdata, length);
}

/**
 * TCP Server��ʼ������
 * @local_port ���ؼ����˿ںţ���Client���ӵĶ˿ں�һ��
 */
void ICACHE_FLASH_ATTR
tcp_server_init(struct espconn* espconn, uint16 local_port) {

	os_printf("tcp server waiting tcp client connect!\r\n");
	espconn->proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;

	espconn->proto.tcp->local_port = local_port;//���ñ��ؼ����Ķ˿ںţ��ȴ�Client����

	espconn_regist_connectcb(espconn, tcp_server_listen_cb);//ע��Server�����ص�����
	espconn_regist_reconcb(espconn, tcp_server_recon_cb);//ע������������ӻص�����

	espconn_accept(espconn);//����Server,��ʼ����
	espconn_regist_time(espconn, 360, 0);//��ʱ�Ͽ�����ʱ��
}
