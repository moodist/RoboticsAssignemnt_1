#!/user/bin/env python
from __future__ import print_function
from arm_gazebo.srv import ik, ikResponse
import rospy
import numpy as np
import tinyik as IK

def handle_ik(req):
    arm = IK.Actuator([
    "z", [0.0, 0.0, req.link_length_1 + req.link_length_2],
    "x", [0.0, 0.0, req.link_length_3],
    "x", [0.0, 0.0, req.link_length_4],
    "x", [0.0, 0.0, req.link_length_5],
    "z", [0.0, 0.0, req.link_length_6],
    "x", [0.0, 0.0, req.link_length_7],
    ])

    arm.ee = [req.x, req.y, req.z]

    angles = arm.angles

    res = ikResponse()
    
    res.angle1 = angles[0]
    res.angle2 = angles[1]
    res.angle3 = angles[2]
    res.angle4 = angles[3]
    res.angle5 = angles[4]
    res.angle6 = angles[5]

    print("Returning......", angles)
    return res

def ik_server():
    rospy.init_node("inverse_kinematics_server")
    s = rospy.Service('ik', ik, handle_ik)
    rospy.spin()

if __name__ == "__main__":
    ik_server()