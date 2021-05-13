#ifndef CPOLYMODEL_H
#define CPOLYMODEL_H

#endif // CPOLYMODEL_H

#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <ros/ros.h>

#include "scanning_system_core/PolygonModel.h"
#include <typeinfo>
#include "math.h"

struct SPolyModel
{
    std::vector<std::vector<int>> Faces;
    std::vector<std::vector<double>> normals;
    std::vector<std::vector<double>> Points;
};

class CPolyModel
{
public:
    SPolyModel model;
    CPolyModel(){}
    ~CPolyModel(){}

    double CalcDist(std::vector<double> p1, std::vector<double> p2);

    std::vector<std::vector<int>> GetFaces(std::vector<std::vector<std::vector<double>>> Profiles,
                                           std::vector<std::vector<float>> RobotPosition);
};



