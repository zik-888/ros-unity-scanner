using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assets.Scripts.Command.PerformanceMethod
{
    public interface ICommandPerformance
    {
        void StartMethod(Action<FormingPoint> addPoint, Action<FormingPoint[]> finish);
        void CancelMethod();
    }
}