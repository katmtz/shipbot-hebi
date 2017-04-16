/**
 * This file demonstrates the ability to get feedback from a group.
 */

#include <stdio.h>
#include <math.h>

#include "lookup.hpp"
#include "mac_address.hpp"
#include "feedback_print_helpers.cpp"
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

  // Ensure there is some data coming from the modules:
  group->setFeedbackFrequencyHz(200);

  // Start logging:
  group->startLog();

  // Wait 10 seconds...
  int wait_period_s = 10;
  hebi_sleep_s(wait_period_s);

  // Stop logging:
  group->stopLog();

  // Stop the feedback loop
  group->setFeedbackFrequencyHz(0);

  // NOTE: destructors automatically clean up remaining objects
  return 0;
}
