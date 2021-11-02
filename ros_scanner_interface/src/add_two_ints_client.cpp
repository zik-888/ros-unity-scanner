#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <ros_scanner_interface/ScanningAction.h>
#include "ros_scanner_interface/Status.h"

void statusCallback(const ros_scanner_interface::Status::ConstPtr& msg)
{
  //ROS_INFO("I heard: [%d]", msg->status);
}

class Localizer
{
public:
  Localizer(ros::NodeHandle& nh)
  {
      ar_sub_ = nh.subscribe<ros_scanner_interface::Status>("status_of_scan_system", 1,
      &Localizer::visionCallback, this);
  }

  void visionCallback(const ros_scanner_interface::StatusConstPtr& msg)
  {
      last_msg_ = msg;
      ROS_INFO_STREAM(last_msg_->status);
  }

  ros::Subscriber ar_sub_;
  ros_scanner_interface::StatusConstPtr last_msg_;
};

void doneCb(const actionlib::SimpleClientGoalState& state, const ros_scanner_interface::ScanningResultConstPtr& result)
{
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  //ROS_INFO("Answer: %i", result->vertices.back());
  //ros::shutdown();
}

// Called once when the goal becomes active
void activeCb()
{
  ROS_INFO("Goal just went active");
}

// Called every time feedback is received for the goal
void feedbackCb(const ros_scanner_interface::ScanningFeedbackConstPtr& feedback)
{
  ROS_INFO("Got Feedback of scanning status %i", feedback->status);
}

int main (int argc, char **argv)
{
  ros::init(argc, argv, "test_fibonacci");
  ros::NodeHandle nh1;
  ros::Subscriber sub = nh1.subscribe("status_of_scan_system", 1000, statusCallback);
  //Localizer localizer(nh1);

  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<ros_scanner_interface::ScanningAction> ac("scanning_node", true);


  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  ros_scanner_interface::ScanningGoal goal;
  //ros_scanner_interface::ProcessingGoal goal_pr;

  
  //ros_scanner_interface::ScanningResultConstPtr result_;


  float x1 = 329.983;
  float y1 = 177.777;

  float x2 = 644.337;
  float y2 = -80.043;


  goal.x1 = x1;
  goal.y1 = y1;
  goal.x2 = x2;
  goal.y2 = y2;
  ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb); //&doneCb, &activeCb,


  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(100.0));
  //ros_scanner_interface::ScanningResult marker;


  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();

    ros_scanner_interface::ScanningResultConstPtr result_ = ac.getResult();

    //result = ac.getResult();


    //result_ = ac.getResult();

    ROS_INFO("Action finished: %s",state.toString().c_str());
    std::cout << "Result  " << result_->vertices.size() << std::endl;
    std::cout << "Result  " << result_->triangles.size() << std::endl;
  }
  else
    ROS_INFO("Action did not finish before the time out.");
    ros_scanner_interface::ScanningResultConstPtr result_ = ac.getResult();
    result_ = ac.getResult();
    std::cout << "Result  " << result_->vertices.size() << std::endl;
    std::cout << "Result  " << result_->triangles.size() << std::endl;

  //exit
  ros::spin();
  return 0;
}
