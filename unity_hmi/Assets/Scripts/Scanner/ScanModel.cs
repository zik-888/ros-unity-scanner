using RosSharp.RosBridgeClient.MessageTypes.Moveit;
using UniRx;

namespace Scanner
{
    public class ScannerModel
    {
        public ReactiveProperty<bool> DisplayScanArea = new ReactiveProperty<bool>(false);
        public ReactiveProperty<ScanArea> ScanArea = new ReactiveProperty<ScanArea>(new ScanArea());
    }
}