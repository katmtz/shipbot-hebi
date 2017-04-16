/**
 * This file demonstrates the module lookup capabilities of the API. The lookup
 * allows you to create modules based on which ones are available on the
 * network.
 */

#include <iostream>
#include "lookup.hpp"
#include "module.hpp"
#include <memory>
#include <chrono>
#include <thread>
// TODO: better mac address wrapper.  Shouldn't have to include anything in /include directory.
#include "hebi_mac_address.h"

template <class T>
void print_bank(const T& pin_bank, char bank_label)
{
  for (int pin = 1; pin <= 8; pin++)
  {
    if (pin_bank.hasFloat(pin))
      std::cout << "Pin " << bank_label << " " << pin << " float data: " << pin_bank.getFloat(pin) << std::endl;
    if (pin_bank.hasInt(pin))
      std::cout << "Pin " << bank_label << " " << pin << " int data: " << pin_bank.getInt(pin) << std::endl;
  }
}

int main()
{
  // Wait for 4 seconds for the lookup to find the module during the search
  long timeout_ms = 2000;

  // TODO: get name from arguments! See C API examples!
  // Get the lookup and wait to populate (TODO: check for null?)
  hebi::Lookup lookup;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  lookup.printTable();
  std::cout << std::endl;

  std::unique_ptr<hebi::Group> group(
    lookup.getGroupFromNames({"IO_BOARD"}, {"HEBI"}, timeout_ms));
  if (!group)
  {
    std::cout << "Module not found on network." << std::endl << std::endl;
    return -1;
  }

  int num_modules = group->size();

  group->addFeedbackHandler(
    [num_modules](const hebi::GroupFeedback* const fbk)->void
      {
        for (int mod = 0; mod < num_modules; mod++)
        {
          print_bank((*fbk)[mod].io().a(),'a');
          print_bank((*fbk)[mod].io().b(),'b');
          print_bank((*fbk)[mod].io().c(),'c');
          print_bank((*fbk)[mod].io().d(),'d');
          print_bank((*fbk)[mod].io().e(),'e');
          print_bank((*fbk)[mod].io().f(),'f');
        }
      });

  // Set the rate at which the callback gets called:
  group->setFeedbackFrequencyHz(4);

  // Wait 10 seconds:
  std::this_thread::sleep_for(std::chrono::seconds(10));

  // Stop the async callback before returning and deleting objects.
  group->clearFeedbackHandlers(); 

  std::this_thread::sleep_for(std::chrono::seconds(1)); // prevent segfaults? (TODO: needed?)

  return 0;
}
