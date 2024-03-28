#include <ros/ros.h>
#include <serial/serial.h>
#include "geometry_msgs/Twist.h"    

uint8_t header=0x55;
uint8_t ender=0x12;

int main(int argc, char** argv)
{
    setlocale(LC_ALL,"");
    // 2.初始化 ROS 节点
    ros::init(argc,argv,"control");
    ros::NodeHandle nh;
    // 3.创建发布者对象
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/odom",1000);
    geometry_msgs::Twist msg;
    // 创建串口对象
    serial::Serial ser;
    try
    {
        ser.setPort("/dev/ttyUSB0");  // 设置串口设备路径
        ser.setBaudrate(9600);      // 设置波特率
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();                   // 打开串口
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open serial port!");
        return -1;
    }

    if (ser.isOpen())
    {
        ROS_INFO_STREAM("Serial port initialized.");

        // 设置要接收数据的长度
        const int data_length = 8;
        uint8_t receivedata[data_length] = {0};

        while (ros::ok())
        {
            // 读取串口数据
            if (ser.available())
            {
                ser.read(receivedata, data_length);

                // 处理接收到的数据
                // ...

                // 打印接收到的数据
                if(receivedata[0]!=header||receivedata[1]!=header)
                {
                    ROS_INFO("error message with header");
                }
                if(receivedata[6]!=ender||receivedata[7]!=ender)
                {
                    ROS_INFO("error message with ender");
                }
                else
                {
                     for (int i = 0; i < data_length; i++)
                    {
                        ROS_INFO("Received data[%d]: %02X", i, receivedata[i]);
                        msg.linear.x=(receivedata[2]+receivedata[3])/200;
                        msg.angular.z=(receivedata[3]-receivedata[2])/50;
                        pub.publish(msg);
                    }
                }
               
            }

            ros::spinOnce();
        }
    }

    return 0;
}
