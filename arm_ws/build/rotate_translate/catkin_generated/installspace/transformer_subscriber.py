#!/usr/bin/env python3
import rospy
import rospy
from rotate_translate.msg import InputType, OutputType
import numpy as np
import math

pub = rospy.Publisher('transformer', OutputType, queue_size=10)
def callback(data):
    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)
    rospy.loginfo("received x: %s, y: %s, z: %s, alpha: %s, beta: %s, gamma: %s, d: %s " % (data.x, data.y, data.z, data.alpha, data.beta, data.gamma, data.d))
    
    given_vector = np.array([data.x, data.y, data.z, 1])

    rotation_along_x_matrix = np.array([ 
        [1, 0, 0, 0 ],
        [0, math.cos(data.alpha), -math.sin(data.alpha), 0],
        [0, math.sin(data.alpha), math.cos(data.alpha), 0],
        [0, 0, 0, 1] 
    ])

    rotation_along_y_matrix = np.array([ 
        [math.cos(data.beta), 0, math.sin(data.beta), 0],
        [0, 1, 0, 0 ],
        [-math.sin(data.beta), 0, math.cos(data.beta), 0],
        [0, 0, 0, 1] 
    ])

    rotation_along_z_matrix = np.array([ 
        [math.cos(data.gamma), -math.sin(data.gamma), 0, 0],
        [math.sin(data.gamma), math.cos(data.gamma), 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1 ]
    ])

    translation_matrix = np.array([
        [1, 0, 0, data.d],
        [0, 1, 0, data.d],
        [0, 0, 1, data.d],
        [0, 0, 0, 1]
    ])

    retVector = np.dot(translation_matrix, np.dot(rotation_along_z_matrix, np.dot(rotation_along_y_matrix, np.dot(rotation_along_x_matrix, given_vector))))
    
    msg = Outputmsg()
    msg.x = retVector[0]
    msg.y = retVector[1]
    msg.z = retVector[2]

    pub.publish(msg)

    
def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("transformer", InputType, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
