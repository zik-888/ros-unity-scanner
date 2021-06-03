#ifndef CSCANNER_H
#define CSCANNER_H

#endif // CSCANNER_H

#pragma once
#include <stdio.h>
#include <iostream>
#include <GoSdk/GoSdk.h>
#include <string>
#include <fstream>

//#include "action_scan.h"

#include <actionlib/server/simple_action_server.h>
#include <scanning_system_core/ScanningAction.h>

#include "scanning_system_core/PolygonModel.h"
#include "scanning_system_core/ScanPositionService.h"
#include <typeinfo>

#include <CPolyModel/CPolyModel.h>

#define DO_NOT_USE_SHARED_MEMORY 1
#define NON_MATLAB_PARSING 1
#define MAX_EXT_API_CONNECTIONS 255
#define PI 3.14159265359

#define RECEIVE_TIMEOUT         (20000000)
#define INVALID_RANGE_16BIT     ((signed short)0x8000)          // gocator transmits range data as 16-bit signed integers. 0x8000 signifies invalid range data.
#define DOUBLE_MAX              ((k64f)1.7976931348623157e+308) // 64-bit double - largest positive value.
#define INVALID_RANGE_DOUBLE    ((k64f)-DOUBLE_MAX)             // floating point value to represent invalid range data.
#define SENSOR_IP               "192.168.1.10"

#define NM_TO_MM(VALUE) (((k64f)(VALUE))/1000000.0)
#define UM_TO_MM(VALUE) (((k64f)(VALUE))/1000.0)

enum StatusOfSystem
{
    INIT_SYSTEM,
    READY_TO_SCAN,
    GO_TO_TARGET_POINT,
    SCANNING,
    SCANNING_SUCCESS,
    ERROR_SENDING_TRAJECTORY,
    ERROR_GET_ROBOT_POSITION,
    ERROR_CONNECTION_TO_SCANNER,
    ALIGN_MODE,
    DEFROM_MODE,
    ALIGN_AND_DEFORM_MODE,
    PROCESSING_SUCCESS,
    ALIGN_ERROR
    // ... e.t.
};


typedef struct
{
    double x;   // x-coordinate in engineering units (mm) - position along laser line
    double z;   // z-coordinate in engineering units (mm) - height (at the given x position)
    unsigned char intensity;
}ProfilePoint;

struct SPoint
{
    float x;
    float y;
    float z;
};

struct PositionTIP
{
public:

    float x;
    float y;
    float z;

    float a;
    float b;
    float c;

    bool Compare(PositionTIP pos, PositionTIP stop_pos, float eps = 0.2f)
    {
        if(abs(pos.x - stop_pos.x) < eps && abs(pos.y - stop_pos.y) < eps && abs(pos.z - stop_pos.z) < eps)
        {
            ROS_INFO("Eps = %f ", eps);

            return true;
        }
        else
        {
            return false;
        }
    }
};

class CScanner
{
public:
    kStatus status;
    kAssembly api = kNULL;
    GoSystem system = kNULL;
    GoSensor sensor = kNULL;
    GoSetup setup = kNULL;
    kIpAddress ipAddress;
    kAssembly apiLib = kNULL;

    k64f currentExposure;
    k64f newExposure;

    unsigned int i, j, k, arrayIndex;
    GoDataSet dataset = kNULL;
    ProfilePoint* profileBuffer = NULL;
    GoStamp *stamp = kNULL;
    GoDataMsg dataObj;
    k32u profilePointCount;

    float offset = 97.0f;

    bool init(kStatus &status, kAssembly &api, kAssembly &apiLib,GoSystem &system, GoSensor &sensor, kIpAddress &ipAddress);

    void configure();

    ProfilePoint* ReceiveProfile();

    SPolyModel scanning(actionlib::SimpleActionServer<scanning_system_core::ScanningAction> &as_,
                        std::string &action_name_,
                        PositionTIP &stopPoint,
                        ros::ServiceClient &client_Scan_Position,
                        CScanner &scanner);

    CScanner(){}
    ~CScanner(){}
};


