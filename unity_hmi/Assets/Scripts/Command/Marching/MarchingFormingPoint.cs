using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MarchingFormingPoint : FormingPoint
{
    public int triangleIndex;

    public MarchingFormingPoint(int idTriangle, Vector3 pos, Vector3 rot)
    {
        this.triangleIndex = idTriangle;
        this.Position = pos;
        this.Rotation = rot;
    }

    public MarchingFormingPoint(int idTriangle, Vector3 pos)
    {
        this.triangleIndex = idTriangle;
        this.Position = pos;
        this.Rotation = Vector3.zero;
    }

    public override string ToString()
    {
        return $"idTriangle: {triangleIndex}, pos: {Position}";
    }
}
