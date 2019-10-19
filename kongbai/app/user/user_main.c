
/**C语言 main.c如何引用另一c文件中的函数
 * 如果main.c需要引用b.c中的void led_gpio_config（）函数，则需要新建一个b.h，在b.h中将所有b.c函数进行声明。
 * 如在b.c中有函数void led_gpio_config()，则在b.h中声明如下：void led_gpio_config(参数，如没有参数用void)。
 * 然后在main.c中 include"b.h" 即可
 */
/**  #include
 * #include 命令告诉预处理器将指定头文件的内容插入到预处理器命令的相应位置
 * 如果需要包含标准库头文件或者实现版本所提供的头文件，应该使用#include <文件名>
 * 如果需要包含针对程序所开发的源文件，则应该使用#include "文件名"
 * 对于命令中用双引号指定的文件（"文件名"），预处理器通常首先在当前目录下寻找，也就是包含该程序其他源文件的目录
 * 对于命令中使用尖括号指定的文件（<文件名>），预处理器通常会在特定的系统路径下搜索
 */
#include "ets_sys.h"
#include "osapi.h" //串口1需要的头文件
#include "ip_addr.h"
#include "espconn.h"//TCP功能需要的头文件
#include "mem.h" //系统操作需要的头文件


#include "user_interface.h"//WIFI连接需要的头文件
#include "smartconfig.h" //智能配置
#include "airkiss.h" //配网
#include "modules/wifi.h"
#include "user_config.h" //配置文件
#include "modules/tcpServerClient.h"
#include "modules/udp.h"
#include "cJSON.h"


//#include "gpio.h"//GPIO口操作
#include "modules/gpio_test.h"
//设置定时器名字00000000000011111111111111111111111111
os_timer_t led_01_timer;

/**  #define
 * #define 定义一个标识符来表示一个常量。其特点是：定义的标识符不占内存，只是一个临时的符号，预编译后这个符号就不存在了。
 * #define  标识符  常量   //注意, 最后没有分号
 * #define 和 #include 一样，也是以“#”开头的。凡是以“#”开头的均为预处理指令
 * #define又称宏定义，标识符为所定义的宏名，简称宏,变量名表示的是一个变量，但宏名表示的是一个常量。可以给变量赋值，但绝不能给常量赋值
 * 宏所表示的常量可以是数字、字符、字符串、表达式。其中最常用的是数字
 */
#define DEVICE_TYPE 		"gh_9e2cff3dfa51" //wechat public number  微信公众数量
#define DEVICE_ID 			"122475" //model ID  模型ID

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
//定时回调函数
blue_led_timer_toggle1(void){
		 /**定时器
		 * 对于同一个定时器，os_time_aem或os_timer_arm_us不能重复调用，必须先用os_time_disarm取消定时器。
		 * 定时器回调os_timer_setfn必须要有，并且必须在定时器未使能的情况下调用，
		 * 在 os_timer_arm 或os_timer_arm_us 之前或者 os_timer_disarm 之后。
		 *os_timer_disarm 取消定时器
		 *os_timer_setfn 定时器回调
		 *os_timer_arm   使能毫秒级定时器
		 *os_timer_arm_us 使能微妙级定时器
		 *system_timer_reinit 重新初始化定时器，当使用微妙级定时器时调用。
		 */
	os_timer_disarm(&led_01_timer);//取消定时器
	uint32 status = GPIO_INPUT_GET(GPIO_ID_PIN(2));//获取蓝灯管脚状态
	GPIO_OUTPUT_SET(GPIO_ID_PIN(2),!status);//取反实现蓝灯管脚电平反转，从而实现亮灭操作
	//os_printf("\n\nchenxin! KO0001 %d gg\n\n",cs1);
	if(status == 1){
		os_printf("\nchenxin! KO0001 %d gg\n",cs1);
		cs1++;
	}

	os_timer_arm(&led_01_timer, 1000, true);//使能定时器
}

