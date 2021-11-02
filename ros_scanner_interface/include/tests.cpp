#include <iostream>
#include <fstream>

#include <omp.h>
#include <stdlib.h>

#include <action_scan.h>
#include <actions_proc/action_proc.h>

#include <ros/ros.h>
#include "ros_scanner_interface/AddTwoInts.h"
//#include "ros_scanner_interface/Mesh.h"
#include "ros_scanner_interface/PolygonModel.h"
#include "ros_scanner_interface/Status.h"

#include "ros_scanner_interface/StartScanMsg.h"

#include <GL/glut.h>

#include <thread>
#include <chrono>

#include <typeinfo>
#include <include/go_modes.h>
#include <include/cmessage.h>
#include <include/deformcontrollib.cpp>


#include <ros_scanner_interface/StartMsg.h>
#include <ros_scanner_interface/StartService.h>

#include "ros_scanner_interface/StatusMsg.h"
#include "ros_scanner_interface/StatusService.h"

#include <GoSdk/GoSdk.h>
#include <stdio.h>


#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>

// -----------------------------------
// -----------------------------------
// base methods from scanning_node.cpp
// -----------------------------------
// -----------------------------------

/*
void thStatusMessage(ros_scanner_interface::Status& status_msg)
{
    ros::Rate loop_rate(1);
    while(ros::ok())
    {
        status_pub.publish(status_msg);
        loop_rate.sleep();
    }
}

class CStatusMessage
{
public:
    CStatusMessage(ros::NodeHandle& nh, ros_scanner_interface::Status& status_msg)
    {
        last_status_.status = status_msg.status;
        last_status_.errorcode = status_msg.errorcode;

        ROS_INFO("Status was sended");
        //std::cout << "Status = " << StatusOfScaner << std::endl;
        //status_msg.status = StatusOfScaner;

        //last_status_ = status_msg;
        status_pub.publish(status_msg);

        //server_status = nh.advertiseService("StatusMsg", &StatusMessage::status_send, this);

    }
    bool status_send(ros_scanner_interface::StatusService::Request &req, ros_scanner_interface::StatusService::Response &res)
    {
        res.status = last_status_.status;
        ROS_INFO("sending back response");
        // get scanning area (req)
        return true;
    }

    ros::ServiceServer server_status;
    ros_scanner_interface::Status last_status_;

};*/

// -----------------------------------
// -----------------------------------
// base methods from scanning_node.cpp
// -----------------------------------
// -----------------------------------


