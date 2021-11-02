#ifndef CTRAJACTORY_H
#define CTRAJACTORY_H

#endif // CTRAJACTORY_H
#include <CTrajectory.h>
#pragma once

#include <CScanner/CScanner.h>

geometry_msgs::Pose set_point(float x,float y,float z,float a,float b,float c)
{
    geometry_msgs::Pose point;
    point.position.x = x;
    point.position.y = y;
    point.position.z = z;
    point.orientation.x = a;
    point.orientation.y = b;
    point.orientation.z = c;
    point.orientation.w = 0;

    return point;
}

ros_scanner_interface::TrajectoryService CTrajectory::TrajectoryForming(float x1, float y1, float x2, float y2)
{
    ros_scanner_interface::TrajectoryService traj;
    std::vector<std::vector<float>> ScanTrajectory;

    PositionTIP orientation;
    orientation.a = 180;
    orientation.b = 0;
    orientation.c = -176.14;

    // points of scanning area
    std::vector<float> point_1 = {x1, y1, HeightOfScanner};
    std::vector<float> point_2 = {x2, y2, HeightOfScanner};

    std::vector<float> point_3 = {x1, y2, HeightOfScanner};
    std::vector<float> point_4 = {x2, y1, HeightOfScanner};

    float a = abs(x2 - x1);
    float b = abs(y2 - y1);
    std::cout << "(x1, y1) = " << x1 << "," << y1 << std::endl;
    std::cout << "(x2, y2) = " << x2 << "," << y2 << std::endl;

    //int count = round(b/L);
    int count = round(a/LeightOfLaserLine);
    if(count == 0) count = 1;

    if(count == 1)
    {
        // 2 points of trajectory
        std::cout << " points: " << count * 2 << std::endl;
        if(x1 > x2)
        {
            ScanTrajectory.push_back({x2 + a/2, y2, HeightOfScanner});
            ScanTrajectory.push_back({x2 + a/2, y1, HeightOfScanner});
        }
        else
        {
            ScanTrajectory.push_back({x1 + a/2, y2, HeightOfScanner});
            ScanTrajectory.push_back({x1 + a/2, y1, HeightOfScanner});
        }

        //ScanTrajectory.push_back({x1, b/2, H});
        //ScanTrajectory.push_back({x2, b/2, H});



    }
    else
    {
        // some points of trajectory
        std::cout << " points: " << count * 2 << std::endl;


        std::vector<std::vector<float>> FirstPoints;
        std::vector<std::vector<float>> SecondPoints;

        FirstPoints.push_back({x1, y1 + LeightOfLaserLine/2, HeightOfScanner});
        SecondPoints.push_back({x2, y1 + LeightOfLaserLine/2, HeightOfScanner});

        for(int i = 0; i < count - 1; i++)
        {
            FirstPoints.push_back({x1, FirstPoints[i][1] + LeightOfLaserLine, HeightOfScanner});
            SecondPoints.push_back({x2, SecondPoints[i][1] + LeightOfLaserLine, HeightOfScanner});
        }

        for (int i = 0; i < FirstPoints.size(); i++)
        {
            if((i % 2) == 0)
            {
                ScanTrajectory.push_back(FirstPoints[i]);
                ScanTrajectory.push_back(SecondPoints[i]);
            }
            else
            {
                ScanTrajectory.push_back(SecondPoints[i]);
                ScanTrajectory.push_back(FirstPoints[i]);
            }
        }
    }

    for(int i = 0; i < ScanTrajectory.size(); i++)
    {
        traj.request.rpose.push_back(set_point(ScanTrajectory[i][0],
                                               ScanTrajectory[i][1],
                                               ScanTrajectory[i][2],
                                               orientation.a,
                                               orientation.b,
                                               orientation.c));


        std::cout << i << " = "
                  << ScanTrajectory[i][0] << " ; "
                  << ScanTrajectory[i][1] << " ; "
                  << ScanTrajectory[i][2] << std::endl;
    }
    ROS_INFO("Trajectory is formed");
    return traj;
}