//如果
#if   TCP_CLIENT
/*******************  struct  *****************************************************
 * 在C语言中，可以使用结构体（Struct）来存放一组不同类型的数据
 * 结构体是一种集合，它里面包含了多个变量或数组，它们的类型可以相同，也可以不同
    struct stu{
    char *name;  //姓名
    int num;  //学号
    int age;  //年龄
    char group;  //所在学习小组
    float score;  //成绩
    };
    既然结构体是一种数据类型，那么就可以用它来定义变量。
    struct stu stu1;或者struct stu stu1, stu2;
    stu1机可以表示stu
    stu 就像一个“模板”，定义出来的变量都具有相同的性质
    也可以在定义结构体的同时定义结构体变量
    struct stu{
    char *name;  //姓名
    int num;  //学号
    int age;  //年龄
    }stu1;
 ************************************************************************************/
struct espconn tcp_client;
//其他条件判断
#elif TCP_SERVER
struct espconn *tcp_server;
#elif UDP_TEST
struct espconn udp_test;
//结束
#endif

uint8  messages_send_buffer[50];
uint16 messages_send_count = 0;

//毫秒定时器，os_timer_t 定时器结构
os_timer_t wifistate_checktimer;
os_timer_t send_data_timer;

//没有返回值的函数，其类型为 void
void ICACHE_FLASH_ATTR
//函数原型的声明，参数列表中的关键字 void 表示该函数没有参数
//tcp功能
TCP_Send_data(void){

#if   TCP_CLIENT
	//os_sprintf 将一个字符串替换另外一个字符串
	os_sprintf(messages_send_buffer,"hi this is ESP8266 TCP client![%d]\r\n",messages_send_count);//23
	//strlen 是一个函数，它用来计算指定字符串 str 的长度，但不包括结束字符（即 null 字符）
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

//TCP自动发送信息回调
void ICACHE_FLASH_ATTR
WifiStatus_Check(void){
	uint8 wifiStatus;

//	uint8 *macaddr;

	wifiStatus = wifi_station_get_connect_status();
	if (wifiStatus == STATION_GOT_IP) {
		os_printf("WiFi connection is successful!\r\n");//17
		os_timer_disarm(&wifistate_checktimer);//取消定时器
		struct ip_info local_ip;
		wifi_get_ip_info(STATION_IF,&local_ip);

//		wifi_get_macaddr(STATION_IF,macaddr);
//		os_sprintf("MAC version:%s\n",MAC2STR(macaddr));

#if   TCP_CLIENT
		tcp_client_init(&tcp_client,TCP_SERVER_IP,&local_ip.ip,TCP_SERVER_PORT);
		os_timer_disarm(&send_data_timer);//取消定时器
		os_timer_setfn(&send_data_timer, (os_timer_func_t *) TCP_Send_data,NULL);//定时器回调函数
		os_timer_arm(&send_data_timer, 40000, true);//使能毫秒级定时器,1000毫秒 = 1秒
#elif TCP_SERVER
		tcp_server_init(tcp_server,TCP_LOCAL_PORT);
		//tcp_client_init1(&tcp_server,TCP_SERVER_IP,&local_ip.ip,TCP_LOCAL_PORT);
		//取消定时器
		os_timer_disarm(&send_data_timer);
		//定时器回调函数
		os_timer_setfn(&send_data_timer, (os_timer_func_t *) TCP_Send_data,NULL);
		//使能毫秒级定时器
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

/**
 * Wi-Fi连接回调函数
 */
void ICACHE_FLASH_ATTR
wifiConnectCb(uint8_t status){
	/*************************开始***************************************
	 * 打印错误信息，
	 * reset reason: 6  外部系统复位
	 * 				 0 电源正常启动
	 * 				 1 硬件看门狗复位
	 *               2 异常复位，gpio状态不变
	 *               3 软件看门狗复位，gpio状态不变
	 *               4 软件重启，system_restart, gpio状态不变
	 *               5 从沉睡中醒来
	 */
	struct rst_info *rtc_info = system_get_rst_info();
		os_printf("reset reason: %x\n", rtc_info->reason);
		if (rtc_info->reason == REASON_WDT_RST ||
		rtc_info->reason == REASON_EXCEPTION_RST ||
		rtc_info->reason == REASON_SOFT_WDT_RST) {
		if (rtc_info->reason == REASON_EXCEPTION_RST) {
		os_printf("Fatal exception (%d):\n", rtc_info->exccause);
		}
		//打印上次崩溃的地址，多用于崩溃时乱码调试
		os_printf("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x,excvaddr=0x%08x, depc=0x%08x\n",
				rtc_info->epc1, rtc_info->epc2, rtc_info->epc3, rtc_info->excvaddr, rtc_info->depc);
		}
	/****************************结束*********************************************/

	/**定时器
	 * 对于同一个定时器，os_time_aem或os_timer_arm_us不能重复调用，必须先用os_time_disarm取消定时器。
	 * 定时器回调os_timer_setfn必须要有，并且必须在定时器未使能的情况下调用，
	 * 在 os_timer_arm 或os_timer_arm_us 之前或者 os_timer_disarm 之后。
	 *os_timer_disarm 取消定时器
	 *os_timer_setfn 定时器回调
	 *os_timer_arm   使能毫秒级定时器
	 *os_timer_arm_us 使能微妙级定时器
	 *system_timer_reinit 重新初始化定时器，当使用微妙级定时器时调用。
	 */
	//取消定时器
	os_timer_disarm(&wifistate_checktimer);
	//定时器回调函数
	os_timer_setfn(&wifistate_checktimer, (os_timer_func_t *) WifiStatus_Check,NULL);
	//使能毫秒级定时器
	os_timer_arm(&wifistate_checktimer, 2000, true);
}

//ICACHE_FLASH_ATTR  相应程序将存放到flash中，被调用时才加载到cache运行。
//添加了IRAM_ATTR宏的函数，会在上电后就加载到iRAM
void ICACHE_FLASH_ATTR
user_pre_init(void)
{
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}

#define lec 2
#define sec 137  //扇区号
#define chang_duc 1
#define sec_pianyi 0



void ICACHE_FLASH_ATTR
user_init(void)
{
	/** wifi_set_opmode
		 * 设置并保存WIFI模块工作模式，默认AP模式
		 * 0x01 连接模式
		 * 0x02 热点AP模式
		 * 0x03混合模式
		 */
	//设置并保存WIFI模块工作模式-客服端模式
	wifi_set_opmode(0x01); 
	//连接AP，并启动回调函数wifiConnectCb
	WIFI_Connect(STA_SSID, STA_PASS, wifiConnectCb);
	//输出SDK版本,%s 输出字符
    os_printf("SDK version:%s\n", system_get_sdk_version());
    //输出芯片ID(唯一),%d 输出十进制整数
    os_printf("ID version:%d\n", system_get_chip_id());
    //内存空间分配情况
    system_print_meminfo();

    //延时50us,最大值65535
    os_delay_us(50);
    //查询系统剩余可用 heap 区空间大小
    os_printf("KJDX version:%d\n", system_get_free_heap_size());
    //查询系统时间
	os_printf("CXSJ version:%d\n", system_get_time());
	
	//打印当前分配的堆空间所有内存块..
	//system_show_malloc();

    os_printf("\n\nHello World0001! KO\n\n");

    uint8 ser_ip[lec*4];
	uint32 value;  //typedef unsigned int        uint32;
	/********************指针**********************************
	&  获取变量内存地址
	*  获取内存地址的值
	   uint8* addr_case1等价于 uint8 *addr_case1
	   uint8* addr_case1  char类型的指针变量  
	   &value  变量value的内存地址
	   (uint8*) 强制类型转换指针

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
	                                          //这里&是取地址运算符,*是间接运算符
	//spi_flash_write(sec*4*1024+sec_pianyi, (uint32 *)&addr_case1, sizeof(addr_case1));
//	spi_flash_write(sec*4*1024+sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));
//	os_printf("@@@@@@FLASH write@@@@@:%02x%02x\r\n", addr_case1[0], addr_case1[1]);

	
	spi_flash_read(sec*4*1024+sec_pianyi, (uint32*)addr_case1, sizeof(addr_case1));//读取flash值
	os_printf("@@@@@@FLASH000123 read@@@@@:%d %d\r\n", addr_case1[0], addr_case1[1]);
	


    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);//蓝灯
    GPIO_OUTPUT_SET(GPIO_ID_PIN(2),1);//设置GPIO2为高电平输出(关)
//    	os_timer_disarm(&led_01_timer);
//    	os_timer_setfn(&led_01_timer, (os_timer_func_t *) blue_led_timer_toggle1,NULL);//定时回调函数
//    	os_timer_arm(&led_01_timer, 1000, true);//设置时间为1s
    //gpio_test();
    //blue_led_timer_toggle1();

}
