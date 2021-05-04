#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <ros/ros.h>
#include <QCoreApplication>
#include <QtDebug>
#include <vector>

#define DO_NOT_USE_SHARED_MEMORY 1
#define NON_MATLAB_PARSING 1
#define MAX_EXT_API_CONNECTIONS 255
#define PI 3.14159265359
// #include <extApi.h>
#include <geometry_msgs/Pose.h>

enum motion_types {SCANNING, WORKING, SPECIAL_SCANNING};

geometry_msgs::Pose return_pose(geometry_msgs::Pose pose);

float ConvertToDegrees(float radian);

float ConvertToRadians(float degree);

geometry_msgs::Pose set_point(float x, float y, float z, float a, float b, float c);

void print_pose(geometry_msgs::Pose pose_to_print);

struct manipulator_handles
{
    const char* Man_Joint[6];
    const char* Man_Target;
    const char* Man_Tip;
    const char* signame;
    const char* possig;
};

class base_manipulator
{
public:

    //manipulator(){};
    virtual void manipulator_start(){}
    virtual geometry_msgs::Pose get_tip_position() = 0;
    virtual void set_target_position (geometry_msgs::Pose pose) = 0;
    virtual void get_joint_angles() = 0;
    virtual bool robot_ready(){return false;}
    virtual void PTP(geometry_msgs::Pose position) = 0;

};

/*class Vrep_man
{
public:
  Vrep_man();
  void man_init(simxInt clientID, manipulator_handles Man_Handles);
  geometry_msgs::Pose get_tip_position();
  geometry_msgs::Pose get_tip_position_from_signal(manipulator_handles Man_Handles);
  void set_target_position(geometry_msgs::Pose pose);
  void joint_init();
  void get_joint_angles();
  void print_angles();
  void print_position();
  bool target_reached(geometry_msgs::Pose target_pose);
  bool robot_ready();
  bool robot_ready_to_go();
  void PTP (geometry_msgs::Pose position);
  void velocity_calc(geometry_msgs::Pose target_position);
  void velocity_remove();
  bool got_trajectiry();
  bool reached_end_point_of_trajectory();
  void set_velocity(int value);

private:
  simxUChar* signal = (simxUChar*) std::malloc(sizeof(simxUChar)*3);
  simxInt sLength;
  simxInt joint_handles[6] = {0,0,0,0,0,0};
  simxFloat joint_angles[6] = {0,0,0,0,0,0};
  geometry_msgs::Pose last_position;
  geometry_msgs::Pose last_target;
  simxFloat delta = 0.001;
  simxInt target_handle = 0;
  simxInt tip_handle = 0;
  simxInt ClientID;
  simxFloat velocity = 0.1;
  const char* signame;
  bool man_ready = false;
  bool ready_to_go = false;

};*/



#endif // MANIPULATOR_H



