# dialogue_as

The dialogue_as package is a very simple action server that subscribes to a 'human_dialogue' topic and allows you to trigger several keywords.

The dialogue_as server will also send you a boolean message (always true) every time your dialog system receives a dialog and finds none of your keywords.

# Simple keywords client

A simple client who wishes to trigger on the keywords 'yes' or 'no' is the following:

```cpp
#include "ros/ros.h"
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <dialogue_as/dialogue_actionAction.h>
#include "std_msgs/String.h"

int main (int argc, char **argv)
{
  ros::init(argc, argv, "test_dialogue_as");

  actionlib::SimpleActionClient<dialogue_as::dialogue_actionAction> ac("dialogue_as", true);

  ROS_INFO("Waiting for action server to start.");
  ac.waitForServer();

  ROS_INFO("Action server started, sending goal.");
  dialogue_as::dialogue_actionGoal goal;
  goal.subjects.push_back("yes");
  goal.subjects.push_back("no");
  goal.enable_only_subject = true;
  ac.sendGoal(goal);

  bool finished_before_timeout = ac.waitForResult(ros::Duration(240.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  return 0;
}

```

## Test it

First, you must launch your dialog system with publishing string message on 'human_dialogue' topic.

After that, you can run the dialogue_as with the following command:
```
$ rosrun dialogue_as dialogue_as
```

Now you can run the simple client presented just before:
```
$ rosrun dialogue_as dialogue_as_tester_yes_no
```
