using Assets.Scripts.Command.Marching;
using MeshSystem;
using MeshSystem.MeshAddition;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class MarchingMethod
{
    /// <summary>
    /// Marching method с определением плоскостей
    /// </summary>
    /// <param name="meshNum"> Mesh id </param>
    /// <param name="pointArray"> 3 points </param>
    public static CommandPose[] GetPositions(MarchingFormingPoint[] referencePoints, int meshNum = 0)
    {
        Vector3[] pointArray = referencePoints.Select(p => p.Position).ToArray();

        Plane marchPlane = new Plane(pointArray[0], pointArray[1], pointArray[2]);

        Vector3 midpoint = (pointArray[0] + pointArray[2]) / 2;

        var marchingPoints = from T in CMeshPool.GetMesh(meshNum).Triangles
                             from E in T.Edges
                             let e = Vector3.Dot(marchPlane.normal, E.v0.Position - E.v1.Position)
                             where e != 0
                             let o = new
                             {
                                 position = E.v1.Position + (E.v0.Position - E.v1.Position)
                                            * Vector3.Dot(marchPlane.normal, pointArray[0] - E.v1.Position) / e,
                                 edge = E
                             }
                             where Vector3.Dot(E.v1.Position - o.position, E.v0.Position - o.position) <= 0
                             select o;

        ///////
        ///    новая версия

        var marchNodeArray = marchingPoints.GroupBy(p => p.edge, new EdgeComparer())
                                           .Select(p => new MarchingNode(p.First().position,
                                                                      p.Select(g => g.edge).ToArray()))
                                           .ToArray();

        Debug.DrawLine(pointArray[0], pointArray[1], Color.black, 1000f);
        Debug.DrawLine(pointArray[1], pointArray[2], Color.black, 1000f);

        Debug.DrawLine(pointArray[0], pointArray[2], Color.blue, 1000f);
        Debug.DrawLine(midpoint, midpoint + marchPlane.normal, Color.blue, 1000f);

        CommandPose[] sortPoints;

        try
        {
            sortPoints = SortingMarchNode(marchNodeArray, referencePoints.Select(p => p.triangleIndex).ToArray(),
                                                          referencePoints.Select(p => (FormingPoint)p).ToArray());
        }
        catch
        {
            sortPoints = referencePoints.Select(p => new CommandPose(p.Position, p.Rotation)).ToArray();
        }
        

        return sortPoints.Select(p => new CommandPose(p.Position, Vector3.zero)).ToArray();

        #region Test

        //Debug.DrawLine(pointArray[0], pointArray[1], Color.black, 1000f);
        //Debug.DrawLine(pointArray[1], pointArray[2], Color.black, 1000f);

        //Debug.DrawLine(pointArray[0], pointArray[2], Color.blue, 1000f);
        //Debug.DrawLine(midpoint, midpoint + marchPlane.normal, Color.blue, 1000f);

        ////Debug.DrawLine(pointArray[0], pointArray[2], Color.green);
        ////Debug.DrawLine(midpoint, midpoint + ortoMarchPlane.normal, Color.green);

        #endregion

    }

    private static CommandPose[] SortingMarchNode(in MarchingNode[] marchNodes, int[] referencePoints, FormingPoint[] p)
    {
        bool orderIsDirect = true;

        LinkedList<MarchingNode> chain = new LinkedList<MarchingNode>();

        int firstTriangleBuff;
        int lastTriangleBuff;

        var firstNode = marchNodes.First(n => n.Triangles.Any(t => t == referencePoints[1]));
        var lastNode = marchNodes.Last(n => n.Triangles.Any(t => t == referencePoints[1]));

        chain.AddFirst(firstNode);
        chain.AddLast(lastNode);

        firstTriangleBuff = firstNode.Triangles.First(t => t != referencePoints[1]);
        lastTriangleBuff = lastNode.Triangles.First(t => t != referencePoints[1]);

        var mLenght = marchNodes.Length;

        while (chain.Count < mLenght)
        {
            firstNode = marchNodes.First(n => !chain.Contains(n) &&
                                              n.Triangles.Any(t => t == firstTriangleBuff));
            firstTriangleBuff = firstNode.Triangles.First(t => !chain.First().Triangles.Contains(t));
            chain.AddFirst(firstNode);

            if (chain.First.Value.Triangles.Contains(referencePoints[0]))
            {
                orderIsDirect = true;
                break;
            }

            if (chain.First.Value.Triangles.Contains(referencePoints[2]))
            {
                orderIsDirect = false;
                break;
            }
        }

        while (chain.Count < mLenght)
        {
            lastNode = marchNodes.Last(n => !chain.Contains(n) &&
                                            n.Triangles.Any(t => t == lastTriangleBuff));
            lastTriangleBuff = lastNode.Triangles.First(t => !chain.Last().Triangles.Contains(t));
            chain.AddLast(lastNode);

            if (chain.Last.Value.Triangles.Contains(referencePoints[0]) ||
                chain.Last.Value.Triangles.Contains(referencePoints[2]))
                break;
        }

        var list = chain.Select(c => new CommandPose(c.Position, c.Normal)).ToList();

        if(orderIsDirect)
        {
            list.Insert(0, new CommandPose(p[0].Position, p[0].Rotation));
            list.Add(new CommandPose(p[2].Position, p[2].Rotation));

        }
        else
        {
            list.Insert(0, new CommandPose(p[2].Position, p[2].Rotation));
            list.Add(new CommandPose(p[0].Position, p[0].Rotation));

            list.Reverse();
        }

        return list.ToArray();
    }

}
