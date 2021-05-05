using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public class CircleMethod
{
    
    public static FormingPoint[] GetArc(Vector3 center, FormingPoint[] points)
    {
        FormingPoint[] formingPoints = new FormingPoint[100];

        formingPoints[0] = points[0];
        
        Vector3 position;
        Vector3 rotation;

        for (int i = 1; formingPoints.Length > i; i++)
        {

            if (i < formingPoints.Length / 2)
            {
                position = Vector3.Slerp(points[0].Position - center, 
                    points[1].Position - center, 
                    (float)i / (formingPoints.Length / 2));
        
                rotation = Vector3.Lerp(points[0].Rotation,
                    points[1].Rotation,
                    (float)i / (formingPoints.Length / 2));
            }
            else
            {
                position = Vector3.Slerp(points[1].Position - center, 
                    points[2].Position - center, 
                    (float)(i - (formingPoints.Length / 2)) / (formingPoints.Length / 2));
        
                rotation = Vector3.Lerp(points[1].Rotation,
                    points[2].Rotation,
                    (float)(i - formingPoints.Length / 2) / (formingPoints.Length / 2));
            }
            
            
            
            formingPoints[i] = new CommandPose(position + center, rotation);
        }
        

        formingPoints[formingPoints.Length - 1] = points[2];

        return formingPoints;
    }
    
    public static Vector3 GetArcCenter(Vector3 point1, Vector3 point2, Vector3 point3)
    {
        Vector3 center;

        Vector3 midl12 = (point1 + point2) / 2;
        Vector3 midl23 = (point2 + point3) / 2;

        Plane circlePlane = new Plane(point1, point2, point3);

        Vector3 upPoint1 = point1 + circlePlane.normal;
        Plane p12Plane = new Plane(upPoint1, point1, point2);
        Line m12 = new Line(midl12, p12Plane.normal + midl12);

        Vector3 upPoint3 = point3 + circlePlane.normal;
        Plane p23Plane = new Plane(upPoint3, point2, point3);
        Plane m23 = new Plane(Vector3.Cross(p23Plane.normal, circlePlane.normal), midl23);

        center = m12.p1 + (m12.p2 - m12.p1) * Vector3.Dot(m23.normal, (midl23 - m12.p1))
                                            / Vector3.Dot(m23.normal, (m12.p2 - m12.p1));

        return center;
    }

    struct Line
    {
        public Vector3 p1;
        public Vector3 p2;

        public Line(Vector3 p1, Vector3 p2)
        {
            this.p1 = p1;
            this.p2 = p2;
        }
    }
}
