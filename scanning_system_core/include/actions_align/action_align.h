#ifndef ACTION_ALIGN_H
#define ACTION_ALIGN_H
#include "scanning_system_core/AlignAction.h"
//#include <scanning_system_core/AlignAction.h>
#include <GoSdk/GoSdk.h>
#include <stdio.h>
#include <Eigen/Eigen>

#include "deformcontrollib.cpp"

#include <CScanner/CScanner.h>
#include <CPolyModel/CPolyModel.h>


#endif // ACTION_ALIGN_H

class AlignAction
{
protected:
    //ros::NodeHandle nh;
    actionlib::SimpleActionServer<scanning_system_core::AlignAction> as_;
    // NodeHandle instance must be created before this line. Otherwise strange error occurs.
    std::string action_name_;
    // create messages that are used to published feedback/result
    scanning_system_core::AlignFeedback feedback_;
    scanning_system_core::AlignResult result_;

    ros::NodeHandle nh;

public:
    AlignAction(std::string name, ros::NodeHandle &nh, SPolyModel *scan_model) :
      as_(nh, name, boost::bind(&AlignAction::executeCB, this, _1, scan_model), false),
      action_name_(name)
  {
      as_.start();
      feedback_.status = INIT_SYSTEM;
      as_.publishFeedback(feedback_);

      ROS_INFO("Align server is ready, wait command");
    }

  ~AlignAction(void)
  {

  }

  void executeCB(const scanning_system_core::AlignGoalConstPtr &goal, SPolyModel *scan_model) //, ros::NodeHandle &nh)
  {
      ROS_WARN("ALIGN MODE");

      SPolyModel SCAN_MODEL = *scan_model;
      SPolyModel CAD_MODEL;
      SPolyModel CAD_MODEL_ALIGNED;

      Eigen::Matrix<float, 4, 4> tr_matrix;

      if(SCAN_MODEL.Points.size() == 0 || goal->vertices.size() == 0)
      {
          ROS_ERROR("Empty CAD-model or Scan-model");
          feedback_.status = ALIGN_ERROR;
          as_.publishFeedback(feedback_);
          as_.setAborted();

          return;
      }

      bool success = false;
      float ERROR;

      result_.matrix.clear();

      // tranformation goal to SPolyModel
      for(int i = 0; i < goal->vertices.size(); i++)
      {
          CAD_MODEL.Points.push_back({goal->vertices[i].x,
                                      goal->vertices[i].y,
                                      goal->vertices[i].z});
      }



      feedback_.status = ALIGN_MODE;
      as_.publishFeedback(feedback_);

      tr_matrix = ICP(&SCAN_MODEL, &CAD_MODEL, 0);

      // test data
      ERROR = 0.111;
      success = true;

      if(success)
      {
          feedback_.status = PROCESSING_SUCCESS;
          as_.publishFeedback(feedback_);

          //scanning_system_core::Mesh marker;
          for(int i = 0; i < 4; i++)
          {
              for(int j = 0; j < 4; j++)
              {
                  result_.AlignResult_::matrix.push_back({tr_matrix(i,j)});
              }
          }
          //result_.trajectory = ...;
          result_.error = ERROR;
          ROS_INFO("%s: Align is succeeded", action_name_.c_str());
          // set the action state to succeeded
          as_.setSucceeded(result_);
      }
  }
};


