#include <ros/ros.h>
#include <ros_robotics_interface/TrajectoryService.h>
#include <ros_robotics_interface/ScanPositionService.h>
#include <mitsubishi/mitsubishi.h>
#include <thread>

Mitsubishi_interface Mitsubishi("192.168.1.20",10004, 10001);

std::vector<geometry_msgs::Pose> Trajectory_vector;

geometry_msgs::Pose HomePos;


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

     if (Mitsubishi.mitsubishi_ready_to_go())
     {
         int number_of_points = req.rpose.size();
         qDebug() <<"Total points"<< number_of_points;
         Trajectory_vector.clear();
         Trajectory_vector.resize(number_of_points);
         Trajectory_vector.swap(req.rpose);
         //Mitsubishi.LIN_C(Trajectory_vector[0]);
         Mitsubishi.PTP_C(Trajectory_vector[0]);
         Mitsubishi.got_trajectiry();
         res.answer = true;
         return true;
     }
     else
     {
         qDebug() << "Already moving or disconnected";
         return true;
     }

    //return true;
}

bool Scan_position_service_callbackfunction(ros_robotics_interface::ScanPositionService::Request  &req, ros_robotics_interface::ScanPositionService::Response &res)
{
    ROS_INFO("GOT SCAN POSITION REQUEST");

    geometry_msgs::Pose current_pose = Mitsubishi.Get_TCP_Postion();

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
    while (Mitsubishi.mitsubishi_ready())
    {
        if(Mitsubishi.mitsubishi_got_new_trajectory())
        {
            sleep(1);
            Mitsubishi.moving();
            for (int i = 1;i < Trajectory_vector.size();i++)
            {
                Mitsubishi.LIN_C(Trajectory_vector[i]);
            }
            //Mitsubishi.GO_HOME();
            sleep(2);
            Mitsubishi.PTP_J(HomePos);
            Mitsubishi.reached_end_point_of_trajectory();
        }
    }

}

//Mitsubishi_interface Mitsubishi("192.168.1.20",48569, 10001);
int main(int argc, char **argv)
{

  HomePos.position.x = 90.00;
  HomePos.position.y = 0.00;
  HomePos.position.z = 90.00;
  HomePos.orientation.x = 0.00;
  HomePos.orientation.y = 90.00;
  HomePos.orientation.z = -3.70;


  ros::init(argc, argv, "mitsubishi_node");
  ros::NodeHandle nh;



  Mitsubishi.Connect();

  ros::ServiceServer HMI_Server = nh.advertiseService("HMI_srv", kuka_mes_processing);
  ros::ServiceServer Scan_Position_Server = nh.advertiseService("Scan_Pos_srv", Scan_position_service_callbackfunction);

  ROS_INFO("HMI server started");

  std::thread RThread(thread_robot_function);
  RThread.detach();

  //Mitsubishi.PTP_J(testpos);

  //qDebug() << Mitsubishi.ros_pose_to_mitsubishi_string(testpos);

  //Mitsubishi.test_sock();

  /*Mitsubishi.Connect();

  Mitsubishi.Get_Angles();

  Mitsubishi.Get_TCP_Postion();

  Mitsubishi.send_cmd();
  */

  ROS_INFO("Hello world!");

  ros::spin();



}
