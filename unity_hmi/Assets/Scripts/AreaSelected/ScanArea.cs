using System;
using RosSharp.RosBridgeClient.MessageTypes.Rosapi;
using System.Collections;
using System.Collections.Generic;
using RosSharp;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

[Serializable]
public class ScanArea
{
    public Vector2 firstPoint;
    public Vector2 secondPoint;
    public float height;

    public Vector3 FirstGroup => new Vector3(firstPoint.x, 0, firstPoint.y);
    public Vector3 SecondGroup => new Vector3(secondPoint.x, 0, secondPoint.y);

    protected Vector2 FirstUnity2Ros => new Vector2(firstPoint.y, -firstPoint.x);
    protected Vector2 SecondUnity2Ros => new Vector2(secondPoint.y, -secondPoint.x);


    public ScanArea()
    {
        this.firstPoint = Vector2.zero;
        this.secondPoint = Vector2.zero;
        this.height = 0f;
    }

    public ScanArea(float x1, float y1, float x2, float y2)
    {
        firstPoint = new Vector2(x1, y1);
        secondPoint = new Vector2(x2, y2);
    }

    public ScanningGoal ToScanningGoal()
    {
        return new ScanningGoal(FirstUnity2Ros.x, FirstUnity2Ros.y, SecondUnity2Ros.x, SecondUnity2Ros.y);
    }
    
    public override string ToString()
    {
        return $"firstPoint: {firstPoint}, secondPoint: {secondPoint}, height: {height}";
    }

    static public Vector2 Vector3to2(Vector3 vector)
    {
        return new Vector2(vector.x, vector.z);
    }
}
