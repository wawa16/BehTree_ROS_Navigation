#include <actions/move_to_goal.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


BT::MoveToGoal::MoveToGoal(std::string name) : ActionNode::ActionNode(name),
action_name_(name)
{
    thread_ = std::thread(&MoveToGoal::WaitForTick, this);
}

BT::MoveToGoal::~MoveToGoal(void){}

void BT::MoveToGoal::WaitForTick(){
 while (true)
    {
        // Waiting for the first tick to come
        DEBUG_STDOUT(get_name() << " WAIT FOR TICK");
        tick_engine.Wait();
        DEBUG_STDOUT(get_name() << " TICK RECEIVED");
        
        if (get_status() != BT::HALTED)  
        {
            getLocation();
                    
            if(bool_status_){
                set_status(BT::SUCCESS);
                DEBUG_STDOUT("move_base_goal achieved");
            } else {
                set_status(BT::RUNNING);
                DEBUG_STDOUT("move_base_goal Failed");
            }
        }
    }

}

void BT::MoveToGoal::getLocation(){
    int choice = 0;
    float dept_[2][4] = {{1.368, -1.728, 0.0, 0.0}, {0.0, 0.0, -0.812, 0.5820}};
    float arri_[2][4] = {{-0.512, -2.581, 0.0, 0.0},{0.0, 0.0, 0.9385, 0.345}};
    float loug_[2][4] = {{-0.8699, -0.718, 0.0, 0.0},{0.0, 0.0, 0.5856, 0.8105}};

    while(choice != 4){
        ROS_INFO("Choose Location: 1.Departure____2.Arrivals____3.Lounge____4.Cancel");
        std::cin >> choice;
         switch (choice)
        {
        case 1:
            ROS_INFO("Moving to DEPARTURE");
            bool_status_ = sendGoal(dept_);
            break;
        case 2:
            ROS_INFO("Moving to ARRIVALS");
            bool_status_ = sendGoal(arri_);
            break;
        case 3:
            ROS_INFO("Moving to LOUNGE");
            bool_status_ = sendGoal(loug_);
            break;
        default:
            break;
        }
    }

}

bool BT::MoveToGoal::sendGoal(float dest[2][4]){
   
    MoveBaseClient client("move_base", true);
    if(client.isServerConnected()) {

    try{ 
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
	goal.target_pose.pose.position.x = dest[0][0];
    goal.target_pose.pose.position.y = dest[0][1];
    goal.target_pose.pose.position.z = dest[0][2];
    goal.target_pose.pose.orientation.z = dest[1][2];
    goal.target_pose.pose.orientation.w = dest[1][3];
   
    while(!client.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
    }


    ROS_INFO("pose value: %f", goal.target_pose.pose.position.x); //For Debugging
    client.sendGoal(goal);
    client.waitForResult();

    if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
        DEBUG_STDOUT("Mission completed"); 	
        bool_status_ = true;
    }	
        else
    {  
        bool_status_ = false;
    }
return bool_status_;
    } catch(const std::exception& e){
        DEBUG_STDOUT("NO SERVER FOUND");
    }
    }
    else {
        DEBUG_STDOUT("NO SERVER RUNNING");
    }
}

void BT::MoveToGoal::Halt()
{
    set_status(BT::HALTED);
    DEBUG_STDOUT("HALTED state set!");
}