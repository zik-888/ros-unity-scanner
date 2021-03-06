/* 
 * This message is auto generated by ROS#. Please DO NOT modify.
 * Note:
 * - Comments from the original code will be written in their own line 
 * - Variable sized arrays will be initialized to array of size 0 
 * Please report any issues at 
 * <https://github.com/siemens/ros-sharp> 
 */



namespace RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore
{
    public class ProcessingAction : Action<ProcessingActionGoal, ProcessingActionResult, ProcessingActionFeedback, ProcessingGoal, ProcessingResult, ProcessingFeedback>
    {
        public const string RosMessageName = "ros_scanner_interface/ProcessingAction";

        public ProcessingAction() : base()
        {
            this.action_goal = new ProcessingActionGoal();
            this.action_result = new ProcessingActionResult();
            this.action_feedback = new ProcessingActionFeedback();
        }

    }
}
