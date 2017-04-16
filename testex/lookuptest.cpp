#include <stdio.h>
#include "lookup.hpp"
#include "mac_address.hpp"
#include "hebi_util.h"
#include "feedback_print_helpers.cpp"
#include "lookup_helpers.cpp"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/**
 * Simple helper function to print out if the group is valid, based on number of
 * modules (-1 for invalid group).
 */
void checkGroup(int num_modules)
{
  if (num_modules > 0)
    std::cout << "Found group with " << num_modules << " modules." << std::endl;
  else
    std::cout << "Group not found on network." << std::endl;
}


void setArmCommand(hebi::GroupCommand command, int config){
	float j1[2] = {0,-0.5*M_PI};
	float j2[2] = {0.125*M_PI, -1.125*M_PI};
	
	if (config == 0){
		command[0].actuator().position().set(j1[0]);
		command[1].actuator().position().set(j2[0]);
	}
	else if (config == 1){
		command[0].actuator().position().set(j1[0]);
		command[1].actuator().position().set(j2[1]);
	}
	else if (config == 2){
		command[0].actuator().position().set(j1[1]);
		command[1].actuator().position().set(j2[0]);
	}
	else{
		std::cout << "Arm configuration not valid" << std::endl;
	}
}



/**
 * Attempts to look for several groups on the network (hardcoded addresses below),
 * and retrieve references to them.
 */
int main()
{

  //Initializing
  hebi::Lookup lookup;
  hebi_sleep_s(2);
  lookup.printTable();
  printf("\n");

  std::vector<std::string> names;
  names.push_back("X-00098");
  //names.push_back("joint2");
  names.push_back("elbow");
  //names.push_back("joint2");
  
  std::vector<std::string> families;
  families.push_back("X5-4");
  //families.push_back("16384");
  families.push_back("TeamJ");
  //families.push_back("16384");
  
  long timeout_ms = 4000;

  printf("Looking up group by name.\n");
  std::unique_ptr<hebi::Group> group;
  group = lookup.getGroupFromNames(names, families, timeout_ms);
  checkGroup(group ? group->size() : -1);
  
  int num_modules = group->size();
  hebi::GroupCommand command(num_modules);

  //setup command strategy. needs testing
/*
  hebi::Command::ControlStrategy new_strategy = hebi::Command::ControlStrategy::Strategy3;
  for (int module_index = 0; module_index < num_modules; module_index++)
  {
    command[module_index].settings().actuator().controlStrategy().set(new_strategy);
  }
  group->sendCommand(command);
*/

  
  //print feedback
/*
  hebi::GroupFeedback feedback(num_modules);
  timeout_ms = 1000;
  float period_s = 0.25f;
  if (group->requestFeedback(&feedback, timeout_ms))
  {
    print_group_feedback(feedback);
  }
  else
  {
    printf("Received no feedback from group!\n");
  }
  hebi_sleep_ms(period_s * 1000);
*/  
  
  
/*
  for (int module_index = 0; module_index < num_modules; module_index++)
  {
    // Set the position for this module, offsetting each with respect to other
    // modules.
    command[module_index].actuator().position().set(0);
  }
*/
  

  //float j1[2] = {0,-0.5*M_PI};
  float j2[2] = {-0.125*M_PI, -1.125*M_PI};
  command[0].actuator().position().set(j1[0]);
  command[1].actuator().position().set(j2[1]);
  //command[0].actuator().position().set(0); //end effector, change to 2
  group->sendCommand(command);
  hebi_sleep_ms(100);
  
  int count = 1;
  int config = 0;
  int current_config = 0;
  int terminate = 0;
  float ende;
  while(terminate == 0){
	if (count>10000){
		terminate = 1;
	}
	//reset to 0
	
	std::cout << "Enter Configuration number" << std::endl;
	std::cin >> config;

	if (config != current_config){
		if (config == 0){
			//command[0].actuator().position().set(j1[0]);
			command[1].actuator().position().set(j2[0]);
		}
		else if (config == 1){
			//command[0].actuator().position().set(j1[0]);
			command[1].actuator().position().set(j2[1]);
		}
		else if (config == 2){
			//command[0].actuator().position().set(j1[1]);
			command[1].actuator().position().set(j2[0]);
		}
		else{
			std::cout << "Arm configuration not valid" << std::endl;
			terminate = 1;
		}
		group->sendCommand(command);
		current_config = config;
	}
	
	/*
	std::cout << "Enter endeffector degree" << std::endl;
	std::cin >> ende;
	if (ende>10){
		terminate=1;
	}	
	else{
		command[0].actuator().position().set(ende*M_PI);
	}
	group->sendCommand(command);
	*/
	count++;
  }


/*
 * command with acknoledgement
  timeout_ms = 100;
  if (group->sendCommandWithAcknowledgement(command, timeout_ms))
  {
    std::cout << "Command got acknowledgement." << std::endl;
  }
  else
  {
    std::cout << "Did not receive acknowledgement for command!" << std::endl;
  }
 */


  // NOTE: destructors automatically clean up objects
  return 0;
}
