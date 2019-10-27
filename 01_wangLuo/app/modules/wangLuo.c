/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>  //版权
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



使用#ifndef、#define、#endif组合宏定义的作用是：防止头文件的重复包含和编译
#ifndef            与#ifdef相反，判断某个宏是否未被定义
#define            定义一个预处理宏
#endif             #if, #ifdef, #ifndef这些条件命令的结束标志.
#undef            取消宏的定义
#ifdef              判断某个宏是否被定义，若已定义，执行随后的语句
#if                   编译预处理中的条件命令，相当于C语法中的if语句
#elif                若#if, #ifdef, #ifndef或前面的#elif条件不满足，则执行#elif之后的语句，相当于C语法中的else-if
#else              与#if, #ifdef, #ifndef对应, 若这些条件不满足，则执行#else之后的语句，相当于C语法中的else
defined         　与#if, #elif配合使用，判断某个宏是否被定义
***************************************************/
#include "modules/wangLuo.h"
#include "user_config.h"

#include "c_types.h"
#include "user_interface.h"
#include "osapi.h"
#include "modules/debug.h"
#include "os_type.h"
#include "espconn.h"
#include "mem.h"

#include "driver/uart.h"

#include "modules/cJSON.h"


static ETSTimer WiFiLinker;  //重新构建定时器
wifi_callback wifiCb = NULL;

static uint8_t wifiStatus = STATION_IDLE, lastWifiStatus = STATION_IDLE;

