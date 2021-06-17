rostopic pub ikPose arm_gazebo/PositionMessage "x: 1.0 y:  2.15 z: 0.1"
rostopic pub moveGripper arm_gazebo/GripperMessage "grab: true"
rostopic pub ikPose arm_gazebo/PositionMessage "x: 1.0 y:  2.15 z: 1.1"
rostopic pub ikPose arm_gazebo/PositionMessage "x: -1.0 y: -2.0 z: 1.1"
rostopic pub ikPose arm_gazebo/PositionMessage "x: -1.0 y:  -2.0 z: 0.1"
rostopic pub moveGripper arm_gazebo/GripperMessage "grab: false"

