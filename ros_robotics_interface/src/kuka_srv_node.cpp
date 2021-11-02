#include <ros/ros.h>
#include <QtDebug>
#include <ros_robotics_interface/TrajectoryService.h>
#include <ros_robotics_interface/ScanPositionService.h>
#include <manipulator.h>


geometry_msgs::Pose set_point(float x,float y,float z,float a,float b,float c)
{
    geometry_msgs::Pose point;
    point.position.x = x;
    point.position.y = y;
    point.position.z = z;
    point.orientation.x = a;
    point.orientation.y = b;
    point.orientation.z = c;
    point.orientation.w = 0;
    return point;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "kuka_srv_node");
  ros::NodeHandle nh;

  ROS_INFO("KUKA Service node started!");

  ros::ServiceClient client1 = nh.serviceClient<ros_robotics_interface::TrajectoryService>("HMI_srv");
  ros::ServiceClient client2 = nh.serviceClient<ros_robotics_interface::ScanPositionService>("Scan_Pos_srv");
  ros::ServiceClient client3 = nh.serviceClient<ros_robotics_interface::TrajectoryService>("KUKA_HMI_srv");
  ros::ServiceClient client4 = nh.serviceClient<ros_robotics_interface::ScanPositionService>("KUKA_Pos_srv");

  ros_robotics_interface::TrajectoryService srv;
  ros_robotics_interface::TrajectoryService srv2;
  ros_robotics_interface::ScanPositionService srv3;

  //srv.request.rpose.push_back(set_point(244.05, 9.32, 682.44,163.76,10.71,178.07));
  //rv.request.rpose.push_back(set_point(64.86, -185.24, 598.40,101.07,-61.28,-107.73));
  //srv.request.rpose.push_back(set_point(293.43, -37.96, 470.51, -107.15,-51.32,-128.25));

  //srv.request.rpose.push_back(set_point(90.0, 0.0, 90.0, 0.0, 90.00, 180.00));
  //srv.request.rpose.push_back(set_point(0.0, 0.0, 90.0, 0.0, 90.00, 180.00));
  //srv.request.rpose.push_back(set_point(45.0, 0.0, 90.0, 0.0, 90.00, 180.00));

  //srv.request.rpose.push_back(set_point(-181.51, 270.00, 380.11, 180.0, 0.00, 90.00));
  //srv.request.rpose.push_back(set_point(202.16, 270.00, 380.11, 180.0, 0.00, 90.00));

  srv.request.rpose.push_back(set_point(-181.51, 270.00, 380.11, 180.0, 0.00, -86.3));
  srv.request.rpose.push_back(set_point(202.16, 270.00, 380.11, 180.0, 0.00, -86.3));
  srv.request.motion_type = WORKING;

  srv2.request.rpose.push_back(set_point(239.6, 104.25, 695.77, 171.62, -38.27, -63.22));
  srv2.request.rpose.push_back(set_point(309.36, -121.11, 900.63, -155.43, 0.56, -170.49));
  srv2.request.motion_type = WORKING;

  ROS_INFO("Set points");
  /*
  while(true)
  {
      client3.call(srv3);
      qDebug() << "Last_position";
      qDebug() << srv3.response.artur_scan_pose.position.x;
      qDebug() << srv3.response.artur_scan_pose.position.y;
      qDebug() << srv3.response.artur_scan_pose.position.z;
      qDebug() << srv3.response.artur_scan_pose.orientation.x;
      qDebug() << srv3.response.artur_scan_pose.orientation.y;
      qDebug() << srv3.response.artur_scan_pose.orientation.z;
  }
  */


  if (client1.call(srv))
  {
    ROS_INFO("Data transmission succeded");
  }
  else
  {
    ROS_INFO("Data transmission failed");
    return 1;
  }


  if (client3.call(srv2))
  {
    ROS_INFO("Data transmission succeded");
  }
  else
  {
    ROS_INFO("Data transmission failed");
    //return 1;
  }

  while(true)
  {
      client2.call(srv3);
      qDebug() << "Last_position";
      qDebug() << srv3.response.artur_scan_pose.position.x;
      qDebug() << srv3.response.artur_scan_pose.position.y;
      qDebug() << srv3.response.artur_scan_pose.position.z;
      qDebug() << srv3.response.artur_scan_pose.orientation.x;
      qDebug() << srv3.response.artur_scan_pose.orientation.y;
      qDebug() << srv3.response.artur_scan_pose.orientation.z;
  }


}
