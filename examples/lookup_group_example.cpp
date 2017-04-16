/**
 * This file demonstrates the group lookup capabilities of the API. The lookup
 * allows you to create groups of modules based on which ones are available on
 * the network.
 */

#include <stdio.h>
#include "lookup.hpp"
#include "mac_address.hpp"
#include "hebi_util.h"

/**
 * Simple helper function to print out if the group is valid, based on number of
 * modules (-1 for invalid group).
 */
void checkGroup(int num_modules)
{
  if (num_modules > 0)
    std::cout << "Found group with " << num_modules << " modules." << std::endl;
  else
    std::cout << "Group not found on network." << std::endl;
}

/**
 * Attempts to look for several groups on the network (hardcoded addresses below),
 * and retrieve references to them.
 */
int main()
{
  // Create the lookup object
  hebi::Lookup lookup;

  // Wait for the module list to populate, and print out its contents
  hebi_sleep_s(2);
  lookup.printTable();
  printf("\n");

  // Define module names/addresses:
  std::vector<hebi::MacAddress> macs;
  macs.emplace_back(hebi::MacAddress::fromBytes(0x00, 0x1e, 0xc0, 0x8d, 0xe0, 0x5c));
  macs.emplace_back(hebi::MacAddress::fromBytes(0x00, 0x1e, 0xc0, 0x8d, 0x79, 0xd8));

  std::vector<std::string> names;
  names.push_back("One");
  names.push_back("Two");

  std::vector<std::string> families;
  families.push_back("CommsTest");

  // Test out lookup methods; use a 4 second timeout.
  long timeout_ms = 4000;

  printf("Looking up group by MAC addresses.\n");
  std::unique_ptr<hebi::Group> group = lookup.getGroupFromMacs(macs, timeout_ms);
  checkGroup(group ? group->size() : -1);

  // NOTE: because we use unique_ptrs, this call frees any previous object and
  // takes ownership of the new one.  Memory leak prevention for the win!
  printf("Looking up group by name.\n");
  group = lookup.getGroupFromNames(names, families, timeout_ms);
  checkGroup(group ? group->size() : -1);

  printf("Looking up group by family.\n");
  group = lookup.getGroupFromFamily(families[0], timeout_ms);
  checkGroup(group ? group->size() : -1);

  printf("Looking up connected group by mac.\n");
  group = lookup.getConnectedGroupFromMac(macs[0], timeout_ms);
  checkGroup(group ? group->size() : -1);

  printf("Looking up connected group by name.\n");
  group = lookup.getConnectedGroupFromName(names[0], families[0], timeout_ms);
  checkGroup(group ? group->size() : -1);

  // NOTE: destructors automatically clean up objects
  return 0;
}
