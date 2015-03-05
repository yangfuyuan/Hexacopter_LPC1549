/******************** (C) COPYRIGHT 2014 ANO Tech ***************************
 * ����		 �������ƴ�
 * �ļ���  ��AnoCopter.cpp
 * ����    ������΢�����������
 * ����汾��V1.0
 * ʱ��		 ��2014/10/13 
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
**********************************************************************************/
#include <rtthread.h>
#include <rtdevice.h>
#include "config.h"
#include "params.h"
#include "pidctrl.h"
#include "commander.h"


static void attitude_thread_entry(void* parameter)
{	
	while(1){
	//���´���������	
	IMU_UpdateSensor();		
	
	//�����������̬
	IMU_GetAttitude();
	
	//��������̬�ڻ�����
	fc.Attitude_Inner_Loop();	
	
  rt_thread_delay(2);	
	}
}


static void attitude_outter_thread_entry(void* parameter)
{	
	while(1){
	//��������̬�⻷����
	fc.Attitude_Outter_Loop();	
	
  rt_thread_delay(5);	
	}
}

static void data_transmit_thread_entry(void* parameter)
{
	while(1){
	//���ͷ���������
	cmd.Data_Exchange();
	rt_thread_delay(10);
	}	
}

static void receiver_thread_entry(void* parameter)
{
	
	while(1){
		
  //ң��ͨ�����ݴ���
	rc.Cal_Command();
	
	//ҡ��λ�ü��
	rc.check_sticks();
	
	//ʧ�ر������
	cmd.Failsafe_Check();
	
	//LEDָʾ�ƿ���
	config.Pilot_Light();
	
	rt_thread_delay(20);	
	}
}

int apps_copter_init(void)
{
  rt_thread_t attitude_thread,data_transmit_thread,receiver_thread,attitude_outter_thread;
	
	//��ʼ��IMU�����Բ�����Ԫ��
	IMU_Init();	
	
	//��ʼ�����п���
	fc.Init();
	
	//��ʼ������
	Params_Init();
	
	//��ʼ��Transmiter
   cmd.Init("tcpserv");
	
	attitude_thread = rt_thread_create("attitude",attitude_thread_entry,RT_NULL,1024,12,5);
	if(attitude_thread != RT_NULL)
	{
		rt_thread_startup(attitude_thread);
	}
	
	attitude_outter_thread = rt_thread_create("att_outter",attitude_outter_thread_entry,RT_NULL,1024,11,5);
	if(attitude_thread != RT_NULL)
	{
		rt_thread_startup(attitude_outter_thread);
	}
	
	
	
	data_transmit_thread = rt_thread_create("transmit",data_transmit_thread_entry,RT_NULL,1024,13,5);
	if(data_transmit_thread != RT_NULL)
	{
		rt_thread_startup(data_transmit_thread);
	}
		receiver_thread = rt_thread_create("receiver",receiver_thread_entry,RT_NULL,1024,14,5);
	if(receiver_thread != RT_NULL)
	{
		rt_thread_startup(receiver_thread);
	}
	return 0;
}


/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/