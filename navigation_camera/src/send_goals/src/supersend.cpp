#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "send_goal_node");
  ros::NodeHandle nh;

  // 创建一个SimpleActionClient，连接到move_base节点的action服务器
  MoveBaseClient ac("move_base", true);

  // 等待action服务器启动
  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("等待move_base服务器启动...");
  }

//   move_base_msgs::MoveBaseGoal goal;
    move_base_msgs::MoveBaseGoal goala,goalb;

  // 设置目标点的坐标（以小车自身为原点）
//   goal.target_pose.header.frame_id = "base_link";
//   goal.target_pose.header.stamp = ros::Time::now();
//   goal.target_pose.pose.position.x = 1.0;   // 目标点的x坐标
//   goal.target_pose.pose.position.y = 0.5;   // 目标点的y坐标
//   goal.target_pose.pose.orientation.w = 1.0; // 目标点的朝向
  goala.target_pose.header.frame_id = "base_link";
  goala.target_pose.header.stamp = ros::Time::now();
  goala.target_pose.pose.position.x = 1.0;   // 目标点的x坐标
  goala.target_pose.pose.position.y = 0.5;   // 目标点的y坐标
  goala.target_pose.pose.orientation.w = 1.0; // 目标点的朝向

  goalb.target_pose.header.frame_id = "base_link";
  goalb.target_pose.header.stamp = ros::Time::now();
  goalb.target_pose.pose.position.x = 1.0;   // 目标点的x坐标
  goalb.target_pose.pose.position.y = 0.5;   // 目标点的y坐标
  goalb.target_pose.pose.orientation.w = 1.0; // 目标点的朝向

  ROS_INFO("发送目标点...");

  // 发送目标点给导航程序
  ac.sendGoal(goala);

  // 等待导航完成
  ac.waitForResult();

  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    ROS_INFO("导航到达目标点！");
  }
  else
  {
    ROS_INFO("导航失败...");
  }

  return 0;
}