#include "ros/ros.h"
#include <actionlib/server/simple_action_server.h>
#include <dialogue_as/dialogue_actionAction.h>
#include "std_msgs/String.h"

class DialogueAction
{
protected:

  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<dialogue_as::dialogue_actionAction> as_;

  dialogue_as::dialogue_actionFeedback feedback_;
  dialogue_as::dialogue_actionResult result_;

  ros::Subscriber sub_;
  std::string text_;
  std::vector<std::string> keywords;

public:

  DialogueAction(std::string name) :
    as_(nh_, name, false)
  {
    as_.registerGoalCallback(boost::bind(&DialogueAction::goalCB, this));
    as_.registerPreemptCallback(boost::bind(&DialogueAction::preemptCB, this));

    sub_ = nh_.subscribe("human_dialogue", 10000, &DialogueAction::messageCallback, this);

    as_.start();
  }

  ~DialogueAction(void)
  {
  }

  void goalCB()
  {
    keywords = as_.acceptNewGoal()->keywords;
  }

  void preemptCB()
  {
    ROS_INFO("dialogue_as: Preempted");
    as_.setPreempted();
  }

  void messageCallback(const std_msgs::String::ConstPtr& msg)
  {
    if (!as_.isActive())
      return;
      
    text_ = msg->data;
    std::string res = "";

    feedback_.speak = true;

    if(res != "")
    {
      result_.word = res;
      ROS_INFO("dialogue_as: Succeeded");
      as_.setSucceeded(result_);
    }
    else
      as_.publishFeedback(feedback_);
  }
};


int main(int argc, char** argv)
{
  ros::init(argc, argv, "dialogue_as");

  DialogueAction dialogue_action("dialogue_as");
  ros::spin();

  return 0;
}
