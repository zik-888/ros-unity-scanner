/* 
 * This message is auto generated by ROS#. Please DO NOT modify.
 * Note:
 * - Comments from the original code will be written in their own line 
 * - Variable sized arrays will be initialized to array of size 0 
 * Please report any issues at 
 * <https://github.com/siemens/ros-sharp> 
 */



namespace RosSharp.RosBridgeClient.MessageTypes.RosBookSamples
{
    public class DoDishesFeedback : Message
    {
        public const string RosMessageName = "ros_book_samples/DoDishesFeedback";

        //  Определение обратной связи (feedback)
        public uint dishes_cleaned { get; set; }
        //  Сколько вымыто посуды сейча

        public DoDishesFeedback()
        {
            this.dishes_cleaned = 0;
        }

        public DoDishesFeedback(uint dishes_cleaned)
        {
            this.dishes_cleaned = dishes_cleaned;
        }
    }
}