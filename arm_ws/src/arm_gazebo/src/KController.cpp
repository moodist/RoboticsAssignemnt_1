#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "ros/ros.h"
#include "arm_gazebo/PositionMessage.h"
#include "arm_gazebo/ik.h"

namespace gazebo
{
  class KinematicsController : public ModelPlugin
  {
    public: KinematicsController() : ModelPlugin()
    {
        printf("Hello World!\n");
    }


    private: void callback(const arm_gazebo::PositionMessage& msg)
    {
        ik(msg.x, msg.y, msg.z);
    }

    public: int ik(const float x, const float y, const float z){
        ros::ServiceClient client = this->rosNode.serviceClient<arm_gazebo::ik>("ik");

        arm_gazebo::ik srv;

        srv.request.link_length_1 = 0.1;
        srv.request.link_length_2 = 0.05;
        srv.request.link_length_3 = 2;
        srv.request.link_length_4 = 1;
        srv.request.link_length_5 = 0.5;
        srv.request.link_length_6 = 0.04;
        srv.request.link_length_7 = 0.1;

        srv.request.x = x;
        srv.request.y = y;
        srv.request.z = z;

        if (client.call(srv))
        {
            changePose(
                srv.response.angle1, 
                srv.response.angle2, 
                srv.response.angle3,
                srv.response.angle4, 
                srv.response.angle5, 
                srv.response.angle6);
        }
        else
        {
            ROS_ERROR("Failed to call service add_two_ints");
        return 1;
        }
        return 0;
    }

    private: void changePose(
            const float angleOne, const float angleTwo,
            const float angleThree, const float angleFour, 
            const float angleFive, const float angleSix) {
        
        std::string j_name_1 = this->j1->GetScopedName();

        this->jointController->SetPositionPID(j_name_1, pJ1);
        this->jointController->SetPositionTarget(j_name_1, angleOne);

        std::string j_name_2 = this->j2->GetScopedName();

        this->jointController->SetPositionPID(j_name_2, pJ2);
        this->jointController->SetPositionTarget(j_name_2, angleTwo);

        std::string j_name_3 = this->j3->GetScopedName();

        this->jointController->SetPositionPID(j_name_3, pJ3);
        this->jointController->SetPositionTarget(j_name_3, angleThree);

        std::string j_name_4 = this->j4->GetScopedName();

        this->jointController->SetPositionPID(j_name_4, pJ4);
        this->jointController->SetPositionTarget(j_name_4, angleFour);

        std::string j_name_5 = this->j5->GetScopedName();

        this->jointController->SetPositionPID(j_name_5, pJ5);
        this->jointController->SetPositionTarget(j_name_5, angleFive);

        std::string j_name_6 = this->j_palm_riser->GetScopedName();

        this->jointController->SetPositionPID(j_name_6, pJ6);
        this->jointController->SetPositionTarget(j_name_6, angleSix);
    }

    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
        this->model = _parent;

        this->jointController = this->model->GetJointController();

        this->pJ1 = common::PID(31.6, 10.02, 15.45);
        this->pJ2 = common::PID(15, 5.1, 10);
        this->pJ3 = common::PID(15, 5.1, 10);
        this->pJ4 = common::PID(15, 5.1, 10);
        this->pJ5 = common::PID(15, 5.1, 10);
        this->pJ6 = common::PID(15, 5.1, 10);

        auto joint_1_name = "chasis_arm1_joint";
        auto joint_2_name = "arm1_arm2_joint";
        auto joint_3_name = "arm2_arm3_joint";
        auto joint_4_name = "arm3_arm4_joint";
        auto joint_5_name = "arm4_arm5_joint";
        auto palm_riser_name = "palm_riser";

        this->j1 = this->model->GetJoint(joint_1_name);
        this->j2 = this->model->GetJoint(joint_2_name);
        this->j3 = this->model->GetJoint(joint_3_name);
        this->j4 = this->model->GetJoint(joint_4_name);
        this->j5 = this->model->GetJoint(joint_5_name);
        this->j_palm_riser = this->model->GetJoint(palm_riser_name);
        
        changePose(-0.4444, -0.37568, -1.04219, -0.4873, -0.44, 0.11432);

        this->rosSub = this->rosNode.subscribe("ikPose", 1000, &KinematicsController::callback, this);

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            std::bind(&KinematicsController::OnUpdate, this)
        );
    }
    
    //called by the world update start event
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


    private: physics::JointPtr j1;
    private: physics::JointPtr j2;
    private: physics::JointPtr j3;
    private: physics::JointPtr j4;
    private: physics::JointPtr j5;
    private: physics::JointPtr j_palm_riser;


    private: ros::NodeHandle rosNode;

    private: ros::Subscriber rosSub;
  };
  GZ_REGISTER_MODEL_PLUGIN(KinematicsController)
}