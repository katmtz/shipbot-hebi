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

  int num_modules = group->size();

  // Create a group feedback object; this will be filled in during the request.
  hebi::GroupFeedback feedback(num_modules);

  // In a loop, send requests for feedback to the group and wait for responses.
  long timeout_ms = 1000;
  float period_s = 0.25f;
  for (int i = 0; i < 20; i++)
  {
    if (group->requestFeedback(&feedback, timeout_ms))
      print_group_feedback(feedback);
    else
      printf("Received no feedback from group!\n");
    hebi_sleep_ms(period_s * 1000);
  }

  // NOTE: destructors automatically clean up remaining objects
  return 0;
}
