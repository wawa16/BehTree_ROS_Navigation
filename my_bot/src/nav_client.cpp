#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


void goalpose(float[3][2][4], int&);
move_base_msgs::MoveBaseGoal goal;

int main(int argc, char **argv){
	ros::init(argc, argv, "MoveBaseGoals");
	MoveBaseClient client("move_base", true);
	int waypoints = 3;
    float pose[3][2][4] = {{{1.168, -1.728, 0.0, 0.0}, {0.0, 0.0, -0.812, 0.5820}},
    {{-0.312, -2.581, 0.0, 0.0},{0.0, 0.0, 0.9385, 0.345}},
    {{-0.9699, -0.718, 0.0, 0.0},{0.0, 0.0, 0.5856, 0.8105}}
	};


    
	 while(!client.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
     }

	     for(int i = 0; i < waypoints; i++)
	    {
	    	goalpose(pose, i);
	    	ROS_INFO("pose value: %f", goal.target_pose.pose.position.x);
	    	client.sendGoal(goal);
			client.waitForResult();
	    }
		   //client.waitForResult();
	       if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    		ROS_INFO("Mission completed");	
    	}	
     
 return 0;
}

void goalpose(float pose[3][2][4], int &waypoints){
	
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = pose[waypoints][0][0];
    goal.target_pose.pose.position.y = pose[waypoints][0][1];
    goal.target_pose.pose.position.z = pose[waypoints][0][2];
    goal.target_pose.pose.orientation.z = pose[waypoints][1][2];
    goal.target_pose.pose.orientation.w = pose[waypoints][1][3];
}
