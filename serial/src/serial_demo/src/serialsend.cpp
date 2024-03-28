#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <serial/serial.h>

   // 创建一个串口对象    
serial::Serial ser;
uint8_t s_buffer[8]={0};
uint8_t buffer[4]={0};

// 回调函数，用于处理接收到的std_msgs的Int32MultiArray消息
void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& msg)
{
    // 将Int32MultiArray消息转换为八位十六进制数组

    for (int i = 0; i < 4; i++)
    {
        buffer[i] = msg->data[i] & 0xff;
    }
    for(int j = 0; j<4 ;j++)
    {
        s_buffer[j+2]=buffer[j];
    }
    s_buffer[0]=0x55;
    s_buffer[1]=0x55;
    s_buffer[6]=0x12;
    s_buffer[7]=buffer[3];
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ros_serial_example");
    ros::NodeHandle nh;
    // 打开串口
    ser.setPort("/dev/ttyUSB0");
    ser.setBaudrate(9600);
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    ser.setTimeout(to);
    ser.open();

    // 创建一个ROS订阅者，用于接收std_msgs的Int32MultiArray消息
    ros::Subscriber sub = nh.subscribe("/camera", 1000, arrayCallback);

    // 循环读取串口数据并发送
    ros::Rate loop_rate(10);
    while (ros::ok())
    {    
        // 向串口发送数据
        ser.write(s_buffer, 8);
        for(int i=0;i<8;i++) {ROS_INFO("%x",s_buffer[i]);}
        // 读取串口数据
        uint8_t rbuffer[8];
        size_t n = ser.read(rbuffer, 8);
        ROS_INFO("%lu",n);
        // 将读取到的数据以十六进制的方式打印到屏幕
        for (int i = 0; i < n; i++)
        {
            std::cout << std::hex << (rbuffer[i] & 0xff) << " ";
        }
        std::cout << std::endl;

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

