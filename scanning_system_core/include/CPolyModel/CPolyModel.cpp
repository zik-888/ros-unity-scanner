#ifndef CMESSAGE_H
#define CMESSAGE_H

#endif // CMESSAGE_H

#pragma once
#include <CPolyModel.h>

using namespace std;

double CPolyModel::CalcDist(std::vector<double> p1, std::vector<double> p2)
{
    return sqrt(pow(p1[0]-p2[0],2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}

vector<vector<int>> CPolyModel::GetFaces(std::vector<std::vector<std::vector<double>>> Profiles,
                                                   std::vector<std::vector<float>> RobotPosition)
{
    std::vector<std::vector<int>> Faces;
    int i = 1;

    double eps = 2;
    // double eps = 0.00000025;
    double maxDist = 6;

    ROS_INFO("Calc faces, Profiles %d, Positions %d \n", Profiles.size(), RobotPosition.size());
    // Попарный перебор всех профилей облака точек

    int currentProfileLength = 0;
    int prevProfileLength = 0;

    if(Profiles.size() == 0 || RobotPosition.size() == 0)
    {
        ROS_ERROR("No Profiles or Positions");
        return(Faces);
    }

    for (i = 1; i < Profiles.size(); i++)
    {
        std::vector<std::vector<double>> currentProfile = Profiles[i];
        std::vector<std::vector<double>> prevProfile = Profiles[i - 1];

        currentProfileLength += prevProfile.size();

        int k = 0;
        int n = 0;
        int kN = k + 1;
        int nN = n + 1;

        int currentPointsCount = currentProfile.size();
        int prevPointsCount = prevProfile.size();

        std::vector<double> currentVm = { RobotPosition[i][0], RobotPosition[i][1], RobotPosition[i][2] };
        std::vector<double> prevVm = { RobotPosition[i-1][0], RobotPosition[i-1][1], RobotPosition[i-1][2] };

           //Следующий цикл нужен для объединения всех точек
           //профилей currentProfile и prevProfile в треугольники.
           //Здесь происходит расчет дистанции между соседними точками
           //этих профилей
           //и ее сравнение с maxDist.
           //Если заданные условия if выполняются, то в коллекцию индексов
           //точек треугольников <faces> производится запись индексов
           //обрабатываемых точек

        while (kN < currentPointsCount - 1 && nN < prevPointsCount - 1)
        {
            //std::cout << k << "   " << kN << "   " << n << "   " << nN << std::endl;
            if (abs(currentProfile[k][1] - prevProfile[n][1]) < eps)   // !!!!!!!!!!!!! 1 потому что профиль по y !!!!!!!!!!!!!!!!!!
            {
                std::vector<double> sumVectorCurrent_kN = { currentProfile[kN][0] + currentVm[0], currentProfile[kN][1] + currentVm[1], currentProfile[kN][2] + currentVm[2]};
                std::vector<double> sumVectorCurrent_k = { currentProfile[k][0] + currentVm[0], currentProfile[k][1] + currentVm[1], currentProfile[k][2] + currentVm[2] };
                std::vector<double> sumVectorPrev_n = { prevProfile[n][0] + prevVm[0], prevProfile[n][1] + prevVm[1], prevProfile[n][2] + prevVm[2] };
                std::vector<double> sumVectorPrev_nN = { prevProfile[nN][0] + prevVm[0], prevProfile[nN][1] + prevVm[1], prevProfile[nN][2] + prevVm[2] };
                if (CalcDist(sumVectorCurrent_kN, sumVectorPrev_n) < maxDist)
                {
                    if (CalcDist(sumVectorCurrent_kN, sumVectorCurrent_k) < maxDist && CalcDist(sumVectorCurrent_k, sumVectorPrev_n) < maxDist)
                    {
                        int a1 = currentProfileLength + k + 1;
                        int a2 = currentProfileLength + kN + 1;
                        int a3 = prevProfileLength + n + 1;

                        std::vector<int> triangle = { a2, a1, a3 };

                        Faces.push_back(triangle);
                    }

                    if (CalcDist(sumVectorPrev_nN, sumVectorPrev_n) < maxDist && CalcDist(sumVectorPrev_nN, sumVectorCurrent_kN) < maxDist)
                    {
                        int a1 = prevProfileLength + n + 1;
                        int a2 = currentProfileLength + kN + 1;
                        int a3 = prevProfileLength + nN + 1;

                        std::vector<int> triangle2 = { a2, a1, a3 };

                        Faces.push_back(triangle2);
                    }
                }
                k += 1;
                n += 1;
                kN = k + 1;
                nN = n + 1;
            }
            else
            {
                if (currentProfile[k][1] - prevProfile[n][1] - eps < 0.0) // !!!!!!!!!!!!!!!! 1 - profile direct  y
                {
                    n += 1;
                    nN = n + 1;
                }
                else
                {
                    k += 1;
                    kN = k + 1;
                }
            }
        }
        prevProfileLength = currentProfileLength;
    }
    ROS_INFO("Faces is ready, count = %lu", Faces.size());
    return Faces;
}


