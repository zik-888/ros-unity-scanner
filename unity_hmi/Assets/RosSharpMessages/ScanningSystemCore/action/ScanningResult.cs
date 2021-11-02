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
    public class ScanningResult : Message
    {
        public const string RosMessageName = "ros_scanner_interface/ScanningResult";

        // result definition
        public MeshTriangle[] triangles { get; set; }
        public Point[] vertices { get; set; }

        public ScanningResult()
        {
            this.triangles = new MeshTriangle[0];
            this.vertices = new Point[0];
        }

        public ScanningResult(MeshTriangle[] triangles, Point[] vertices)
        {
            this.triangles = triangles;
            this.vertices = vertices;
        }
    }
}
