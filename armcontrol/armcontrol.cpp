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

bool commandline = false;

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

  float vel = 0.02*M_PI;
  //set every motor to position 0
  for (int i = 0; i < num_modules; i++){
	command[i].actuator().position().set(0);
	//command[i].actuator().velocity().set(vel);
  }
  group->sendCommand(command);
  std::cout << "Initializd"<<std::endl;
  hebi_sleep_ms(100);
  
  int terminate = 0;
  float pos[3] = {0,0,0};
  float prev_pos[3] = {0,0,0};
  float fdbk_pos[3] = {0,0,0};
  while(terminate != 1){
	
	/*
	hebi::GroupFeedback feedback(num_modules);
	long timeout_ms = 1000;
	group->requestFeedback(&feedback, timeout_ms);
	
	for (int i=0; i<num_modules; i++){
		fdbk_pos[i] = feedback[i].actuator().position().get();
		//std::cout << "feedback: " << i << "  " << prev_pos[i] << std::endl;
	}
	hebi_sleep_ms(100);
	*/
	
	int sender = 0;
	std::ifstream file("../../shipbot/dist/devices/actuators/HEBI.txt");
	std::string str;
	std::string symb;
	char buf[50];
	file >> buf;
	file >> sender;
	if (sender == 1 && commandline == false){
		file >> terminate;
		if (terminate == 1){
			std::cout<<"arm terminated"<<std::endl;	
			break;
		}
		if (terminate == 0){
			file >> buf >> pos[0];
			file >> buf >> pos[1];
			file >> buf >> pos[2];
			//std::cout << terminate << pos[0] << pos[1] << pos[2] << std::endl;
			std::ofstream file("../../shipbot/dist/devices/actuators/HEBI.txt");
			file << "@ 0" << std::endl;
			file << terminate <<  std::endl;
			file << "s " << pos[0] << std::endl;
			file << "e " << pos[1] << std::endl;
			file << "h " << pos[2] << std::endl;
		}

	}
	
	if (commandline == true){
		std::cout << "Enter shoulder degree" << std::endl;
		std::cin >> pos[0];
		if (pos[0]>1000){
			terminate = 1;
			break;
		}	
		std::cout << "Enter elbow degree" << std::endl;
		std::cin >> pos[1];
		std::cout << "Enter hand degree" << std::endl;
		std::cin >> pos[2];
	}
	
	if (sender == 1 || commandline == true) {
					
		if (pos[2]>prev_pos[2]){
			command[2].actuator().velocity().set(vel);
		}
		else{
			command[2].actuator().velocity().set(-1*vel);
			//std::cout<< "negative velocity" << std::endl;
		}
			
		for (int i = 0; i<num_modules; i++){
			command[i].actuator().position().set(pos[i]/180*M_PI);
		}

		group->sendCommand(command);
	}
	
	for (int i = 0; i < num_modules; i++){
		prev_pos[i] = pos[i];
	}
	
  }

  return 0;
}

