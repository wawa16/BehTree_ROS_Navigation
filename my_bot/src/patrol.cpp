#include <actions/patrol.h>
#include <string>
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <behavior_tree_core/BTAction.h>
#include <action_node.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

enum Status {RUNNING, SUCCESS, FAILURE};  // BT return status

BT::Patrol::Patrol(std::string name) : ActionNode::ActionNode(name),
action_name_(name)
{
    
     boolean_value_ = false;
     bool_status_ = false;
     patrol_ = true;
     thread_ = std::thread(&Patrol::WaitForTick, this);

}

BT::Patrol::~Patrol(void){}

void BT::Patrol::Halt()
{
    set_status(BT::HALTED);
    DEBUG_STDOUT("HALTED state set!");
}

bool BT::Patrol::sendGoal(){

    try{

    
    MoveBaseClient client("move_base", true);
    float pose_[3][2][4] = {{{1.168, -1.728, 0.0, 0.0}, {0.0, 0.0, -0.812, 0.5820}},
    {{-0.312, -2.581, 0.0, 0.0},{0.0, 0.0, 0.9385, 0.345}},
    {{-0.9699, -0.718, 0.0, 0.0},{0.0, 0.0, 0.5856, 0.8105}}
    };    
    waypoints_ = 3;
    while(!client.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
    }

       
    for(int i = 0; i < waypoints_; i++)
    {

  
        goalpose(pose_, i);
        ROS_INFO("pose value: %f", goal.target_pose.pose.position.x);
        client.sendGoal(goal);
        // if(preemptGoal()){
        //     client.cancelGoal();
        //     patrol_ = false;
        // }
        client.waitForResult();
        //bool_status_ = true;
      
    }
        


        if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
            DEBUG_STDOUT("Mission completed"); 	
            bool_status_ = true;
        }	
            else
        {  
            bool_status_ = false;
        }
  

return bool_status_;
    }
    catch(const std::exception& e){
        DEBUG_STDOUT("NO SERVER FOUND");
    }
}

void BT::Patrol::WaitForTick(){
    while (true)
    {
        // Waiting for the first tick to come
        DEBUG_STDOUT(get_name() << " WAIT FOR TICK");

        tick_engine.Wait();
        DEBUG_STDOUT(get_name() << " TICK RECEIVED");
        //preemptGoal();
        //as_.start();
        // Running state
        //set_status(BT::RUNNING);
       
     
        if (get_status() != BT::HALTED)
        
        {
               bool_status_= sendGoal();

               if(bool_status_){
                   set_status(BT::FAILURE);
                   DEBUG_STDOUT("move_base_goal achieved");
               } else {
                   set_status(BT::FAILURE);
                   DEBUG_STDOUT("move_base_goal Failed");
               }
        }
    }

}
bool BT::Patrol::preemptGoal(){
    int choice = 0;
    bool preemptFlag = false;
    ROS_INFO("Enter 1 To Cancel Patrol");
    std::cin >> choice;
    if(choice == 1){
    ROS_INFO("Cancelling Goal...");
    preemptFlag = true;
    }
    return preemptFlag;
}

void BT::Patrol::goalpose(float pose[3][2][4], int &waypoints_){
	
	goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
	goal.target_pose.pose.position.x = pose[waypoints_][0][0];
    goal.target_pose.pose.position.y = pose[waypoints_][0][1];
    goal.target_pose.pose.position.z = pose[waypoints_][0][2];
    goal.target_pose.pose.orientation.z = pose[waypoints_][1][2];
    goal.target_pose.pose.orientation.w = pose[waypoints_][1][3];
}