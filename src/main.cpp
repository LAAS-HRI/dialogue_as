#include "ros/ros.h"
#include <actionlib/server/simple_action_server.h>
#include <dialogue_as/dialogue_actionAction.h>
#include "std_msgs/String.h"
#include <iostream>
#include <algorithm>

class DialogueAction
{
protected:

  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<dialogue_as::dialogue_actionAction> as_;

  dialogue_as::dialogue_actionFeedback feedback_;
  dialogue_as::dialogue_actionResult result_;

  ros::Subscriber sub_;
  std::string text_;

  std::vector<std::string> verbs;
  std::vector<std::string> subjects;

  bool enable_only_verb;
  bool enable_only_subject;

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
    std::cout << "Receive goal for : " << std::endl;

    boost::shared_ptr<const dialogue_as::dialogue_actionGoal> goal = as_.acceptNewGoal();
    verbs = goal->verbs;
    for(int i = 0; i < verbs.size(); i++)
      std::cout << verbs[i] << " ";
    std::cout << std::endl;

    subjects = goal->subjects;
    for(int i = 0; i < subjects.size(); i++)
      std::cout << subjects[i] << " ";
    std::cout << std::endl;

    enable_only_verb = goal->enable_only_verb;
    enable_only_subject = goal->enable_only_subject;
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
    std::transform(text_.begin(), text_.end(), text_.begin(), ::tolower);
    std::cout << "callback " << text_ << std::endl;
    std::string res_verb = "";
    std::string res_subject = "";
    text_ = " " + text_ + " ";

    for(int i = 0; i < verbs.size(); i++)
    	if(text_.find(" " + verbs[i] + " ") != std::string::npos)
    	{
    		res_verb = verbs[i];
        feedback_.verb = res_verb;
    		break;
    	}

    for(int i = 0; i < subjects.size(); i++)
    	if(text_.find(" " + subjects[i] + " ") != std::string::npos)
    	{
    		res_subject = subjects[i];
        feedback_.subject = res_subject;
    		break;
    	}

    bool succed = false;
    if((res_verb != "") && (res_subject != ""))
      succed = true;
    else if((res_verb == "") && (res_subject != "") && enable_only_subject)
      succed = true;
    else if((res_verb != "") && (res_subject == "") && enable_only_verb)
      succed = true;

    if(succed)
    {
      result_.verb = res_verb;
      result_.subject = res_subject;
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
