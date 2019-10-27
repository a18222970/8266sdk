/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>  //版权
File name: user_config.h // 文件名
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
*************************************************/

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__
//
#define USE_OPTIMIZE_PRINTF

/*wifi名字*/
#define STA_SSID "cccxxx"

/*wifi密码*/
#define STA_PASS "chenxin1234567890"

/*选择模式，TCP客服端，服务器端，UDP*/
#define TCP_CLIENT 1  //客户端
#define TCP_SERVER 0  //服务端

/*选择GET或者POST*/
#define GET_HTTP  0  //get模式
#define POST_HTTP 0  //post模式

/*网页IP*/
#define HTTP_IP    "116.62.81.138"

/*TCP服务器地址*/
#define TCP_SERVER_IP    "192.168.2.23" 

/*TCP服务器端口*/
#define TCP_SERVER_PORT   6666

/*8266本地端口*/
#define LOCAL_PORT        8266

/*布尔类型true和false*/
#define ZI_DONG_LIAN_JIE       1
#define BU_LIAN_JIE       0









#endif

