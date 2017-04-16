/**
 * This file demonstrates the group lookup capabilities of the API. The lookup
 * allows you to create groups of modules based on which ones are available on
 * the network.
 */

#include "lookup.hpp"
#include "mac_address.hpp"
#include "hebi_util.h"
#include <iostream>

/**
 * Attempts to look for several groups on the network (hardcoded addresses below),
 * and retrieve references to them.
 */
int main()
{
  // Create the lookup object
  hebi::Lookup lookup;

  // Wait for the module list to populate, and print out its contents
  hebi_sleep_s(4);

  auto entries = lookup.getEntryList();
  int num_entries = entries->size();
  std::cout << "entries " << num_entries << std::endl;
  for (int i = 0; i < num_entries; ++i)
  {
    auto entry = entries->getEntry(i);
    auto mac = entry.mac_address_;
    std::cout << entry.name_ << " " << entry.family_ << " " << 
      std::hex << static_cast<int>(mac[0]) << ":" << std::hex << static_cast<int>(mac[1]) << ":" <<
      std::hex << static_cast<int>(mac[2]) << ":" << std::hex << static_cast<int>(mac[3]) << ":" <<
      std::hex << static_cast<int>(mac[4]) << ":" << std::hex << static_cast<int>(mac[5]) << std::endl;
  }

  // NOTE: destructors automatically clean up objects
  return 0;
}
