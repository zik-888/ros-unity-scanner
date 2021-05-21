using MeshSystem.MeshAddition;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace MeshSystem.Surfaces
{
    public class Surface
    {
        public int MeshId { private set; get; }
        public int Id { private set; get; }

        public List<int> Triangles = new List<int>();

        private CMesh GetMesh => CMeshPool.GetMesh(MeshId);

        public Surface
            (int meshId, 
            int id, 
            int startTriangle, 
            float angle = 20f)
        {
            this.MeshId = meshId;
            this.Id = id;

            // создаём очередь
            Queue<int> queueOfTriangles = new Queue<int>();
            
            //добавляем в очередь стартовый триангл
            queueOfTriangles.Enqueue(startTriangle);
            
            while (queueOfTriangles.Count() != 0)
            {
                // добавляем в буфер соседей первого в очереди
                var triangleBuf = queueOfTriangles.Dequeue();
                
                var linkedTriangleBuf = GetMesh
                    .Triangles[triangleBuf]
                    .GetLinkedTriangleWhithCommonSurface();
                
                Triangles.Add(triangleBuf);
                // перебираем буфер
                foreach (var b in linkedTriangleBuf)
                {
                    // если в конечном 
                    if(!queueOfTriangles.Contains(b) && 
                       !Triangles.Contains(b))
                        queueOfTriangles.Enqueue(b);
                }
            }
        }

        public int[] GetVertices()
        {
            var edges = from tIndex in Triangles
                        from edge in CMeshPool.GetMesh(MeshId).Triangles[tIndex].Edges
                        select edge;

            edges = edges.AsParallel()
                    .GroupBy(x => x, new EdgeComparer())
                    .Where(g => g.Count() == 1)
                    .Select(g => g.Key);

            return edges.Select(e => e.v0.Index).Union(edges.Select(e => e.v1.Index)).Distinct().ToArray();
        }

        public AnchorPoint[] GetAnchorPoints()
        {
            // получаем все эджи плоскости
            var edges = from tIndex in Triangles
                from edge in CMeshPool.GetMesh(MeshId).Triangles[tIndex].Edges
                select edge;

            // фильтруем, получаем только эджи по периметру
            edges = edges.AsParallel()
                .GroupBy(x => x, new EdgeComparer())
                .Where(g => g.Count() == 1)
                .Select(g => g.Key);

            var edgeArray = edges.ToArray();
            var edgeCount = edgeArray.Count();

            List<AnchorPoint> anchorPoints = new List<AnchorPoint>();
            
            for (int i = 0; i < edgeCount; i++)
            {
                var firstEdge = edgeArray[i];
                var secondEdge = edgeArray.First(e => Vector3Comparer.StaticEquals(firstEdge.v1.Position, e.v0.Position) || 
                                                      Vector3Comparer.StaticEquals(firstEdge.v1.Position, e.v1.Position));
                
                anchorPoints.Add(new AnchorPoint()
                {
                    VertexId = firstEdge.v1.Index,
                    Edges = new [] { firstEdge, secondEdge }
                });
            }
            
            return anchorPoints.ToArray();
        }

        public void SetColorDebuging()
        {
            var color = new Color(UnityEngine.Random.value,
                                  UnityEngine.Random.value, 
                                  UnityEngine.Random.value, 1);

            var edges = from tIndex in Triangles
                        from edge in CMeshPool.GetMesh(MeshId).Triangles[tIndex].Edges
                        select edge;

            edges = edges.AsParallel()
                    .GroupBy(x => x, new EdgeComparer())
                    .Where(g => g.Count() == 1)
                    .Select(g => g.Key);

            foreach (var a in edges)
            {
                Debug.DrawLine(a.v0.Position, a.v1.Position, color, 300f);
            }
        }

        public override string ToString()
        {
            string str = "";

            foreach (var a in Triangles)
            {
                str += a + " ";
            }

            return str;
        }
    }


    public struct AnchorPoint
    {
        public int VertexId;
        public Edge[] Edges;

        public Vector3 GetPosition()
        {
            return CMeshPool.GetMesh(0).Vertices[VertexId];
        }
    }
}
