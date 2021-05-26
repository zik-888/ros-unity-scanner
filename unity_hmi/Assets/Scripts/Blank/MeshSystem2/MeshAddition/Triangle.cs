using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace MeshSystem.MeshAddition
{
    public struct Triangle
    {
        private int _index;
        public int Index
        {
            get { return _index; }
            //set
            //{
            //    _index = value;
            //    if (_edges != null)
            //    {
            //        _edges[0].TriangleIndex = value;
            //        _edges[1].TriangleIndex = value;
            //        _edges[2].TriangleIndex = value;
            //    }
            //}
        }

        private int _meshId;
        public int MeshId
        {
            get { return _meshId; }
            private set { _meshId = value; }
        }

        public Vector3[] GetVertexPosition => new Vector3[]
        {
            Edges[0].v0.Position,
            Edges[1].v0.Position,
            Edges[2].v0.Position
        };

        public int[] GetVertexIndeces
            => new int[] {Edges[0].v0.Index, Edges[1].v0.Index, Edges[2].v0.Index};

        private Edge[] _edges;
        public Edge[] Edges
        {
            get { return _edges; }
        }

        public int[] linkedTriangle;

        public Triangle(int meshId, int index, int v0, int v1, int v2)
        {
            _index = index;
            _meshId = meshId;

            var edges = new Edge[3];
            edges[0] = new Edge(meshId, index, v0, v1);
            edges[1] = new Edge(meshId, index, v1, v2);
            edges[2] = new Edge(meshId, index, v2, v0);

            linkedTriangle = new int[3];

            _edges = edges;
        }

        /// <summary>
        /// Возвращает вектор нормали к плоскости треугольника
        /// </summary>
        public Vector3 GetNormal()
        {
            Vector3 point1 = _edges[0].v0.Position;
            Vector3 point2 = _edges[1].v0.Position;
            Vector3 point3 = _edges[2].v0.Position;

            Vector3 planeParam = Vector3.Cross(point2 - point1, point3 - point1).normalized;

            return new Vector3(planeParam.x, planeParam.y, planeParam.z);
        }

        /// <summary>
        /// Возвращает преобразование нормали
        /// в виде углов эйлера
        /// </summary>
        public Vector3 GetEulerAngle() 
        {
            var normal = GetNormal();

            var a0 = Mathf.Atan2(Mathf.Sqrt(Mathf.Pow(normal.z, 2) + 
                                            Mathf.Pow(normal.x, 2)), normal.y) * Mathf.Rad2Deg;
            var a1 = Mathf.Atan2(normal.x, normal.z) * Mathf.Rad2Deg;

            return new Vector3(a0, a1, 0);
        }



public void SetLinkedTriangle()
{
    var triangleBuf = GetVertexPosition;

    var linkedTriangle = CMeshPool
        .GetMesh(MeshId).Triangles
        .Where(t => IsOneEdgeCommon(triangleBuf, t.GetVertexPosition))
        .Select(t => t.Index)
        .ToArray();

    if (linkedTriangle.Length != 3)
        throw new Exception("Mesh is not loop");

    this.linkedTriangle = linkedTriangle;
}

public static bool IsOneEdgeCommon(in Vector3[] thisTriangle, in Vector3[] anotherTriangle)
{
    var countIntersection = thisTriangle.Intersect(anotherTriangle).Count();
    return countIntersection == 2;
}

public int[] GetLinkedTriangleWhithCommonSurface(float angle = 20f)
{
    int mesh = MeshId;
    Vector3 normal = GetNormal();

    var a = linkedTriangle.Where
    (
        i => Vector3.
        Angle(CMeshPool
            .GetMesh(mesh)
            .Triangles[i]
            .GetNormal(), normal) < angle
    );

    return a.ToArray();
}


        
    }
}
