#include <actions/patrol.h>

#include <behavior_tree.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "BehaviorTree");
    try
    {
        int TickPeriod_milliseconds = 1000;

        BT::Patrol* patrol1 = new BT::Patrol("Patrol");
        BT::ConditionTestNode* condition1 = new BT::ConditionTestNode("AmIneeded?");
        BT::MoveToGoal* movedest1 = new BT::MoveToGoal("MoveToCaller");
        BT::FallbackNodeWithMemory* parallel1 = new BT::FallbackNodeWithMemory("paral1");
        BT::SequenceNodeWithMemory* sequence1 = new BT::SequenceNodeWithMemory("seq1");

        sequence1->AddChild(parallel1);
        sequence1->AddChild(movedest1);
        
        
        parallel1->AddChild(condition1);
        parallel1->AddChild(patrol1);
      

        Execute(sequence1, TickPeriod_milliseconds);  // from BehaviorTree.cpp
    }
    catch (BT::BehaviorTreeException& Exception)
    {
        std::cout << Exception.what() << std::endl;
    }

    return 0;
}