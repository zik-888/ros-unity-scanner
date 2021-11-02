#include <ros/ros.h>
#include <kukavarproxy/kukavarproxy.h>
#include <eki_interface/eki_interface.h>
#include <ros_robotics_interface/TrajectoryService.h>
#include <ros_robotics_interface/ScanPositionService.h>
#include <thread>

EKI_interface eki;
//KukaVarProxy_interface inter("172.31.1.147",7000);
KukaVarProxy_interface inter("192.168.1.186",7000);
std::vector<geometry_msgs::Pose> Trajectory_vector;

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

bool kuka_mes_processing(ros_robotics_interface::TrajectoryService::Request  &req, ros_robotics_interface::TrajectoryService::Response &res)
{
    ROS_INFO("GOT HMI REQUEST");

     if (eki.kuka_ready_to_go())
     {
         int number_of_points = req.rpose.size();
         qDebug() <<"Total points"<< number_of_points;
         Trajectory_vector.clear();
         Trajectory_vector.resize(number_of_points);
         Trajectory_vector.swap(req.rpose);
         eki.got_trajectiry();
     }
     else
     {
         qDebug() << "Already moving or disconnected";
     }

    return true;
}

bool Scan_position_service_callbackfunction(ros_robotics_interface::ScanPositionService::Request  &req, ros_robotics_interface::ScanPositionService::Response &res)
{
    ROS_INFO("GOT SCAN POSITION REQUEST");

    geometry_msgs::Pose current_pose = inter.ROS_Get_Angles();

    res.artur_scan_pose.position.x = current_pose.position.x;
    res.artur_scan_pose.position.y = current_pose.position.y;
    res.artur_scan_pose.position.z = current_pose.position.z;
    res.artur_scan_pose.orientation.x = current_pose.orientation.x;
    res.artur_scan_pose.orientation.y = current_pose.orientation.y;
    res.artur_scan_pose.orientation.z = current_pose.orientation.z;

    return true;
}

void thread_robot_function()
{
    qDebug()<< "Thread started";
    while (eki.kuka_ready())
    {
        if(eki.kuka_got_new_trajectory())
        {
            eki.moving();
            for (int i = 0;i < Trajectory_vector.size();i++)
            {
                eki.PTP(Trajectory_vector[i]);
            }
            eki.GO_HOME();
            eki.reached_end_point_of_trajectory();
        }
    }

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "KUKA_NODE");
  ros::NodeHandle nh;

  //KukaVarProxy_interface inter("172.31.1.147",7000);
  ////KukaVarProxy_interface inter("192.168.50.223",7000);
  //KukaVarProxy_interface inter("192.168.50.223",7000);
  //inter.Connect();

  ROS_INFO("KUKA_NODE Hello world!");

  inter.Connect();

  ros::ServiceServer HMI_Server = nh.advertiseService("KUKA_HMI_srv", kuka_mes_processing);
  ros::ServiceServer Scan_Position_Server = nh.advertiseService("KUKA_Pos_srv", Scan_position_service_callbackfunction);

  ROS_INFO("HMI server started");

  eki.wait_for_kuka();
  //inter.ROS_Get_Angles();

  geometry_msgs::Pose new_pose = set_point(244.05, 9.32, 682.44,163.76,10.71,178.07);
  geometry_msgs::Pose new_pose2 = set_point(64.86, -185.24, 598.40,101.07,-61.28,-107.73);
  geometry_msgs::Pose new_pose3 = set_point(293.43, -37.96, 470.51, -107.15,-51.32,-128.25);



  //inter.ROS_Get_TCP_Postion();
  std::thread RThread(thread_robot_function);
  RThread.detach();

  /*
  sleep(5);
  ROS_INFO("LET'S MOVE");
  eki.PTP(new_pose);
  //sleep(1);
  eki.LIN(new_pose2);
  eki.PTP(new_pose3);
  eki.LIN(new_pose2);
  eki.PTP(new_pose3);
  eki.GO_HOME();
  eki.end_kuka_program();
  //float targ = 66.666;
*/
  //memcpy(eki.EkiSendBuff,&targ,sizeof (targ));

  //char fuuuuuu[100];
  //memset(&fuuuuuu,99,sizeof (fuuuuuu));
  //memcpy(eki.EkiSendBuff,fuuuuuu,sizeof(fuuuuuu));
  //eki.send_byte_data();


  //ROS_INFO("SENT");

  //eki.disconnect_kuka();

  ros::spin();

}
