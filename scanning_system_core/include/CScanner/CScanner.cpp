#ifndef CSCANNER_H
#define CSCANNER_H
#include <CScanner/CScanner.h>

#include <stdio.h>
#include <iostream>
#include <GoSdk/GoSdk.h>
#include <string>

#include <ros/ros.h>
#include "scanning_system_core/PolygonModel.h"
#include <typeinfo>

#endif // CSCANNER_H

bool CScanner::init(kStatus &status, kAssembly &api, kAssembly &apiLib, GoSystem &system, GoSensor &sensor, kIpAddress &ipAddress)
{
    if ((status = GoSdk_Construct(&api)) != kOK)
    {
        ROS_ERROR("Error: GoSdk_Construct:%d\n", status);
        return false;
    }

    // construct GoSystem object
    if ((status = GoSystem_Construct(&system, kNULL)) != kOK)
    {
        ROS_ERROR("Error: GoSystem_Construct:%d\n", status);
        return false;
    }

    // Parse IP address into address data structure
    kIpAddress_Parse(&ipAddress, SENSOR_IP);

    // obtain GoSensor object by IP address
    if ((status = GoSystem_FindSensorByIpAddress(system, &ipAddress, &sensor)) != kOK)
    {
        ROS_ERROR("Error: GoSystem_FindSensor:%d\n", status);
        return false;
    }

    // create connection to GoSensor object
    if ((status = GoSensor_Connect(sensor)) != kOK)
    {
        ROS_ERROR("Error: GoSensor_Connect:%d\n", status);
        return false;
    }

    for (int i = 0; i < GoSystem_SensorCount(system); i++)
    {
        GoAddressInfo addressInfo;
        GoSensor sensor = GoSystem_SensorAt(system, i);

        if ((status = GoSensor_Address(sensor, &addressInfo)) == kOK)
        {
            kChar addressString[64];
            kIpAddress_Format(addressInfo.address, addressString, 64);
            ROS_INFO("Discovers sensor %d with IP %s\n", GoSensor_Id(sensor), addressString);
        }
    }
}

void CScanner::configure()
{
    if ((status = GoSdk_Construct(&api)) != kOK)
    {
        printf("Error: GoSdk_Construct:%d\n", status);
        return;
    }

    // construct GoSystem object
    if ((status = GoSystem_Construct(&system, kNULL)) != kOK)
    {
        printf("Error: GoSystem_Construct:%d\n", status);
        return;
    }

    // Parse IP address into address data structure
    kIpAddress_Parse(&ipAddress, SENSOR_IP);

    // obtain GoSensor object by IP address
    if ((status = GoSystem_FindSensorByIpAddress(system, &ipAddress, &sensor)) != kOK)
    {
        printf("Error: GoSystem_FindSensor:%d\n", status);
        return;
    }

    // create connection to GoSensor object
    if ((status = GoSensor_Connect(sensor)) != kOK)
    {
        printf("Error: GoSensor_Connect:%d\n", status);
        return;
    }

    // retrieve setup handle
    if ((setup = GoSensor_Setup(sensor)) == kNULL)
    {
        printf("Error: GoSensor_Setup: Invalid Handle\n");
        return;
    }

    if ((status = GoSensor_CopyFile(sensor, "_live.job", "oldExposure.job")) != kOK)
    {
        printf("Error: GoSensor_CopyFile:%d\n", status);
        return;
    }

    // read current parameters
    currentExposure = GoSetup_Exposure(setup, GO_ROLE_MAIN);
    printf("Current Parameters:\n");
    printf("-------------------\n");
    printf("Exposure:%f us\n\n", currentExposure);

    // modify parameter in main sensor
    if ((status = GoSetup_SetExposure(setup, GO_ROLE_MAIN, currentExposure + 200)) != kOK)
    {
        printf("Error: GoSetup_SetExposure:%d\n", status);
        return;
    }

    // GoSensorFlush() - immediately synchronizes configuration changes to the sensor
    // *The changes will be shown on the web browser GUI after the browser has been refreshed.
    // NOTE: Sensor is not automatically synchronized with every call to function that modifies a setting.
    // This allows for rapid configuring sensors without delay caused by synchronization after every call.
    // Generally functions that retreieve setting values causes automatic synchronization while functions that set values don't.
    // Synchronization is also always guranteed prior to sensor entering running state. The GoSensor_Flush() function
    // should only be used when configuration changes are needed to be seen immediately.
    GoSensor_Flush(sensor);

    newExposure = GoSetup_Exposure(setup, GO_ROLE_MAIN);

    printf("New Parameters:\n");
    printf("---------------\n");
    printf("Exposure:%f us\n\n", newExposure);

    // Save the configuration and template into a new file set. This is the same behavior
    // as if the user clicks the save button in the toolbar.

    if ((status = GoSensor_CopyFile(sensor, "_live.job", "newExposure.job")) != kOK)
    {
        printf("Error: GoSensor_CopyFile:%d\n", status);
        return;
    }

    // Set the saved configuration as default job. This makes it the active configuration when the sensor powers up.
    if ((status = GoSensor_SetDefaultJob(sensor, "newExposure.job")) != kOK)
    {
        printf("Error: GoSensor_SetDefaultJob:%d\n", status);
        return;
    }

    // Switches back to the original exposure
    if ((status = GoSensor_CopyFile(sensor, "oldExposure.job", "_live.job")) != kOK)
    {
        printf("Error: GoSensor_CopyFile:%d\n", status);
        return;
    }


    return;
}

