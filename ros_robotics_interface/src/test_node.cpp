#include <ros/ros.h>
#include <QCoreApplication>
#include <QtDebug>
#include <QThread>
#include <vector>

#include <ros_robotics_interface/TrajectoryService.h>

#define DO_NOT_USE_SHARED_MEMORY 1
#define NON_MATLAB_PARSING 1
#define MAX_EXT_API_CONNECTIONS 255
#define PI 3.14159265359

#include <extApi.h>
#include <extApiInternal.h>
#include <simConst.h>
#include <simLib.h>
#include <extApiPlatform.h>

geometry_msgs::Pose return_pose(geometry_msgs::Pose pose)
{
    return pose;
}

simxFloat ConvertToDegrees(simxFloat radian)
{
    return(radian * (180/PI));
}
simxFloat ConvertToRadians(simxFloat degree)
{
    return(degree * (PI/180));
}

geometry_msgs::Pose set_point(simxFloat x,simxFloat y,simxFloat z,simxFloat a,simxFloat b,simxFloat c)
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
//geometry_msgs::Pose pos;
class Vrep_man
{
public:

  simxInt joint_handles[6] = {0,0,0,0,0,0};
  Vrep_man()
  {

  }
  void man_init(simxInt clientid)
  {
      simxInt ret = 0;
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint1",&joint_handles[0],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint2",&joint_handles[1],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint3",&joint_handles[2],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint4",&joint_handles[3],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint5",&joint_handles[4],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint6",&joint_handles[5],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"TT1",&target_handle,simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_target",&tip_handle,simx_opmode_blocking);
      clientID = clientid;
      man_ready = true;

  }
  geometry_msgs::Pose get_tip_position()
  {
      simxFloat position[3] = {0, 0, 0};
      simxFloat orientation[3] = {0, 0, 0};

      simxGetObjectPosition(clientID,tip_handle,-1,position,simx_opmode_blocking);
      simxGetObjectOrientation(clientID,tip_handle,-1,orientation,simx_opmode_blocking);

      geometry_msgs::Pose retpos;

      retpos.position.x = position[0];
      retpos.position.y = position[1];
      retpos.position.z = position[2];
      retpos.orientation.x = orientation[0];
      retpos.orientation.y = orientation[1];
      retpos.orientation.z = orientation[2];

      last_position = retpos;

      return retpos;
  }
  void set_target_position(geometry_msgs::Pose pose)
  {
      simxFloat position[3] = {(simxFloat)pose.position.x, (simxFloat)pose.position.y, (simxFloat)pose.position.z};
      simxFloat orientation[3] = {(simxFloat)pose.orientation.x, (simxFloat)pose.orientation.y, (simxFloat)pose.orientation.z};
      last_target = pose;
      simxSetObjectPosition(clientID,target_handle,-1,position,simx_opmode_blocking);
      simxSetObjectOrientation(clientID,target_handle,-1,orientation,simx_opmode_blocking);
  }
  void joint_init()
  {
      for (int i = 0;i < 6; i++)
      {
           simxGetJointPosition(clientID,joint_handles[i],&joint_angles[i],simx_opmode_streaming);
           extApi_sleepMs(100);
      }
  }
  void get_joint_angles()
  {
      for (int i = 0;i < 6; i++)
      {
           simxGetJointPosition(clientID,joint_handles[i],&joint_angles[i],simx_opmode_buffer);
      }
  }
  void print_angles()
  {
      qDebug() << "Robot angles";
      for (int i = 0;i < 6; i++)
      {
          qDebug() << ConvertToDegrees(joint_angles[i]);
      }
  }
  void print_position()
  {
      qDebug() << "Last_position";
      qDebug() << last_position.position.x;
      qDebug() << last_position.position.y;
      qDebug() << last_position.position.z;
      qDebug() << last_position.orientation.x;
      qDebug() << last_position.orientation.y;
      qDebug() << last_position.orientation.z;
  }
  bool target_reached(geometry_msgs::Pose target_pose)
  {

      geometry_msgs::Pose current_pose = get_tip_position();
      simxFloat delta_x = abs(abs(target_pose.position.x) - abs(current_pose.position.x));
      simxFloat delta_y = abs(abs(target_pose.position.y) - abs(current_pose.position.y));
      simxFloat delta_z = abs(abs(target_pose.position.z) - abs(current_pose.position.z));

      if ((delta_x <= delta) && (delta_y <= delta) && (delta_z <= delta))
      {
          return true;
      }
      else
      {
          return false;
      }

  }
  bool robot_ready()
  {
      return man_ready;
  }
  void PTP (geometry_msgs::Pose position)
  {

      //get_tip_position();
      set_target_position(position);
      velocity_calc(position);

      while (target_reached(position) == false )
      {
          get_joint_angles();
          //print_angles();
      }

      simxFloat sig  = 0;
      simxGetFloatSignal(clientID,&signame[0],&sig,simx_opmode_oneshot_wait);
      qDebug() << "recvvel" << sig;
      velocity_remove();

  }
  void velocity_calc(geometry_msgs::Pose target_position)

