/* 
 * This message is auto generated by ROS#. Please DO NOT modify.
 * Note:
 * - Comments from the original code will be written in their own line 
 * - Variable sized arrays will be initialized to array of size 0 
 * Please report any issues at 
 * <https://github.com/siemens/ros-sharp> 
 */



using RosSharp.RosBridgeClient.MessageTypes.Geometry;

namespace RosSharp.RosBridgeClient.MessageTypes.MyPkg
{
    public class ScanPositionServiceResponse : Message
    {
        public const string RosMessageName = "ros_robotics_interface/ScanPositionService";

        public Pose artur_scan_pose { get; set; }

        public ScanPositionServiceResponse()
        {
            this.artur_scan_pose = new Pose();
        }

        public ScanPositionServiceResponse(Pose artur_scan_pose)
        {
            this.artur_scan_pose = artur_scan_pose;
        }
    }
}