//wifi连接定时器回调
/*************************************************
Function: wifi_check_ip(void* arg) // 函数名称
Description: wifi连接定时器回调
Calls:// 被本函数调用的函数清单
	wifi_station_get_connect_status(): 返回连接AP状态;
							   STATION IDLE            站闲置
							   STATION CONNECTING      wifi连接
							   STATION WRONG PASSWORD  密码错误
							   STATION NO AP FOUND     未找到AP热点
							   STATION CONNECT FAIL    连接失败
							   STATION GOT IP          获得了IP
Called By: // 调用本函数的函数清单

Input: // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。

Output: // 对输出参数的说明。
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
static void ICACHE_FLASH_ATTR
wifi_check_ip(void* arg)
{
	//使用结构体（Struct）来保存不同类型的数据,struct 结构体变量名.成员名;
	struct ip_info ipConfig;

	os_timer_disarm(&WiFiLinker);
	//查询8266的IP地址
	wifi_get_ip_info(STATION_IF, &ipConfig);
	//wifi_station_get_connect_status() 查询 ESP8266 Wi-Fi Station 接⼝连接 AP 的状态。
	wifiStatus = wifi_station_get_connect_status();
	//STATION_GOT_IP//获得IP
	if (wifiStatus == STATION_GOT_IP && ipConfig.ip.addr != 0) //获得IP
	{
		os_timer_setfn(&WiFiLinker, (os_timer_func_t*)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 2000, 0);
	}
	else
	{
		if (wifi_station_get_connect_status() == STATION_WRONG_PASSWORD)//错误的密码
		{

			INFO("STATION_WRONG_PASSWORD\r\n");  //需要debug.h
			wifi_station_connect(); //连接wifi
		}
		else if (wifi_station_get_connect_status() == STATION_NO_AP_FOUND)////没有发现热点AP
		{
			INFO("STATION_NO_AP_FOUND\r\n");
			wifi_station_connect(); //连接wifi
		}
		else if (wifi_station_get_connect_status() == STATION_CONNECT_FAIL)//连接失败
		{
			INFO("STATION_CONNECT_FAIL\r\n");
			wifi_station_connect(); //连接wifi
		}
		else
		{
			INFO("STATION_IDLE\r\n");//闲置
		}

		os_timer_setfn(&WiFiLinker, (os_timer_func_t*)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 500, 0);
	}
	if (wifiStatus != lastWifiStatus) {
		lastWifiStatus = wifiStatus;
		if (wifiCb)
			wifiCb(wifiStatus);
	}
}

/*************************************************
Function: lian_jie_AP(uint8_t* ssid, uint8_t* pass, wifi_callback hui_diao)  // 函数名称
Description: wifi客户端模式连接路由器 // 函数功能、性能等的描述
Calls:// 被本函数调用的函数清单
	wifi_set_opmode_current(): 设置wifi工作模式，不保存到flash;
							   NULL_MODE       0x00  关闭wifi功能
							   STATION_MODE    0x01  客户端模式_连接路由器
							   SOFTAP_MODE     0x02  AP模式_热点
							   STATIONAP_MODE  0x03  混合模式
	os_memset():在一段内存块中填充某个给定值，当前的作用是
			   //用长度为sizeof(struct station_config)字节的0填充到地址为wifi_station_config的内存中，
			   //就是初始化wifi_station_config的值。
			   内存块指针，,填充值，填充大小
	os_sprintf():这个函数就是把第三部分的数据，按照第二部分格式化字符的格式，把第三部分的数据进行”格式化“，
				 //然后在把格式化后的数据类型，赋值给第一个参数。
	wifi_station_set_config_current():设置wifi_station接口配置，不保存到flash.
									  wifi_station_config 构建体
	wifi_station_set_auto_connect(): 设置是否上电连接已记录的AP(路由)，默认自动连接。
	wifi_station_connect(): 连接路由器.
Called By: // 调用本函数的函数清单
	user_main.c: user_init(void)
Input: // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
	ssid：wifi名称；
	pass: wifi密码；
	hui_diao: 回调函数名
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
void ICACHE_FLASH_ATTR
lian_jie_AP(uint8_t* ssid, uint8_t* pass, wifi_callback hui_diao) //uint8_t 需要c_types.h
{
	struct station_config wifi_station_config; //station_config  需要user_interface.h
	INFO("WIFI_INIT\r\n");
	wifi_set_opmode_current(STATION_MODE);     //需要user_interface.h
	wifiCb = hui_diao;
	os_memset(&wifi_station_config, 0, sizeof(struct station_config)); //需要osapi.h
	os_sprintf(wifi_station_config.ssid, "%s", ssid);
	os_sprintf(wifi_station_config.password, "%s", pass);
	wifi_station_set_config_current(&wifi_station_config);
	os_timer_disarm(&WiFiLinker);//关闭定时器
	os_timer_setfn(&WiFiLinker, (os_timer_func_t*)wifi_check_ip, NULL);// wifi_check_i 回调函数
	os_timer_arm(&WiFiLinker, 1000, 0);
	wifi_station_set_auto_connect(ZI_DONG_LIAN_JIE);
	wifi_station_connect();
}

/*tcp 客户端静态 函数声明*/
static void tcp_client_sent_cb(void* arg);
static void tcp_client_recv_cb(void* arg, char* pdata, unsigned short length);
static void tcp_client_recon_cb(void* arg, sint8 error);
static void tcp_client_discon_cb(void* arg);
static void tcp_client_connect_cb(void* arg);

/* TCP Client数据发送回调函数*/
static void ICACHE_FLASH_ATTR
tcp_client_sent_cb(void* arg)
{
	os_printf("tcp client send data successful\r\n");//20
}

