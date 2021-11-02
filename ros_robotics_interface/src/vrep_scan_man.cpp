#include <ros/ros.h>
#include <QCoreApplication>
#include <QtDebug>
#include <QThread>
#include <vector>
#include <thread>


#include <ros_robotics_interface/TrajectoryService.h>

#include <ros_robotics_interface/ScanPositionService.h>
//#include<ros_robotics_interface/ScanPo

#include <manipulator.h>

Vrep_man IRB_140;

std::vector<geometry_msgs::Pose> Trajectory_vector;

motion_types motion_mode;

geometry_msgs::Pose XHOME = set_point(-0.2310, 0.0,1.6516,0,0,0);





bool Scan_mes_processing(ros_robotics_interface::TrajectoryService::Request  &req, ros_robotics_interface::TrajectoryService::Response &res)
{
    ROS_INFO("GOT SCAN REQUEST");

      if (IRB_140.robot_ready_to_go())
      {
          qDebug() << "Already moving";
      }
      else
      {
          int number_of_points = req.rpose.size();
          qDebug() <<"Total points"<< number_of_points;
          motion_mode = (motion_types)req.motion_type;
          Trajectory_vector.clear();
          Trajectory_vector.resize(number_of_points);
          Trajectory_vector.swap(req.rpose);
          IRB_140.set_velocity(50);
          if(motion_mode == SCANNING)
          {
              IRB_140.PTP(Trajectory_vector[0]);
              Trajectory_vector.erase(Trajectory_vector.begin());
              res.answer = true;

              sleep(2);
          }
          IRB_140.set_velocity(10);


          IRB_140.got_trajectiry();


      }


    return true;
}

bool Scan_position_service_callbackfunction(ros_robotics_interface::ScanPositionService::Request  &req, ros_robotics_interface::ScanPositionService::Response &res)
{
    ROS_INFO("GOT SCAN POSITION REQUEST");

    geometry_msgs::Pose current_pose = IRB_140.get_tip_position();

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
    while (IRB_140.robot_ready())
    {
        if(IRB_140.robot_ready_to_go())
        {
            for (int i = 0;i < Trajectory_vector.size();i++)
            {
                IRB_140.PTP(Trajectory_vector[i]);
            }
            IRB_140.reached_end_point_of_trajectory();
            //IRB_140.set_velocity(20);
            IRB_140.PTP(XHOME);
        }
    }

}


/*
void thread_robot_function()
{
    qDebug()<< "Thread started";
    while (IRB_140.robot_ready())
    {
        if(IRB_140.robot_ready_to_go())
        {
            switch (motion_mode)
            {
            case SPECIAL_SCANNING:
                {
                   for (int i = 0;i < Trajectory_vector.size();i++)
                    {
                        IRB_140.PTP(Trajectory_vector[i]);
                    }
                    IRB_140.reached_end_point_of_trajectory();
                }
            case WORKING:
                {
                    for (int i = 0;i < Trajectory_vector.size();i++)
                    {
                        IRB_140.PTP(Trajectory_vector[i]);
                    }
                    IRB_140.reached_end_point_of_trajectory();
                }
            }


        }
    }

}
*/

int main(int argc, char **argv)
{
  QCoreApplication a(argc, argv);

  ros::init(argc, argv, "VREP_SCAN_MAN");
  ros::NodeHandle nh;
  // geometry_msgs::Pose pos;
  ROS_INFO("VREP_SCAN_MAN Hello world!");



  manipulator_handles ManOne_handles;
  ManOne_handles.Man_Joint[0] = "IRB140_joint1";
  ManOne_handles.Man_Joint[1] = "IRB140_joint2";
  ManOne_handles.Man_Joint[2] = "IRB140_joint3";
  ManOne_handles.Man_Joint[3] = "IRB140_joint4";
  ManOne_handles.Man_Joint[4] = "IRB140_joint5";
  ManOne_handles.Man_Joint[5] = "IRB140_joint6";
  ManOne_handles.Man_Target =  "TT1";
  ManOne_handles.Man_Tip = "IRB140_target";
  ManOne_handles.signame = "TestSig";
  ManOne_handles.possig = "ScanManTipPosition";

  ros::ServiceServer Scan_Server = nh.advertiseService("Scan_srv", Scan_mes_processing);
  ros::ServiceServer Scan_Position_Server = nh.advertiseService("Scan_Pos_srv", Scan_position_service_callbackfunction);
  ROS_INFO("Scan server started");

  simxInt clientID = simxStart("127.0.0.1",19999,true,true,2000,5);
  //ID = clientID;
  simxFloat sig = 0;

  //geometry_msgs::Pose[] rposel;
  if (clientID!=-1)
  {
      ROS_INFO ("Connected to remote API server") ;
      IRB_140.man_init(clientID,ManOne_handles);
      IRB_140.joint_init();
      IRB_140.get_joint_angles();
      IRB_140.print_angles();
      std::thread RThread(thread_robot_function);
      RThread.detach();
      ros::spin();

  return 0;
  }

}
