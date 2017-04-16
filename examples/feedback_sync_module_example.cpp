/**
 * This file demonstrates the ability to get feedback from a module.
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
  // Try and get the requested module.
  std::unique_ptr<hebi::Module> module = getModuleFromArgs(argc, argv);
  if (!module)
  {
    std::cout << "No module found!" << std::endl;
    return -1;
  }

  // Create a feedback object; this will be filled in during the request.
  hebi::Feedback feedback;

  // In a loop, send requests for feedback to the module and wait for responses.
  long timeout_ms = 1000;
  float period_s = 0.25f;
  for (int i = 0; i < 20; i++)
  {
    if (module->requestFeedback(&feedback, timeout_ms))
      print_module_feedback(feedback);
    else
      printf("Received no feedback from module!\n");
    hebi_sleep_ms(period_s * 1000);
  }

  // NOTE: destructors automatically clean up remaining objects
  return 0;
}
