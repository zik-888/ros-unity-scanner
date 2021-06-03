#include <iostream>
#include <fstream>

#include <omp.h>
#include <stdlib.h>

#include <actions_scan/action_scan.h>
#include <actions_proc/action_proc.h>
#include <actions_align/action_align.h>

#include <CScanner/CScanner.h>
#include <actions_status/action_status.h>

#include <ros/ros.h>
#include "scanning_system_core/AddTwoInts.h"
//#include "scanning_system_core/Mesh.h"
#include "scanning_system_core/PolygonModel.h"
#include "scanning_system_core/Status.h"

#include "scanning_system_core/StartScanMsg.h"

#include <GL/glut.h>

#include <thread>
#include <chrono>

#include <typeinfo>
#include <include/deformcontrollib.cpp>

#include <scanning_system_core/StartMsg.h>
#include <scanning_system_core/StartService.h>

#include "scanning_system_core/StatusMsg.h"
#include "scanning_system_core/StatusService.h"

#include <GoSdk/GoSdk.h>
#include <stdio.h>

/*
 * #include <pcl_conversions/pcl_conversions.h>
#include <pcl-1.10/pcl/point_types.h>
#include <pcl-1.10/pcl/PCLPointCloud2.h>
#include <pcl-1.10/pcl/conversions.h>
#include <pcl_ros/transforms.h>
 */

//ros::Publisher status_pub;

void ServiceGetPose(ros::NodeHandle nh)
{

    ROS_INFO("Thread_Pos: waiting pos \n");
    scanning_system_core::PoseRobotService srvPose;
    ros::ServiceClient client_pos = nh.serviceClient<scanning_system_core::PoseRobotService>("GetPose");
    ros::Rate loop_rate(1);

    srvPose.request.request = "GetPos";

    while(!client_pos.call(srvPose))
    {
        //ROS_INFO("Thread_Pos: waiting pos \n");
        loop_rate.sleep();
    }
    while(client_pos.call(srvPose))// && srvPose.request.getpose == "GetPose")
    {
        ROS_INFO("Thread_Pos: pose is responsed"); //, (scanning_system_core::PolygonModel::Requestsrv.response.mesh);
        std::cout << srvPose.response.artur_scan_pose << std::endl;
        loop_rate.sleep();
    }
    loop_rate.sleep();
}

void thStatusAction(ros::NodeHandle nh, SPolyModel *scan_model)
{
    AlignAction align_action("align_node", nh, &(*scan_model));
    ros::Rate r(1);
    StatusAction status_action("status_node", nh);

    while(true)
    {
        // ROS_INFO("I'M ALIVE");
        r.sleep();
    }
}

/*static void StatusTopic (const scanning_system_core::Status& m)
{
  scanning_system_core::Status marker;
  marker.status = m.status;
  marker.errorcode = m.errorcode;

  status_pub.publish(marker);
}*/

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");
    // ROS init
    ros::init(argc, argv, "scanning_node");
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);

    SPolyModel SCAN_MODEL;

    std::thread thStatus(thStatusAction, nh, &SCAN_MODEL);
    //struct SPolyModel SCAN_MODEL;

    //Action server init
    ScanningAction scanning_action("scanning_node", nh, &SCAN_MODEL);
    // ProcessingAction proc_action("proc_node", nh, &SCAN_MODEL);
    // AlignAction align_action("align_node", nh, &SCAN_MODEL);

    // // // StatusAction status_action("status_node", nh);

    // Topic status init
    // status_pub = nh.advertise<scanning_system_core::Status>("status_of_scan_system", 1000);
    //CScanner scanner2;
    //scanner2.init(scanner2.status, scanner2.api, scanner2.apiLib,
    //             scanner2.system, scanner2.sensor, scanner2.ipAddress);
    scanning_system_core::Status statusMsg;

    statusMsg.status = 0;
    statusMsg.errorcode = "err";

    //ros::Rate loop_rate(1);

    scanning_system_core::Status st;//pause
    thStatus.detach();


    ros::spin();
    return 0;
}



