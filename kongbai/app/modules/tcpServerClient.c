/*MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "modules/tcpServerClient.h"
//#include "modules/gpio_test.h"
#include "gpio.h"//GPIO口操作

#include "cJSON.h"


/****************************
 *   TCP CLIENT FUNCTIONS TCP客户机功能  *
 ****************************/

/**********************************
 *  TCP CLIENT STATIC PROTOTYPES TCP客户端静态原型 *
 **********************************/
static void tcp_client_sent_cb(void *arg);
static void tcp_client_recv_cb(void *arg,char *pdata,unsigned short length);
static void tcp_client_recon_cb(void *arg,sint8 error);
static void tcp_client_discon_cb(void *arg);
static void tcp_client_connect_cb(void *arg);

/**********************************
 *   TCP CLIENT STATIC VARIABLES TCP客户端静态变量 *
 **********************************/

/**********************************
 *   TCP CLIENT STATIC FUNCTIONS TCP客户端静态函数 *
 **********************************/

/**
 * TCP Client数据发送回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_client_sent_cb(void *arg){
	os_printf("tcp client send data successful\r\n");//20
}

//Flash读写定义变量
#define sec 137  //扇区号
#define sec_pianyi 0
uint32 value;

/****************** struct 创建结构体 *******************************************************************
struct du_xie_shu_ju_01 {
	uint8 *zhang_hao[32];  //*zhang_hao[32]的指针变量（输入字符串需要）[]里需要是4的倍数，表示多少字节
	uint8 *mi_ma[64];
	uint32 id[4];
} shuju_zu_01 ;

struct du_xie_shu_ju_01 shuju_zu_02;//给结构体一个变量名shuju_zu_02,一般用在具体使用的时候

struct 关键字
du_xie_shu_ju_01 结构体名
shuju_zu_01 结构体变量1
shuju_zu_02 结构体变量2
 结构体成员的获取   结构体变量名.成员名;

 记住：指针用 = 赋值。数组用strcpy赋值
************************************* *******************************************************************/
struct du_xie_shu_ju_01 {
	uint8 *zhang_hao[32];  //字符串
	uint8 *mi_ma[64];      //字符串
	uint32 id[4];          //整形
} shuju_zu_01 ;//给结构体一个变量名shuju_zu_01，并对内容赋值

struct du_xie_shu_ju_01 shuju_zu_02;//给结构体一个变量名shuju_zu_02,一般用在具体使用的时候

/**
 * TCP Client数据接收回调函数，可以在这处理收到Server发来的数据
 */
