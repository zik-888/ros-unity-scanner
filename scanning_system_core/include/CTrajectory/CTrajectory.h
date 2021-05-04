#ifndef CTRAJECTORY_H
#define CTRAJECTORY_H

#endif // CTRAJECTORY_H

#include "scanning_system_core/PolygonModel.h"
#include "scanning_system_core/TrajectoryService.h"
#include <typeinfo>

class CTrajectory
{
public:
    CTrajectory(){}
    ~CTrajectory(){}

    scanning_system_core::TrajectoryService TrajectoryForming(float x1, float y1, float x2, float y2);

};


