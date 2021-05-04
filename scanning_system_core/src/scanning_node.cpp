#include <iostream>
#include <fstream>

#include <omp.h>
#include <stdlib.h>

#include <actions_scan/action_scan.h>
#include <actions_proc/action_proc.h>

#include <CScanner/CScanner.h>

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


ros::Publisher status_pub;

/*simxInt ConnectToVREP (int port)
{
    simxFinish(-1);

    const simxChar* connectionAdr = "127.0.0.1";
    simxInt clientID;

    clientID = simxStart(connectionAdr, port, 0, 0, 5000, 5);
    //std::cout << "Connected to VREP \n ClientID = " << clientID << std::endl;
    ROS_INFO("Connected to VREP \n ClientID = %d ", clientID);
    return clientID;
}

void thStatusMessage(scanning_system_core::Status& status_msg)
{
    ros::Rate loop_rate(1);
    while(ros::ok())
    {
        status_pub.publish(status_msg);
        loop_rate.sleep();
    }
}*/

/*class CStatusMessage
{
public:
    CStatusMessage(ros::NodeHandle& nh, scanning_system_core::Status& status_msg)
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
    bool status_send(scanning_system_core::StatusService::Request &req, scanning_system_core::StatusService::Response &res)
    {
        res.status = last_status_.status;
        ROS_INFO("sending back response");
        // get scanning area (req)
        return true;
    }

    ros::ServiceServer server_status;
    scanning_system_core::Status last_status_;

};*/

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

static void StatusTopic (const scanning_system_core::Status& m)
{
  scanning_system_core::Status marker;
  marker.status = m.status;
  marker.errorcode = m.errorcode;

  status_pub.publish(marker);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");
    // ROS init
    ros::init(argc, argv, "scanning_node");
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);

    SPolyModel SCAN_MODEL;
    //struct SPolyModel SCAN_MODEL;

    //Action server init
    ScanningAction scanning_action("scanning_node", nh, &SCAN_MODEL);
    ProcessingAction proc_action("proc_node", nh, &SCAN_MODEL);

    // Topic status init
    status_pub = nh.advertise<scanning_system_core::Status>("status_of_scan_system", 1000);
    scanning_system_core::Status statusMsg;

    statusMsg.status = 0;
    statusMsg.errorcode = "err";

    //ros::Rate loop_rate(1);

    scanning_system_core::Status st;//pause


    ros::spin();
    return 0;
}



