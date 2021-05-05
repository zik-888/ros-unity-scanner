using System.Collections;
using System.Collections.Generic;
using RosSharp;
using UnityEngine;

[System.Serializable]
public class CommandPose : FormingPoint
{
    public CommandPose(Vector3 position, Vector3 rotation)
    {
        this.Position = position;
        this.Rotation = rotation;
    }

    public override string ToString()
    {
        return $"Pos: {Position}, rot: {Rotation}";
    }
}

public abstract class FormingPoint
{
    public Vector3 Position;
    public Vector3 Rotation;
    
    public Vector3 RosPosition 
    { 
        set => Position = TransformExtensions.Ros2Unity(value);
        get => TransformExtensions.Unity2Ros(Position);
    }
    
    public Vector3 RosRotation
    {
        set => Rotation = TransformExtensions.Ros2Unity(value);
        get => TransformExtensions.Unity2Ros(Rotation);
    }
}
