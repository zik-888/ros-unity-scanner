using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace MeshSystem.MeshAddition
{
    public struct Vertex
    {
        public int Index { private set; get; }

        private int _meshId;
        public int MeshId
        {
            get { return _meshId; }
            private set { _meshId = value; }
        }

        public Vertex(int meshId, int vIndex)
        {
            Index = vIndex;
            _meshId = meshId;
        }

        public Vector3 Position
        {
            get { return CMeshPool.GetMesh(_meshId).Vertices[Index]; }
            set { CMeshPool.GetMesh(_meshId).Vertices[Index] = value; }
        }
    }


    class Vector3Comparer : IEqualityComparer<Vector3>
    {
        public bool Equals(Vector3 vector1, Vector3 vector2)
        {
            if (Vector3.SqrMagnitude(vector1 - vector2) < 0.0001)
                return true;
            else
                return false;
        }

        public int GetHashCode(Vector3 vector)
        {
            unchecked
            {
                var hashCode = -731044709;
                hashCode = hashCode * -1521134295 + EqualityComparer<int>.Default.GetHashCode((int)(vector.x * 100))
                                                  + EqualityComparer<int>.Default.GetHashCode((int)(vector.x * 100))
                                                  + EqualityComparer<int>.Default.GetHashCode((int)(vector.x * 100));
                return hashCode;
            }
        }

        
    }
}
