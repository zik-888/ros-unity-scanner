#include <ros/ros.h>
#include <QCoreApplication>
#include <QtDebug>
#include <QThread>
#include <vector>
#include <thread>


#include <my_pkg/TrajectoryService.h>

#include <manipulator.h>

std::vector<geometry_msgs::Pose> Trajectory_vector;

Vrep_man IRB_140_2;

bool HMI_mes_processing(my_pkg::TrajectoryService::Request  &req, my_pkg::TrajectoryService::Response &res)
{
    ROS_INFO("GOT HMI REQUEST");

    if (IRB_140_2.robot_ready_to_go())
    {
        qDebug() << "Already moving";
    }
    else
    {
        int number_of_points = req.rpose.size();
        qDebug() <<"Total points"<< number_of_points;
        Trajectory_vector.clear();
        Trajectory_vector.resize(number_of_points);
        Trajectory_vector.swap(req.rpose);
        qDebug() <<"Total points"<< number_of_points;
        IRB_140_2.got_trajectiry();
    }


    /*
    if (IRB_140_2.robot_ready())
    {
        for (int i = 0;i < number_of_points;i++)
        {
            IRB_140_2.PTP(req.rpose[i]);
            //simxClearFloatSignal()
        }
    }*/

    return true;
}

void thread_robot_function()
{
    qDebug()<< "Thread started";
    while (IRB_140_2.robot_ready())
    {
        if(IRB_140_2.robot_ready_to_go())
        {
            for (int i = 0;i < Trajectory_vector.size();i++)
            {
                IRB_140_2.PTP(Trajectory_vector[i]);
            }
            IRB_140_2.reached_end_point_of_trajectory();
        }
    }

}


int main(int argc, char **argv)
{
  QCoreApplication a(argc, argv);
  ros::init(argc, argv, "VREP_WORK_MAN");
  ros::NodeHandle nh;

  manipulator_handles ManTwo_handles;
  ManTwo_handles.Man_Joint[0] = "IRB140_joint1#0";
  ManTwo_handles.Man_Joint[1] = "IRB140_joint2#0";
  ManTwo_handles.Man_Joint[2] = "IRB140_joint3#0";
  ManTwo_handles.Man_Joint[3] = "IRB140_joint4#0";
  ManTwo_handles.Man_Joint[4] = "IRB140_joint5#0";
  ManTwo_handles.Man_Joint[5] = "IRB140_joint6#0";
  ManTwo_handles.Man_Target =  "TT1#0";
  ManTwo_handles.Man_Tip = "IRB140_target#0";
  ManTwo_handles.signame = "TestSig2";
  ManTwo_handles.possig = "WorkManTipPosition";

  ROS_INFO("VREP_WORK_MAN Hello world!");

  ros::ServiceServer HMI_Server = nh.advertiseService("HMI_srv", HMI_mes_processing);

  ROS_INFO("HMI server started");

  simxInt clientID = simxStart("127.0.0.1",19998,true,true,2000,5);
  //ID = clientID;
  simxFloat sig = 0;

  if (clientID!=-1)
  {
      ROS_INFO ("Connected to remote API server") ;
      IRB_140_2.man_init(clientID,ManTwo_handles);
      IRB_140_2.joint_init();
      IRB_140_2.get_joint_angles();
      IRB_140_2.print_angles();
      std::thread RThread(thread_robot_function);
      RThread.detach();
      //IRB_140_2.get_tip_position_from_signal(ManTwo_handles);
;

    //  while(true)
     // {
     //     IRB_140_2.get_tip_position_from_signal(ManTwo_handles);
     //     //IRB_140_2.print_position();
     //     sleep(2);
     // }
      /*
      simxUChar* signal = (simxUChar*) std::malloc(sizeof(simxUChar)*3);
      simxInt sLength;

      if (simxGetStringSignal(clientID,"WorkManTipPosition",&signal,&sLength,simx_opmode_buffer)==simx_return_ok)
      {
          ROS_INFO ("GET SIGNAL") ;
      }
      else
      {
          qDebug() << sLength;
          ROS_INFO ("LOST SIGNAL") ;
          simxGetStringSignal(clientID,"WorkManTipPosition",&signal,&sLength,simx_opmode_streaming);
          simxGetStringSignal(clientID,"WorkManTipPosition",&signal,&sLength,simx_opmode_streaming);
          simxGetStringSignal(clientID,"WorkManTipPosition",&signal,&sLength,simx_opmode_streaming);
      }
      //int kf = 0;
      while (true)
      {
          sleep(3);
          if (simxGetStringSignal(clientID,"WorkManTipPosition",&signal,&sLength,simx_opmode_streaming)==simx_return_ok)
          {
              qDebug() << "GET SIGNAL";
              qDebug() << "Size = "<< sLength/sizeof (simxFloat);

              simxFloat x  = 0;
              simxFloat y  = 0;
              simxFloat z  = 0;
              std::memcpy(&x, signal, sizeof(simxFloat));
              std::memcpy(&y, signal+sizeof (simxFloat), sizeof(simxFloat));
              std::memcpy(&z, signal+2*sizeof (simxFloat), sizeof(simxFloat));
              qDebug() << x;
              qDebug() << y;
              qDebug() << z;
             // kf++;
          }
          else
          {
              //qDebug() << sLength;
              ROS_INFO ("LOST SIGNAL") ;
             // kf++;
          }

      }

*/
     ros::spin();

  return 0;

}

}
