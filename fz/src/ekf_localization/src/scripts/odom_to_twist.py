#!/usr/bin/env python
import rospy
from nav_msgs.msg import Odometry
from geometry_msgs.msg import TwistWithCovarianceStamped

class OdomToTwistWithCovariance:
    def __init__(self):
        rospy.init_node('odom_to_twist')

        self.twist_pub = rospy.Publisher('twist', TwistWithCovarianceStamped, queue_size=10)
        rospy.Subscriber('/odometry/filtered_01', Odometry, self.odom_callback)

    def odom_callback(self, msg):
        twist_msg = TwistWithCovarianceStamped()

        twist_msg.header.stamp = rospy.Time.now()
        twist_msg.header.frame_id = msg.child_frame_id

        # 复制速度和角速度
        twist_msg.twist.twist = msg.twist.twist

        # 假设一个简单的协方差矩阵，实际应用中可能需要根据你的传感器特性进行调整
        covariance = [0.01, 0, 0, 0, 0, 0,
                      0, 0.01, 0, 0, 0, 0,
                      0, 0, 0.01, 0, 0, 0,
                      0, 0, 0, 0.01, 0, 0,
                      0, 0, 0, 0, 0.01, 0,
                      0, 0, 0, 0, 0, 0.01]
        twist_msg.twist.covariance = covariance

        self.twist_pub.publish(twist_msg)

if __name__ == '__main__':
    try:
        converter = OdomToTwistWithCovariance()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass