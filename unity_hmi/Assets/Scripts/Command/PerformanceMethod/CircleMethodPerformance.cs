using Assets.Scripts.Command.PutPointOnSurface;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using thelab.mvc;
using UnityEngine;

namespace Assets.Scripts.Command.PerformanceMethod
{
    public class CircleMethodPerformance : Element<DemonOLPApplication>, ICommandPerformance
    {
        protected PutThreePointOnSurface PUTMethod;
        protected Action<FormingPoint> AddPointEventHandler;
        protected Action<FormingPoint[]> FinishEventHandler;

        protected string message = "Метод круга: тыкни три точки";

        public void StartMethod(Action<FormingPoint> addPoint, Action<FormingPoint[]> finish)
        {
            DataLog.Log(message);

            this.AddPointEventHandler = addPoint;
            this.FinishEventHandler = finish;

            PUTMethod = gameObject.AddComponent<PutThreePointOnSurface>();
            PUTMethod.finishMethod = FinishMethodHandle;
            PUTMethod.putPoint = PutPointHandle;
        }

        public void CancelMethod() => Destroy(PUTMethod);
        protected void FinishMethodHandle(MarchingFormingPoint[] points)
        {
            var a = points.Select(p => p.Position).ToArray();

            var center = CircleMethod.GetArcCenter(a[0], a[1], a[2]);

            FinishEventHandler(CircleMethod.GetArc(center, points));
        }

        protected void PutPointHandle(MarchingFormingPoint point) 
            => AddPointEventHandler(new CommandPose(point.Position, point.Rotation));

    }
}

