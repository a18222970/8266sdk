/*************************************************
Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>  //��Ȩ
File name: user_config.h // �ļ���
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

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__
//
#define USE_OPTIMIZE_PRINTF

/*wifi����*/
#define STA_SSID "cccxxx"

/*wifi����*/
#define STA_PASS "chenxin1234567890"

/*ѡ��ģʽ��TCP�ͷ��ˣ��������ˣ�UDP*/
#define TCP_CLIENT 0  //�ͻ���
#define TCP_SERVER 1  //�����

/*TCP��������ַ*/
#define TCP_SERVER_IP    "192.168.2.23" 

/*TCP�������˿�*/
#define TCP_SERVER_PORT   6666

/*8266���ض˿�*/
#define LOCAL_PORT        8266

/*��������true��false*/
#define ZI_DONG_LIAN_JIE       1
#define BU_LIAN_JIE       0




#endif

