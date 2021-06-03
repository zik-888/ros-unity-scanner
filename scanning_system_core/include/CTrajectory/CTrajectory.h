#ifndef CTRAJECTORY_H
#define CTRAJECTORY_H

#endif // CTRAJECTORY_H

#include "scanning_system_core/PolygonModel.h"
#include "scanning_system_core/TrajectoryService.h"
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
    scanning_system_core::TrajectoryService TrajectoryForming(float x1, float y1, float x2, float y2);

};


