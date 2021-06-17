#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "ros/ros.h"
#include "arm_gazebo/GripperMessage.h"


namespace gazebo
{
  class GripperController : public ModelPlugin
  {
    public: GripperController() : ModelPlugin()
    {
        printf("Hello World!\n");
    }

    private: void catchReleaseBox(const arm_gazebo::GripperMessage& msg)
    {
         std::string palm_left_finger_name = this->joint_palm_left_finger->GetScopedName();
        std::string palm_right_finger_name = this->joint_palm_right_finger->GetScopedName();

        this->jointController->SetPositionPID(palm_left_finger_name, this->pJ7);
        this->jointController->SetPositionPID(palm_right_finger_name, this->pJ8);


        if(msg.release){
            this->jointController->SetPositionTarget(palm_left_finger_name, 0);
            this->jointController->SetPositionTarget(palm_right_finger_name, 0);
        } else{
            this->jointController->SetPositionTarget(palm_left_finger_name, -0.21);
            this->jointController->SetPositionTarget(palm_right_finger_name, 0.21);
        }
    }



    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
        this->model = _parent;

        this->jointController = this->model->GetJointController();

        auto joint_1_name = "chasis_arm1_joint";
        auto joint_2_name = "arm1_arm2_joint";
        auto joint_3_name = "arm2_arm3_joint";
        auto joint_4_name = "arm3_arm4_joint";
        auto joint_5_name = "arm4_arm5_joint";
        auto palm_riser_name = "palm_riser";
        auto palm_left_joint_name = "palm_left_finger";
        auto palm_right_joint_name = "palm_right_finger";

        this->j1 = this->model->GetJoint(joint_1_name);
        this->j2 = this->model->GetJoint(joint_2_name);
        this->j3 = this->model->GetJoint(joint_3_name);
        this->j4 = this->model->GetJoint(joint_4_name);
        this->j5 = this->model->GetJoint(joint_5_name);
        this->j_palm_riser = this->model->GetJoint(palm_riser_name);
        this->joint_palm_left_finger = this->model->GetJoint(palm_left_joint_name);
        this->joint_palm_right_finger = this->model->GetJoint(palm_right_joint_name);

        this->pJ1 = common::PID(31.6, 10.02, 15.45);

        this->pJ2 = common::PID(15, 5.1, 10);

        this->pJ3 = common::PID(15, 5.1, 10);

        this->pJ4 = common::PID(15, 5.1, 10);

        this->pJ5 = common::PID(15, 5.1, 10);

        this->pJ6 = common::PID(15, 5.1, 10);

        this->pJ7 = common::PID(5, 2.1, 3);

        this->pJ8 = common::PID(5, 2.1, 3);

        this->rosSub = this->rosNode.subscribe("moveGripper", 1000, &GripperController::catchReleaseBox, this);

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            std::bind(&GripperController::OnUpdate, this)
        );
    }
    public : void OnUpdate()
    {
        this->jointController->Update();
    }

    //Pointer to the model
    private: physics::ModelPtr model;

    //Pointer to the model
    private: physics::JointControllerPtr jointController;

    //Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;

    private: common::PID pJ1;
    private: common::PID pJ2;
    private: common::PID pJ3;
    private: common::PID pJ4;
    private: common::PID pJ5;
    private: common::PID pJ6;
    private: common::PID pJ7;
    private: common::PID pJ8;

    private: physics::JointPtr j1;
    private: physics::JointPtr j2;
    private: physics::JointPtr j3;
    private: physics::JointPtr j4;
    private: physics::JointPtr j5;
    private: physics::JointPtr j_palm_riser;
    private: physics::JointPtr joint_palm_left_finger;
    private: physics::JointPtr joint_palm_right_finger;

    private: ros::NodeHandle rosNode;

    private: ros::Subscriber rosSub;
  };
  GZ_REGISTER_MODEL_PLUGIN(GripperController)
}