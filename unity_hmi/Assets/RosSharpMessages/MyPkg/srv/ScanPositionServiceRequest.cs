/* 
 * This message is auto generated by ROS#. Please DO NOT modify.
 * Note:
 * - Comments from the original code will be written in their own line 
 * - Variable sized arrays will be initialized to array of size 0 
 * Please report any issues at 
 * <https://github.com/siemens/ros-sharp> 
 */



namespace RosSharp.RosBridgeClient.MessageTypes.MyPkg
{
    public class ScanPositionServiceRequest : Message
    {
        public const string RosMessageName = "my_pkg/ScanPositionService";

        public string request { get; set; }

        public ScanPositionServiceRequest()
        {
            this.request = "";
        }

        public ScanPositionServiceRequest(string request)
        {
            this.request = request;
        }
    }
}