/* TCP Client数据接收回调函数，可以在这处理收到Server发来的数据*/
static void ICACHE_FLASH_ATTR
tcp_client_recv_cb(void* arg, char* pdata, unsigned short len)
{
	os_printf("tcp client receive tcp server data\r\n");//21
	os_printf("length: %d \r\ndata: %s\r\n", len, pdata);//22
	/*解析以下一段json数据
    {"mac":"84:f3:eb:b3:a7:05", "number":2,"value":{"name":"xuhongv","age":18,"blog":"https://blog.csdn.net/xh870189248"},"hexArry":[51,15,63,22,96]}*/

	//u8* jsonRoot = "{\"mac\":\"84:f3:eb:b3:a7:05\", \"number\":2,\"value\":{\"name\":\"xuhongv\",\"age\":18,\"blog\":\"https://blog.csdn.net/xh870189248\"},\"hexArry\":[51,15,63,22,96]}";
	/*结果
	get MacAdress:84:f3:eb:b3:a7:05 
	get Number:2 
	get value->Name : xuhongv 
	get value->Name : xuhongv 
	get value->Age : 18 
	get value->pBlog	 : https://blog.csdn.net/xh870189248 
	get arryLength : 5 
	cJSON_GetArrayItem(pArry, 0)= 51 
	cJSON_GetArrayItem(pArry, 1)= 15 
	cJSON_GetArrayItem(pArry, 2)= 63 
	cJSON_GetArrayItem(pArry, 3)= 22 
	cJSON_GetArrayItem(pArry, 4)= 96 
	get freeHeap: 64368 */

	//cJSON* pJsonRoot = cJSON_Parse(jsonRoot); //解析json
	cJSON* pJsonRoot = cJSON_Parse(pdata);
	if (pJsonRoot != NULL) //判断,成功返回cJSON,失败返回NULL
	{
		char* s = cJSON_Print(pJsonRoot); //返回一个json格式的数据，返回时char*类型，注意使用后要释放内存。调用cJSON_free()即可！！
		os_printf("pJsonRoot: %s\r\n", s);
		cJSON_free((void*)s);
		// 解析mac字段字符串内容
		cJSON * pMacAdress = cJSON_GetObjectItem(pJsonRoot, "mac"); //从一个json格式数据剖析一个指定的字段的数据，返回数值就是一个cJSON，如果返回的cJSON为NULL则表示获取失败，否则获取成功！
		//判断mac字段是否json格式
		if (pMacAdress) 
		{
			if (cJSON_IsString(pMacAdress)) //判断mac字段是否string类型,返回数值就是一个布尔值，如果返回false则表示这个字段是字符串类型，否则不是
			{
				os_printf("get MacAdress:%s \n", pMacAdress->valuestring);
			}
		}
		else
		{
			os_printf("get MacAdress failed \n");
		}

		//解析number字段int内容
		cJSON* pNumber = cJSON_GetObjectItem(pJsonRoot, "number");
		//判断number字段是否存在
		if (pNumber) 
		{
			if (cJSON_IsNumber(pNumber)) {
				os_printf("get Number:%d \n", pNumber->valueint);
			}
		}
		else 
		{
			os_printf("get Number failed \n");
		}
		//解析value字段内容，判断是否为json
		cJSON* pValue = cJSON_GetObjectItem(pJsonRoot, "value");
		if (pValue) 
		{
			//进一步剖析里面的name字段:注意这个根节点是 pValue
			cJSON* pName = cJSON_GetObjectItem(pValue, "name");
			if (pName)
			{
				if (cJSON_IsString(pName)) 
				{
					os_printf("get value->Name : %s \n", pName->valuestring);
				}
			}
			//进一步剖析里面的age字段:注意这个根节点是 pValue
			cJSON* pAge = cJSON_GetObjectItem(pValue, "age");
			if (pAge) 
			{
				if (cJSON_IsNumber(pAge)) 
				{
					os_printf("get value->Age : %d \n", pAge->valueint);
				}
			}
			//进一步剖析里面的blog字段:注意这个根节点是 pValue
			cJSON* pBlog = cJSON_GetObjectItem(pValue, "blog");
			if (pBlog) 
			{
				if (cJSON_IsString(pBlog)) 
				{
					os_printf("get value->pBlog	 : %s \n", pBlog->valuestring);
				}
			}
		}
		//剖析
		cJSON* pArry = cJSON_GetObjectItem(pJsonRoot, "hexArry");
		if (pArry) 
		{
			//获取数组长度
			int arryLength = cJSON_GetArraySize(pArry);
			os_printf("get arryLength : %d \n", arryLength);
			//逐个打印
			int i;
			for (i = 0; i < arryLength; i++)
				os_printf("cJSON_GetArrayItem(pArry, %d)= %d \n", i, cJSON_GetArrayItem(pArry, i)->valueint);
		}


	}
	else 
	{
		os_printf("this is not a json data ... \n");
	}
	cJSON_Delete(pJsonRoot); //释放内存
	os_printf("get freeHeap: %d \n\n", system_get_free_heap_size());	

	if (strcmp(pdata, "a001") == 0)
	{
		uint8 tempMessage[6];
		//获取station下的mac地址
		uint8 macAdress[6];
		if (!wifi_get_macaddr(STATION_IF, macAdress)) 
		{
			INFO("Failed to get mac... \r\n");
		}
		else 
		{
			INFO("succeed to get mac...\r\n");
		}
		os_sprintf(tempMessage, "%02x:%02x:%02x:%02x:%02x:%02x", macAdress[0],
			macAdress[1], macAdress[2], macAdress[3], macAdress[4],
			macAdress[5]);
		cJSON* pRoot = cJSON_CreateObject();
		cJSON* pValue = cJSON_CreateObject();


		cJSON_AddStringToObject(pRoot, "mac", tempMessage);
		cJSON_AddNumberToObject(pRoot, "number", 2);

		cJSON_AddStringToObject(pValue, "mac", "xuhongv");
		cJSON_AddNumberToObject(pValue, "age", 18);
		cJSON_AddStringToObject(pValue, "mac", "https://blog.csdn.net/xh870189248");

		cJSON_AddItemToObject(pRoot, "value", pValue);

		int hex[5] = { 51,15,63,22,96 };
		cJSON* pHex = cJSON_CreateIntArray(hex, 5);
		cJSON_AddItemToObject(pRoot, "hex", pHex);

		char* s = cJSON_Print(pRoot);
		os_printf("\r\n creatJson : %s\r\n", s);
		cJSON_free((void*)s);

		cJSON_Delete(pRoot);
	}
}

