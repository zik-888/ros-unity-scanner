#!/usr/bin/env python

__version__ = "0.1"

'''
This is a python script to start parallel servo node for controlling joint 2. It creates another node for parallel servo. The names of the 
controller for the arm are the dual servo joint 2 are dummy_joint2_controller and dummy_joint8_controller. 
The command is sent to /joint2_controller/command. This value is published to dummy_joint2_controller/command and 
dummy_joint8_controller/command simultaneously.
'''


import rospy
import roslib
from dynamixel_msgs.msg import JointState
from std_msgs.msg import Float64


parallel_servo_1 = rospy.Publisher('/joint2_dummy_controller/command',Float64)

def callback(data):
    curr_pos = data.data
    parallel_servo_1.publish(curr_pos)
   
def listener():
    rospy.init_node('parallel_servo_starter', anonymous=True)
    print "parallel_servo_starter node started"
    rospy.Subscriber("/joint2_controller/command",Float64, callback)
    rospy.spin()


if __name__ == '__main__':
    listener()
