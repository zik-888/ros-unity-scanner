/* 
 * This message is auto generated by ROS#. Please DO NOT modify.
 * Note:
 * - Comments from the original code will be written in their own line 
 * - Variable sized arrays will be initialized to array of size 0 
 * Please report any issues at 
 * <https://github.com/siemens/ros-sharp> 
 */

using RosSharp.RosBridgeClient.MessageTypes.Std;
using RosSharp.RosBridgeClient.MessageTypes.Actionlib;

namespace RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore
{
    public class ProcessingActionResult : ActionResult<ProcessingResult>
    {
        public const string RosMessageName = "ros_scanner_interface/ProcessingActionResult";

        public ProcessingActionResult() : base()
        {
            this.result = new ProcessingResult();
        }

        public ProcessingActionResult(Header header, GoalStatus status, ProcessingResult result) : base(header, status)
        {
            this.result = result;
        }
    }
}
