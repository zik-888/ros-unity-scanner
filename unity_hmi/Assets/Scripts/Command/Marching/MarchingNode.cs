using MeshSystem;
using MeshSystem.MeshAddition;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace Assets.Scripts.Command.Marching
{
    class MarchingNode
    {
        public Vector3 Position { set; get; }
        public int[] Triangles { set; get; } = new int[2];

        public Vector3 Normal 
        { 
            get 
            {
                var angle = Vector3.Lerp(CMeshPool.GetMesh(0).Triangles[Triangles[0]].GetEulerAngle(),
                                         CMeshPool.GetMesh(0).Triangles[Triangles[1]].GetEulerAngle(), 0.5f);

                return angle;
            }
        }


        public MarchingNode(Vector3 position, Edge[] edgeArray)
        {
            Position = position;

            Triangles = edgeArray.Select(e => e.TriangleIndex).ToArray();


        }

    }
}
