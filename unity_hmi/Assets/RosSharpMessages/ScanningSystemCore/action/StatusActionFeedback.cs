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
    public class StatusActionFeedback : ActionFeedback<StatusFeedback>
    {
        public const string RosMessageName = "scanning_system_core/StatusActionFeedback";

        public StatusActionFeedback() : base()
        {
            this.feedback = new StatusFeedback();
        }

        public StatusActionFeedback(Header header, GoalStatus status, StatusFeedback feedback) : base(header, status)
        {
            this.feedback = feedback;
        }
    }
}