  {
      geometry_msgs::Pose poss;
      poss = get_tip_position();
      simxFloat distance = sqrt(std::pow((target_position.position.x - poss.position.x),2) + std::pow((target_position.position.y - poss.position.y),2) + std::pow((target_position.position.z - poss.position.z),2));
      qDebug() << "distance" << distance;
      simxInt res = simxSetFloatSignal(clientID,&signame[0],velocity/distance,simx_opmode_oneshot_wait);
      //simxInt res = simxSetFloatSignal(clientID,"TestSig",0.3,simx_opmode_oneshot_wait);
      qDebug() << "calcvel" << velocity/distance;
      qDebug() << "callres" << res;
  }

  void velocity_remove()
  {
      simxInt res = simxClearFloatSignal(clientID,&signame[0],simx_opmode_oneshot_wait);
  }

private:

  simxFloat joint_angles[6] = {0,0,0,0,0,0};
  geometry_msgs::Pose last_position;
  geometry_msgs::Pose last_target;
  simxFloat delta = 0.00001;
  simxInt target_handle = 0;
  simxInt tip_handle = 0;
  simxInt clientID;
  simxFloat velocity = 0.5;
  const char signame[8] = "TestSig" ;
  bool man_ready = false;

};


class Vrep_man2
{
public:

  simxInt joint_handles[6] = {0,0,0,0,0,0};

  Vrep_man2()
  {

  }

  void man_init(simxInt clientid)
  {
      qDebug() << "LOOOL";
      simxInt ret = 0;
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint1#0",&joint_handles[0],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint2#0",&joint_handles[1],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint3#0",&joint_handles[2],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint4#0",&joint_handles[3],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint5#0",&joint_handles[4],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_joint6#0",&joint_handles[5],simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"TT1#0",&target_handle,simx_opmode_blocking);
      ret = simxGetObjectHandle(clientID,(simxChar*)"IRB140_target#0",&tip_handle,simx_opmode_blocking);
      clientID = clientid;
      man_ready = true;

  }

  geometry_msgs::Pose get_tip_position()
  {
      simxFloat position[3] = {0, 0, 0};
      simxFloat orientation[3] = {0, 0, 0};

      simxGetObjectPosition(clientID,tip_handle,-1,position,simx_opmode_blocking);
      simxGetObjectOrientation(clientID,tip_handle,-1,orientation,simx_opmode_blocking);

      geometry_msgs::Pose retpos;

      retpos.position.x = position[0];
      retpos.position.y = position[1];
      retpos.position.z = position[2];
      retpos.orientation.x = orientation[0];
      retpos.orientation.y = orientation[1];
      retpos.orientation.z = orientation[2];

      last_position = retpos;

      return retpos;
  }

  void set_target_position(geometry_msgs::Pose pose)
  {
      simxFloat position[3] = {(simxFloat)pose.position.x, (simxFloat)pose.position.y, (simxFloat)pose.position.z};
      simxFloat orientation[3] = {(simxFloat)pose.orientation.x, (simxFloat)pose.orientation.y, (simxFloat)pose.orientation.z};
      last_target = pose;
      simxSetObjectPosition(clientID,target_handle,-1,position,simx_opmode_blocking);
      simxSetObjectOrientation(clientID,target_handle,-1,orientation,simx_opmode_blocking);
  }
  void joint_init()
  {
      for (int i = 0;i < 6; i++)
      {
           simxGetJointPosition(clientID,joint_handles[i],&joint_angles[i],simx_opmode_streaming);
           extApi_sleepMs(100);
      }
  }
  void get_joint_angles()
  {
      for (int i = 0;i < 6; i++)
      {
           simxGetJointPosition(clientID,joint_handles[i],&joint_angles[i],simx_opmode_buffer);
      }
  }
  void print_angles()
  {
      qDebug() << "Robot angles";
      for (int i = 0;i < 6; i++)
      {
          qDebug() << ConvertToDegrees(joint_angles[i]);
      }
  }
  void print_position()
  {
      qDebug() << "Last_position";
      qDebug() << last_position.position.x;
      qDebug() << last_position.position.y;
      qDebug() << last_position.position.z;
      qDebug() << last_position.orientation.x;
      qDebug() << last_position.orientation.y;
      qDebug() << last_position.orientation.z;
  }

  bool target_reached(geometry_msgs::Pose target_pose)
  {

      geometry_msgs::Pose current_pose = get_tip_position();
      simxFloat delta_x = abs(abs(target_pose.position.x) - abs(current_pose.position.x));
      simxFloat delta_y = abs(abs(target_pose.position.y) - abs(current_pose.position.y));
      simxFloat delta_z = abs(abs(target_pose.position.z) - abs(current_pose.position.z));

      if ((delta_x <= delta) && (delta_y <= delta) && (delta_z <= delta))
      {
          return true;
      }
      else
      {
          return false;
      }

  }

