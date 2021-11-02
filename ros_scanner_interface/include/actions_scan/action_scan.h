#ifndef ACTION_SCAN_H
#define ACTION_SCAN_H

#include <ros/ros.h>
#include <Eigen/Eigen>
#include <CPolyModel/CPolyModel.h>
#include <CScanner/CScanner.h>
#include <CTrajectory/CTrajectory.h>

#include <actionlib/server/simple_action_server.h>
#include <ros_scanner_interface/ScanningAction.h>

#include "ros_scanner_interface/PoseRobotMsg.h"
#include "ros_scanner_interface/PoseRobotService.h"
#include "ros_scanner_interface/TrajectoryService.h"

#include <GoSdk/GoSdk.h>
#include <stdio.h>

#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <cstdio>
#include <tf2/LinearMath/Quaternion.h>

#endif // ACTION_SCAN_H

class ScanningAction
{
protected:
    //ros::NodeHandle nh;
    actionlib::SimpleActionServer<ros_scanner_interface::ScanningAction> as_;
    // NodeHandle instance must be created before this line. Otherwise strange error occurs.
    std::string action_name_;
    // create messages that are used to published feedback/result
    ros_scanner_interface::ScanningFeedback feedback_;
    ros_scanner_interface::ScanningResult result_;
    ros_scanner_interface::PoseRobotService srv_robot_pos;

    ros::NodeHandle nh;

public:
  ScanningAction(std::string name, ros::NodeHandle &nh, SPolyModel *scan_model) :
      as_(nh, name, boost::bind(&ScanningAction::executeCB, this, _1, scan_model), false),   //scan_model
      action_name_(name)
  {
      as_.start();
      feedback_.status = INIT_SYSTEM;
      as_.publishFeedback(feedback_);
      ROS_INFO("Scanner is ready, wait command");
   }

  ~ScanningAction(){}

  void transformation(std::vector<double> *point);
  void transformation_inv(std::vector<double> *point);

  void executeCB(const ros_scanner_interface::ScanningGoalConstPtr &goal, SPolyModel *scan_model)
  {
      result_.vertices.clear();
      result_.triangles.clear();

      //  init scanner and connect to scanner
      //
      CScanner scanner;
      CTrajectory trajectory;

      // Initialization scanner;
      if(scanner.init(scanner.status, scanner.api, scanner.apiLib,
                      scanner.system, scanner.sensor, scanner.ipAddress) == true)
      {
          ROS_INFO("INITIALIZATION SUCCESS");
          std::cout << "Status: " << scanner.status << std::endl
                    << "api:    " << scanner.api << std::endl
                    << "system: " << scanner.system << std::endl
                    << "sensor: " << scanner.sensor << std::endl;

          scanner.status = GoSensor_Trigger(scanner.sensor);
      }
      else
      {
          ROS_ERROR("INITIALIZATION FAILED");
      }

      // helper variables
      ros::Rate r(1);
      bool success = true;

      // init services
      ros::ServiceClient client_scan_traj = nh.serviceClient<ros_scanner_interface::TrajectoryService>("HMI_srv");
      ros::ServiceClient client_Scan_Position = nh.serviceClient<ros_scanner_interface::PoseRobotService>("Scan_Pos_srv");

      // publish info to the console for the user
      ROS_INFO("%s: Executing, staring scanning of area (%f, %f), (%f, %f)", action_name_.c_str(), goal->x1, goal->y1, goal->x2, goal->y2);
      ros_scanner_interface::TrajectoryService srv_trajectory;
      srv_trajectory.response.answer = false;

      std::cout << "FROM HMI:" << std::endl
                << goal->x1 << " ; " << goal->y1 << std::endl
                << goal->x2 << " ; " << goal->y2 << std::endl;

      // tranformation
      std::vector<double> Point1_transform = {goal->x1, goal->y1, 0};
      std::vector<double> Point2_transform = {goal->x2, goal->y2, 0};

      transformation_inv(&Point1_transform);
      transformation_inv(&Point2_transform);

      //float x1_rot = -goal->x1 + 890;
      //float y1_rot = -goal->y1;

      //float x2_rot = -goal->x2 + 890;
      //float y2_rot = -goal->y2;


      std::cout << "FROM HMI TRANSFORM:" << std::endl
                << Point1_transform[0] << " ; " << Point1_transform[1] << std::endl
                << Point2_transform[0] << " ; " << Point2_transform[1] << std::endl;

      //srv_trajectory = trajectory.TrajectoryForming(x1_rot, y1_rot, x2_rot, y2_rot);
      srv_trajectory = trajectory.TrajectoryForming(Point1_transform[0], Point1_transform[1], Point2_transform[0], Point2_transform[1]);
      std::cout << "Size of srv_trajectory = " << srv_trajectory.request.rpose.size() << std::endl
                << "first point = " << srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-2] << std::endl
                << "last point = " << srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-1] << std::endl;

      PositionTIP stopPosition;
      stopPosition.x = srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-1].position.x;
      stopPosition.y = srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-1].position.y;
      stopPosition.z = srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-1].position.z;

      feedback_.status = GO_TO_TARGET_POINT;
      as_.publishFeedback(feedback_);
      if(client_scan_traj.call(srv_trajectory))
      {
          ROS_INFO("Success! Trajectory sended.");
          if(srv_trajectory.response.answer == true)
          {
              ROS_WARN("SCANNING");
              *scan_model = scanner.scanning(as_, action_name_, stopPosition, client_Scan_Position, scanner);
              ROS_INFO("Finish action");
          }
          else
          {
              ROS_ERROR("Mitsubish not response");
          }
      }
      else
      {
          ROS_ERROR("Failed send trajectory");
          feedback_.status = ERROR_SENDING_TRAJECTORY;  // Failed trajectory
          as_.publishFeedback(feedback_);

          success = false;
          as_.setAborted(result_, "Failed");
      }
      if(success)
      {
          feedback_.status = SCANNING_SUCCESS;
          as_.publishFeedback(feedback_);

          for(int j = 0; j < (*scan_model).Points.size(); j++)
          {
              result_.ScanningResult_::vertices.push_back({});

              transformation(&((*scan_model).Points[j]));

              result_.vertices[j].x = (*scan_model).Points[j][0];
              result_.vertices[j].y = (*scan_model).Points[j][1];
              result_.vertices[j].z = (*scan_model).Points[j][2];

              //ROS_INFO("x = %f, y = %f, z = %f", result_.vertices[j].x, result_.vertices[j].y, result_.vertices[j].z);
          }

          for(int k = 0; k < (*scan_model).Faces.size(); k++)
          {
              result_.ScanningResult_::triangles.push_back({});
              result_.triangles[k].vertex_indices[0] = (*scan_model).Faces[k][0];
              result_.triangles[k].vertex_indices[1] = (*scan_model).Faces[k][1];
              result_.triangles[k].vertex_indices[2] = (*scan_model).Faces[k][2];

              //ROS_INFO("triangle [%d; %d; %d]", result_.triangles[k].vertex_indices[0],result_.triangles[k].vertex_indices[1],result_.triangles[k].vertex_indices[2]);
          }

          ROS_INFO("Packing data (Points = %lu, Faces = %lu)", result_.vertices.size(), result_.triangles.size());

          ROS_INFO("%s: Scanning is succeeded", action_name_.c_str());
          // set the action state to succeeded
          as_.setSucceeded(result_);
      }
  }
};
