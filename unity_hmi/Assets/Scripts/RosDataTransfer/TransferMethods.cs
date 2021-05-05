using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using RosSharp.RosBridgeClient.MessageTypes.Shape;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using Vector3 = UnityEngine.Vector3;

public class TransferMethods
{
    public static MeshTriangle[] TraingleConvert(int[] triangles)
    {
        var meshTriangleArray = new MeshTriangle[triangles.Length / 3];

        meshTriangleArray = meshTriangleArray
                            .Select((t, i) => new MeshTriangle(new uint[]
                            {
                                (uint)triangles[i * 3],
                                (uint)triangles[i * 3 + 1],
                                (uint)triangles[i * 3 + 2]

                            })).ToArray();

        return meshTriangleArray;
    }

    public static int[] TraingleConvert(MeshTriangle[] triangles)
    {
        int lenth = triangles.Length;
        int[] triangle = new int[lenth * 3];

        for (int i = 0; lenth > i; i++)
        {
            triangle[i * 3] = (int)triangles[i].vertex_indices[0];
            triangle[i * 3 + 1] = (int)triangles[i].vertex_indices[1];
            triangle[i * 3 + 2] = (int)triangles[i].vertex_indices[2];
        }

        return triangle;
    }

    public static Point[] VertexConvert(Vector3[] vertices)
    {
        var pointArray = vertices.Select(v => RosSharp.TransformExtensions.Unity2Ros(v))
                                 .Select(p => new Point(p.x, p.y, p.z)).ToArray();

        return pointArray;
    }

    public static Vector3[] VertexConvert(Point[] pointArray)
    {
        var vertices = pointArray.Select(p => RosSharp.TransformExtensions.Ros2Unity(new Vector3((float)p.x, (float)p.y, (float)p.z)));

        return vertices.ToArray();
    }
    
    
}


