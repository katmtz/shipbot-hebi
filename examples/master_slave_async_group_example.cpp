/**
 * This file demonstrates master-slave control from one module to another, with
 * the feedback loop handled by the API. There must be two modules in the group;
 * the first one controls the second.
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

  hebi::GroupCommand cmd(group->size());

  // Add a feedback handler to send feedback from one module to control the
  // other
  group->addFeedbackHandler(
    [&group, &cmd](const hebi::GroupFeedback* const feedback)->void
      {
        const auto& position_fbk = (*feedback)[0].actuator().position();
        if (position_fbk.has())
        {
          cmd[1].actuator().position().set(position_fbk.get());
          group->sendCommand(cmd);
        }
        else
        {
          std::cout << "No position feedback!" << std::endl;
        }
      });

  // Start feedback callbacks
  group->setFeedbackFrequencyHz(200);

  hebi_sleep_s(20); // In seconds

  // Stop the async callback before returning and deleting objects.
  group->setFeedbackFrequencyHz(0);
  group->clearFeedbackHandlers();

  // NOTE: destructors automatically clean up remaining objects
  return 0;
}
