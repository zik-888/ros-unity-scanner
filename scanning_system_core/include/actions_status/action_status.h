#ifndef ACTION_STATUS_H
#define ACTION_STATUS_H
#include <scanning_system_core/StatusAction.h>

#include <GoSdk/GoSdk.h>
#include <stdio.h>
#include <include/deformcontrollib.cpp>

#include <CScanner/CScanner.h>
#include <CPolyModel/CPolyModel.h>

#endif // ACTION_STATUS_H

class StatusAction
{
protected:
    //ros::NodeHandle nh;
    actionlib::SimpleActionServer<scanning_system_core::StatusAction> as_;
    // NodeHandle instance must be created before this line. Otherwise strange error occurs.
    std::string action_name_;
    // create messages that are used to published feedback/result
    scanning_system_core::StatusFeedback feedback_;
    scanning_system_core::StatusResult result_;

    ros::NodeHandle nh;

public:
  StatusAction(std::string name, ros::NodeHandle &nh) :
      as_(nh, name, boost::bind(&StatusAction::executeCB, this, _1), false),
      action_name_(name)
  {
      as_.start();
      feedback_.status = true;
      as_.publishFeedback(feedback_);

      ROS_INFO("Status action ready!");
    }

  ~StatusAction(void)
  {

  }

  void executeCB(const scanning_system_core::StatusGoalConstPtr &goal) //, ros::NodeHandle &nh)
  {
      bool success = true;
      ros::Rate r(1);

      if(success)
      {
          feedback_.status = true;
          as_.publishFeedback(feedback_);
          result_.status = true;
          as_.setSucceeded(result_);
      }

      for(int i=1; i<=10; i++)
      {
        // check that preempt has not been requested by the client
        if (as_.isPreemptRequested() || !ros::ok())
        {
            ROS_INFO("%s: Preempted", action_name_.c_str());
            // set the action state to preempted
            as_.setPreempted();
            success = false;
            break;
        }
        //feedback_.status.push_back(feedback_.status[i] + feedback_.status[i-1]);
        // publish the feedback
        as_.publishFeedback(feedback_);

        // this sleep is not necessary, the sequence is computed at 1 Hz for demonstration purposes
        r.sleep();
      }
  }
};


