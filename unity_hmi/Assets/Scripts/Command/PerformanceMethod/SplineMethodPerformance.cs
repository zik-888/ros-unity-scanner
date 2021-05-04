using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assets.Scripts.Command.PerformanceMethod
{
    public class SplineMethodPerformance : LineMethodPerformance
    {
        protected new string message = "Метод сплайна: тыкни много точек";

        protected new void FinishMethodHandle()
        {
            ////
            // Реализация метода
            ////
            finish(pointList.ToArray());
        }
    }
}