static void ICACHE_FLASH_ATTR
tcp_client_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("tcp client receive tcp server data\r\n");//21
	os_printf("length: %d \r\ndata: %s\r\n",len,pdata);//22
	/********************指针**********************************
	&  获取变量内存地址
	*  获取内存地址的值
	   uint8* addr_case1等价于 uint8 *addr_case1
	   uint8* addr_case1  char类型的指针变量
	   &value  变量value的内存地址
	   (uint8*) 强制类型转换指针
	   记住：指针用 = 赋值。数组用strcpy赋值

	**********************************************************/

		uint8* addr_case1 = (uint8*)&value;   //typedef unsigned char       uint8;
		uint32 sec200 = 200;  //扇区号          typedef unsigned int        uint32;
		uint32 sec138 = 138;  //扇区号
		uint32 sec140 = 140;  //扇区号
		//strcmp比较字符串str1和str2是否相同。如果相同则返回0
	if(strcmp(pdata,"a001") == 0){
		os_printf("\nok  01\n");
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);//蓝灯
		GPIO_OUTPUT_SET(GPIO_ID_PIN(2),0);//设置GPIO2为低电平输出(开)
		//uint32 value;
			uint8* addr_case1 = (uint8*)&value;
			addr_case1[0] = 10;
			addr_case1[1] = 11;
			spi_flash_erase_sector(sec);
			spi_flash_write(sec*4*1024+sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
				os_printf("@@@@@@FLASH write@@@@@:%02x%02x\r\n", addr_case1[0], addr_case1[1]);
	}
	//strcmp比较字符串str1和str2是否相同。如果相同则返回0
	if (strcmp(pdata, "a002") == 0) {
		os_printf("\nok  02\n");
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);//蓝灯
		GPIO_OUTPUT_SET(GPIO_ID_PIN(2), 1);//设置GPIO2为高电平输出(关)
		spi_flash_read(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//读取flash值
		os_printf("@@@@@@du_QU 000123 read@@@@@:%d%d\r\n", addr_case1[0], addr_case1[1]);
	}
	//strcmp比较字符串str1和str2是否相同。如果相同则返回0
	if(strcmp(pdata,"a003") == 0){
			os_printf("\nok  03\n");
			//uint32 value;
				uint8* addr_case1 = (uint8*)&value;
				addr_case1[0] = 152;
				addr_case1[1] = 99;
				spi_flash_erase_sector(sec);
				spi_flash_write(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
					os_printf("@@@@@@FLASH write@@@@@:%d%d\r\n", addr_case1[0], addr_case1[1]);
		}
	if(strcmp(pdata,"a004") == 0){
				os_printf("\nok  04\n");
				//uint32 value;
					uint8* addr_case1 = (uint8*)&value;
					addr_case1[0] = 12;
					addr_case1[1] = 13;
					addr_case1[2] = 14;
					addr_case1[3] = 15;
					spi_flash_erase_sector(sec);
					spi_flash_write(sec*4*1024+sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
						os_printf("@@@@@@FLASH write@@@@@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
			}
	if(strcmp(pdata,"a005") == 0){
	    	os_printf("\nok  05\n");
			spi_flash_read(sec*4*1024+sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//读取flash值
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
		spi_flash_read(sec200 * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//读取flash值
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
		spi_flash_read(sec138 * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//读取flash值
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
		spi_flash_read(sec * 4 * 1024 + sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//读取flash值
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
		system_param_save_with_protect(0x8C, (uint32*)addr_case1, sizeof(addr_case1));//保护模式写入flash值
		os_printf("@@@@@@xie_ru@@@@@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0014") == 0){
		system_param_load(0x8C, 0, (uint32*)addr_case1, sizeof(addr_case1));//保护模式读取flash值
		os_printf("@du_qu@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0015") == 0){
			spi_flash_read(140*4096, (uint32*)addr_case1, sizeof(addr_case1));//读取flash值
			os_printf("@du_qu@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
		}
	if (strcmp(pdata, "a0016") == 0){
		spi_flash_read(141*4096, (uint32*)addr_case1, sizeof(addr_case1));//读取flash值
		os_printf("@du_qu@:%d %d %d %d\r\n", addr_case1[0], addr_case1[1], addr_case1[2], addr_case1[3]);
	}
	if (strcmp(pdata, "a0017") == 0){
		shuju_zu_01.zhang_hao[32] = "cccooxxx";   //结构体成员的获取和赋值，只能在函数体内
		shuju_zu_01.mi_ma[64] = "cxshn1234er567890";
		shuju_zu_01.id[4] = system_get_chip_id();
		spi_flash_erase_sector(139);
		spi_flash_write(139 * 4096, (uint32*)&shuju_zu_01, sizeof(shuju_zu_01));//写入flash
		os_printf("@@@@@@xie_ru@@@@@:%s %s %d\r\n", shuju_zu_01.zhang_hao[32], shuju_zu_01.mi_ma[64], shuju_zu_01.id[4]);
	}
	if (strcmp(pdata, "a0018") == 0) {
		//struct du_xie_shu_ju_01 shuju_zu_02;//声明一个结构体变量名shuju_zu_02，一般用在具体使用的位置，如需要多次调用应该在函数外声明；
		shuju_zu_02.zhang_hao[32] = "bbcxcxcx";   //结构体成员的获取和赋值，只能在函数体内；
		shuju_zu_02.mi_ma[64] = "ecxf125318090";
		shuju_zu_02.id[4] = system_get_chip_id();
		spi_flash_erase_sector(139);
		spi_flash_write(139 * 4096, (uint32*)&shuju_zu_02, sizeof(shuju_zu_02));//写入flash
		os_printf("@@@@@@xie_ru@@@@@:%s %s %d\r\n", shuju_zu_02.zhang_hao[32], shuju_zu_02.mi_ma[64], shuju_zu_02.id[4]);
	}
	if (strcmp(pdata, "a0019") == 0) {
		spi_flash_read(139 * 4096, (uint32*)&shuju_zu_01, sizeof(shuju_zu_01));//读取flash值
		os_printf("@du_qu@:%s %s %d\r\n", shuju_zu_01.zhang_hao[32], shuju_zu_01.mi_ma[64], shuju_zu_01.id[4]);
	}
	if (strcmp(pdata, "a0020") == 0) {
		//struct du_xie_shu_ju_01 shuju_zu_02;
		spi_flash_read(139 * 4096, (uint32*)&shuju_zu_02, sizeof(shuju_zu_02));//读取flash值
		os_printf("@du_qu@:%s %s %d\r\n", shuju_zu_02.zhang_hao[32], shuju_zu_02.mi_ma[64], shuju_zu_02.id[4]);
	}
	//TO DO

	/**
	 *process the receive data
	 */
}

/**
 * TCP Client重连回调函数，可以在此函数里做重连接处理
 */
static void ICACHE_FLASH_ATTR
tcp_client_recon_cb(void *arg,sint8 error){
	os_printf("tcp client connect tcp server error %d\r\n",error);
}

/**
 * TCP Client断开连接回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_client_discon_cb(void *arg){
	os_printf("tcp client disconnect tcp server successful\r\n");
}

/**
 * TCP Client连接成功回调函数，必须调用以下3个函数
 * espconn_regist_recvcb 数据接收完成
 * espconn_regist_sentcb tcp数据发送完成
 * espconn_regist_disconcb 连接正常断开
 */
static void ICACHE_FLASH_ATTR
tcp_client_connect_cb(void *arg){
	struct espconn *pespconn = arg;

	os_printf("tcp client connect tcp server successful\r\n");//19
	//espconn_regist_recvcb 数据接收完成
	espconn_regist_recvcb(pespconn,tcp_client_recv_cb);//注册接收数据回调函数
	//espconn_regist_sentcb tcp数据发送完成
	espconn_regist_sentcb(pespconn,tcp_client_sent_cb);//注册数据发送完成回调函数
	//espconn_regist_disconcb 连接正常断开
	espconn_regist_disconcb(pespconn,tcp_client_discon_cb);//注册断开连接回调函数

}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS TCP客户端全局函数 *
 **********************************/
/**
 * TCP Client定时发送数据回调函数
 */
sint8 ICACHE_FLASH_ATTR
tcp_client_send_data(struct espconn *espconn,uint8 *pdata,uint16 length){
    if (espconn == NULL) {
    	return ESPCONN_ARG;
    }
    espconn_send(espconn,pdata,length);
}

/**
 * TCP Client初始化函数
 * @remote_ip    要连接的TCP Server IP地址
 * @local_ip     ESP8266 IP地址
 * @remote_port  要连接的TCP Server端口号
 */
void ICACHE_FLASH_ATTR
                             //指针变量
tcp_client_init(struct espconn *espconn,uint8 *remote_ip,struct ip_addr *local_ip, int remote_port){

	uint32 server_ip = ipaddr_addr(remote_ip);

	os_printf("tcp client connect to tcp server\r\n");//18
	espconn->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;

	os_memcpy(espconn->proto.tcp->remote_ip,&server_ip,4);//设置要连接的Server IP地址
	espconn->proto.tcp->remote_port = remote_port;//设置要连接的Server 端口号
	os_memcpy(espconn->proto.tcp->local_ip,local_ip,4);//设置本地IP地址
	espconn->proto.tcp->local_port = TCP_LOCAL_PORT;//设置本地端口号

	espconn_regist_connectcb(espconn,tcp_client_connect_cb);//注册连接成功回调函数
	espconn_regist_reconcb(espconn,tcp_client_recon_cb);//注册断连重新连接回调函数

	espconn_connect(espconn);//Client连接Server
}



/****************************
 *   TCP SERVER FUNCTIONS  TCP服务器功能 *
 ****************************/
/**********************************
 *  TCP SERVER STATIC PROTOTYPES TCP服务器静态原型 *
 **********************************/
static void tcp_server_sent_cb(void *arg);
static void tcp_server_recv_cb(void *arg,char *pdata,unsigned short length);
static void tcp_server_recon_cb(void *arg,sint8 error);
static void tcp_server_discon_cb(void *arg);
static void tcp_server_listen_cb(void *arg);

/**********************************
 *   TCP SERVER STATIC VARIABLES TCP服务器静态变量 *
 **********************************/


/**********************************
 *   TCP server STATIC FUNCTIONS TCP服务器静态函数 *
 **********************************/

/**
 * TCP Server数据发送回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_server_sent_cb(void *arg){
	os_printf("tcp server send data successful\r\n");

}

/**
 * TCP Server数据接收回调函数，可以在这处理收到Client发来的数据
 */
static void ICACHE_FLASH_ATTR
tcp_server_recv_cb(void *arg,char *pdata,unsigned short len){
	//os_printf("tcp server receive tcp client data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n",len,pdata);

//	cJSON *root = cJSON_Parse(pdata);
//		if(root != NULL){
//			uint8 *SSD = cJSON_GetObjectItem(root,"SSD")->valuestring;//解析SSID key对应的Value值，也就是我们发送的Wi-Fi名称
//			uint8 *PWD = cJSON_GetObjectItem(root,"PWD")->valuestring;//解析PWD key对应的Value值，也就是我们发送的Wi-Fi密码
//			os_printf("SSID: %s   PWD: %s\r\n",SSD,PWD);//打印我们收到的密码
//			}else{
//			os_printf("json data format error!\r\n");//收到的数据不正确就打印
//			}
//			cJSON_Delete(root);//解析完JSON后一定要记得释放！

	//TO DO

	/**
	 *process the receive data
	 */
}

/**
 * TCP Server重连回调函数，可以在此函数里做重连接处理
 */
static void ICACHE_FLASH_ATTR
tcp_server_recon_cb(void *arg,sint8 error){
	os_printf("tcp server connect tcp client error %d\r\n",error);
}

/**
 * TCP Server断开连接回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_server_discon_cb(void *arg){
	os_printf("tcp server disconnect tcp client successful\r\n");
}

/**
 * TCP Server监听Client连接回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_server_listen_cb(void *arg){
	struct espconn *pespconn = arg;
	//system_soft_wdt_feed(); //喂狗

	os_printf("tcp server have tcp client connect\r\n");
	espconn_regist_recvcb(pespconn,tcp_server_recv_cb);//注册收到数据回调函数
	espconn_regist_sentcb(pespconn,tcp_server_sent_cb);//注册发送完数据回调函数
	espconn_regist_disconcb(pespconn,tcp_server_discon_cb);//注册断开连接回调函数
//	struct rst_info *rtc_info = system_get_rst_info();
//	os_printf("reset reason: %x\n", rtc_info->reason);
//	if (rtc_info->reason == REASON_WDT_RST ||
//	rtc_info->reason == REASON_EXCEPTION_RST ||
//	rtc_info->reason == REASON_SOFT_WDT_RST) {
//	if (rtc_info->reason == REASON_EXCEPTION_RST) {
//	os_printf("Fatal exception (%d):\n", rtc_info->exccause);
//	}
//	os_printf("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x,excvaddr=0x%08x, depc=0x%08x\n",
//			rtc_info->epc1, rtc_info->epc2, rtc_info->epc3, rtc_info->excvaddr, rtc_info->depc);
//	}

}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS TCP客户端全局函数 *
 **********************************/

/**
 * TCP Server定时发送数据回调函数
 */
sint8 ICACHE_FLASH_ATTR
tcp_server_send_data(struct espconn *espconn,uint8 *pdata,uint16 length){
    if (espconn == NULL) {
    	return ESPCONN_ARG;
    }

	espconn_send(espconn,pdata,length);
}

/**
 * TCP Server初始化函数
 * @local_port 本地监听端口号，与Client连接的端口号一致
 */
void ICACHE_FLASH_ATTR
tcp_server_init(struct espconn *espconn,uint16 local_port){

	os_printf("tcp server waiting tcp client connect!\r\n");
	/* "."一般情况下读作"的” “->”一般读作"指向的结构体的" */
	espconn->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;  //espconn->type   等价于 *(espconn).type ； ->（结构体，类，共同体等）取成员运算符

	espconn->proto.tcp->local_port = local_port;//设置本地监听的端口号，等待Client连接
	

	espconn_regist_connectcb(espconn,tcp_server_listen_cb);//注册Server监听回调函数
	espconn_regist_reconcb(espconn,tcp_server_recon_cb);//注册断连重新连接回调函数

	espconn_accept(espconn);//创建Server,开始监听
	espconn_regist_time(espconn,360,0);//超时断开连接时间
}
void ICACHE_FLASH_ATTR
tcp_client_init1(struct espconn *espconn,uint8 *remote_ip,struct ip_addr *local_ip, int remote_port){
	uint32 server_ip = ipaddr_addr(remote_ip);


	os_printf("tcp server waiting tcp client connect!\r\n");
	espconn->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;

	os_memcpy(espconn->proto.tcp->remote_ip,&server_ip,4);//设置要连接的Server IP地址
	espconn->proto.tcp->remote_port = remote_port;//设置要连接的Server 端口号
	os_memcpy(espconn->proto.tcp->local_ip,local_ip,4);//设置本地IP地址
	espconn->proto.tcp->local_port = TCP_LOCAL_PORT;//设置本地端口号

	espconn_regist_connectcb(espconn,tcp_server_listen_cb);//注册Server监听回调函数
		espconn_regist_reconcb(espconn,tcp_server_recon_cb);//注册断连重新连接回调函数

		espconn_accept(espconn);//创建Server,开始监听
		espconn_regist_time(espconn,360,0);//超时断开连接时间
}
