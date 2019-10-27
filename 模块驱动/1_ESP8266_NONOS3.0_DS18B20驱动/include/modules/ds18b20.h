/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
File name:ds18b20.h
Author: cx
Version: 1.0
Date: 2019.10.27
Description: DS18B20温度读取;
Function List:
1. uint8 Ds18b20Init(): DS18B20初始化
2. int Ds18b20ReadTemp(): 读出温度
*************************************************/

uint8 Ds18b20Init();
int Ds18b20ReadTemp();