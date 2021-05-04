#ifndef ACTION_PROC_H
#define ACTION_PROC_H

#include <scanning_system_core/ProcessingAction.h>

#include "scanning_system_core/PoseRobotMsg.h"
#include "scanning_system_core/PoseRobotService.h"

#include <GoSdk/GoSdk.h>
#include <stdio.h>
#include <include/deformcontrollib.cpp>

#include <CScanner/CScanner.h>
#include <CPolyModel/CPolyModel.h>

#endif // ACTION_PROC_H

class ProcessingAction
{
protected:

    //ros::NodeHandle nh;
    actionlib::SimpleActionServer<scanning_system_core::ProcessingAction> as_;
    // NodeHandle instance must be created before this line. Otherwise strange error occurs.
    std::string action_name_;
    // create messages that are used to published feedback/result
    scanning_system_core::ProcessingFeedback feedback_;
    scanning_system_core::ProcessingResult result_;

    ros::NodeHandle nh;

public:
  ProcessingAction(std::string name, ros::NodeHandle &nh, SPolyModel *scan_model) :
      as_(nh, name, boost::bind(&ProcessingAction::executeCB, this, _1, scan_model), false),
      action_name_(name)
  {
      as_.start();
      feedback_.status = INIT_SYSTEM;
      as_.publishFeedback(feedback_);

      ROS_INFO("Processing lib ready, wait command");
    }

  ~ProcessingAction(void)
  {

  }