/* TCP Client重连回调函数，可以在此函数里做重连接处理*/
static void ICACHE_FLASH_ATTR
tcp_client_recon_cb(void* arg, sint8 error)
{
	os_printf("tcp client connect tcp server error %d\r\n", error);
}

/* TCP Client断开连接回调函数*/
static void ICACHE_FLASH_ATTR
tcp_client_discon_cb(void* arg)
{
	os_printf("tcp client disconnect tcp server successful\r\n");
}

/*TCP Client连接成功回调函数*/
static void ICACHE_FLASH_ATTR
tcp_client_connect_cb(void* arg)
{
	struct espconn* pespconn = arg;

	os_printf("tcp client connect tcp server successful\r\n");//19
	espconn_regist_recvcb(pespconn, tcp_client_recv_cb);//注册接收数据回调函数
	espconn_regist_sentcb(pespconn, tcp_client_sent_cb);//注册数据发送完成回调函数
	espconn_regist_disconcb(pespconn, tcp_client_discon_cb);//注册断开连接回调函数

}

/*TCP Client定时发送数据回调函数*/
sint8 ICACHE_FLASH_ATTR
tcp_client_send_data(struct espconn* espconn, uint8* pdata, uint16 length)
{
	if (espconn == NULL) {
		return ESPCONN_ARG;
	}
	espconn_send(espconn, pdata, length);
}

/*************************************************
Function: tcp_client_init(struct espconn* espconn, uint8* remote_ip, struct ip_addr* local_ip, int remote_port)
Description: tcp 客户端配置.
Calls:// 被本函数调用的函数清单
	os_memcpy(): 内存拷贝。os_memcpy(*des, *src, size_t n);
			   //3个参数；需要拷贝进去的内存块指针，拷贝的目标内存块指针，拷贝的内存大小，
			   //就是将size_t长度的src拷贝到des
	ipaddr_addr():地址解释程序,返回的值按网络顺序排列
Called By: // 调用本函数的函数清单
	user_main.c: station_cnnect_callback(uint8_t status)
*************************************************/
void ICACHE_FLASH_ATTR
tcp_client_init(struct espconn* espconn, uint8* remote_ip, struct ip_addr* local_ip, int remote_port)
{

	uint32 server_ip = ipaddr_addr(remote_ip);

	os_printf("tcp client connect to tcp server\r\n");//18
	/* "." 结构体名，一般情况下读作"结构体的” “->”需要*结构体名，一般读作"结构体指向的" */
	espconn->proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;

	os_memcpy(espconn->proto.tcp->remote_ip, &server_ip, 4);//设置要连接的Server IP地址
	espconn->proto.tcp->remote_port = remote_port;//设置要连接的Server 端口号
	os_memcpy(espconn->proto.tcp->local_ip, local_ip, 4);//设置本地IP地址
	espconn->proto.tcp->local_port = LOCAL_PORT;//设置本地端口号

	espconn_regist_connectcb(espconn, tcp_client_connect_cb);//注册连接成功回调函数
	espconn_regist_reconcb(espconn, tcp_client_recon_cb);//注册断连重新连接回调函数

	espconn_connect(espconn);//Client连接Server
}

