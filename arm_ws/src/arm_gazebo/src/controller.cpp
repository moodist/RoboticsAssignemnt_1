#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "ros/ros.h"
#include "arm_gazebo/JointPose.h"

namespace gazebo
{
  class Controller1 : public ModelPlugin
  {
    public: Controller1() : ModelPlugin()
    {
        printf("Hello World!\n");
    }
    
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
        this->model = _parent;

        this->jointController = this->model->GetJointController();
        float angleDegree = 50; 
        float rad = M_PI * angleDegree/180 ;

        this->joint_1_pid = common::PID(31.6, 10.02, 15.45);

        this->joint_2_pid = common::PID(15, 5.1, 10);

        this->joint_3_pid = common::PID(15, 5.1, 10);

        this->joint_4_pid = common::PID(15, 5.1, 10);

        this->joint_5_pid = common::PID(15, 5.1, 10);

        this->joint_1_name = "chasis_arm1_joint";
        this->joint_2_name = "arm1_arm2_joint";
        this->joint_3_name = "arm2_arm3_joint";
        this->joint_4_name = "arm3_arm4_joint";
        this->joint_5_name = "arm4_arm5_joint";

               

        auto j1 = this->model->GetJoint(joint_1_name);
        auto j2 = this->model->GetJoint(joint_2_name);
        auto j3 = this->model->GetJoint(joint_3_name);
        auto j4 = this->model->GetJoint(joint_4_name);
        auto j5 = this->model->GetJoint(joint_5_name);

        std::string j_name_1 = j1->GetScopedName();

        this->jointController->SetPositionPID(j_name_1, joint_1_pid);
        this->jointController->SetPositionTarget(j_name_1, 0.4);

        std::string j_name_2 = j2->GetScopedName();

        this->jointController->SetPositionPID(j_name_2, joint_2_pid);
        this->jointController->SetPositionTarget(j_name_2, 0.3);

        std::string j_name_3 = j3->GetScopedName();

        this->jointController->SetPositionPID(j_name_3, joint_3_pid);
        this->jointController->SetPositionTarget(j_name_3, 1.0);

        std::string j_name_4 = j4->GetScopedName();

        this->jointController->SetPositionPID(j_name_4, joint_4_pid);
        this->jointController->SetPositionTarget(j_name_4, 0.4);

        std::string j_name_5 = j4->GetScopedName();

        this->jointController->SetPositionPID(j_name_5, joint_5_pid);
        this->jointController->SetPositionTarget(j_name_5, 0.3);

        
        this->rosPub = this->rosNode.advertise<arm_gazebo::JointPose>("jointpose", 10);

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            std::bind(&Controller1::OnUpdate, this)
        );
 

        this->rosSub = this->rosNode.subscribe("arm_control",1000,&Controller1::chatterCallback,this);

    }

    void chatterCallback(const arm_gazebo::JointPose::ConstPtr& msg)
    {
        float angleDegree = 50; 
        float rad = M_PI * angleDegree/180 ;

        auto j1 = this->model->GetJoint(this->joint_1_name);
        auto j2 = this->model->GetJoint(this->joint_2_name);
        auto j3 = this->model->GetJoint(this->joint_3_name);
        auto j4 = this->model->GetJoint(this->joint_4_name);

        std::string j_name_1 = j1->GetScopedName();

        this->jointController->SetPositionPID(j_name_1, this->joint_1_pid);
        this->jointController->SetPositionTarget(j_name_1, msg->angleOne);

        std::string j_name_2 = j2->GetScopedName();

        this->jointController->SetPositionPID(j_name_2, this->joint_2_pid);
        this->jointController->SetPositionTarget(j_name_2, msg->angleTwo);

        std::string j_name_3 = j3->GetScopedName();

        this->jointController->SetPositionPID(j_name_3, this->joint_3_pid);
        this->jointController->SetPositionTarget(j_name_3, msg->angleThree);

        std::string J_name_4 = j4->GetScopedName();

        this->jointController->SetPositionPID(J_name_4, this->joint_4_pid);
        this->jointController->SetPositionTarget(J_name_4, msg->angleFour);



    }
    
    //called by the world update start event
    public : void OnUpdate()
    {
        auto j1 = this->model->GetJoint(this->joint_1_name);
        auto j2 = this->model->GetJoint(this->joint_2_name);
        auto j3 = this->model->GetJoint(this->joint_3_name);
        auto j4 = this->model->GetJoint(this->joint_4_name);
        auto j5 = this->model->GetJoint(this->joint_5_name);

        arm_gazebo::JointPose msg;

        msg.angleOne = j1->Position();
        msg.angleTwo = j2->Position();
        msg.angleThree = j3->Position();
        msg.angleFour = j4->Position();

        this->rosPub.publish(msg);

        this->jointController->Update();
    }

    //Pointer to the model
    private: physics::ModelPtr model;

    //Pointer to the model
    private: physics::JointControllerPtr jointController;

    //Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;

    private: common::PID joint_1_pid;

    private: common::PID joint_2_pid;

    private: common::PID joint_3_pid;

    private: common::PID joint_4_pid;

    private: common::PID joint_5_pid;

    private: std::string joint_1_name;
    private: std::string joint_2_name;
    private: std::string joint_3_name;
    private: std::string joint_4_name;
    private: std::string joint_5_name;


    private: ros::NodeHandle rosNode;
    private: ros::Publisher rosPub;
    private: ros::Subscriber rosSub;
 

  };
  GZ_REGISTER_MODEL_PLUGIN(Controller1)
}


 
//  chatter_pub.publish(msg);