SPolyModel CScanner::scanning(actionlib::SimpleActionServer<scanning_system_core::ScanningAction> &as_,
                               std::string &action_name_,
                               PositionTIP &stopPoint,
                               ros::ServiceClient &client_Scan_Position,
                               CScanner &scanner)
{
    ROS_INFO("Scanning...");

    scanning_system_core::ScanPositionService pos_srv;
    scanning_system_core::ScanningFeedback feedback_;
    feedback_.status = SCANNING;

    // init help variable
    bool success = true;
    int ProfileInd = 0;
    int ind = 0;
    int k = 0;
    PositionTIP pos_tip;

    // init variable for getting SCAN_MODEL
    CPolyModel scan_model;

    std::vector<std::vector<int>>                 Faces;
    std::vector<std::vector<double>>              Points;
    std::vector<std::vector<float>>               RobotPosition;
    std::vector<std::vector<std::vector<double>>> Profiles;

    // init stream for write data
    std::ofstream out;

    out.open("/home/Desktop/real_scan.obj");//, std::ios::app);
    out << "o Object.1" << std::endl;

    // variable may be deleted
    std::vector<float> PointsData = {};
    std::vector<float> Time = {};

    // enable sensor data channel
    if ((scanner.status = GoSystem_EnableData(scanner.system, kTRUE)) != kOK)
    {
        printf("Error: GoSensor_EnableData:%d\n", scanner.status);
    }

    // retrieve setup handle
    if ((scanner.setup = GoSensor_Setup(scanner.sensor)) == kNULL)
    {
        printf("Error: GoSensor_Setup: Invalid Handle\n");
    }

    // retrieve total number of profile points prior to starting the sensor
    if (GoSetup_UniformSpacingEnabled(scanner.setup))
    {
        // Uniform spacing is enabled. The number is based on the X Spacing setting
        scanner.profilePointCount = GoSetup_XSpacingCount(scanner.setup, GO_ROLE_MAIN);
    }
    else
    {
        // non-uniform spacing is enabled. The max number is based on the number of columns used in the camera.
        scanner.profilePointCount = GoSetup_FrontCameraWidth(scanner.setup, GO_ROLE_MAIN);
    }

    if ((scanner.profileBuffer = static_cast<ProfilePoint*>(malloc(scanner.profilePointCount * sizeof(ProfilePoint)))) == kNULL)
    {
        printf("Error: Cannot allocate profileData, %d points\n", scanner.profilePointCount);
    }

    // start Gocator sensor
    if ((scanner.status = GoSystem_Start(scanner.system)) != kOK)
    {
        printf("Error: GoSensor_Start:%d\n", scanner.status);
    }

    GoSetup_SetTriggerSource(scanner.setup, GO_TRIGGER_SOFTWARE);

    while (scanner.status == 1)
    {
        if(client_Scan_Position.call(pos_srv))
        {
            //std::cout << "FROM ROBOT NODE: " << pos_srv.response.artur_scan_pose.position.x << " ; " << pos_srv.response.artur_scan_pose.position.y << " ; " << pos_srv.response.artur_scan_pose.position.z << std::endl;
            //std::cout << "FROM V-REP: " << pos_tip[0] << " ; " << pos_tip[1] << " ; " << pos_tip[2] << std::endl;
            pos_tip.x = pos_srv.response.artur_scan_pose.position.x;
            pos_tip.y = pos_srv.response.artur_scan_pose.position.y;
            pos_tip.z = pos_srv.response.artur_scan_pose.position.z;
            //ROS_INFO("Coordinates are responsed");
        }

        if (pos_tip.Compare(pos_tip, stopPoint) == true)//pos_tip[1] >= 0.3)
        {
            ROS_INFO("STOP SCANNING");
            std::cout << "Pos TIP " << pos_tip.x << " ; " << pos_tip.y << " ; " << pos_tip.z << std::endl
                      << "Stop Pos "<< stopPoint.x << " ; " << stopPoint.y << " ; " << stopPoint.z << std::endl;
            break;
        }

        scanner.status = GoSensor_Trigger(scanner.sensor);

        if(GoSystem_ReceiveData(scanner.system, &scanner.dataset, RECEIVE_TIMEOUT) == kOK)
        {

            if(client_Scan_Position.call(pos_srv))
            {
                //std::cout << "FROM ROBOT NODE: " << pos_srv.response.artur_scan_pose.position.x << " ; " << pos_srv.response.artur_scan_pose.position.y << " ; " << pos_srv.response.artur_scan_pose.position.z << std::endl;
                //std::cout << "FROM V-REP: " << pos_tip[0] << " ; " << pos_tip[1] << " ; " << pos_tip[2] << std::endl;
                pos_tip.x = pos_srv.response.artur_scan_pose.position.x;
                pos_tip.y = pos_srv.response.artur_scan_pose.position.y;
                pos_tip.z = pos_srv.response.artur_scan_pose.position.z;
                //ROS_INFO("Coordinates are responsed");
            }

            Profiles.push_back({});
            std::vector<std::vector<double>> currentProfile;
            for(i = 0; i < GoDataSet_Count(scanner.dataset); ++i)
            {
                scanner.dataObj = GoDataSet_At(scanner.dataset, i);

                //Retrieve GoStamp message
                switch(GoDataMsg_Type(scanner.dataObj))
                {

                case GO_DATA_MESSAGE_TYPE_UNIFORM_PROFILE:
                    {
                        GoResampledProfileMsg profileMsg = scanner.dataObj;

                        if(client_Scan_Position.call(pos_srv))
                        {
                            //std::cout << "FROM ROBOT NODE: " << pos_srv.response.artur_scan_pose.position.x << " ; " << pos_srv.response.artur_scan_pose.position.y << " ; " << pos_srv.response.artur_scan_pose.position.z << std::endl;
                            //std::cout << "FROM ROBOT NODE: " << pos_tip[0] << " ; " << pos_tip[1] << " ; " << pos_tip[2] << std::endl;
                            pos_tip.x = pos_srv.response.artur_scan_pose.position.x;
                            pos_tip.y = pos_srv.response.artur_scan_pose.position.y;
                            pos_tip.z = pos_srv.response.artur_scan_pose.position.z;
                            //ROS_INFO("Coordinates are responsed");
                        }

                        //printf("Resampled Profile Message batch count: %u\n", GoResampledProfileMsg_Count(profileMsg));

                        for (k = 0; k < GoResampledProfileMsg_Count(profileMsg); ++k)
                        {


                            unsigned int validPointCount = 0;
                            short* data = GoResampledProfileMsg_At(profileMsg, k);
                            double XResolution = NM_TO_MM(GoResampledProfileMsg_XResolution(profileMsg));
                            double ZResolution = NM_TO_MM(GoResampledProfileMsg_ZResolution(profileMsg));
                            double XOffset = UM_TO_MM(GoResampledProfileMsg_XOffset(profileMsg));
                            double ZOffset = UM_TO_MM(GoResampledProfileMsg_ZOffset(profileMsg));

                            if(client_Scan_Position.call(pos_srv))
                            {
                                //std::cout << "FROM ROBOT NODE: " << pos_srv.response.artur_scan_pose.position.x << " ; " << pos_srv.response.artur_scan_pose.position.y << " ; " << pos_srv.response.artur_scan_pose.position.z << std::endl;
                                //std::cout << "FROM ROBOT NODE: " << pos_tip[0] << " ; " << pos_tip[1] << " ; " << pos_tip[2] << std::endl;
                                pos_tip.x = pos_srv.response.artur_scan_pose.position.x;
                                pos_tip.y = pos_srv.response.artur_scan_pose.position.y;
                                pos_tip.z = pos_srv.response.artur_scan_pose.position.z;
                                //ROS_INFO("Coordinates are responsed");
                            }

                            //translate 16-bit range data to engineering units and copy profiles to memory array
                            for (scanner.arrayIndex = 0; scanner.arrayIndex < GoResampledProfileMsg_Width(profileMsg); ++scanner.arrayIndex)
                            {
                                if (data[scanner.arrayIndex] != INVALID_RANGE_16BIT )
                                {
                                    scanner.profileBuffer[scanner.arrayIndex].x = XOffset + XResolution * scanner.arrayIndex;
                                    scanner.profileBuffer[scanner.arrayIndex].z = ZOffset + ZResolution * data[scanner.arrayIndex];

                                    //scanner.profileBuffer[scanner.arrayIndex].z = pos_tip[2] - scanner.profileBuffer[scanner.arrayIndex].z;

                                    //out << "v "
                                    //    << scanner.profileBuffer[scanner.arrayIndex].x << " "
                                    //    << pos_tip[1] << " "
                                    //    << scanner.profileBuffer[scanner.arrayIndex].z
                                    //    << std::endl;

                                    if(scanner.profileBuffer[scanner.arrayIndex].z > 4 && (pos_tip.x + scanner.profileBuffer[scanner.arrayIndex].x) < 450
                                            && (pos_tip.x + scanner.profileBuffer[scanner.arrayIndex].x) > 200)
                                    {

                                    out << "v "
                                        << pos_tip.x + scanner.profileBuffer[scanner.arrayIndex].x << " "
                                        << pos_tip.y - scanner.offset << " "
                                        << scanner.profileBuffer[scanner.arrayIndex].z
                                        << std::endl;

                                    //Profiles[ProfileInd].push_back({scanner.profileBuffer[scanner.arrayIndex].x, pos_tip[1], scanner.profileBuffer[scanner.arrayIndex].z});
                                    Profiles[ProfileInd].push_back({pos_tip.x + scanner.profileBuffer[scanner.arrayIndex].x,
                                                                    pos_tip.y - scanner.offset,
                                                                    scanner.profileBuffer[scanner.arrayIndex].z});

                                    // std::vector<float> RobotPos = { pos_tip[0], pos_tip[1], pos_tip[2] };
                                    // RobotPosition.push_back(RobotPos);

                                    //Points.push_back({scanner.profileBuffer[scanner.arrayIndex].x, pos_tip[1], scanner.profileBuffer[scanner.arrayIndex].z});
                                    Points.push_back({pos_tip.x + scanner.profileBuffer[scanner.arrayIndex].x,
                                                      pos_tip.y - scanner.offset ,
                                                      scanner.profileBuffer[scanner.arrayIndex].z});

                                    //currentProfile.push_back({scanner.profileBuffer[scanner.arrayIndex].x, pos_tip[1], scanner.profileBuffer[scanner.arrayIndex].z});
                                    currentProfile.push_back({pos_tip.x + scanner.profileBuffer[scanner.arrayIndex].x,
                                                              pos_tip.y - scanner.offset,
                                                              scanner.profileBuffer[scanner.arrayIndex].z});

                                    }

                                    //std::cout << "x =" << scanner.profileBuffer[scanner.arrayIndex].x << std::endl;
                                    //std::cout << "z =" << scanner.profileBuffer[scanner.arrayIndex].z << std::endl;
                                    validPointCount++;
                                    //ROS_INFO("DEFINE");
                                }
                                else
                                {
                                    scanner.profileBuffer[scanner.arrayIndex].x = XOffset + XResolution * scanner.arrayIndex;
                                    scanner.profileBuffer[scanner.arrayIndex].z = INVALID_RANGE_DOUBLE;
                                    // ROS_WARN("NOT DEFINE");
                                    //std::cout << "x =" << scanner.profileBuffer[scanner.arrayIndex].x << std::endl;
                                    //std::cout << "z =" << scanner.profileBuffer[scanner.arrayIndex].z << std::endl;
                                }
                            }

                            // std::vector<float> RobotPos = { pos_tip[0], pos_tip[1], pos_tip[2] };
                            // RobotPosition.push_back(RobotPos);



                            ProfileInd++;
                            //printf("  Profile Valid Point %d out of max %d\n", validPointCount, scanner.profilePointCount);
                        }
                    }
                    break;
                }
            }

            std::vector<float> RobotPos = { pos_tip.x, pos_tip.y, pos_tip.z };
            RobotPosition.push_back(RobotPos);
        }
        else
        {
            printf ("Error: No data received during the waiting period\n");
        }

    }
    GoDestroy(scanner.dataset);
    free(scanner.profileBuffer);

    // stop Gocator sensor
    if ((scanner.status = GoSystem_Stop(scanner.system)) != kOK)
    {
        printf("Error: GoSensor_Stop:%d\n", scanner.status);
    }

    // destroy handles
    GoDestroy(scanner.system);
    GoDestroy(scanner.api);

    std::vector<std::vector<std::vector<double>>> Profiles_new;
    std::vector<std::vector<float>>               RobotPosition_new;

    int count = 0;

    for(int i = 0; i < Profiles.size(); i++)
    {

        if(Profiles[i].size() != 0)
        {
            Profiles_new.push_back(Profiles[i]);
            RobotPosition_new.push_back(RobotPosition[i]);
        }
    }
    scan_model.model.Faces = scan_model.GetFaces(Profiles_new, RobotPosition_new);
    scan_model.model.Points = Points;
    //Faces = GetFaces(Profiles, RobotPosition);
    //int c = 0;
    ROS_INFO("succeeded 1");

    out << std::endl << std::endl;
    for (int c = 0; c < scan_model.model.Faces.size(); c++)
    {
        out << "f "
            << scan_model.model.Faces[c][0] << " "
            << scan_model.model.Faces[c][1] << " "
            << scan_model.model.Faces[c][2] << std::endl;
    }
    out.close();

    ROS_INFO("succeeded 2");

    // publish feedback
    feedback_.status = SCANNING_SUCCESS;
    as_.publishFeedback(feedback_);

    ROS_INFO("succeeded 3");
    ROS_INFO("Scanned succeeded!");
    return scan_model.model;
}