/*tcp 服务端函数声明 静态*/
static void tcp_server_sent_cb(void* arg);
static void tcp_server_recv_cb(void* arg, char* pdata, unsigned short length);
static void tcp_server_recon_cb(void* arg, sint8 error);
static void tcp_server_discon_cb(void* arg);
static void tcp_server_listen_cb(void* arg);

/*************************************************
 Function: tcp_server_sent_cb(void* arg)
 Description: tcp 服务发送数据成功回调
 Calls:// 被本函数调用的函数清单

 Called By: // 调用本函数的函数清单
	 wangLuo.c: espconn_regist_recvcb(pespconn, tcp_server_recv_cb)
 *************************************************/
static void ICACHE_FLASH_ATTR
tcp_server_sent_cb(void* arg)
{
	os_printf("tcp server send data successful\r\n");

}

/*************************************************
 Function: tcp_server_recv_cb(void* arg, char* pdata, unsigned short len)
 Description: tcp 服务接收处理数据回调
 Calls:// 被本函数调用的函数清单
	strcmp(): 比较字符串str1和str2是否相同。如果相同则返回0
	os_sprintf(): 将第二个参数格式化打印到第一个参数，于赋值差不多。
				   //除了前两个参数固定外，可选参数可以是任意个
				   //只要在printf中可以使用的格式化字符串，在sprintf都可以使用
	strlen(): 它用来计算指定字符串 str 的长度，但不包括结束字符（即 null 字符）
	espconn_send(): 发送信息；原型声明 sint8 espconn_send(struct espconn *espconn, uint8 *psent, uint16 length)
				   //1.结构体类型参数，2.发送的信息，3.信息长度
 Called By: // 调用本函数的函数清单
	 wangLuo.c: espconn_regist_recvcb(pespconn, tcp_server_recv_cb)
 *************************************************/
static void ICACHE_FLASH_ATTR
tcp_server_recv_cb(void* arg, char* pdata, unsigned short len)
{
	os_printf("length: %d \r\ndata: %s\r\n", len, pdata);
	/*判断，结果为0就发送信息，并把每次发送的序号+1*/
	if (strcmp(pdata, "a001") == 0)
	{
		uint16 xu_hao = 1;
		uint8 fa_song_shu_ju[50];
		os_sprintf(fa_song_shu_ju, "hi this is ESP8266 TCP SERVER![%d]\r\n", xu_hao);//23
	   /*strlen 是一个函数，它用来计算指定字符串 str 的长度，但不包括结束字符（即 null 字符）*/
		espconn_send((struct espconn*) arg, fa_song_shu_ju, strlen(fa_song_shu_ju));
		xu_hao++;
	}
}

/*************************************************
 Function: tcp_server_recon_cb(void* arg, sint8 error)
 Description: tcp 服务重新连接听回调
 Calls:// 被本函数调用的函数清单

 Called By: // 调用本函数的函数清单
	 wangLuo.c: espconn_regist_recvcb(pespconn, tcp_server_recv_cb)
 *************************************************/
static void ICACHE_FLASH_ATTR
tcp_server_recon_cb(void* arg, sint8 error)
{
	os_printf("tcp server connect tcp client error %d\r\n", error);
	//重新连接程序....
}

/*************************************************
 Function: tcp_server_discon_cb(void* arg)
 Description: tcp 服务端断开回调
 Calls:// 被本函数调用的函数清单
 Called By: // 调用本函数的函数清单
	 wangLuo.c: espconn_regist_disconcb(pespconn, tcp_server_discon_cb)
 *************************************************/
