#pragma once
#include <stdio.h>
#include <iostream>
#include <GoSdk/GoSdk.h>
#include <string>
#include <fstream>
#include <random>
#include <map>

#include <ros/ros.h>
#include <CPolyModel/CPolyModel.h>

#include "pcl-1.10/pcl/point_cloud.h"
#include <pcl-1.10/pcl/conversions.h>
#include <pcl-1.10/pcl/search/kdtree.h>
#include <pcl-1.10/pcl/point_cloud.h>
#include <pcl-1.10/pcl/kdtree/kdtree.h>
#include <pcl-1.10/pcl/kdtree/kdtree_flann.h>
#include <pcl-1.10/pcl/io/obj_io.h>
#include <pcl-1.10/pcl/common/common.h>
#include <pcl-1.10/pcl/visualization/pcl_visualizer.h>
#include <pcl-1.10/pcl/io/vtk_lib_io.h>
#include <pcl-1.10/pcl/registration/icp.h>

#include <iostream>
#include "vector"
#include <math.h>
#include "stdlib.h"

#define PI 3.14159265359


bool calculateThreeCircleIntersection(float x0, float y0, float r0,
                                      float x1, float y1, float r1,
                                      float x2, float y2, float r2)
{
    float EPSILON = 0.000001;

    float a, dx, dy, d, h, rx, ry;
    float point2_x, point2_y;

    /* dx and dy are the vertical and horizontal distances between
    * the circle centers.
    */
    dx = x1 - x0;
    dy = y1 - y0;

    /* Determine the straight-line distance between the centers. */
    d = sqrt((dy*dy) + (dx*dx));

    /* Check for solvability. */
    if (d > (r0 + r1))
    {
        /* no solution. circles do not intersect. */
        return false;
    }
    if (d < abs(r0 - r1))
    {
        /* no solution. one circle is contained in the other */
        return false;
    }

    /* 'point 2' is the point where the line through the circle
    * intersection points crosses the line between the circle
    * centers.
    */

    /* Determine the distance from point 0 to point 2. */
    a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;

    /* Determine the coordinates of point 2. */
    point2_x = x0 + (dx * a/d);
    point2_y = y0 + (dy * a/d);

    /* Determine the distance from point 2 to either of the
    * intersection points.
    */
    h = sqrt((r0*r0) - (a*a));

    /* Now determine the offsets of the intersection points from
    * point 2.
    */
    rx = -dy * (h/d);
    ry = dx * (h/d);

    /* Determine the absolute intersection points. */
    float intersectionPoint1_x = point2_x + rx;
    float intersectionPoint2_x = point2_x - rx;
    float intersectionPoint1_y = point2_y + ry;
    float intersectionPoint2_y = point2_y - ry;

    //Log.d("INTERSECTION Circle1 AND Circle2:", "(" + intersectionPoint1_x + "," + intersectionPoint1_y + ")" + " AND (" + intersectionPoint2_x + "," + intersectionPoint2_y + ")");

    /* Lets determine if circle 3 intersects at either of the above intersection points. */
    dx = intersectionPoint1_x - x2;
    dy = intersectionPoint1_y - y2;
    float d1 = sqrt((dy*dy) + (dx*dx));

    dx = intersectionPoint2_x - x2;
    dy = intersectionPoint2_y - y2;
    float d2 = sqrt((dy*dy) + (dx*dx));

    if(abs(d1 - r2) < EPSILON) {
        //Log.d("INTERSECTION Circle1 AND Circle2 AND Circle3:", "(" + intersectionPoint1_x + "," + intersectionPoint1_y + ")");
    }
    else if(abs(d2 - r2) < EPSILON) {
        //Log.d("INTERSECTION Circle1 AND Circle2 AND Circle3:", "(" + intersectionPoint2_x + "," + intersectionPoint2_y + ")"); //here was an error
    }
    else {
        //Log.d("INTERSECTION Circle1 AND Circle2 AND Circle3:", "NONE");
    }
    return true;
}

enum ModeOfProcessing
{
    ALIGN,
    DEFORM,
    ALIGN_AND_DEFORM,
    // ... e.t.
};

struct STrajectory
{
    std::vector<std::vector<double>> points;
    std::vector<int> number_of_points;
};

class CSprings
{
public:
    int first_point;
    int second_point;
    double length;

    CSprings() {}
};

struct SAreasOfModel
{
    std::vector<std::vector<std::vector<int>>> AreasDef;
    std::vector<std::vector<std::vector<int>>> AreasNotDef;
};

pcl::PolygonMesh ObjLoad(const std::string &path_file)
{
    pcl::PolygonMesh mesh;
    pcl::io::loadOBJFile(path_file, mesh);
    return mesh;
}
/*struct SPolyModel
{
    std::vector<std::vector<int>> Faces;
    std::vector<std::vector<double>> normals;
    std::vector<std::vector<double>> Points;
};*/
                                                             // -1 !!!!!!!!!!!!!!
int get_points(std::vector<int> triangle, int num_of_point) { return triangle[num_of_point]; }

std::vector<int> RemoveElementFromMas(std::vector<int> massive, int element)
{
    for (int ind = 0; ind < massive.size();)
    {
        if (massive[ind] == element)
        {
            massive.erase(massive.begin() + ind);
        }
        else
        {
            ++ind;
        }
    }
    return massive;
}

bool CheckElementContain(std::vector<int> massive, int element)
{
    for(int ind = 0; ind < massive.size(); ind++)
    {
        if(element == massive[ind])
        {
            //std::cout << "Contained" << std::endl;
            return true; //элемент есть
        }
    }
    return false;
}

bool CheckElementContain(std::vector<double> massive, double element)
{
    for(int ind = 0; ind < massive.size(); ind++)
    {
        if(element == massive[ind])
        {
            //std::cout << "Contained" << std::endl;
            return true; //элемент есть
        }
    }
    return false;
}

double DoubleRand(double _max, double _min) { return _min + double(rand()) / RAND_MAX * (_max - _min); } //random number in range (_min; _max)

std::vector<int> GetPointsFromTwoDimensionalMas(std::vector<std::vector<int>> TwoDimMas)
{
    std::vector<int> OneDimMas = {};
    for(int i = 0; i < TwoDimMas.size(); i++)
    {
        for(int k = 0; k < 3; k++)
        {
            if (CheckElementContain(OneDimMas, TwoDimMas[i][k]) == false)
            {
                OneDimMas.push_back(TwoDimMas[i][k]);
            }
        }
    }
    return OneDimMas;
}

