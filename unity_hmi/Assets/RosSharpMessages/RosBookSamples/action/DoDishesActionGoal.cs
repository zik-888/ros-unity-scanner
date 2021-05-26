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

namespace RosSharp.RosBridgeClient.MessageTypes.RosBookSamples
{
    public class DoDishesActionGoal : ActionGoal<DoDishesGoal>
    {
        public const string RosMessageName = "ros_book_samples/DoDishesActionGoal";

        public DoDishesActionGoal() : base()
        {
            this.goal = new DoDishesGoal();
        }

        public DoDishesActionGoal(Header header, GoalID goal_id, DoDishesGoal goal) : base(header, goal_id)
        {
            this.goal = goal;
        }
    }
}