static void ICACHE_FLASH_ATTR
tcp_server_discon_cb(void* arg)
{
	os_printf("tcp server disconnect tcp client successful\r\n");
	//断开后的程序....
}

/*************************************************
 Function: tcp_server_listen_cb(void* arg)
 Description: tcp 服务端监听回调
 Calls:// 被本函数调用的函数清单
	espconn_regist_recvcb(pespconn, tcp_server_recv_cb);//注册收到数据回调函数
	espconn_regist_sentcb(pespconn, tcp_server_sent_cb);//注册发送完数据回调函数
	espconn_regist_disconcb(pespconn, tcp_server_discon_cb);//注册断开连接回调函数
 Called By: // 调用本函数的函数清单
	 wangLuo.c: tcp_server_init(struct espconn* espconn, uint16 local_port)
	 wangLuo.c: tcp_server_init1(struct espconn* espconn, uint16 local_port)
 *************************************************/
static void ICACHE_FLASH_ATTR
tcp_server_listen_cb(void* arg)
{
	struct espconn* pespconn = arg;

	os_printf("tcp server have tcp client connect\r\n");
	espconn_regist_recvcb(pespconn, tcp_server_recv_cb);//注册收到数据回调函数
	espconn_regist_sentcb(pespconn, tcp_server_sent_cb);//注册发送完数据回调函数
	espconn_regist_disconcb(pespconn, tcp_server_discon_cb);//注册断开连接回调函数

}

/*************************************************
 Function: tcp_server_send_data(struct espconn* espconn, uint8* pdata, uint16 length)
 Description: tcp 服务端发送信息
 Calls:// 被本函数调用的函数清单
	 espconn_send(espconn, pdata, length);
 Called By: // 调用本函数的函数清单
	 wangLuo.c: tcp_server_init(struct espconn* espconn, uint16 local_port)
	 wangLuo.c: tcp_server_init1(struct espconn* espconn, uint16 local_port)
 *************************************************/
sint8 ICACHE_FLASH_ATTR
tcp_server_send_data(struct espconn* espconn, uint8* pdata, uint16 length)
{
	if (espconn == NULL) {
		return ESPCONN_ARG;
	}

	espconn_send(espconn, pdata, length);
}

/*************************************************
Function: tcp_server_init1(struct espconn* espconn, struct ip_addr* local_ip, uint16 local_port)
Description: tcp 服务端配置，混合模式。
Calls:// 被本函数调用的函数清单
	os_zalloc: 申请(开辟)内存;(esp_tcp*)os_zalloc(sizeof(esp_tcp))
			   //申请一块大小为sizeof(esp_tcp)的内存，并将申请的内存强制转换
			   //为esp_tcp类型.
   espconn_regist_connectcb(espconn, tcp_server_listen_cb);//注册Server监听回调函数
   espconn_regist_reconcb(espconn, tcp_server_recon_cb);//注册断连重新连接回调函数
   espconn_accept(espconn);//创建Server,开始监听
   espconn_regist_time(espconn, 360, 0);//超时断开连接时间
Called By: // 调用本函数的函数清单
	user_main.c: station_cnnect_callback(uint8_t status)
*************************************************/
void ICACHE_FLASH_ATTR
tcp_server_init1(struct espconn* espconn, struct ip_addr* local_ip, uint16 local_port)
{
	//uint32 server_ip = ipaddr_addr(remote_ip);
	os_printf("tcp server waiting tcp client connect!\r\n");
	espconn->proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;
	os_memcpy(espconn->proto.tcp->local_ip, local_ip, 4);//设置本地IP地址
	espconn->proto.tcp->local_port = local_port;//设置本地监听的端口号，等待Client连接
	espconn_regist_connectcb(espconn, tcp_server_listen_cb);//注册Server监听回调函数
	espconn_regist_reconcb(espconn, tcp_server_recon_cb);//注册断连重新连接回调函数
	espconn_accept(espconn);//创建Server,开始监听
	espconn_regist_time(espconn, 360, 0);//超时断开连接时间
}