  void executeCB(const scanning_system_core::ProcessingGoalConstPtr &goal, SPolyModel *scan_model) //, ros::NodeHandle &nh)
  {
      SPolyModel SCAN_MODEL = *scan_model;
      SPolyModel CAD_MODEL;
      SPolyModel CAD_MODEL_ALIGNED;

      std::cout << "Mode = " << goal->mode << std::endl;

      bool success = false;
      ros::Rate r(1);
      float ERROR;

      result_.vertices_CAD.clear();
      result_.triangles_CAD.clear();

      // tranformation goal to SPolyModel
      for(int i = 0; i < goal->vertices_CAD.size(); i++)
      {
          CAD_MODEL.Points.push_back({goal->vertices_CAD[i].x,
                                      goal->vertices_CAD[i].y,
                                      goal->vertices_CAD[i].z});
      }

      for(int i = 0; i < goal->triangles_CAD.size(); i++)
      {
          CAD_MODEL.Faces.push_back({goal->triangles_CAD[i].vertex_indices[0],
                                     goal->triangles_CAD[i].vertex_indices[1],
                                     goal->triangles_CAD[i].vertex_indices[2]});
      }

      std::ofstream out10;
      out10.open("/home/ros-industrial/Desktop/CAD_MODEL.obj");//, std::ios::app);
      out10 << "o Object.1" << std::endl;

      out10 << std::endl << std::endl;
      for (int i = 0; i < CAD_MODEL.Points.size(); i++)
      {
          out10 << "v "
               << CAD_MODEL.Points[i][0] << " "
               << CAD_MODEL.Points[i][1] << " "
               << CAD_MODEL.Points[i][2] << std::endl;
      }
      for (int c = 0; c < CAD_MODEL.Faces.size(); c++)
      {
          out10 << "f "
               << CAD_MODEL.Faces[c][0] + 1 << " "
               << CAD_MODEL.Faces[c][1] + 1 << " "
               << CAD_MODEL.Faces[c][2] + 1 << std::endl;
      }

      std::cout << "***********************" << std::endl
                << "CAD_MODEL: " << std::endl
                << "Size of Points:    " << CAD_MODEL.Points.size() << std::endl
                << "Size of Triangles: " << CAD_MODEL.Faces.size() << std::endl;

      std::cout << "**********************" << std::endl
                << "SCAN_MODEL: " << std::endl
                << "Size of Points:    " << SCAN_MODEL.Points.size() << std::endl
                << "Size of Triangles: " << SCAN_MODEL.Faces.size() << std::endl;

      switch(goal->mode)
      {
      case ALIGN:
          ROS_WARN("ALIGN MODE");

          feedback_.status = ALIGN_MODE;
          as_.publishFeedback(feedback_);

          CAD_MODEL_ALIGNED = ICP(&SCAN_MODEL, &CAD_MODEL);

          // test data
          ERROR = 0.111;
          success = true;

          if(success)
          {
            feedback_.status = PROCESSING_SUCCESS;
            as_.publishFeedback(feedback_);

            //scanning_system_core::Mesh marker;

            for(int j = 0; j < CAD_MODEL_ALIGNED.Points.size(); j++)
            {
                result_.ProcessingResult_::vertices_CAD.push_back({});
                result_.vertices_CAD[j].x = CAD_MODEL_ALIGNED.Points[j][0];
                result_.vertices_CAD[j].y = CAD_MODEL_ALIGNED.Points[j][1];
                result_.vertices_CAD[j].z = CAD_MODEL_ALIGNED.Points[j][2];

                //ROS_INFO("x = %f, y = %f, z = %f", result_.vertices[j].x, result_.vertices[j].y, result_.vertices[j].z);
            }

            for(int k = 0; k < CAD_MODEL_ALIGNED.Faces.size(); k++)
            {
                result_.ProcessingResult_::triangles_CAD.push_back({});
                result_.triangles_CAD[k].vertex_indices[0] = CAD_MODEL_ALIGNED.Faces[k][0] + 1;
                result_.triangles_CAD[k].vertex_indices[1] = CAD_MODEL_ALIGNED.Faces[k][1] + 1;
                result_.triangles_CAD[k].vertex_indices[2] = CAD_MODEL_ALIGNED.Faces[k][2] + 1;

                //ROS_INFO("triangle [%d; %d; %d]", result_.triangles[k].vertex_indices[0],result_.triangles[k].vertex_indices[1],result_.triangles[k].vertex_indices[2]);
            }
            //result_.trajectory = ...;
            result_.error = ERROR;
            ROS_INFO("%s: Align is succeeded", action_name_.c_str());
            ROS_INFO("Packing data (Points = %lu, Faces = %lu)", result_.vertices_CAD.size(), result_.triangles_CAD.size());
            // set the action state to succeeded
            as_.setSucceeded(result_);

            std::ofstream out9;
            out9.open("/home/ros-industrial/Desktop/aligned_cad_hui.obj");//, std::ios::app);
            out9 << "o Object.1" << std::endl;

            out9 << std::endl << std::endl;
            for (int i = 0; i < CAD_MODEL_ALIGNED.Points.size(); i++)
            {
                out9 << "v "
                     << CAD_MODEL_ALIGNED.Points[i][0] << " "
                     << CAD_MODEL_ALIGNED.Points[i][1] << " "
                     << CAD_MODEL_ALIGNED.Points[i][2] << std::endl;
            }
            for (int c = 0; c < CAD_MODEL_ALIGNED.Faces.size(); c++)
            {
                out9 << "f "
                     << CAD_MODEL_ALIGNED.Faces[c][0] + 1 << " "
                     << CAD_MODEL_ALIGNED.Faces[c][1] + 1 << " "
                     << CAD_MODEL_ALIGNED.Faces[c][2] + 1 << std::endl;
            }
          }

          break;
      case DEFORM:
          ROS_WARN("DEFORM MODE");

          feedback_.status = DEFROM_MODE;
          as_.publishFeedback(feedback_);

          ERROR = DeformationsControl(SCAN_MODEL, CAD_MODEL, 5000000, 18, 188, 137, 0);

          // test data
          ERROR = 0.222;
          CAD_MODEL.Faces = {{1, 2, 3},
                             {2, 3, 4},
                             {3, 4, 6},
                             {3, 6, 5}};
          CAD_MODEL.Points = {{0, 0, 0},
                              {0, 1, 0},
                              {1, 0, 0},
                              {1, 1, 0},
                              {2, 0, 0},
                              {2, 1, 0}};

          success = true;

          if(success)
          {
            feedback_.status = PROCESSING_SUCCESS;
            as_.publishFeedback(feedback_);

            //scanning_system_core::Mesh marker;

            for(int j = 0; j < CAD_MODEL.Points.size(); j++)
            {
                result_.ProcessingResult_::vertices_CAD.push_back({});
                result_.vertices_CAD[j].x = CAD_MODEL.Points[j][0];
                result_.vertices_CAD[j].y = CAD_MODEL.Points[j][1];
                result_.vertices_CAD[j].z = CAD_MODEL.Points[j][2];

                //ROS_INFO("x = %f, y = %f, z = %f", result_.vertices[j].x, result_.vertices[j].y, result_.vertices[j].z);
            }

            for(int k = 0; k < CAD_MODEL.Faces.size(); k++)
            {
                result_.ProcessingResult_::triangles_CAD.push_back({});
                result_.triangles_CAD[k].vertex_indices[0] = CAD_MODEL.Faces[k][0];
                result_.triangles_CAD[k].vertex_indices[1] = CAD_MODEL.Faces[k][1];
                result_.triangles_CAD[k].vertex_indices[2] = CAD_MODEL.Faces[k][2];

                //ROS_INFO("triangle [%d; %d; %d]", result_.triangles[k].vertex_indices[0],result_.triangles[k].vertex_indices[1],result_.triangles[k].vertex_indices[2]);
            }
            ROS_INFO("%s: Align is succeeded", action_name_.c_str());

            //result_.trajectory = ...;
            result_.error = ERROR;

            // set the action state to succeeded
            as_.setSucceeded(result_);
          }

          break;

      case ALIGN_AND_DEFORM:
          ROS_WARN("ALIGN_AND_DEFORM MODE");

          feedback_.status = ALIGN_AND_DEFORM_MODE;
          as_.publishFeedback(feedback_);

          CAD_MODEL_ALIGNED = ICP(&SCAN_MODEL, &CAD_MODEL);

          ERROR = DeformationsControl(SCAN_MODEL, CAD_MODEL_ALIGNED, 5000000, 18, 188, 137, 0);


          // test data
          ERROR = 0.333;
          CAD_MODEL_ALIGNED.Faces = {{1, 2, 3},
                             {2, 3, 4},
                             {3, 4, 6},
                             {3, 6, 5}};
          CAD_MODEL_ALIGNED.Points = {{0, 0, 0},
                              {0, 1, 0},
                              {1, 0, 0},
                              {1, 1, 0},
                              {2, 0, 0},
                              {2, 1, 0}};

          success = true;

          if(success)
          {
            feedback_.status = PROCESSING_SUCCESS;
            as_.publishFeedback(feedback_);

            //scanning_system_core::Mesh marker;

            for(int j = 0; j < CAD_MODEL_ALIGNED.Points.size(); j++)
            {
                result_.ProcessingResult_::vertices_CAD.push_back({});
                result_.vertices_CAD[j].x = CAD_MODEL_ALIGNED.Points[j][0];
                result_.vertices_CAD[j].y = CAD_MODEL_ALIGNED.Points[j][1];
                result_.vertices_CAD[j].z = CAD_MODEL_ALIGNED.Points[j][2];

                //ROS_INFO("x = %f, y = %f, z = %f", result_.vertices[j].x, result_.vertices[j].y, result_.vertices[j].z);
            }

            for(int k = 0; k < CAD_MODEL_ALIGNED.Faces.size(); k++)
            {
                result_.ProcessingResult_::triangles_CAD.push_back({});
                result_.triangles_CAD[k].vertex_indices[0] = CAD_MODEL_ALIGNED.Faces[k][0];
                result_.triangles_CAD[k].vertex_indices[1] = CAD_MODEL_ALIGNED.Faces[k][1];
                result_.triangles_CAD[k].vertex_indices[2] = CAD_MODEL_ALIGNED.Faces[k][2];

                //ROS_INFO("triangle [%d; %d; %d]", result_.triangles[k].vertex_indices[0],result_.triangles[k].vertex_indices[1],result_.triangles[k].vertex_indices[2]);
            }
            //result_.trajectory = ...;
            result_.error = ERROR;
            ROS_INFO("%s: Align is succeeded", action_name_.c_str());
            // set the action state to succeeded
            as_.setSucceeded(result_);
          }

          break;
      }


      for(int i=1; i<=goal->mode; i++)
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