// generation random points inside triangles
std::vector<std::vector<double>> sampling(SPolyModel &model, int number_of_sampling)
{
    // pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

    std::vector<float> SquareOfFaces;
    std::vector<float> RateOfFaces;
    std::vector<std::vector<double>> SamplePoints;

    float semi_perimetr = 0;
    float square;

    int density = 0;
    int N = 0; // количество точек в наименьшем треугольнике
    float minSquare = 0;

    int PointsInTriangle = 0;
    double a = 0;
    double b = 0;

    //std::cout << "size of Faces =" << model.Faces.size() << std::endl;
    // std::cout << "size of Points =" << model.Points.size() << std::endl;

    for(int i = 0; i < model.Faces.size(); i++)  //расчет площадей треугольников
    {
        // std::cout << model.Faces[i][0] << " " << model.Faces[i][1] << " " << model.Faces[i][2] << std::endl;

        /*double Ax = model.Points[model.Faces[i][0] - 1][0];
        double Ay = model.Points[model.Faces[i][0] - 1][1];
        double Az = model.Points[model.Faces[i][0] - 1][2];

        double Bx = model.Points[model.Faces[i][1] - 1][0];
        double By = model.Points[model.Faces[i][1] - 1][1];
        double Bz = model.Points[model.Faces[i][1] - 1][2];

        double Cx = model.Points[model.Faces[i][2] - 1][0];
        double Cy = model.Points[model.Faces[i][2] - 1][1];
        double Cz = model.Points[model.Faces[i][2] - 1][2];*/

        double Ax = model.Points[model.Faces[i][0]][0];
        double Ay = model.Points[model.Faces[i][0]][1];
        double Az = model.Points[model.Faces[i][0]][2];

        double Bx = model.Points[model.Faces[i][1]][0];
        double By = model.Points[model.Faces[i][1]][1];
        double Bz = model.Points[model.Faces[i][1]][2];

        double Cx = model.Points[model.Faces[i][2]][0];
        double Cy = model.Points[model.Faces[i][2]][1];
        double Cz = model.Points[model.Faces[i][2]][2];


        //std::cout << Ax << " " << Ay << " " << Az << std::endl;
        // std::cout << Bx << " " << By << " " << Bz << std::endl;
        //std::cout << Cx << " " << Cy << " " << Cz << std::endl;

        // std::cout << model.Faces[i][0] << " " << model.Faces[i][1] << " " << model.Faces[i][2] << std::endl;


        double AB = sqrt((pow((Bx - Ax), 2) + pow((Bz - Az), 2) + pow((By - Ay), 2))); // длина вектора
        double BC = sqrt((pow((Cx - Bx), 2) + pow((Cz - Bz), 2) + pow((Cy - By), 2)));
        double AC = sqrt((pow((Cx - Ax), 2) + pow((Cz - Az), 2) + pow((Cy - Ay), 2)));

        // std::cout << AB << " " << BC << " " << AC << std::endl;

        semi_perimetr = (AB + BC + AC) / 2;
        square = sqrt((semi_perimetr - AB) * (semi_perimetr - AC) * (semi_perimetr - BC) * semi_perimetr);

        // std::cout << "square = " << square << std::endl;
        if(isnan(square))
        {
            square = 0.1;
        }

        SquareOfFaces.push_back(square);
    }
    // std::cout << "SquareOfFaces SIZE = " << SquareOfFaces.size() << std::endl;

    // minSquare = *std::min_element(SquareOfFaces.begin(), SquareOfFaces.end());
    // minSquare = *std::min_element(std::begin(SquareOfFaces), std::end(SquareOfFaces));

    for(int i = 0; i < SquareOfFaces.size(); i++)
    {
        minSquare = *std::min(SquareOfFaces.begin(), SquareOfFaces.end());

        // std::cout << "minSquare =" << minSquare << std::endl;

        /*if(isnan(SquareOfFaces[i]))
        {
            //std::cout << "SquareOfFaces[i] = " << SquareOfFaces[i] << std::endl;
            SquareOfFaces[i] = 0.1;
            //break;
        }*/


        RateOfFaces.push_back(round(SquareOfFaces[i] / minSquare));
        N += round(SquareOfFaces[i] / minSquare);
        //std::cout << "SquareOfFaces[i] = " << SquareOfFaces[i] << std::endl
        //         << "N = " << N << std::endl;

        //if(i == (SquareOfFaces.size() - 1))
        if(i == (SquareOfFaces.size() - 1))
        {
            density = number_of_sampling / N;
            //std::cout << "density =" << density << std::endl;
        }
    }

    for(int i = 0; i < model.Faces.size(); i++)
    {
        PointsInTriangle = static_cast<int>(RateOfFaces[i] * density);

        for(int k = 0; k < PointsInTriangle; k++)
        {
            a = DoubleRand(0,1);
            b = DoubleRand(0,1);

            if((a + b) >= 1)
            {
                a = 1 - a;
                b = 1 - b;
            }

            // int A_0 = model.Faces[i][0]-1;
            // int B_0 = model.Faces[i][1]-1;
            // int C_0 = model.Faces[i][2]-1;

            int A_0 = model.Faces[i][0];
            int B_0 = model.Faces[i][1];
            int C_0 = model.Faces[i][2];



        // расчет координат случайной точки внутри треугольника

            double point_x = model.Points[A_0][0] + a * (model.Points[B_0][0] - model.Points[A_0][0]) + b * (model.Points[C_0][0] - model.Points[A_0][0]);
            double point_y = model.Points[A_0][1] + a * (model.Points[B_0][1] - model.Points[A_0][1]) + b * (model.Points[C_0][1] - model.Points[A_0][1]);
            double point_z = model.Points[A_0][2] + a * (model.Points[B_0][2] - model.Points[A_0][2]) + b * (model.Points[C_0][2] - model.Points[A_0][2]);

            /*pcl::PointXYZ sample_point;

            sample_point._PointXYZ::x = point_x;
            sample_point._PointXYZ::y = point_y;
            sample_point._PointXYZ::z = point_z;

            cloud->push_back(sample_point);
            */

            SamplePoints.push_back({point_x, point_y, point_z});
        }
    }
    //pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;

    //kdtree.setInputCloud(cloud);
    std::cout << "Size of SamplePointsCloud: " << SamplePoints.size() << std::endl;


    return SamplePoints;
}

float AngleBetweenVectors(std::vector<double> vector1, std::vector<double> vector2)
{
    float nom = vector1[0] * vector2[0] + vector1[1] * vector2[1] + vector1[2] * vector2[2];
    float denom = sqrt(pow(vector1[0], 2) + pow(vector1[1], 2) +
            pow(vector1[2], 2)) * sqrt(pow(vector2[0], 2) + pow(vector2[1], 2) + pow(vector2[2], 2));

    //std::cout << "denom = " << denom << std::endl;

    float angle = acos(nom/denom);

    return angle; //, angle * 180 / math.pi
}

double VectorLen(std::vector<double> vector) { return sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2)); }

float VectorLen(std::vector<float> vector) { return sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2)); }

std::vector<CSprings> SpringLengthCalc(SPolyModel &model)
{
    std::vector<CSprings> SpringsLength;
    CSprings Spring;
    int A, B, C;
    double Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz;
    for(int i = 0; i < model.Faces.size(); i++)
    {
        A = model.Faces[i][0];
        B = model.Faces[i][1];
        C = model.Faces[i][2];

        Ax = model.Points[A][0];
        Ay = model.Points[A][1];
        Az = model.Points[A][2];

        Bx = model.Points[B][0];
        By = model.Points[B][1];
        Bz = model.Points[B][2];

        Cx = model.Points[C][0];
        Cy = model.Points[C][1];
        Cz = model.Points[C][2];

        double AB = sqrt((pow((Bx - Ax), 2) + pow((Bz - Az), 2) + pow((By - Ay), 2))); // длина вектора
        double BC = sqrt((pow((Cx - Bx), 2) + pow((Cz - Bz), 2) + pow((Cy - By), 2)));
        double AC = sqrt((pow((Cx - Ax), 2) + pow((Cz - Az), 2) + pow((Cy - Ay), 2)));

        Spring.first_point = A;
        Spring.second_point = B;
        Spring.length = AB;

        SpringsLength.push_back(Spring);

        Spring.first_point = B;
        Spring.second_point = C;
        Spring.length = BC;

        SpringsLength.push_back(Spring);

        Spring.first_point = A;
        Spring.second_point = C;
        Spring.length = AC;

        SpringsLength.push_back(Spring);
    }
    return SpringsLength;
}