/*************************************************
Function: tcp_server_init1(struct espconn* espconn, struct ip_addr* local_ip, uint16 local_port)
Description: tcp 服务端配置,在wifi客户端模式和AP模式都能正常运行
Calls:// 被本函数调用的函数清单
	os_zalloc: 申请(开辟)内存;(esp_tcp*)os_zalloc(sizeof(esp_tcp))
			   //申请一块大小为sizeof(esp_tcp)的内存，并将申请的内存强制转换
			   //为esp_tcp类型.
	espconn_regist_connectcb(espconn, tcp_server_listen_cb);//注册Server监听回调函数
	espconn_regist_reconcb(espconn, tcp_server_recon_cb);//注册断连重新连接回调函数
	espconn_accept(espconn);//创建Server,开始监听
	espconn_regist_time(espconn, 360, 0);//超时断开连接时间
Called By: // 调用本函数的函数清单
	user_main.c: station_cnnect_callback(uint8_t status)
*************************************************/
void ICACHE_FLASH_ATTR
tcp_server_init(struct espconn* espconn, uint16 local_port)
{
	os_printf("tcp server waiting tcp client connect!\r\n");
	espconn->proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	espconn->type = ESPCONN_TCP;
	espconn->proto.tcp->local_port = local_port;//设置本地监听的端口号，等待Client连接
	espconn_regist_connectcb(espconn, tcp_server_listen_cb);//注册Server监听回调函数
	espconn_regist_reconcb(espconn, tcp_server_recon_cb);//注册断连重新连接回调函数
	espconn_accept(espconn);//创建Server,开始监听
	espconn_regist_time(espconn, 360, 0);//超时断开连接时间
}



/***********************获取网页数据*********************************************************/



//剖析URL
void ICACHE_FLASH_ATTR
fen_xi_url(char* URL, char* host, char* filename, unsigned short* port)
{


	char* PA;
	char* PB;

	memset(host, 0, sizeof(host));
	memset(filename, 0, sizeof(filename));

	*port = 0;

	if (!(*URL)) {
		uart0_sendStr("\r\n ----- URL return -----  \r\n");
		return;
	}

	PA = URL;
	//strncmp 把 str1 和 str2 进行比较，最多比较前 n 个字节。
	//strlen 计算字符串 str 的长度，直到空结束字符，但不包括空结束字符。
	if (!strncmp(PA, "http://", strlen("http://"))) {
		PA = URL + strlen("http://");
	}
	else if (!strncmp(PA, "https://", strlen("https://"))) {
		PA = URL + strlen("https://");
	}
	//strchr 在参数1中搜索第一次出现参数2的位置
	PB = strchr(PA, '/');  //PB=7

	if (PB) {
		uart0_sendStr("\r\n ----- PB=true -----  \r\n");
		//memcpy 从参数2复制数据到参数1,大小为参数3
		memcpy(host, PA, strlen(PA) - strlen(PB));
		if (PB + 1) {
			memcpy(filename, PB + 1, strlen(PB - 1));
			filename[strlen(PB) - 1] = 0;
		}
		host[strlen(PA) - strlen(PB)] = 0;

		//uart0_sendStr(host, strlen(host));
		os_printf("shu ju wei:%s\n", host);
		uart0_sendStr(host);


	}
	else {
		uart0_sendStr("\r\n ----- PB=false -----  \r\n");
		memcpy(host, PA, strlen(PA));
		host[strlen(PA)] = 0;
		uart0_sendStr(host);
		//uart0_sendStr(host, strlen(host));
	}

	PA = strchr(host, ':');

	if (PA) {
		*port = atoi(PA + 1);
	}
	else {
		*port = 80;
	}
}

/*接收数据*/
void ICACHE_FLASH_ATTR user_tcp_recv_cb(void* arg, char* pdata, unsigned short len) 
{
	uart0_sendStr("\r\n ----- kaiShiJieShouShuJu----- \r\n ");
	uart0_tx_buffer(pdata, strlen(pdata));//用uart0传输缓冲区
	uart0_sendStr("\r\n -----jieSuJieShouShuJu-----  \r\n ");

}

/*成功发送数据后*/
void ICACHE_FLASH_ATTR user_tcp_sent_cb(void* arg) 
{
	uart0_sendStr("faSongChengGong\r\n ");
}

