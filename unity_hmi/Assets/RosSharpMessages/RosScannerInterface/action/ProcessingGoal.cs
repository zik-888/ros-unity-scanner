/* 
 * This message is auto generated by ROS#. Please DO NOT modify.
 * Note:
 * - Comments from the original code will be written in their own line 
 * - Variable sized arrays will be initialized to array of size 0 
 * Please report any issues at 
 * <https://github.com/siemens/ros-sharp> 
 */



using RosSharp.RosBridgeClient.MessageTypes.Shape;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;

namespace RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore
{
    public class ProcessingGoal : Message
    {
        public const string RosMessageName = "ros_scanner_interface/ProcessingGoal";

        // goal definition
        public MeshTriangle[] triangles_CAD { get; set; }
        public Point[] vertices_CAD { get; set; }
        public Pose[] trajectory { get; set; }
        public int mode { get; set; }
        // int32 x1

        public ProcessingGoal()
        {
            this.triangles_CAD = new MeshTriangle[0];
            this.vertices_CAD = new Point[0];
            this.trajectory = new Pose[0];
            this.mode = 0;
        }

        public ProcessingGoal(MeshTriangle[] triangles_CAD, Point[] vertices_CAD, Pose[] trajectory, int mode)
        {
            this.triangles_CAD = triangles_CAD;
            this.vertices_CAD = vertices_CAD;
            this.trajectory = trajectory;
            this.mode = mode;
        }
    }
}
