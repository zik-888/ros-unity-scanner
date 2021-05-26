/* 
 * This message is auto generated by ROS#. Please DO NOT modify.
 * Note:
 * - Comments from the original code will be written in their own line 
 * - Variable sized arrays will be initialized to array of size 0 
 * Please report any issues at 
 * <https://github.com/siemens/ros-sharp> 
 */



using System;

namespace RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore { 

    public class Status : Message
    {
        public const string RosMessageName = "scanning_system_core/Status";

        public string errorcode { get; set; }
        public long status { get; set; }

        public Status()
        {
            this.errorcode = "";
            this.status = 0;
        }

        public Status(string errorcode, long status)
        {
            this.errorcode = errorcode;
            this.status = status;
        }
    }
}