/*连接断开*/
void ICACHE_FLASH_ATTR user_tcp_discon_cb(void* arg) 
{
	uart0_sendStr("lianJieDuanKair\n ");
}

/*连接成功*/
void ICACHE_FLASH_ATTR user_tcp_connect_cb(void* arg) 
{
	struct espconn* pespconn = arg;
	espconn_regist_recvcb(pespconn, user_tcp_recv_cb); //接收到数据
	espconn_regist_sentcb(pespconn, user_tcp_sent_cb);  //成功发送数据
	espconn_regist_disconcb(pespconn, user_tcp_discon_cb); //连接断开

	uart0_sendStr("\r\n ----- qingQiuKaiShi----- \r\n");
	uart0_tx_buffer(buffer, strlen(buffer));
	uart0_sendStr("\r\n -----qingQiuJieSu-----  \r\n");
	espconn_sent(pespconn, buffer, strlen(buffer));  //发送数据到服务器

}

/*连接失败,err为错误代码*/
void ICACHE_FLASH_ATTR user_tcp_recon_cb(void* arg, sint8 err) {
	os_printf("lianJieCuoWu,daiMa: %d\r\n", err);
	espconn_connect((struct espconn*) arg);
}

/**网页连接配置,TCP配置**/
void ICACHE_FLASH_ATTR 
my_station_init(struct ip_addr* remote_ip, struct ip_addr* local_ip, int remote_port) 
{
	//
	user_tcp_conn.type = ESPCONN_TCP;
	user_tcp_conn.state = ESPCONN_NONE;//使TCP功能失效
	user_tcp_conn.proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	os_memcpy(user_tcp_conn.proto.tcp->local_ip, local_ip, 4);
	os_memcpy(user_tcp_conn.proto.tcp->remote_ip, remote_ip, 4);
	user_tcp_conn.proto.tcp->local_port = espconn_port();
	user_tcp_conn.proto.tcp->remote_port = remote_port;
	//注册
	espconn_regist_connectcb(&user_tcp_conn, user_tcp_connect_cb);//连接成功回调
	espconn_regist_reconcb(&user_tcp_conn, user_tcp_recon_cb);//连接失败回调
	espconn_connect(&user_tcp_conn);
}

/**网页连接配置,TCP配置,未成功**/
void ICACHE_FLASH_ATTR
my_station_init_ip(uint8* remote_ip, struct ip_addr* local_ip, int remote_port)
{
	uint32 server_ip = ipaddr_addr(remote_ip);
	user_tcp_conn.type = ESPCONN_TCP;
	user_tcp_conn.state = ESPCONN_NONE;//使TCP功能失效
	user_tcp_conn.proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	os_memcpy(user_tcp_conn.proto.tcp->local_ip, local_ip, 4);
	os_memcpy(user_tcp_conn.proto.tcp->remote_ip, &server_ip, 4);
	user_tcp_conn.proto.tcp->local_port = espconn_port();
	user_tcp_conn.proto.tcp->remote_port = 80;
	//注册
	espconn_regist_connectcb(&user_tcp_conn, user_tcp_connect_cb);//连接成功回调
	espconn_regist_reconcb(&user_tcp_conn, user_tcp_recon_cb);//连接失败回调
	espconn_connect(&user_tcp_conn);
}

/*DNS解析*/
void ICACHE_FLASH_ATTR
dns_cu_wu_callback(const char* name, ip_addr_t* ipaddr, void* arg) {

	struct ip_info info;
	wifi_get_ip_info(STATION_IF, &info);
	my_station_init(ipaddr, &info.ip, port);

	//my_station_init_ip(HTTP_IP, &info.ip, port);

}

/***get请求******/
void ICACHE_FLASH_ATTR
get_acquire(char* URL)
{
	struct ip_addr addr;
	os_memset(buffer, 0, 1024);
	fen_xi_url(URL, host, filename, &port);
	os_sprintf(buffer, GET, filename, host);
	espconn_gethostbyname(&user_tcp_conn, host, &addr, dns_cu_wu_callback);//将域名转换为IP
}
