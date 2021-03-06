#ifndef CTRAJECTORY_H
#define CTRAJECTORY_H

#endif // CTRAJECTORY_H

#include "ros_scanner_interface/PolygonModel.h"
#include "ros_scanner_interface/TrajectoryService.h"
#include <typeinfo>

class CTrajectory
{
private:
    float HeightOfScanner = 470; // high of sensor
    float LeightOfLaserLine = 400; // L = 460 mm - Laser line width
    float Overlap = 0.9;  //10% ovelap

public:
    CTrajectory(){}
    ~CTrajectory(){}
    ros_scanner_interface::TrajectoryService TrajectoryForming(float x1, float y1, float x2, float y2);

};


