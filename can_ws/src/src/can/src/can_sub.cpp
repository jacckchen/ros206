/*
    需求: 实现基本的话题通信，一方发布数据，一方接收数据，
         实现的关键点:
         1.发送方
         2.接收方
         3.数据(此处为普通文本)


    消息订阅方:
        订阅话题并打印接收到的消息

    实现流程:
        1.包含头文件 
        2.初始化 ROS 节点:命名(唯一)
        3.实例化 ROS 句柄
        4.实例化 订阅者 对象
        5.处理订阅的消息(回调函数)
        6.设置循环调用回调函数

*/
// 1.包含头文件 
#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int32.h"

//can相关头文件
#include <stdio.h> //各种输入输出函数
#include <sys/types.h> //规定数据类型
#include <sys/stat.h> //文件信息
#include <fcntl.h> //文件操作函数
#include <pthread.h> //pthread操作系统
#include "controlcan.h" 

#include <ctime> //时间相关
#include <cstdlib> //各类操作函数
#include "unistd.h" //linux底层函数

using namespace std;

//打开和初始化设备的一些参数配置
int nDeviceType = 4; //设备类型：CANalyst-II就用4
int nDeviceInd = 0; //设备索引：1个USB-CAN适配器就是0
int nCANInd0 = 0;//CAN1
int nCANInd1 = 1;//CAN2

//初始化can1
bool Init_CAN1(int nDeviceType, int nDeviceInd, int nCANInd, VCI_INIT_CONFIG config) {
	VCI_ResetCAN(nDeviceType, nDeviceInd, nCANInd);
    if (VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd, &config) != 1)
    {
        VCI_CloseDevice(nDeviceType, nDeviceInd);
        ROS_INFO_STREAM(">>Init CAN1 error");
        return(0);
    }
    VCI_ClearBuffer(nDeviceType, nDeviceInd, nCANInd);
    if (VCI_StartCAN(nDeviceType, nDeviceInd, nCANInd) != 1)
    {
        VCI_CloseDevice(nDeviceType, nDeviceInd);
        ROS_INFO_STREAM(">>Start CAN1 error");
        return(0);
    }
	else
	{
    	ROS_INFO_STREAM(">>Start CAN1 success");
    	return(1);
	}
}

//初始化can2
bool Init_CAN2(int nDeviceType, int nDeviceInd, int nCANInd, VCI_INIT_CONFIG config) {
	VCI_ResetCAN(nDeviceType, nDeviceInd, nCANInd);
    if (VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd, &config) != 1)
    {
        VCI_CloseDevice(nDeviceType, nDeviceInd);
        ROS_INFO_STREAM(">>Init CAN2 error");
        return(0);
    }
    VCI_ClearBuffer(nDeviceType, nDeviceInd, nCANInd);
    if (VCI_StartCAN(nDeviceType, nDeviceInd, nCANInd) != 1)
    {
        VCI_CloseDevice(nDeviceType, nDeviceInd);
        ROS_INFO_STREAM(">>Start CAN2 error");
        return(0);
    }
	else
	{
    	ROS_INFO_STREAM(">>Start CAN2 success");
    	return(1);
	}
}


bool CAN_ConfigAndInit ()
{
    //打开设备：注意一个设备只能打开一次
	if(VCI_OpenDevice(nDeviceType,nDeviceInd,0)==1)
	{
        ROS_INFO_STREAM(">>open deivce success!");//打开设备成功
	}else
	{
        ROS_INFO_STREAM(">>open deivce error!");//打开设备失败
		exit(1); //退出整个程序，终止进程：返回1给操作系统说明是非正常运行导致程序退出
	}

	//配置CAN
	//初始化参数，严格参数二次开发函数库说明书。
	VCI_INIT_CONFIG config;
	config.AccCode=0;//过滤帧配置
	config.AccMask=0xFFFFFFFF;
	config.Filter=1;//接收所有帧(标准帧和拓展帧)
	//config.Timing0=0x03;/*波特率125 Kbps  0x03  0x1C*/
	//config.Timing1=0x1C;
	//config.Timing0=0x01;/*波特率250 Kbps  0x00  0x1C*/
	//config.Timing1=0x1C; 	 
	config.Timing0=0x00;/*波特率500 Kbps  0x00  0x1C*/
	config.Timing1=0x1C; 
	config.Mode = 0; //正常模式	
	
	//初始化用来接收的数据帧，帧结构体数组的长度设置为1


    if ( Init_CAN1(nDeviceType, nDeviceInd, nCANInd0, config) )
	{
        printf("CAN_Init Success!");
    }
    else
    {
        printf("CAN_Init Error!");
    }

    return (1);
}