/*
// -----------___--___---____-___--------_TEST-------------------------------_____----_____---
//test();
// pcl::PolygonMesh CAD_mesh = ObjLoad("/home/ros-industrial/Desktop/detail_data/CAD.obj");
// pcl::PolygonMesh SCAN_mesh = ObjLoad("/home/ros-industrial/Desktop/detail_data/SCAN.obj");

//pcl::PolygonMesh CAD_mesh = ObjLoad("/home/ros-industrial/Desktop/real_scan.obj");
//pcl::PolygonMesh SCAN_mesh = ObjLoad("/home/ros-industrial/Desktop/real_scan.obj");

pcl::PolygonMesh CAD_mesh = ObjLoad("/home/ros-industrial/Desktop/detail_data/cad_align_manual.obj");
// pcl::PolygonMesh SCAN_mesh = ObjLoad("/home/ros-industrial/Desktop/detail_data/SCAN_NON_DEFORM.obj");
pcl::PolygonMesh SCAN_mesh = ObjLoad("/home/ros-industrial/Desktop/detail_data/SCAN_FROM_CAD.obj");

pcl::PointCloud<pcl::PointXYZ>::Ptr CAD_cloud(new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZ>::Ptr SCAN_cloud(new pcl::PointCloud<pcl::PointXYZ>);

pcl::fromPCLPointCloud2(CAD_mesh.cloud, *CAD_cloud);
pcl::fromPCLPointCloud2(SCAN_mesh.cloud, *SCAN_cloud);

std::cout << "Size polygon mesh = " << CAD_mesh.polygons.size() << std::endl;
std::cout << "Size polygon mesh = " << SCAN_mesh.polygons.size() << std::endl;

std::cout << "Size Points mesh = " << CAD_cloud->size() << std::endl;
std::cout << "Size Points mesh = " << SCAN_cloud->size() << std::endl;

SPolyModel CAD_MODEL;

//struct SPolyModel SCAN_MODEL;

for(int i = 0; i < CAD_cloud->points.size(); i++)
{
    CAD_MODEL.Points.push_back({CAD_cloud->points[i]._PointXYZ::x,
                                CAD_cloud->points[i]._PointXYZ::y,
                                CAD_cloud->points[i]._PointXYZ::z});
}

for(int i = 0; i < SCAN_cloud->points.size(); i++)
{
    SCAN_MODEL.Points.push_back({SCAN_cloud->points[i]._PointXYZ::x,
                                 SCAN_cloud->points[i]._PointXYZ::y,
                                 SCAN_cloud->points[i]._PointXYZ::z});

}


for(int i = 0; i < CAD_mesh.polygons.size(); i++)
{
    CAD_MODEL.Faces.push_back({CAD_mesh.polygons[i].vertices[0],
                               CAD_mesh.polygons[i].vertices[1],
                               CAD_mesh.polygons[i].vertices[2]});
}

for(int i = 0; i < SCAN_mesh.polygons.size(); i++)
{
    SCAN_MODEL.Faces.push_back({SCAN_mesh.polygons[i].vertices[0],
                                SCAN_mesh.polygons[i].vertices[1],
                                SCAN_mesh.polygons[i].vertices[2]});
}


std::cout << "CAD Faces size = " << CAD_MODEL.Faces.size() << std::endl;
std::cout << "SCAN Faces size = " << SCAN_MODEL.Faces.size() << std::endl;\

std::cout << "CAD Points size = " << CAD_MODEL.Points.size() << std::endl;
std::cout << "SCAN Points size = " << SCAN_MODEL.Points.size() << std::endl;


//double err_0 = getError(SCAN_MODEL, CAD_MODEL);

//double err = DeformationsControl(SCAN_MODEL, CAD_MODEL, 5000000, 2572, 188, 72*0.8, 1000);
// double err = DeformationsControl(SCAN_MODEL, CAD_MODEL, 5000000, 18, 188, 137, 700);

struct STrajectory trajectory = TrajectoryForm(CAD_MODEL);

for(int i = 0; i < trajectory.points.size(); i++)
{
    std::cout << trajectory.number_of_points[i] << std::endl;
}




std::ofstream out1;
out1.open("/home/ros-industrial/Desktop/trajectory_before_correct.txt");//, std::ios::app);
for (int i = 0; i < trajectory.points.size(); i++)
{
    out1  << trajectory.points[i][0] << " " << trajectory.points[i][1] << " " << trajectory.points[i][2] << std::endl;
}
out1 << "" << std::endl;
out1 << "" << std::endl;
for (int i = 0; i < trajectory.number_of_points.size(); i++)
{
    out1  << trajectory.number_of_points[i] << std::endl;
}





MakeNoiseModel(SCAN_MODEL);

SPolyModel cad_after_align = ICP(&SCAN_MODEL, &CAD_MODEL);

// double err = DeformationsControl(SCAN_MODEL, CAD_MODEL, 5000000, 18, 188, 137, 0);
// double err = DeformationsControl(CAD_MODEL, SCAN_MODEL, 5000000, 18, 188, 137, 0);
//  std::cout << "Deformaion Control is succesed. Error = " << err << std::endl;


double traj_err = CalcTrajectoryError(trajectory, SCAN_MODEL, cad_after_align, 1500000);


//test(CAD_MODEL);





std::ofstream out3;
out3.open("/home/ros-industrial/Desktop/test_cad_after_correct.obj");//, std::ios::app);
out3 << "o Object.1" << std::endl;

for (int i = 0; i < CAD_MODEL.Points.size(); i++)
{
    out3 << "v " << CAD_MODEL.Points[i][0] << " " << CAD_MODEL.Points[i][1] << " " << CAD_MODEL.Points[i][2] << std::endl;
}
//for (int d = 0; d < SAMPLE_POINTS_SCAN.size(); d++)
//{
//    out3 << "v " << SAMPLE_POINTS_SCAN[d][0] << " " << SAMPLE_POINTS_SCAN[d][1] << " " << SAMPLE_POINTS_SCAN[d][2] << std::endl;
//}
for (int c = 0; c < CAD_MODEL.Faces.size(); c++)
{
    out3 << "f " << CAD_MODEL.Faces[c][0] + 1 << " "
         << CAD_MODEL.Faces[c][1] + 1 << " "
         << CAD_MODEL.Faces[c][2] + 1 << std::endl;
}


std::ofstream out4;
out4.open("/home/ros-industrial/Desktop/test_scan_model.obj");//, std::ios::app);
out4 << "o Object.1" << std::endl;

for (int i = 0; i < SCAN_MODEL.Points.size(); i++)
{
    out4 << "v " << SCAN_MODEL.Points[i][0] << " " << SCAN_MODEL.Points[i][1] << " " << SCAN_MODEL.Points[i][2] << std::endl;
}
//for (int d = 0; d < SAMPLE_POINTS_SCAN.size(); d++)
//{
//    out3 << "v " << SAMPLE_POINTS_SCAN[d][0] << " " << SAMPLE_POINTS_SCAN[d][1] << " " << SAMPLE_POINTS_SCAN[d][2] << std::endl;
//}
for (int c = 0; c < SCAN_MODEL.Faces.size(); c++)
{
    out4 << "f " << SCAN_MODEL.Faces[c][0] + 1 << " "
         << SCAN_MODEL.Faces[c][1] + 1 << " "
         << SCAN_MODEL.Faces[c][2] + 1<< std::endl;
}


std::ofstream out5;
out5.open("/home/ros-industrial/Desktop/test_cad_align.obj");//, std::ios::app);
out5 << "o Object.1" << std::endl;

for (int i = 0; i < cad_after_align.Points.size(); i++)
{
    out5 << "v "
         << cad_after_align.Points[i][0] << " "
         << cad_after_align.Points[i][1] << " "
         << cad_after_align.Points[i][2] << std::endl;
}

for (int c = 0; c < cad_after_align.Faces.size(); c++)
{
    out5 << "f " << cad_after_align.Faces[c][0] + 1 << " "
                 << cad_after_align.Faces[c][1] + 1 << " "
                 << cad_after_align.Faces[c][2] + 1<< std::endl;
}



ScanningMode scantraj;
ros_scanner_interface::TrajectoryService traj = scantraj.TrajectoryForming(1, 0, 7, 10);

struct SPolyModel model_test;

model_test.Points = {{1, 0.5, 1},
                     {2, 4,   1},
                     {4, 1,   1}};

model_test.Faces = {{0, 1, 2}};

std::ofstream out6;
out6.open("/home/ros-industrial/Desktop/test_trajectory/test_trajectory_link_0.obj");//, std::ios::app);
out6 << "o Object.1" << std::endl;

std::vector<std::vector<float>> traj1 = {{2, 3, 1}, {3, 1.5, 1}, {2, 1.5, 1}};
//std::vector<std::vector<float>> traj1 = {{8, 3, 1}};

for (int i = 0; i < model_test.Points.size(); i++)
{
    out6 << "v "
         << model_test.Points[i][0] << " "
         << model_test.Points[i][1] << " "
         << model_test.Points[i][2] << std::endl;
}

for(int u = 0; u < model_test.Faces.size(); u++)
{
    out6 << "f "
         << model_test.Faces[u][0] + 1 << " "
         << model_test.Faces[u][1] + 1 << " "
         << model_test.Faces[u][2] + 1 << std::endl;
}

LinkTrajectoryToModel(model_test, traj1);

std::ofstream out7;
out7.open("/home/ros-industrial/Desktop/test_trajectory/test_trajectory_link.obj");//, std::ios::app);
out7 << "o Object.1" << std::endl;

for (int i = 0; i < model_test.Points.size(); i++)
{
    out7 << "v "
         << model_test.Points[i][0] << " "
         << model_test.Points[i][1] << " "
         << model_test.Points[i][2] << std::endl;
}

for(int u = 0; u < model_test.Faces.size(); u++)
{
    out7 << "f "
         << model_test.Faces[u][0] + 1 << " "
         << model_test.Faces[u][1] + 1 << " "
         << model_test.Faces[u][2] + 1 << std::endl;
}


// destroy handles

//GoDestroy(sensorScan.system);
//GoDestroy(sensorScan.api);

//free(sensorScan.profileBuffer);


//coordinateTh.detach();
//statusTh.detach();

//th.detach();
//thStart.join();
