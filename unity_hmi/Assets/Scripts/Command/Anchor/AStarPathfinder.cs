using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

[System.Serializable]
public class AStarPoint
{
    public string name;

    public Vector3 position;

    public AStarPoint[] neighbourPoints;
    
    public AStarPoint cameFrom;
    /// <summary>
    /// Стоимость пути от начальной вершины.
    /// </summary>
    public float g;
    /// <summary>
    /// Эвристическая оценка расстояние до цели.
    /// </summary>
    public float h;
    /// <summary>
    /// h(x) + g(x)
    /// </summary>
    public float f => g + h;
    public AStarPoint(Vector3 selfPosition, AStarPoint[] neighbourPoints)
    {
        position = selfPosition;
        this.neighbourPoints = neighbourPoints;
    }

    public override string ToString()
    {
        return $"name: {name}, pos: {position}";
    }

    public override bool Equals(object obj)
    {
        return obj is AStarPoint point &&
               position.Equals(point.position);
    }

    public override int GetHashCode()
    {
        return 1206833562 + position.GetHashCode();
    }
}

public class AStarPathfinder
{
    public static List<AStarPoint> AStarCalc(AStarPoint start, AStarPoint goal)
    {
        List<AStarPoint> closedset = new List<AStarPoint>(); // Множество вершин, которые уже были обработаны(раскрыты)
        List<AStarPoint> openset = new List<AStarPoint>(); // Множество вершин(очередь), которые предстоит обработать(раскрыть). 
        openset.Add(start);                              // Изначально здесь присутствует только начальная вершина start.

        List<AStarPoint> path_map = new List<AStarPoint>();  // Карта пройденных вершин. Используется функцией reconstruct_path

        while (openset.Count != 0)
        {
            AStarPoint currentPoint = openset.First(); //вершина из openset имеющая самую низкую оценку f(x)
            foreach (var a in openset)
                if (a.f < currentPoint.f)
                    currentPoint = a;

            if (currentPoint == goal)
            {
                reconstructPath();
                return path_map;
            }

            openset.Remove(currentPoint);
            closedset.Add(currentPoint);

            foreach (var a in currentPoint.neighbourPoints)
            {
                if (closedset.Contains(a))
                    continue;

                float tempG = currentPoint.g + Vector3.Distance(currentPoint.position, a.position);

                bool tempFlag;

                if (!openset.Contains(a))
                {
                    openset.Add(a);
                    tempFlag = true;
                }
                else
                {
                    if (tempG < a.g)
                        tempFlag = true;
                    else
                        tempFlag = false;
                }

                if (tempFlag == true)
                {
                    a.cameFrom = currentPoint; //a. можно добавить вершину из которой пришли
                    a.g = tempG;
                    a.h = Vector3.Distance(goal.position, a.position);
                }
            }
        }

        throw new Exception("Путь не найден");

        void reconstructPath()
        {
            var currentNode = goal; // Добавляем в карту все вершины от finish_node до start_node.

            while (currentNode != null)
            {
                path_map.Add(currentNode);
                currentNode = currentNode.cameFrom;
            }
        }
    }


    
}