  bool robot_ready()
  {
      return man_ready;
  }

  void PTP (geometry_msgs::Pose position)
  {

      //get_tip_position();
      set_target_position(position);
      velocity_calc(position);

      while (target_reached(position) == false )
      {
          get_joint_angles();
          //print_angles();
      }

      simxFloat sig  = 0;
      simxGetFloatSignal(clientID,(const char*) &signame2[0],&sig,simx_opmode_oneshot_wait);
      qDebug() << "recvvel" << sig;
      velocity_remove();

  }

  void velocity_calc(geometry_msgs::Pose target_position)

  {
      geometry_msgs::Pose poss;
      poss = get_tip_position();
      simxFloat distance = sqrt(std::pow((target_position.position.x - poss.position.x),2) + std::pow((target_position.position.y - poss.position.y),2) + std::pow((target_position.position.z - poss.position.z),2));
      qDebug() << "distance" << distance;
      simxInt res = simxSetFloatSignal(clientID,(const char*) &signame2[0],velocity/distance,simx_opmode_oneshot_wait);
      //simxInt res = simxSetFloatSignal(clientID,"TestSig",0.3,simx_opmode_oneshot_wait);
      qDebug() << "calcvel" << velocity/distance;
      qDebug() << "callres" << res;
  }

  void velocity_remove()
  {
      simxInt res = simxClearFloatSignal(clientID,(const char*) &signame2[0],simx_opmode_oneshot_wait);
  }

private:

  simxFloat joint_angles[6] = {0,0,0,0,0,0};
  geometry_msgs::Pose last_position;
  geometry_msgs::Pose last_target;
  simxFloat delta = 0.00001;
  simxInt target_handle = 0;
  simxInt tip_handle = 0;
  simxInt clientID;
  simxFloat velocity = 0.5;
  const char signame[9] = "TestSig2" ;

  char signame2[9] = "TestSig2";
  bool man_ready = false;

};



Vrep_man IRB_140;
Vrep_man2 IRB_140_2;


bool HMI_mes_processing(ros_robotics_interface::TrajectoryService::Request  &req, ros_robotics_interface::TrajectoryService::Response &res)
{
    ROS_INFO("GOT HMI REQUEST");
    int number_of_points = req.rpose.size();
    qDebug() <<"Total points"<< number_of_points;

    if (IRB_140_2.robot_ready())
    {
        for (int i = 0;i < number_of_points;i++)
        {

            IRB_140_2.PTP(req.rpose[i]);
            //simxClearFloatSignal()

        }

    }

    return true;
}


bool Scan_mes_processing(ros_robotics_interface::TrajectoryService::Request  &req, ros_robotics_interface::TrajectoryService::Response &res)
{
    ROS_INFO("GOT SCAN REQUEST");
    //int number_of_points = sizeof (req.rpose)/sizeof (geometry_msgs::Pose);
    int number_of_points = req.rpose.size();
    qDebug() <<"Total points"<< number_of_points;

    if (IRB_140.robot_ready())
    {
        for (int i = 0;i < number_of_points;i++)
        {

            IRB_140.PTP(req.rpose[i]);
            //simxClearFloatSignal()

        }

    }

    return true;
}



int main(int argc, char **argv)
{
  QCoreApplication a(argc, argv);
  ros::init(argc, argv, "test_node");
  ros::NodeHandle nh;

  ROS_INFO("Hello world! ROS");

  ros::ServiceServer HMI_Server = nh.advertiseService("HMI_srv", HMI_mes_processing);
  ros::ServiceServer Scan_Server = nh.advertiseService("Scan_srv", Scan_mes_processing);

  ROS_INFO("HMI and Scan servers started");

  simxInt clientID = simxStart("127.0.0.1",19999,true,true,2000,5);
  //ID = clientID;
  simxFloat sig = 0;



  if (clientID!=-1)
  {
      ROS_INFO ("Connected to remote API server") ;
      IRB_140.man_init(clientID);
      IRB_140_2.man_init(clientID);
      IRB_140.joint_init();
      IRB_140.get_joint_angles();
      IRB_140.print_angles();
      geometry_msgs::Pose first_point = set_point(0, -0.25, 1.42,0,0,0);
      geometry_msgs::Pose second_point = set_point(0, 0.325, 1.42,0,0,0);
      //IRB_140.PTP(first_point);
      //IRB_140.PTP(second_point);
      //qDebug() << sizeof (geometry_msgs::Pose);

      //simxGetFloatSignal(clientID,&signame[0],&sig,simx_opmode_oneshot);

      //qDebug() << sig ;

      ros::spin();

  return 0;
}


}
