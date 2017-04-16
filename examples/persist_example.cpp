/**
 * This file demonstrates the ability to command settings to a group.
 */

#include <iostream>
#include <math.h>

#include "lookup.hpp"
#include "group_command.hpp"
#include "command.hpp"
#include "mac_address.hpp"
#include "lookup_helpers.cpp"
#include "hebi_util.h"

int main(int argc, char* argv[])
{
  // Try and get the requested group.
  std::unique_ptr<hebi::Group> group = getGroupFromArgs(argc, argv);
  if (!group)
  {
    std::cout << "No group found!" << std::endl;
    return -1;
  }

  int num_modules = group->size();

  // Create a command object; this can be sent to the group
  hebi::GroupCommand command(num_modules);

  // Mark that settings should be persisted across reboots.
  command[0].settings().saveCurrentSettings().set();

  long timeout_ms = 100;
  for (int i = 0; i < 1000; i++)
  {
    if (i%3 == 0)
      command[0].led().setOverrideColor(hebi::Color(255, 0, 0));
    else if (i%3 == 1)
      command[0].led().setOverrideColor(hebi::Color(0, 0, 255));
    else if (i%3 == 2)
      command[0].led().setOverrideColor(hebi::Color(0, 255, 0));
    if (group->sendCommandWithAcknowledgement(command, timeout_ms))
      std::cout << "Got acknowledgement.\n" << std::endl;
    else
      std::cout << "Did not receive acknowledgement!\n" << std::endl;
    hebi_sleep_ms(1);
  }
  command[0].led().clear();
  group->sendCommandWithAcknowledgement(command, timeout_ms);

  // NOTE: destructors automatically clean up group command and group
  return 0;
}