double getLen (int point_1, int point_2, std::vector<CSprings> SpringsLength)
{
    for(int i = 0; i < SpringsLength.size(); i++)
    {
        if(point_1 == SpringsLength[i].first_point && point_2 == SpringsLength[i].second_point)
        {
            return SpringsLength[i].length;
        }
    }
}

std::vector<int> FindBorder (std::vector<std::vector<int>> NotDefPlane, std::vector<std::vector<int>> DefPlane)
{
    std::vector<int> border = {};
    for(int i = 0; i < NotDefPlane.size(); i++)
    {
        for(int j = 0; j < DefPlane.size(); j++)
        {
            for(int k = 0; k < 3; k++)
            {
                if(NotDefPlane[i][k] == DefPlane[j][0] || NotDefPlane[i][k] == DefPlane[j][1] || NotDefPlane[i][k] == DefPlane[j][2])
                {
                    if(CheckElementContain(border, NotDefPlane[i][k]) == false)
                    {
                        border.push_back(NotDefPlane[i][k]);
                    }
                }
            }
        }
    }
    return border;
}

SPolyModel ICP(SPolyModel *reference_model, SPolyModel *aligned_model)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr reference_cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr aligned_cloud (new pcl::PointCloud<pcl::PointXYZ>);

    pcl::PointCloud<pcl::PointXYZ> output_cloud;
    SPolyModel output_model;

    // Generate REFERENCE pointcloud data
    reference_cloud->width = (*reference_model).Points.size();
    reference_cloud->height = 1;
    reference_cloud->points.resize(reference_cloud->width * reference_cloud->height);

    std::cout << "---------------------" << std::endl
              << "REFERENCE: " << std::endl
              << "Points model size = " << (*reference_model).Points.size() << std::endl
              << "Points cloud size = " << reference_cloud->size() << std::endl;

    for (std::size_t i = 0; i < reference_cloud->size(); ++i)
    {
      (*reference_cloud)[i].x = (*reference_model).Points[i][0];
      (*reference_cloud)[i].y = (*reference_model).Points[i][1];
      (*reference_cloud)[i].z = (*reference_model).Points[i][2];
    }

    // Generate ALIGNED pointcloud data
    aligned_cloud->width = (*aligned_model).Points.size();
    aligned_cloud->height = 1;
    aligned_cloud->points.resize(aligned_cloud->width * aligned_cloud->height);

    std::cout << "---------------------" << std::endl
              << "ALIGNED: " << std::endl
              << "Points model size = " << (*aligned_model).Points.size() << std::endl
              << "Points cloud size = " << aligned_cloud->size() << std::endl;

    for (std::size_t i = 0; i < aligned_cloud->size(); ++i)
    {
      (*aligned_cloud)[i].x = (*aligned_model).Points[i][0];
      (*aligned_cloud)[i].y = (*aligned_model).Points[i][1];
      (*aligned_cloud)[i].z = (*aligned_model).Points[i][2];
    }


    pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
    // Устанавливаем максимальное расстояние соответствия
    icp.setMaxCorrespondenceDistance(1000);
    // Устанавливаем максимальное количество итераций (критерий 1)
    icp.setMaximumIterations(20000);
    // Устанавливаем преобразование эпсилон (критерий 2)
    icp.setTransformationEpsilon(1e-8);
    // Устанавливаем эпсилон евклидовой разности расстояний (критерий 3)
    icp.setEuclideanFitnessEpsilon(1e-8);



    // Set the input source and target
    icp.setInputCloud(aligned_cloud);
    icp.setInputTarget(reference_cloud);

    icp.align(output_cloud);



    for(std::size_t i = 0; i < output_cloud.size(); i++)
    {
        output_model.Points.push_back({output_cloud[i].x, output_cloud[i].y, output_cloud[i].z});
    }

    output_model.Faces = (*aligned_model).Faces;

    std::cout << "---------------------" << std::endl
              << "FINAL ICP: " << std::endl
              << "Output cloud size = " << output_cloud.size() << std::endl
              << "Output model points size = " << output_model.Points.size() << std::endl
              << "Output model faces size = " << output_model.Faces.size() << std::endl;

    std::cout << "has converged:" << icp.hasConverged() << " score: " <<
    icp.getFitnessScore() << std::endl;
    //Output the final transformation matrix (4x4)
    std::cout << icp.getFinalTransformation() << std::endl;

    return output_model;

}

pcl::KdTreeFLANN<pcl::PointXYZ> KDTreeCreate(std::vector<std::vector<double>> sample_points_on_def)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr SamplePointsCloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::KdTreeFLANN<pcl::PointXYZ> KDTree;

    SamplePointsCloud->width = sample_points_on_def.size();
    SamplePointsCloud->height = 1;
    SamplePointsCloud->points.resize(SamplePointsCloud->width * SamplePointsCloud->height);

    for (std::size_t i = 0; i < SamplePointsCloud->size(); ++i)
    {
        (*SamplePointsCloud)[i].x = sample_points_on_def[i][0];
        (*SamplePointsCloud)[i].y = sample_points_on_def[i][1];
        (*SamplePointsCloud)[i].z = sample_points_on_def[i][2];
    }
    //std::cout << "Size of SamplePointsCloud: " << SamplePointsCloud->size() << std::endl;

    KDTree.setInputCloud(SamplePointsCloud);
    return KDTree;
}

