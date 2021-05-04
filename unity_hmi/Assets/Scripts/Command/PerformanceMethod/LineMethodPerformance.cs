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
    public class LineMethodPerformance : Element<DemonOLPApplication>, ICommandPerformance
    {
        protected PutManyPointOnSurface putMethod;
        protected Action<FormingPoint> addPoint;
        protected Action<FormingPoint[]> finish;
        protected LineRenderer line;

        protected List<FormingPoint> pointList = new List<FormingPoint>();

        protected string message = "Метод линии: тыкни много точек";

        public void CancelMethod()
        {
            Destroy(putMethod);
            FinishMethodHandle();
        }

        public void StartMethod(Action<FormingPoint> addPoint, Action<FormingPoint[]> finish)
        {
            DataLog.Log(message);

            this.addPoint = addPoint;
            this.finish = finish;

            putMethod = gameObject.AddComponent<PutManyPointOnSurface>();
            putMethod.putPoint = PutPointHandle;
        }

        protected void FinishMethodHandle()
        {
            //line = gameObject.AddComponent<LineRenderer>();
            //line.positionCount = pointList.Count;
            //line.SetPositions(pointList.Select(p => p.position).ToArray());


            finish(pointList.ToArray());
        }

        protected void PutPointHandle(FormingPoint point)
        {
            pointList.Add(point);
            addPoint(point);
        }
    }
}
