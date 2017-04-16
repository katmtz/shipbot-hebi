#include <stdio.h>
#include "lookup.hpp"
#include "mac_address.hpp"
#include "hebi_util.h"
#include "feedback_print_helpers.cpp"
#include "lookup_helpers.cpp"
#include <cmath>
#include <fstream>
#include <string>


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



int main()
{
  //Initializing
  hebi::Lookup lookup;
  hebi_sleep_s(2);
  lookup.printTable();
  printf("\n");

  std::vector<std::string> names;
  names.push_back("Shoulder");
  names.push_back("elbow");
  names.push_back("Hand");
  
  std::vector<std::string> families;
  families.push_back("TeamJ");
  families.push_back("TeamJ");
  families.push_back("TeamJ");
  
  long timeout_ms = 4000;

  printf("Looking up group by name.\n");
  std::unique_ptr<hebi::Group> group;
  group = lookup.getGroupFromNames(names, families, timeout_ms);
  checkGroup(group ? group->size() : -1);
  
  //creates command module
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
  
  
  //float j1[2] = {0,-0.5*M_PI};
  //float j2[2] = {-0.125*M_PI, -1.125*M_PI};
  //float j2zero = -0.125*M_PI;
  float j2zero = 0;
  
  //set every motor to position 0
  command[0].actuator().position().set(0);
  command[1].actuator().position().set(j2zero);
  command[2].actuator().position().set(0); //end effector, change to 2
  group->sendCommand(command);
  std::cout << "Initializd"<<std::endl;
  hebi_sleep_ms(100);
  
  int count = 0;
  int terminate = 0;
  float shoulder_pos = 0;
  float elbow_pos = 0;
  float hand_pos = 0;
  while(terminate == 0){
	if (count>10000){
		terminate = 1;
	}
	
	int sender = 0;
	std::ifstream file("hebi.txt");
	std::string str;
	std::string symb;
	file >> symb;
	file >> sender;
	if (sender == 1){
		file >> shoulder_pos;
		file >> elbow_pos;
		file >> hand_pos;
		std::ofstream file("hebi.txt");
		file << "@ 0" << std::endl;
		file << shoulder_pos << " " << elbow_pos << " " << hand_pos;
	}
	
	command[0].actuator().position().set(shoulder_pos*M_PI);
    command[1].actuator().position().set(elbow_pos*M_PI+j2zero);
    command[2].actuator().position().set(hand_pos*M_PI); //end effector, change to 2
	group->sendCommand(command);
	
	/*
	std::cout << "Enter shoulder degree" << std::endl;
	std::cin >> shoulder_pos;
	if (shoulder_pos>100){
		
		break;
		terminate = 1;
	}	
	else{
		command[0].actuator().position().set(shoulder_pos*M_PI);
	}
	std::cout << "Enter elbow degree" << std::endl;
	std::cin >> elbow_pos;
	command[1].actuator().position().set(elbow_pos*M_PI+j2zero);
	std::cout << "Enter hand degree" << std::endl;
	std::cin >> hand_pos;
	command[2].actuator().position().set(hand_pos*M_PI);
	group->sendCommand(command);
	*/
	count++;
  }

  // NOTE: destructors automatically clean up objects
  return 0;
}
