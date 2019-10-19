/*
 * wifi.h
 *
 *  Created on: Dec 30, 2014
 *      Author: Minh
 */

#ifndef USER_WIFI_H_
#define USER_WIFI_H_
#include "os_type.h"
/*****  typedef void   ����һ������ָ�����ͣ����ڴ�������ͷ���ֵ����ʽ��һ�������ǹ��ܲ�ȷ����һ�麯��������ʹ�ú���ָ�롣*************************************************
*1   typedef void (*WifiCallback)(uint8_t); typedef�Ĺ����Ƕ����µ����͡���һ����Ƕ�����һ��WifiCallback�����ͣ�
*                                           ��������������Ϊָ��ĳ�ֺ�����ָ�룬���ֺ�����һ��uint8_tΪ����������unsigned char���͡�����Ϳ�����ʹ��int,charһ��ʹ��WifiCallback�ˡ�
*2   (uint8_t)                 ���ڲ� (uint8_t * ) �����Ĳ����б� ������һ��������Ϊuint8_t���͵�ָ��    
*3   void (*WifiCallback)      ����ָ�� �ú���ָ�� ָ��һ�� ����һ���Ǹ����͵ĺ���,��������ֵΪvoid
*    WifiCallback wifiCb = NULL;            WifiCallback wifiCb ʹ����������Ͷ����˱���wifiCb�����Ա���wifiCb�����ֵ
***********************************************************************************************************************************************************************************/
typedef void (*WifiCallback)(uint8_t);
void ICACHE_FLASH_ATTR WIFI_Connect(uint8_t* ssid, uint8_t* pass, WifiCallback cb);


#endif /* USER_WIFI_H_ */
