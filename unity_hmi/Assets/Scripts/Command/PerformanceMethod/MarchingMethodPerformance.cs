using Assets.Scripts.Command.PutPointOnSurface;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using thelab.mvc;
using UnityEngine;

namespace Assets.Scripts.Command.PerformanceMethod
{
    public class MarchingMethodPerformance : Element<DemonOLPApplication>, ICommandPerformance
    {
        protected PutThreePointOnSurface PUTMethodEventHandler;
        protected Action<FormingPoint> AddPointEventHandler;
        protected Action<FormingPoint[]> FinishEventHandler;

        protected GameObject PlaneRender;

        protected string message = "Метод марчинга: тыкни три точки";

        public void StartMethod(Action<FormingPoint> addPoint, Action<FormingPoint[]> finish)
        {
            DataLog.Log(message);

            this.AddPointEventHandler = addPoint;
            this.FinishEventHandler = finish;

            PUTMethodEventHandler = gameObject.AddComponent<PutThreePointOnSurface>();
            PUTMethodEventHandler.finishMethod = FinishMethodHandle;
            PUTMethodEventHandler.putPoint = PutPointHandle;
        }

        public void CancelMethod() => Destroy(PUTMethodEventHandler);
        protected void FinishMethodHandle(MarchingFormingPoint[] points)
        {
            ////
            // Реализация метода
            var marchPoints = MarchingMethod.GetPositions(points);
            Plane marchPlane = new Plane(points[0].Position, points[1].Position, points[2].Position);
            

            #region Plane
            
            // PlaneRender = new GameObject("PlaneRender", typeof(MeshFilter), typeof(MeshRenderer));
            //
            // Vector3[] vertex = new []{points[0].Position, points[1].Position, points[2].Position};
            // int[] triangle = new int[] { 0, 1, 2 };
            //
            // Mesh mesh = new Mesh();
            // mesh.vertices = vertex;
            // mesh.triangles = triangle;
            // mesh.RecalculateNormals();
            //
            // PlaneRender.GetComponent<MeshFilter>().mesh = mesh;

            #endregion
            
                
            ////
            FinishEventHandler(marchPoints.Select(p => (FormingPoint)p).ToArray());
        }

        protected void PutPointHandle(MarchingFormingPoint point) 
            => AddPointEventHandler(point);

        private void OnDestroy()
        {
            Destroy(PlaneRender);
        }
    }


    public static class Vector3Extension
    {
        public static Vector3 Average(this Vector3[] points)
        {
            Vector3 b = Vector3.back;
            
            for (int i = 0; points.Length > i; i++)
            {
                b += points[i];
            }
            
            b /= points.Length;

            return b;
        }
    }
}
