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
  ros::init(argc, argv, "service_client_node");
  ros::NodeHandle nh;
  qDebug() << "Hello world QTDEBUG node 3";

  ros::ServiceClient client = nh.serviceClient<ros_robotics_interface::TrajectoryService>("Scan_srv");
  ros::ServiceClient client2 = nh.serviceClient<ros_robotics_interface::TrajectoryService>("HMI_srv");
  ros::ServiceClient client3 = nh.serviceClient<ros_robotics_interface::ScanPositionService>("Scan_Pos_srv");


  ros_robotics_interface::TrajectoryService srv;
  ros_robotics_interface::TrajectoryService srv2;
  ros_robotics_interface::ScanPositionService srv3;
  //srv.request.rpose.reserve(2);
  //srv.request.rpose[0] = set_point(0, -0.25, 1.42,0,0,0);
  //srv.request.rpose[1] = set_point(0, 0.325, 1.42,0,0,0);


  srv.request.rpose.push_back(set_point(0, -0.25, 1.42,0,0,3.14/2));
  srv.request.rpose.push_back(set_point(0, 0.375, 1.42,0,0,3.14/2));
  srv.request.motion_type = SCANNING;


  /*
  srv.request.rpose.push_back(set_point(0, -0.25, 1.42,0,0,0));
  srv.request.rpose.push_back(set_point(0, 0.325, 1.42,0,0,0));
  srv.request.rpose.push_back(set_point(-0.2, 0.375, 1.42,0,0,0));
  srv.request.rpose.push_back(set_point(-0, 0.175, 1.42,0,0,0));
  srv.request.rpose.push_back(set_point(-0.15, 0.05, 1.42,0,0,0));
  srv.request.rpose.push_back(set_point(-0.15, 0.025, 1.42,0,0,0));
  srv.request.motion_type = WORKING;
  */




  srv2.request.rpose.push_back(set_point(0.2, 0, 1.22,0,0,0));
  srv2.request.rpose.push_back(set_point(0.325, 0.3, 1.22,0,0,0));
  srv2.request.rpose.push_back(set_point(0.3, -0.275, 1.22,0,0,0));
  srv2.request.motion_type = WORKING;

 qDebug() << "Hello world QTDEBUG node 3";


 while (true)
 {

     //     qDebug() << client3.call(srv3);
     //     qDebug() << "Last_position";
     //     qDebug() << srv3.response.artur_scan_pose.position.x;
     //     qDebug() << srv3.response.artur_scan_pose.position.y;
     //     qDebug() << srv3.response.artur_scan_pose.position.z;
     //     qDebug() << srv3.response.artur_scan_pose.orientation.x;
     //     qDebug() << srv3.response.artur_scan_pose.orientation.y;
     //     qDebug() << srv3.response.artur_scan_pose.orientation.z;

     sleep(2);
     if (client.call(srv) && client2.call(srv2))
     {
       //client2.call(srv2);
       qDebug() << "Ok";
     }
     else
     {
       qDebug() << "Failed";
       return 1;
     }
 }

    client.call(srv2);
    //sleep(20);
    //client.call(srv);


}

