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
    public class TrajectoryServiceRequest : Message
    {
        public const string RosMessageName = "ros_robotics_interface/TrajectoryService";

        public int motion_type { get; set; }
        public Pose[] rpose { get; set; }

        public TrajectoryServiceRequest()
        {
            this.motion_type = 0;
            this.rpose = new Pose[0];
        }

        public TrajectoryServiceRequest(int motion_type, Pose[] rpose)
        {
            this.motion_type = motion_type;
            this.rpose = rpose;
        }
    }
}