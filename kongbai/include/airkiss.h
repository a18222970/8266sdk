/*
 * airkiss.h
 *
 *  Created on: 2015-1-26
 *      Author: peterfan
 */

//airkiss 配网
#ifndef AIRKISS_H_
#define AIRKISS_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef void* (*airkiss_memset_fn) (void* ptr, int value, unsigned int num);
typedef void* (*airkiss_memcpy_fn) (void* dst, const void* src, unsigned int num);
typedef int (*airkiss_memcmp_fn) (const void* ptr1, const void* ptr2, unsigned int num);
typedef int (*airkiss_printf_fn) (const char* format, ...);



typedef struct
{
	airkiss_memset_fn memset;
	airkiss_memcpy_fn memcpy;
	airkiss_memcmp_fn memcmp;
	airkiss_printf_fn printf;

} airkiss_config_t;

/**
  * @brief     Get airkiss lib version.获得airkiss库版本
  *
  * @attention 版本长度未知
  *
  * @param     null.
  *
  * @return    const char*
  */

const char* airkiss_version(void);


typedef enum
{
	/* the length of the data buffer is lack 数据缓冲区的长度不足*/
	AIRKISS_LAN_ERR_OVERFLOW = -5,

	/* Do not support the type of instruction 不支持的指令类型*/
	AIRKISS_LAN_ERR_CMD = -4,

	/* Error reading data package 误读数据包*/
	AIRKISS_LAN_ERR_PAKE = -3,

	/* Error function passing parameters 误差函数传递参数*/
	AIRKISS_LAN_ERR_PARA = -2,

	/* Packet data error 分组数据错误*/
	AIRKISS_LAN_ERR_PKG = -1,

	/* Message format is correct 消息格式正确*/
	AIRKISS_LAN_CONTINUE = 0,

	/* Find equipment request packet is received 接收到设备请求包*/
	AIRKISS_LAN_SSDP_REQ = 1,

	/* Packet packaging complete 包包装完成*/
	AIRKISS_LAN_PAKE_READY = 2


} airkiss_lan_ret_t;


typedef enum
{
	AIRKISS_LAN_SSDP_REQ_CMD = 0x1,
	AIRKISS_LAN_SSDP_RESP_CMD = 0x1001,
	AIRKISS_LAN_SSDP_NOTIFY_CMD = 0x1002
} airkiss_lan_cmdid_t;

/**
  * @brief     Receive UDP packet and input this API for analyzing.接收UDP数据包并输入此API进行分析
  *
  * @attention null.
  *
  * @param     const void* body : The start of the UDP message body data pointer.
  * @param     unsigned short length : the effective length of data.
  * @param     const airkiss_config_t* config : input struct airkiss_config_t
  *
  * @return    >=0 : succeed (reference airkiss_lan_ret_t)
  * @return    <0  : error code (reference airkiss_lan_ret_t)
  */

int airkiss_lan_recv(const void* body, unsigned short length, const airkiss_config_t* config);


/**
  * @brief     Packaging the UDP packet to send.
  *
  * @attention null.
  *
  * @param     airkiss_lan_cmdid_t ak_lan_cmdid : The packet type.
  * @param     void* appid : Vendor's Wechat public number id.
  * @param     void* deviceid : device model id.
  * @param     void* _datain : the data to be sent.
  * @param     unsigned short inlength : the lenth of data to be sent.
  * @param     void* _dataout : Data buffer addr.
  * @param     unsigned short* outlength : the size of data buffer.
  * @param     const airkiss_config_t* config : input struct airkiss_config_t
  *
  * @return    >=0 : succeed (reference airkiss_lan_ret_t)
  * @return    <0  : error code (reference airkiss_lan_ret_t)
  */

int airkiss_lan_pack(airkiss_lan_cmdid_t ak_lan_cmdid, void* appid, void* deviceid, void* _datain, unsigned short inlength, void* _dataout, unsigned short* outlength, const airkiss_config_t* config);

#ifdef __cplusplus
}
#endif

#endif /* AIRKISS_H_ */
