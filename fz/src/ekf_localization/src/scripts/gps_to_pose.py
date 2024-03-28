#!/usr/bin/env python
import rospy
from sensor_msgs.msg import NavSatFix
from geometry_msgs.msg import PoseWithCovarianceStamped
import pyproj

class GPSToPose:
    def __init__(self):
        rospy.init_node('gps_to_pose', anonymous=True)

        self.pose_pub = rospy.Publisher('gps_pose', PoseWithCovarianceStamped, queue_size=10)
        rospy.Subscriber('fix', NavSatFix, self.gps_callback)

        # UTM投影
        self.utm_projector = pyproj.Proj(proj='utm', zone=33, ellps='WGS84', preserve_units=False)

        self.frame_id = rospy.get_param('~frame_id', 'map')

    def gps_callback(self, msg):
        pose_msg = PoseWithCovarianceStamped()

        pose_msg.header.stamp = rospy.Time.now()
        pose_msg.header.frame_id = self.frame_id

        # 转换GPS坐标到UTM
        x, y = self.utm_projector(msg.longitude, msg.latitude)
        pose_msg.pose.pose.position.x = x
        pose_msg.pose.pose.position.y = y
        pose_msg.pose.pose.position.z = 0

        # 假设的方差，需要根据你的GPS精度调整
        covariance = [0.1, 0, 0, 0, 0, 0,
                      0, 0.1, 0, 0, 0, 0,
                      0, 0, 0.1, 0, 0, 0,
                      0, 0, 0, 0.1, 0, 0,
                      0, 0, 0, 0, 0.1, 0,
                      0, 0, 0, 0, 0, 0.1]
        pose_msg.pose.covariance = covariance

        self.pose_pub.publish(pose_msg)

if __name__ == '__main__':
    try:
        gps_to_pose = GPSToPose()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass