/**
 * This file demonstrates master-slave control from one module to another.
 * There must be two modules in the group; the first one controls the second.
 */

#include "lookup.hpp"
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
  if (group->size() != 2)
  {
    std::cout << "Group must have two modules for master/slave control." << std::endl;
    return -1;
  }

  // In a loop, get position feedback from the first half of the group, and send
  // as commands to the second half.
  hebi::GroupCommand cmd(group->size());
  hebi::GroupFeedback fbk(group->size());
  
  int totalCycles = 10000;
  long timeout_ms = 100;
  for (int i = 0; i < totalCycles; i++)
  {
    group->requestFeedback(&fbk, timeout_ms);
    // Get actuator-specific feedback sub-object
    const auto& position_fbk = fbk[0].actuator().position();
    if (position_fbk.has())
    {
      cmd[1].actuator().position().set(position_fbk.get());
      group->sendCommand(cmd);
    }
    else
    {
      std::cout << "No position feedback!" << std::endl;
    }
    hebi_sleep_ms(1000);
  }

  // NOTE: destructors automatically clean up remaining objects
  return 0;
}
