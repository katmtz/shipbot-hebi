#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "lookup.hpp"
#include "module.hpp"
#include "group.hpp"
#include "group_command.hpp"
#include "command.hpp"
#include "mac_address.hpp"
#include "lookup_helpers.cpp"
#include "feedback_print_helpers.cpp"
#include "hebi_util.h"
#include "optionparser.h"

using namespace std;

//#define DEBUG

// Test parameters
#define START_POS -M_PI/4.0
#define END_POS M_PI/4.0
#define START_WAIT_TIME 5.0
#define INIT_PAUSE 0.5
#define TEST_DURATION 3.0 //secs

// Actuators used
//#define ACT_1 "X-00072"
//#define ACT_2 "X-00068"
//#define ACT_3 "X-00073"
//#define ACT_FAM "X5-4"

int main()
{
  vector<string> names;
  vector<string> families;

  long timeout_ms = 4000; // Give the modules plenty of time to appear
  hebi::Lookup lookup;
  
  names.push_back("wrist2");
  families.push_back("capsenArm");
/*  names.push_back(ACT_1);
  names.push_back(ACT_2);
  names.push_back(ACT_3);
  families.push_back(ACT_FAM);
  families.push_back(ACT_FAM);
  families.push_back(ACT_FAM);
 */ 
  // Try and get the requested group.
  unique_ptr<hebi::Group> group = lookup.getGroupFromNames(names, families, timeout_ms);

  if (!group)
  {
    cout << "No group found!" << endl;
    return -1;
  }

  group->setFeedbackFrequencyHz(500);

  int num_modules = group->size();

  // Create a group feedback object; this will be filled in during the request.
  hebi::GroupFeedback feedback(num_modules);
  group->requestFeedback(&feedback, timeout_ms); // Grab an initial feedback so modules start reporting
  timeout_ms = 200; // Reduce timeout after our initial request

  // Create a command object; this can be sent to the group
  hebi::GroupCommand command(num_modules);

  // Switch the control strategy over to direct PWM
  hebi::Command::ControlStrategy new_strategy = hebi::Command::ControlStrategy::DirectPWM;
  command[0].settings().actuator().controlStrategy().set(new_strategy);
  group->sendCommand(command);
  command[0].settings().actuator().controlStrategy().clear();

  timeval readTime;
  gettimeofday(&readTime, NULL);
  double testStartTime = readTime.tv_sec + readTime.tv_usec/1000000.0;
  double currentTime = 0;

  while (currentTime < TEST_DURATION)
  {
    command[0].actuator().torque().set(0.6);
    group->sendCommand(command);

    // Take readings from the actuators and print to file with current time
    // group->requestFeedback(&feedback, timeout_ms);
    gettimeofday(&readTime, NULL);
    currentTime = readTime.tv_sec + (readTime.tv_usec/1000000.0) - testStartTime;

    usleep(100);
  }

  return 0;
}
