/*
 * wifi.h
 *
 *  Created on: Dec 30, 2014
 *      Author: Minh
 */

#ifndef USER_WIFI_H_
#define USER_WIFI_H_
#include "os_type.h"
/*****  typedef void   定义一个函数指针类型，用于处理参数和返回值的形式都一样，但是功能不确定的一组函数，可以使用函数指针。*************************************************
*1   typedef void (*WifiCallback)(uint8_t); typedef的功能是定义新的类型。这一句就是定义了一种WifiCallback的类型，
*                                           并定义这种类型为指向某种函数的指针，这种函数以一个uint8_t为参数并返回unsigned char类型。后面就可以像使用int,char一样使用WifiCallback了。
*2   (uint8_t)                 最内层 (uint8_t * ) 函数的参数列表 ，仅有一个参数，为uint8_t类型的指针    
*3   void (*WifiCallback)      函数指针 该函数指针 指向一个 上面一行那个类型的函数,函数返回值为void
*    WifiCallback wifiCb = NULL;            WifiCallback wifiCb 使用这个新类型定义了变量wifiCb，并对变量wifiCb赋予空值
***********************************************************************************************************************************************************************************/
typedef void (*WifiCallback)(uint8_t);
void ICACHE_FLASH_ATTR WIFI_Connect(uint8_t* ssid, uint8_t* pass, WifiCallback cb);


#endif /* USER_WIFI_H_ */
