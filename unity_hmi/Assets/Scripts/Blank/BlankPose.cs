using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using RosSharp;

[Serializable]
public class BlankPose
{
    public Vector3 UnityPosition;
    public Vector3 UnityRotation;
    public float UnityScale;

    public Vector3 RosPosition 
    { 
        set 
        {
            UnityPosition = TransformExtensions.Ros2Unity(value);
        } 
        get 
        {
            return TransformExtensions.Unity2Ros(UnityPosition);
        } 
    }

    public Vector3 RosRotation
    {
        set
        {
            UnityRotation = TransformExtensions.Ros2Unity(value);
        }
        get
        {
            return TransformExtensions.Unity2Ros(UnityRotation);
        }
    }

    public float RosScale
    {
        set
        {
            UnityScale = TransformExtensions.Ros2UnityScale(Vector3.one * value).x;
        }
        get
        {
            return TransformExtensions.Unity2RosScale(Vector3.one * UnityScale).x;
        }
    }

    public BlankPose(BlankPose pose)
    {
        this.UnityPosition = pose.UnityPosition;
        this.UnityRotation = pose.UnityRotation;
        this.UnityScale = pose.UnityScale;
    }

    public BlankPose(Vector3 position, Vector3 rotation, float scale)
    {
        this.UnityPosition = position;
        this.UnityRotation = rotation;
        this.UnityScale = scale;
    }

    public override string ToString()
    {
        return $" p: {UnityPosition}";
    }

}
