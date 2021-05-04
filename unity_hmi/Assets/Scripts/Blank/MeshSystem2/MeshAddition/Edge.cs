using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace MeshSystem.MeshAddition
{
    public struct Edge
    {
        public Vertex v0;
        public Vertex v1;

        public Vertex[] Vertices
        {
            get { return new Vertex[] { v0, v1 }; }
        }

        private int _meshId;
        public int MeshId
        {
            get { return _meshId; }
            private set { _meshId = value; }
        }

        private int _triangleIndex;
        public int TriangleIndex
        {
            get { return _triangleIndex; }
            private set { _triangleIndex = value; }
        }

        public Edge(int meshId, int triangleIndex, int v0Index, int v1Index)
        {
            _meshId = meshId;
            _triangleIndex = triangleIndex;

            v0 = new Vertex(meshId, v0Index);
            v1 = new Vertex(meshId, v1Index);
        }


    }


    class EdgeComparer : IEqualityComparer<Edge>
    {
        public bool Equals(Edge edge1, Edge edge2)
        {
            if (edge1.v0.Position == edge2.v0.Position && edge1.v1.Position == edge2.v1.Position
             || edge1.v1.Position == edge2.v0.Position && edge1.v0.Position == edge2.v1.Position)
                return true;
            else
                return false;
        }

        public int GetHashCode(Edge edge)
        {
            unchecked
            {
                var hashCode = -731044709;
                hashCode = hashCode * -1521134295 + EqualityComparer<int>.Default.GetHashCode((int)(edge.v0.Position.x * 100))
                                                  + EqualityComparer<int>.Default.GetHashCode((int)(edge.v0.Position.y * 100))
                                                  + EqualityComparer<int>.Default.GetHashCode((int)(edge.v0.Position.z * 100))
                                                  + EqualityComparer<int>.Default.GetHashCode((int)(edge.v1.Position.x * 100))
                                                  + EqualityComparer<int>.Default.GetHashCode((int)(edge.v1.Position.y * 100))
                                                  + EqualityComparer<int>.Default.GetHashCode((int)(edge.v1.Position.z * 100));
                return hashCode;
            }

        }
    }

}
