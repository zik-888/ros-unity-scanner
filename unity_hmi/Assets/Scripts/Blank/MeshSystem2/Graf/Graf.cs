using System.Collections.Generic;
using System.Linq;
using MeshSystem.MeshAddition;
using MeshSystem.Surfaces;

namespace MeshSystem.Graf
{
    public class Graf
    {
        private List<AStarPoint> _points = new List<AStarPoint>();


        public Graf(Surface[] surfaces)
        {
            Queue<AnchorPoint> vertexesQueue = new Queue<AnchorPoint>();
            
            foreach (var s in surfaces) // заполняем очередь
            {
                var vertices = s.GetAnchorPoints();
                
                foreach (var v in vertices)
                {
                    vertexesQueue.Enqueue(v);
                }
            }

            while (vertexesQueue.Count != 0) // заполняем _points
            {
                var firstVertex = vertexesQueue.Peek();
                
                var buferAnchorPoints = vertexesQueue
                    .Where(v => Vector3Comparer.StaticEquals(firstVertex.GetPosition(), v.GetPosition()));
                
                
                //_points.Add(new AStarPoint(firstVertex.GetPosition(), buferAnchorPoints);

            }
            
        }
        
        
    }
}