float getError(SPolyModel cad_model,
               std::vector<int> OneDimMassivePoint,
               pcl::KdTreeFLANN<pcl::PointXYZ> &KDTree_POLY)
{
    pcl::PointXYZ searchPoint;
    std::vector<int> pointIdxNKNSearch(1);
    std::vector<float> pointNKNSquaredDistance(1);

    std::vector<double> err = {};
    std::vector<double> errVector;

    for (int i = 0; i < OneDimMassivePoint.size(); i++)
    {
        searchPoint.x = cad_model.Points[OneDimMassivePoint[i]][0];
        searchPoint.y = cad_model.Points[OneDimMassivePoint[i]][1];
        searchPoint.z = cad_model.Points[OneDimMassivePoint[i]][2];

        if(KDTree_POLY.nearestKSearch(searchPoint, 1, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
        {

        }

        /*errVector = {poly_model.Points[i][0] - cad_model.Points[i][0],
                     poly_model.Points[i][1] - cad_model.Points[i][1],
                     poly_model.Points[i][2] - cad_model.Points[i][2]};

        err.push_back(VectorLen(errVector));*/
        err.push_back(pointNKNSquaredDistance[0]);
    }

    return *std::max_element(err.begin(), err.end());
}

SAreasOfModel SearchDeformation(pcl::KdTreeFLANN<pcl::PointXYZ> &KDTree_SCAN, SPolyModel &cad_model)
{
    float eps = 0.2;
    int area_num = 500;

    //init variable
    SAreasOfModel areas;
    std::vector<std::vector<int>> SubAreasDef;
    std::vector<std::vector<int>> SubAreasNotDef;

    std::vector<std::vector<int>> PartOfArea;
    std::vector<std::vector<int>> FacesCAD_copy = cad_model.Faces;

    int number = 0;

    int k = 0;
    int SizeAfterErase = 0;

    float x_center, y_center, z_center; // координаты центра i-го треугольника
    float x_center_tri, y_center_tri, z_center_tri;

    int A, B, C;
    int ATri, BTri, CTri;

    pcl::PointXYZ searchPoint;
    std::vector<int> pointIdxNKNSearch(1);
    std::vector<float> pointNKNSquaredDistance(1);

    while(FacesCAD_copy.size() > 0) //for(int i = 0; i < cad_model.Faces.size(); i++)
    {
        int i = 0;
        std::vector<int> pointIdxNKNSearch(1);
        std::vector<float> pointNKNSquaredDistance(1);

        //A = get_points(FacesCAD_copy[i], 0);
        //B = get_points(FacesCAD_copy[i], 1);
        //C = get_points(FacesCAD_copy[i], 2);

        A = FacesCAD_copy[i][0];
        B = FacesCAD_copy[i][1];
        C = FacesCAD_copy[i][2];

        x_center = (cad_model.Points[A][0] + cad_model.Points[B][0] + cad_model.Points[C][0]) / 3;
        y_center = (cad_model.Points[A][1] + cad_model.Points[B][1] + cad_model.Points[C][1]) / 3;    // координаты центра i-го треугольника
        z_center = (cad_model.Points[A][2] + cad_model.Points[B][2] + cad_model.Points[C][2]) / 3;

        /*A = get_points(cad_model.Faces[i], 0) - 1;
        B = get_points(cad_model.Faces[i], 1) - 1;
        C = get_points(cad_model.Faces[i], 2) - 1;*/

        searchPoint.x = x_center;
        searchPoint.y = y_center;
        searchPoint.z = z_center;

        //int K = 1; //number of serching nearest points
        if(KDTree_SCAN.nearestKSearch(searchPoint, 1, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
        {
        }
        else
        {
            std::cout << "NOT FOUND" << std::endl;
        }

        if(pointNKNSquaredDistance[0] <= eps)
        {
            //std::cout << "pointNKNSquaredDistance[0] = " << pointNKNSquaredDistance[0] << std::endl;
            PartOfArea.clear();

            /*std::cout << std::endl
                      << "K = " << k
                      << std::endl
                      << "Triangle : ["
                      << A << ","
                      << B << ","
                      << C << "]"
                      << std::endl
                      << "Search point: [ "
                      << searchPoint.x << " ; "
                      << searchPoint.y << " ; "
                      << searchPoint.z << " ] "
                      << "Distance = " << pointNKNSquaredDistance[0] << std::endl
                      << "ADD TO NOT DEF PLANE" << std::endl;*/

            PartOfArea.push_back(cad_model.Faces[i]); // массив с областью
            SubAreasNotDef.push_back(cad_model.Faces[i]);

            FacesCAD_copy.erase(FacesCAD_copy.begin() + i); // + i ??????????????????

            SizeAfterErase = FacesCAD_copy.size();
            number = 1;

            // std::cout << "SizeAfterErase = " << SizeAfterErase << std::endl;

            while(true)
            {
                k = 0;
                while(SizeAfterErase > k)
                {
                    for(int m = 0; m < 3; m++)
                    {
                        //if(A == FacesCAD_copy[k][m] - 1 || B == FacesCAD_copy[k][m] - 1 || C == FacesCAD_copy[k][m] - 1)
                        if(A == FacesCAD_copy[k][m] || B == FacesCAD_copy[k][m] || C == FacesCAD_copy[k][m])
                        {

                            //ATri = get_points(FacesCAD_copy[k], 0) - 1;
                            //BTri = get_points(FacesCAD_copy[k], 1) - 1;
                            //CTri = get_points(FacesCAD_copy[k], 2) - 1;

                            ATri = get_points(FacesCAD_copy[k], 0);
                            BTri = get_points(FacesCAD_copy[k], 1);
                            CTri = get_points(FacesCAD_copy[k], 2);

                            x_center_tri = (cad_model.Points[ATri][0] + cad_model.Points[BTri][0] + cad_model.Points[CTri][0]) / 3;
                            y_center_tri = (cad_model.Points[ATri][1] + cad_model.Points[BTri][1] + cad_model.Points[CTri][1]) / 3;    // координаты центра i-го треугольника
                            z_center_tri = (cad_model.Points[ATri][2] + cad_model.Points[BTri][2] + cad_model.Points[CTri][2]) / 3;

                            searchPoint.x = x_center_tri;
                            searchPoint.y = y_center_tri;
                            searchPoint.z = z_center_tri;

                            if(KDTree_SCAN.nearestKSearch(searchPoint, 1, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
                            {
                            }
                            else
                            {
                                std::cout << "NOT FOUND" << std::endl;
                            }

                            if(pointNKNSquaredDistance[0] <= eps)
                            {
                                /*std::cout << std::endl
                                          << "K = " << k
                                          << std::endl
                                          << "Triangle : ["
                                          << ATri << ","
                                          << BTri << ","
                                          << CTri << "]"
                                          << std::endl
                                          << "Search point: [ "
                                          << searchPoint.x << " ; "
                                          << searchPoint.y << " ; "
                                          << searchPoint.z << " ] "
                                          << "Distance = " << pointNKNSquaredDistance[0] << std::endl
                                          << "ADD TO NOT DEF PLANE" << std::endl;*/


                                PartOfArea.push_back(FacesCAD_copy[k]);
                                SubAreasNotDef.push_back(FacesCAD_copy[k]);

                                FacesCAD_copy.erase(FacesCAD_copy.begin() + k);
                                //leight -= 1
                                SizeAfterErase = FacesCAD_copy.size();
                                // std::cout << "SizeAfterErase = " << SizeAfterErase << std::endl;

                                if (PartOfArea.size() >= area_num)
                                {
                                    break;
                                }
                            }
                        }
                        if(k >= SizeAfterErase)
                        {
                            break;
                        }
                    }
                    k += 1;
                    if (PartOfArea.size() >= area_num)
                    {
                        break;
                    }
                }

                // -----------------------------------------------------------------------------------------------------
                // После прохода цикла получаем массив с соседними треугольниками к исходному
                // Далее необходимо последовательно искать соседей для каждого треугольника из полученного массива
                if(PartOfArea.size() >= area_num || number >= PartOfArea.size())
                {
                    break;
                }
                //A = get_points(PartOfArea[number], 0) - 1;
                //B = get_points(PartOfArea[number], 1) - 1;
                //C = get_points(PartOfArea[number], 2) - 1;

                A = get_points(PartOfArea[number], 0);
                B = get_points(PartOfArea[number], 1);
                C = get_points(PartOfArea[number], 2);

                number += 1;
            }

            areas.AreasNotDef.push_back(PartOfArea);
        }
        else
        {
            //std::cout << "pointNKNSquaredDistance[0] = " << pointNKNSquaredDistance[0] << std::endl;
            PartOfArea.clear();

            /*std::cout << std::endl
                      << "K = " << k
                      << std::endl
                      << "Triangle : ["
                      << A << ","
                      << B << ","
                      << C << "]"
                      << std::endl
                      << "Search point: [ "
                      << searchPoint.x << " ; "
                      << searchPoint.y << " ; "
                      << searchPoint.z << " ] "
                      << "Distance = " << pointNKNSquaredDistance[0] << std::endl
                      << "ADD TO DEF PLANE" << std::endl;*/

            PartOfArea.push_back(cad_model.Faces[i]); // массив с областью
            SubAreasDef.push_back(cad_model.Faces[i]);

            FacesCAD_copy.erase(FacesCAD_copy.begin() + i); // + i ??????????????????

            SizeAfterErase = FacesCAD_copy.size();
            number = 1;

            // std::cout << "SizeAfterErase = " << SizeAfterErase << std::endl;

            while(true)
            {
                k = 0;
                while(SizeAfterErase > k)
                {
                    for(int m = 0; m < 3; m++)
                    {
                        //if(A == FacesCAD_copy[k][m] - 1 || B == FacesCAD_copy[k][m] - 1 || C == FacesCAD_copy[k][m] - 1)
                        if(A == FacesCAD_copy[k][m] || B == FacesCAD_copy[k][m] || C == FacesCAD_copy[k][m])
                        {
                            // ATri = get_points(FacesCAD_copy[k], 0) - 1;
                            // BTri = get_points(FacesCAD_copy[k], 1) - 1;
                            // CTri = get_points(FacesCAD_copy[k], 2) - 1;

                            ATri = get_points(FacesCAD_copy[k], 0);
                            BTri = get_points(FacesCAD_copy[k], 1);
                            CTri = get_points(FacesCAD_copy[k], 2);

                            x_center_tri = (cad_model.Points[ATri][0] + cad_model.Points[BTri][0] + cad_model.Points[CTri][0]) / 3;
                            y_center_tri = (cad_model.Points[ATri][1] + cad_model.Points[BTri][1] + cad_model.Points[CTri][1]) / 3;    // координаты центра i-го треугольника
                            z_center_tri = (cad_model.Points[ATri][2] + cad_model.Points[BTri][2] + cad_model.Points[CTri][2]) / 3;

                            searchPoint.x = x_center_tri;
                            searchPoint.y = y_center_tri;
                            searchPoint.z = z_center_tri;

                            if(KDTree_SCAN.nearestKSearch(searchPoint, 1, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
                            {
                            }
                            else
                            {
                                std::cout << "NOT FOUND" << std::endl;
                            }

                            if(pointNKNSquaredDistance[0] > eps)
                            {
                                /*std::cout << std::endl
                                          << "K = " << k
                                          << std::endl
                                          << "Triangle : ["
                                          << ATri << ","
                                          << BTri << ","
                                          << CTri << "]"
                                          << std::endl
                                          << "Search point: [ "
                                          << searchPoint.x << " ; "
                                          << searchPoint.y << " ; "
                                          << searchPoint.z << " ] "
                                          << "Distance = " << pointNKNSquaredDistance[0] << std::endl
                                          << "ADD TO DEF PLANE" << std::endl;*/

                                PartOfArea.push_back(FacesCAD_copy[k]);
                                SubAreasDef.push_back(FacesCAD_copy[k]);

                                FacesCAD_copy.erase(FacesCAD_copy.begin() + k);
                                //leight -= 1
                                SizeAfterErase = FacesCAD_copy.size();
                                // std::cout << "SizeAfterErase = " << SizeAfterErase << std::endl;

                                if (PartOfArea.size() >= area_num)
                                {
                                    break;
                                }
                            }
                        }
                        if(k >= SizeAfterErase)
                        {
                            break;
                        }
                    }
                    k += 1;
                    if (PartOfArea.size() >= area_num)
                    {
                        break;
                    }
                }

                // -----------------------------------------------------------------------------------------------------
                // После прохода цикла получаем массив с соседними треугольниками к исходному
                // Далее необходимо последовательно искать соседей для каждого треугольника из полученного массива
                if(PartOfArea.size() >= area_num || number >= PartOfArea.size())
                {
                    break;
                }
                // A = get_points(PartOfArea[number], 0) - 1;
                // B = get_points(PartOfArea[number], 1) - 1;
                // C = get_points(PartOfArea[number], 2) - 1;

                A = get_points(PartOfArea[number], 0);
                B = get_points(PartOfArea[number], 1);
                C = get_points(PartOfArea[number], 2);

                number += 1;
            }
            if(PartOfArea.size() <= 8)
            {
                areas.AreasNotDef.push_back(PartOfArea);
            }
            else
            {
                areas.AreasDef.push_back(PartOfArea);
            }
            //areas.AreasDef.push_back(PartOfArea);
        }
        // leight = len(tri_copy)
        //i++;

    }
    //std::cout << "pointNKNSquaredDistance SIZE = " << pointNKNSquaredDistance.size() << std::endl;

    return areas;
}
std::vector<float> GetLenOfSprings(std::vector<int> OneDimDefPlane, SPolyModel &cad_model)
{
    std::vector<std::vector<int>> dots;
    std::vector<float> MassiveLength;
    std::vector<double> vSpring;

    for(int i = 0; i < OneDimDefPlane.size(); i++)
    {
        for (int k = 0; k < cad_model.Faces.size(); k++)
        {
            if(CheckElementContain(cad_model.Faces[k], OneDimDefPlane[i]) == true)
            {
                dots.push_back(cad_model.Faces[k]);
            }
        }
        std::vector<int> dotsOneDim = GetPointsFromTwoDimensionalMas(dots);
        dotsOneDim = RemoveElementFromMas(dotsOneDim, OneDimDefPlane[i]);

        for(int m = 0; m < dotsOneDim.size(); m++)
        {
            //int P = dotsOneDim[m] - 1;
            int P = dotsOneDim[m];
            //std::cout << "P = " << P << " : " << cad_model.Points[P][0] << " ; " << cad_model.Points[P][1] << " ; " << cad_model.Points[P][2] << std::endl;

            /*vSpring = {cad_model.Points[P][0] - cad_model.Points[OneDimDefPlane[i] - 1][0],
                       cad_model.Points[P][1] - cad_model.Points[OneDimDefPlane[i] - 1][1],
                       cad_model.Points[P][2] - cad_model.Points[OneDimDefPlane[i] - 1][2]};*/

            vSpring = {cad_model.Points[P][0] - cad_model.Points[OneDimDefPlane[i]][0],
                       cad_model.Points[P][1] - cad_model.Points[OneDimDefPlane[i]][1],
                       cad_model.Points[P][2] - cad_model.Points[OneDimDefPlane[i]][2]};

            MassiveLength.push_back(VectorLen(vSpring));
        }
    }
    return MassiveLength;
}

//OneDimMassivePoint - massive DefPlane, without dublicate, one-dimentional
float GoMassSpringSystem(float coef_elast,
                          float damping,
                          float mass,
                          int iter_number,
                          std::vector<int> BorderOfAreas,
                          std::vector<CSprings> SpringsLength,
                          std::vector<int> OneDimMassivePoint,
                          SPolyModel &poly_model,
                          SPolyModel &cad_model,
                          std::vector<std::vector<double>> sample_points_on_def,
                          pcl::KdTreeFLANN<pcl::PointXYZ> &KDTree_POLY)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cad (new pcl::PointCloud<pcl::PointXYZ>);

    // Generate pointcloud data
    cloud_cad->width = cad_model.Points.size();
    cloud_cad->height = 1;
    cloud_cad->points.resize(cloud_cad->width * cloud_cad->height);

    for (std::size_t i = 0; i < cloud_cad->size(); ++i)
    {
      (*cloud_cad)[i].x = cad_model.Points[i][0];
      (*cloud_cad)[i].y = cad_model.Points[i][1];
      (*cloud_cad)[i].z = cad_model.Points[i][2];
    }

    //pcl::KdTreeFLANN<pcl::PointXYZ> KDTree_SCAN = KDTreeCreate(sample_points_on_def);


    float step = 0.1;
    float err = 0;
    double RefLen = 0;

    for(int q = 0; q < iter_number; q++)
    {
        //std::cout << "iter: " << q << std::endl;

        //init of variable

        int trigger = 0;
        float Gravity = 0.0;
        std::vector<double> ForceVector(3);
        std::vector<std::vector<double>> MassiveVelocity = {};

        float SpringForce = 0.0;

        float dampingForceX = 0.0;
        float dampingForceY = 0.0;
        float dampingForceZ = 0.0;

        float ForceX = 0.0;
        float ForceY = 0.0;
        float ForceZ = 0.0;

        float AccelX = 0.0;
        float AccelY = 0.0;
        float AccelZ = 0.0;

        float velocityX = 0.0;
        float velocityY = 0.0;
        float velocityZ = 0.0;

        std::vector<double> Ox = {1, 0, 0};
        std::vector<double> Oy = {0, 1, 0};
        std::vector<double> Oz = {0, 0, 1};

        float alpha = 0.0;
        float beta = 0.0;
        float gamma = 0.0;

        float angleX = 0.0;
        float angleY = 0.0;
        float angleZ = 0.0;

        // OneDimMassivePoint - одномерный массив без повторяющихся жлементов
        for(int i = 0; i < OneDimMassivePoint.size(); i++)
        {
            std::vector<std::vector<int>> dots;

            for(int k = 0; k < cad_model.Faces.size(); k++)
            {
                if(CheckElementContain(cad_model.Faces[k], OneDimMassivePoint[i]) == true)
                {
                    dots.push_back(cad_model.Faces[k]);
                }
            }

            std::vector<int> dotsOneDim = GetPointsFromTwoDimensionalMas(dots);

            //dotsOneDim = RemoveElementFromMas(dotsOneDim, OneDimMassivePoint[i] - 1);
            dotsOneDim = RemoveElementFromMas(dotsOneDim, OneDimMassivePoint[i]);

            pcl::PointXYZ searchPoint;
            std::vector<int> pointIdxNKNSearch(1);
            std::vector<float> pointNKNSquaredDistance(1);

            //searchPoint.x = cad_model.Points[i][0];
            //searchPoint.y = cad_model.Points[i][1];
            //searchPoint.z = cad_model.Points[i][2];

            /*searchPoint.x = cad_model.Points[OneDimMassivePoint[i] - 1][0];
            searchPoint.y = cad_model.Points[OneDimMassivePoint[i] - 1][1];
            searchPoint.z = cad_model.Points[OneDimMassivePoint[i] - 1][2];*/

            searchPoint.x = cad_model.Points[OneDimMassivePoint[i]][0];
            searchPoint.y = cad_model.Points[OneDimMassivePoint[i]][1];
            searchPoint.z = cad_model.Points[OneDimMassivePoint[i]][2];


            //int K = 1; //number of serching nearest points
            if(KDTree_POLY.nearestKSearch(searchPoint, 1, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
            {
                // std::cout << "OK" << std::endl;

            }
            /*ForceVector = {sample_points_on_def[pointIdxNKNSearch[0]][0] - cad_model.Points[OneDimMassivePoint[i] - 1][0],
                           sample_points_on_def[pointIdxNKNSearch[0]][1] - cad_model.Points[OneDimMassivePoint[i] - 1][1],
                           sample_points_on_def[pointIdxNKNSearch[0]][2] - cad_model.Points[OneDimMassivePoint[i] - 1][2]};*/

            ForceVector = {sample_points_on_def[pointIdxNKNSearch[0]][0] - cad_model.Points[OneDimMassivePoint[i]][0],
                           sample_points_on_def[pointIdxNKNSearch[0]][1] - cad_model.Points[OneDimMassivePoint[i]][1],
                           sample_points_on_def[pointIdxNKNSearch[0]][2] - cad_model.Points[OneDimMassivePoint[i]][2]};



            alpha = AngleBetweenVectors(ForceVector, Ox);
            beta = AngleBetweenVectors(ForceVector, Oy);
            gamma = AngleBetweenVectors(ForceVector, Oz);

            Gravity = VectorLen(ForceVector);
            //std::cout << "Gravity = " << Gravity << std::endl;

            for (int j = 0; j < dotsOneDim.size(); j++)
            {
                //std::cout << "-------------------------------------------" << std::endl;
                //std::cout << " j = " << j << std::endl;
                //std::cout << "-------------------------------------------" << std::endl;
                //int t = dotsOneDim[j] - 1;
                int t = dotsOneDim[j];
                /*std::vector<double> vector1 = {cad_model.Points[t][0] - cad_model.Points[OneDimMassivePoint[i] - 1][0],
                                               cad_model.Points[t][1] - cad_model.Points[OneDimMassivePoint[i] - 1][1],
                                               cad_model.Points[t][2] - cad_model.Points[OneDimMassivePoint[i] - 1][2]};*/

                std::vector<double> vector1 = {cad_model.Points[t][0] - cad_model.Points[OneDimMassivePoint[i]][0],
                                               cad_model.Points[t][1] - cad_model.Points[OneDimMassivePoint[i]][1],
                                               cad_model.Points[t][2] - cad_model.Points[OneDimMassivePoint[i]][2]};

                //std::cout << "vector1 = " << vector1[0] << " ; " << vector1[1] << " ; " << vector1[2] << std::endl;

                /*if(VectorLen(vector1) != 0)
                {
                    angleX = AngleBetweenVectors(vector1, Ox);
                    angleY = AngleBetweenVectors(vector1, Oy);
                    angleZ = AngleBetweenVectors(vector1, Oz);
                }
                else
                {
                    angleX = 0;
                    angleY = 0;
                    angleZ = 0;
                }*/
                angleX = AngleBetweenVectors(vector1, Ox);
                angleY = AngleBetweenVectors(vector1, Oy);
                angleZ = AngleBetweenVectors(vector1, Oz);

                //std::cout << std::endl;
                //std::cout << "angleX = " << angleX << std::endl;
                //std::cout << "angleY = " << angleY << std::endl;
                //std::cout << "angleZ = " << angleZ << std::endl;

                double ACLenChange = VectorLen(vector1);
                RefLen = getLen(t, OneDimMassivePoint[i], SpringsLength);

                SpringForce = - coef_elast * (ACLenChange - RefLen);


                // SpringForce = - coef_elast * (ACLenChange - MassiveOfLength[trigger]);
                //std::cout << "SpringForce = " << SpringForce << std::endl;

                dampingForceX = damping * velocityX;
                dampingForceY = damping * velocityY;
                dampingForceZ = damping * velocityZ;

                //std::cout << std::endl;
                //std::cout << "dampingForceX = " << dampingForceX << std::endl;
                //std::cout << "dampingForceY = " << dampingForceY << std::endl;
                //std::cout << "dampingForceZ = " << dampingForceZ << std::endl;

                if(CheckElementContain(BorderOfAreas, OneDimMassivePoint[i]))
                {
                    ForceX = - SpringForce * cos(angleX) - dampingForceX;
                    ForceY = - SpringForce * cos(angleY) - dampingForceY;
                    ForceZ = - SpringForce * cos(angleZ) - dampingForceZ;

                    AccelX = 0;
                    AccelY = 0;
                    AccelZ = 0;
                }
                else
                {

                    /*
                    ForceX = - SpringForce * cos(angleX) - dampingForceX - mass * Gravity * cos(PI - alpha);
                    ForceY = - SpringForce * cos(angleY) - dampingForceY - mass * Gravity * cos(PI - beta);
                    ForceZ = - SpringForce * cos(angleZ) - dampingForceZ - mass * Gravity * cos(PI - gamma);*/

                    ForceX = - SpringForce * cos(angleX) - dampingForceX + mass * Gravity * cos(alpha);
                    ForceY = - SpringForce * cos(angleY) - dampingForceY + mass * Gravity * cos(beta);
                    ForceZ = - SpringForce * cos(angleZ) - dampingForceZ + mass * Gravity * cos(gamma);



                    //std::cout << std::endl;
                    //std::cout << "ForceX = " << ForceX << std::endl;
                    //std::cout << "ForceY = " << ForceY << std::endl;
                    //std::cout << "ForceZ = " << ForceZ << std::endl;

                    AccelX = ForceX / mass;
                    AccelY = ForceY / mass;
                    AccelZ = ForceZ / mass;
                }
                velocityX += AccelX * step;
                velocityY += AccelY * step;
                velocityZ += AccelZ * step;

                /*std::cout << std::endl;
                std::cout << "velocityX = " << velocityX << std::endl;
                std::cout << "velocityY = " << velocityY << std::endl;
                std::cout << "velocityZ = " << velocityZ << std::endl;*/



                trigger += 1;
            }
            MassiveVelocity.push_back({velocityX, velocityY, velocityZ});
        }
        for(int i = 0; i < MassiveVelocity.size(); i++)
        {
            // cad_model.Points[OneDimMassivePoint[i] - 1][0] += MassiveVelocity[i][0] * step;
            // cad_model.Points[OneDimMassivePoint[i] - 1][1] += MassiveVelocity[i][1] * step;
            // cad_model.Points[OneDimMassivePoint[i] - 1][2] += MassiveVelocity[i][2] * step;

            cad_model.Points[OneDimMassivePoint[i]][0] += MassiveVelocity[i][0] * step;
            cad_model.Points[OneDimMassivePoint[i]][1] += MassiveVelocity[i][1] * step;
            cad_model.Points[OneDimMassivePoint[i]][2] += MassiveVelocity[i][2] * step;
        }
        err = getError(cad_model, OneDimMassivePoint, KDTree_POLY);
        if(err < 0.1)
        {
            damping -= 0.1;
        }
        if(err < 0.07)
        {
            break;
        }
        std::cout << "ERROR = " << err << std::endl;
    }
    return err;
}

STrajectory TrajectoryForm(SPolyModel &cad_model)
{
    pcl::PointXYZ searchPoint;
    std::vector<int> pointIdxNKNSearch(1);
    std::vector<float> pointNKNSquaredDistance(1);

    STrajectory trajectory;

    trajectory.points = {{24.07287979, -321.95117188, 224.89033508},
                         {51.07153320, -422.08224487, 218.18055725},
                         {36.86752319, -370.20080566, 191.59786987},
                         {36.01466370, -367.03659058, 191.90055847},
                         {34.33252716, -360.77999878, 193.04223633},
                         {32.69847870, -354.67919922, 194.88606262},
                         {31.13062096, -348.80950928, 197.40956116},
                         {29.64940834, -343.23974609, 200.58293152},
                         {28.27103043, -338.03863525, 204.36885071},
                         {27.01373100, -333.26736450, 208.72071838},
                         {25.89177513, -328.98468018, 213.58755493},
                         {24.91815186, -325.24029541, 218.90965271},
                         {24.10600662, -322.08099365, 224.62217712},
                         {42.80951309, -392.12142944, 194.50663757},
                         {41.14205933, -385.99343872, 192.78691101},
                         {44.42217255, -398.02938843, 196.91119385},
                         {45.96153259, -403.64541626, 199.97236633},
                         {47.40824509, -408.90402222, 203.65190125},
                         {48.74514008, -413.74090576, 207.90803528},
                         {49.95595551, -418.09777832, 212.68664551},
                         {51.02629089, -421.92163086, 217.93116760},
                         {39.44017029, -379.72061157, 191.77275085},
                         {37.72423172, -373.37634277, 191.47581482}};

    /*trajectory.points = {{70.94300079, -322.01214600, 216.83119202},
                                                   {90.28488922, -424.10604858, 215.10180664},
                                                   {80.81139374, -372.51358032, 185.97201538},
                                                   {80.19844818, -369.28320313, 186.11680603},
                                                   {78.97671509, -362.87243652, 186.94645691},
                                                   {77.77224731, -356.59060669, 188.48506165},
                                                   {76.59949493, -350.51287842, 190.71423340},
                                                   {75.47260284, -344.71261597, 193.60784912},
                                                   {74.40514374, -339.25976563, 197.13166809},
                                                   {73.40995789, -334.21966553, 201.24151611},
                                                   {72.49916077, -329.65411377, 205.89044189},
                                                   {71.68341064, -325.61611938, 211.02041626},
                                                   {70.97280884, -322.15582275, 216.56971741},
                                                   {84.96577454, -394.67321777, 189.96623230},
                                                   {83.82101440, -388.51821899, 187.94424438},
                                                   {86.05587769, -400.57519531, 192.66159058},
                                                   {87.07814789, -406.15264893, 195.99853516},
                                                   {88.02039337, -411.33917236, 199.93521118},
                                                   {88.87101746, -416.07095337, 204.42657471},
                                                   {89.62012482, -420.29296875, 209.41668701},
                                                   {90.25848389, -423.95297241, 214.84536743},
                                                   {82.63540649, -382.18429565, 186.61935425},
                                                   {81.42311859, -375.74728394, 186.00729370}};*/

    pcl::KdTreeFLANN<pcl::PointXYZ> KDTree_CAD = KDTreeCreate(cad_model.Points);

    for(int i = 0; i < trajectory.points.size(); i++)
    {
        searchPoint.x = trajectory.points[i][0];
        searchPoint.y = trajectory.points[i][1];
        searchPoint.z = trajectory.points[i][2];

        if(KDTree_CAD.nearestKSearch(searchPoint, 1, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
        {
            std::cout << "Distance = " << pointNKNSquaredDistance[0] << std::endl;
            trajectory.number_of_points.push_back(pointIdxNKNSearch[0]);
        }
    }
    return trajectory;
}

float CalcTrajectoryError(STrajectory &trajectory, SPolyModel &poly_model,
                          SPolyModel &cad_model, int number_sampling)
{
    float traj_error = 0;
    float traj_error_0 = 0;
    pcl::PointXYZ searchPoint;
    std::vector<int> pointIdxNKNSearch(1);
    std::vector<float> pointNKNSquaredDistance(1);

    std::vector<float> distances;
    std::vector<int> index;

    std::vector<std::vector<double>> sample_points_scan = sampling(poly_model, number_sampling);

    // create K-d Tree
    pcl::KdTreeFLANN<pcl::PointXYZ> KDTree_scan = KDTreeCreate(sample_points_scan);

    for(int i = 0; i < trajectory.number_of_points.size(); i++)
    {
        searchPoint.x = cad_model.Points[trajectory.number_of_points[i]][0]; //x
        searchPoint.y = cad_model.Points[trajectory.number_of_points[i]][1]; //x
        searchPoint.z = cad_model.Points[trajectory.number_of_points[i]][2]; //x

        if(KDTree_scan.nearestKSearch(searchPoint, 1, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
        {
            index.push_back(pointIdxNKNSearch[0]);
            distances.push_back(pointNKNSquaredDistance[0]);

            //std::cout << "OK,  dist = " << pointNKNSquaredDistance[0] << std::endl;
        }
    }
    for(int i = 0; i < distances.size(); i++)
    {
        traj_error_0 += distances[i] * distances[i];
    }

    traj_error = sqrt(traj_error_0 / distances.size());

    std::cout << "OK,  traj_error = " << traj_error << std::endl;




    std::ofstream out2;
    out2.open("/home/ros-industrial/Desktop/trajectory_after_correct.txt");//, std::ios::app);
    for (int i = 0; i < index.size(); i++)
    {
        out2  << sample_points_scan[index[i]][0] << " "
              << sample_points_scan[index[i]][1] << " "
              << sample_points_scan[index[i]][2] << std::endl;
    }
    out2 << "" << std::endl;
    out2 << "" << std::endl;
    for (int i = 0; i < trajectory.number_of_points.size(); i++)
    {
        out2  << trajectory.number_of_points[i] << std::endl;
    }





    return traj_error;
}

float DeformationsControl(SPolyModel &poly_model,
                          SPolyModel &cad_model,
                          int number_sampling,
                          float coef_elast,
                          float damping,
                          float mass,
                          int iter_number)
{

    float error_of_correct;

    // sample points on deformed surface
    std::vector<std::vector<double>> SAMPLE_POINTS_POLY = sampling(poly_model, number_sampling);

    // create K-d Tree
    pcl::KdTreeFLANN<pcl::PointXYZ> KDTree_POLY = KDTreeCreate(SAMPLE_POINTS_POLY);

    // search deformed areas
    struct SAreasOfModel Areas = SearchDeformation(KDTree_POLY, cad_model);

    std::vector<std::vector<int>> DefPlane;               // массив с треугольниками деформированной части плоскости (без объединения в области)
    std::vector<std::vector<int>> NotDefPlane;            // массив с треугольниками НЕдеформированной части плоскости (без объединения в области)

    for (int i = 0; i < Areas.AreasDef.size(); i++)
    {
        for (int k = 0; k < Areas.AreasDef[i].size(); k++)
        {
            DefPlane.push_back(Areas.AreasDef[i][k]);
        }

    }
    for (int i = 0; i < Areas.AreasNotDef.size(); i++)
    {
        for (int k = 0; k < Areas.AreasNotDef[i].size(); k++)
        {
            NotDefPlane.push_back(Areas.AreasNotDef[i][k]);
        }

    }
    std::cout << "Size of DefPlane =" << DefPlane.size() << std::endl;
    std::cout << "Size of NotDefPlane =" << NotDefPlane.size() << std::endl;

    //Search points in Border
    std::vector<int> Border = FindBorder(NotDefPlane, DefPlane);
    Border = {};

    //Transfer points of DefPlane to One dimentional massive
    std::vector<int> OneDimMassivePoint = GetPointsFromTwoDimensionalMas(DefPlane);

    // Calculation lenghts of Springs
    std::vector<CSprings> SpringsLength = SpringLengthCalc(cad_model);

    // Begin transfer of trajectory
    error_of_correct = GoMassSpringSystem(coef_elast, damping, mass, iter_number, Border, SpringsLength,
                                          OneDimMassivePoint, poly_model, cad_model, SAMPLE_POINTS_POLY, KDTree_POLY);

    return error_of_correct;
}

int test(SPolyModel &model)
{
    std::vector<CSprings> SpringsLength = SpringLengthCalc(model);

    for(int i = 0; i < SpringsLength.size(); i++)
    {
        std::cout << "P1 = "
                  << SpringsLength[i].first_point
                  << std::endl
                  << "P2 = "
                  << SpringsLength[i].second_point
                  << std::endl
                  << "Len = "
                  << SpringsLength[i].length
                  << std::endl;

        std::cout << "Get len with function = "
                  << getLen(SpringsLength[i].first_point, SpringsLength[i].second_point, SpringsLength)
                  << std::endl;
    }


    return 0;
}

void MakeNoiseModel(struct SPolyModel &model)
{
    double a = 0;
    double b = 0;

    const int range_from  = 0;
    const int range_to    = 1;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::normal_distribution<double>  distr{0, 0.1};



    for (int i = 0; i < model.Points.size(); i++)
    {
        /*a = DoubleRand(-1.0,1.0);
        b = DoubleRand(0.0,0.1);

        if((a + b) >= 1)
        {
            a = 1 - a;
            b = 1 - b;
        }
        a = a/5;*/

        //std::cout << distr(generator) << '\n';

        a = -1;


        //model.Points[i][0] += a;
        //model.Points[i][1] += a;
        while(a < 0)
        {
           a = distr(generator);

        }
        model.Points[i][0] += distr(generator);
        model.Points[i][1] += distr(generator);
        model.Points[i][2] += a;
    }
}

void LinkTrajectoryToModel(SPolyModel &model, std::vector<std::vector<float>> traj)
{
    std::vector<double> PointP(3);
    double eps = 0.05;

    std::vector<double> VectorPA(3);
    std::vector<double> VectorPB(3);
    std::vector<double> VectorPC(3);

    double angle = 0;

    int A = 0;
    int B = 0;
    int C = 0;

    for(int i = 0; i < traj.size(); i++)
    {
        std::cout << "i = " << i << std::endl;
        PointP = {traj[i][0], traj[i][1], traj[i][2]};

        for (int k = 0; k < model.Faces.size(); k++)
        {
            std::cout << "k = " << k << std::endl;

            A = model.Faces[k][0];
            B = model.Faces[k][1];
            C = model.Faces[k][2];

            VectorPA = {model.Points[A][0] - PointP[0], model.Points[A][1] - PointP[1], model.Points[A][2] - PointP[2]};
            VectorPB = {model.Points[B][0] - PointP[0], model.Points[B][1] - PointP[1], model.Points[B][2] - PointP[2]};
            VectorPC = {model.Points[C][0] - PointP[0], model.Points[C][1] - PointP[1], model.Points[C][2] - PointP[2]};

            double ang1 = AngleBetweenVectors(VectorPA, VectorPB);
            double ang2 = AngleBetweenVectors(VectorPA, VectorPC);
            double ang3 = AngleBetweenVectors(VectorPB, VectorPC);

            std::cout << "Sum of angle = " << ang1 + ang2 + ang3 << std::endl;

            if((ang1 + ang2 + ang3) >= (2*PI - eps) && (ang1 + ang2 + ang3) <= (2*PI + eps))
            {
                std::cout << "Point in triangle" << std::endl;


                model.Faces.erase(model.Faces.begin() + k);



                int new_point = model.Points.size(); //edit

                model.Points.push_back(PointP);

                model.Faces.push_back({A, B, new_point});
                model.Faces.push_back({A, new_point, C});
                model.Faces.push_back({new_point, B, C});

                break;

            }
            else
            {
                std::cout << "Point NOT in triangle" << std::endl;
            }
        }
    }
    return;
}