void doMsg(const std_msgs::Int32::ConstPtr& msg_p){
    ROS_INFO("我听见:%d",msg_p->data);
    
    //初始化用来接收的数据帧，帧结构体数组的长度设置为50
    int reclen = 0;
    int i,j,k, order;
    int num = 50;
    VCI_CAN_OBJ rec[num];

    VCI_CAN_OBJ send[0];//包含CAN1和CAN2的帧
    send[0].ID=0x603;
    send[0].SendType=0;//0正常发送，失败则重新发生，1只发送一次
    send[0].RemoteFlag=0;//1远程帧即遥控帧，用于请求数据，0数据帧，用于发送数据
    send[0].ExternFlag=0;//0标准帧，1拓展帧
    send[0].DataLen=8;//数据8字节

    const unsigned char Stop[]      = { 0x2B,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }; //停止
    const unsigned char Forward[]   = { 0x2B,0x00,0x00,0x00,msg_p->data,msg_p->data,0x00,0x00 }; //前进
    const unsigned char Back[]      = { 0x2B,0x00,0x00,0x00,0xCE,0xCE,0x00,0x00 }; //后退
    const unsigned char TurnRight[] = { 0x00,0x00,0x00,0x00,0xF4,0x01,0x00,0x00 }; //左转
    const unsigned char TurnLeft[]  = { 0x00,0x00,0x00,0x00,0x00,0x00,0xF4,0x01 }; //右转

    for (k = 0; k < 8; k++)
			{
				send[0].Data[k] = Forward[k];
			}
            VCI_Transmit(nDeviceType, nDeviceInd, nCANInd0, send, 1);
            ROS_INFO(" Forward\n");
}

  /*  order = ( msg_p->data ) % 2;

    switch (order) 
    {
	    case 0: //默认
        {   
            for (k = 0; k < 8; k++)
            {
                send[0].Data[k] = Forward[k];
            }bool CAN_ConfigAndInit ();
            VCI_Transmit(nDeviceType, nDeviceInd, nCANInd0, send, 1);
            ROS_INFO(" Forward\n");
            break;
        }
        	   
         case 1: //默认
        {   
            for (k = 0; k < 8; k++)
            {
                send[0].Data[k] = Back[k];
            }
            VCI_Transmit(nDeviceType, nDeviceInd, nCANInd0, send, 1);
            ROS_INFO(" Back\n");
            break;
        }
					
		default: //检查和处理错误情况
		{
			for (k = 0; k < 8; k++)
			{
				send[0].Data[k] = Stop[k];
			}
            VCI_Transmit(nDeviceType, nDeviceInd, nCANInd0, send, 1);
            ROS_INFO(" Stop\n");
			break;
			}
	}

    */

int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    
    CAN_ConfigAndInit () ;

    //2.初始化 ROS 节点:命名(唯一)
    ros::init(argc,argv,"listener");
    //3.实例化 ROS 句柄
    ros::NodeHandle nh;

    //4.实例化 订阅者 对象
    ros::Subscriber sub = nh.subscribe<std_msgs::Int32>("chatter",10,doMsg);
    //5.处理订阅的消息(回调函数)

    //6.设置循环调用回调函数
    ros::spin();//循环读取接收的数据，并调用回调函数处理

    return 0;
}
