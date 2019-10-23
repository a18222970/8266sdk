/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>   //��Ȩ
File name: wangLuo.c // �ļ���
Author: cx           // ����
Version: 1.0        //�汾
Date: 2019.10.23    //�������
Description: // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
// �ơ�˳�򡢶����������ȹ�ϵ
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
1. Date:
Author:
Modification:
2. ...
*************************************************/
/************************************************
ʹ��#ifndef��#define��#endif��Ϻ궨��������ǣ���ֹͷ�ļ����ظ������ͱ���
#ifndef            ��#ifdef�෴���ж�ĳ�����Ƿ�δ������
#define            ����һ��Ԥ�����
#endif             #if, #ifdef, #ifndef��Щ��������Ľ�����־.
#undef            ȡ����Ķ���
#ifdef              �ж�ĳ�����Ƿ񱻶��壬���Ѷ��壬ִ���������
#if                   ����Ԥ�����е���������൱��C�﷨�е�if���
#elif                ��#if, #ifdef, #ifndef��ǰ���#elif���������㣬��ִ��#elif֮�����䣬�൱��C�﷨�е�else-if
#else              ��#if, #ifdef, #ifndef��Ӧ, ����Щ���������㣬��ִ��#else֮�����䣬�൱��C�﷨�е�else
defined         ����#if, #elif���ʹ�ã��ж�ĳ�����Ƿ񱻶���
***************************************************/

#ifndef USER_WIFI_H_
#define USER_WIFI_H_

/*��������ͷ�ļ�*/
#include "os_type.h"

/***************************
typedef void (*WifiCallback)(uint8_t);
������һ��ָ������ָ��WifiCallback���䷵��ֵ void ���ͣ������Ǻ����(uint8_t).
uint8_t pr = "6";
WifiCallback f = pr;  /*�������f����ֵpr
f(6);        /*ִ�У��൱��pr(6)
**************************************/
typedef void (*wifi_callback)(uint8_t);

/*************************************************
Function: lian_jie_AP // ��������
Description: wifi�ͻ���ģʽ����·���� // �������ܡ����ܵȵ�����
Calls: // �����������õĺ����嵥
Called By: // ���ñ������ĺ����嵥
	user_main.c: user_init(void)
Input: // �������˵��������ÿ�����������á�ȡֵ˵�����������ϵ��
    ssid��wifi���ƣ�
	pass: wifi���룻
	hui_diao: �ص�������.ע�⣺�˻ص���user_init(void)��ʼִ��
	          //ʱ�ͻ����ִ�У���wifi���ӳɹ����ֻ����ִ�С�
Output: // �����������˵����
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void ICACHE_FLASH_ATTR
lian_jie_AP(uint8_t* ssid, uint8_t* pass, wifi_callback hui_diao);










#endif
