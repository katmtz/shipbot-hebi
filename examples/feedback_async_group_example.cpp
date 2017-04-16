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

// This is a callback function for the async feedback loop:
void feedback_callback(const hebi::GroupFeedback* const feedback)
{
  // Print out position of first module:
  const auto& position = (*feedback)[0].actuator().position();
  if (position.has())
    std::cout << position.get() << std::endl;
  else
    std::cout << "no position feedback!" << std::endl;
}

// This is a callback function for the async feedback loop using an external parameter
void feedback_callback_with_param(const hebi::GroupFeedback* const feedback, int* feedback_count)
{
  // Print out position of first module:
  const auto& position = (*feedback)[0].actuator().position();
  if (position.has())
    std::cout << position.get() << std::endl;
  else
    std::cout << "no position feedback!" << std::endl;

  // Increment feedback count:
  (*feedback_count)++;
}

int main(int argc, char* argv[])
{
  // Try and get the requested group.
  std::unique_ptr<hebi::Group> group = getGroupFromArgs(argc, argv);
  if (!group)
  {
    std::cout << "No group found!" << std::endl;
    return -1;
  }

  // Three examples of using C++11 functions to register feedback handlers and to
  // share data.
  // (Because these occur in a separate thread, use standard multi-threaded
  // programming practice to control access to the variables whilst avoiding
  // deadlocks)

  // See http://en.cppreference.com/w/cpp/utility/functional/function
  // Note: this could also wrap member functions via std::bind

  int total_feedback_calls = 0;
  // This calls an external (free) function:
  std::function<void(const hebi::GroupFeedback* const)> free_function = feedback_callback;
  group->addFeedbackHandler(free_function);
  // This calls an free function with an extra parameter
  group->addFeedbackHandler([&total_feedback_calls](const hebi::GroupFeedback* const fbk)->void
    { feedback_callback_with_param(fbk, &total_feedback_calls); });
  // This calls a lambda function:
  group->addFeedbackHandler(
    [&total_feedback_calls](const hebi::GroupFeedback* const feedback)->void
      {
        // Print out position of first module:
        const auto& position = (*feedback)[0].actuator().position();
        if (position.has())
          std::cout << position.get() << std::endl;
        else
          std::cout << "no position feedback!" << std::endl;

        // Increment feedback count:
        total_feedback_calls++;
      });

  // Start 200Hz feedback loop.
  printf("Starting asynchronous feedback callbacks\n");
  group->setFeedbackFrequencyHz(200);
  // Wait 10 seconds. This should result in about 2,000 callbacks (if you have
  // all handlers enabled above, this will result in about 4,000)
  int wait_period_s = 10;
  hebi_sleep_s(wait_period_s);
  // Stop the feedback loop, and unrelease our callback:
  group->setFeedbackFrequencyHz(0);
  group->clearFeedbackHandlers();

  printf("Feedback callback thread stopped; %d callbacks were made.\n", total_feedback_calls);

  // NOTE: destructors automatically clean up remaining objects
  return 0;
}
