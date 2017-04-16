/**
 * This file demonstrates the module lookup capabilities of the API. The lookup
 * allows you to create modules based on what ones are available on the network.
 */

#include <stdio.h>
#include "lookup.hpp"
#include "mac_address.hpp"
#include "hebi_util.h"

/**
 * Attempts to look for two modules on the network (hardcoded addresses below),
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

  // Test out lookup methods; use a 4 second timeout.
  long timeout_ms = 4000;

  printf("Looking up module by MAC address.\n");
  hebi::MacAddress mac = hebi::MacAddress::fromBytes(0x00, 0x1e, 0xc0, 0x8d, 0xe0, 0x5c);
  std::unique_ptr<hebi::Module> module = lookup.getModuleFromMac(mac, timeout_ms);
  std::cout << (module ? "Found module." : "Module not found on network.") << std::endl;

  printf("Looking up module by name.\n");
  module = lookup.getModuleFromName("SA034", "arm", timeout_ms);
  std::cout << (module ? "Found module." : "Module not found on network.") << std::endl;

  // NOTE: destructors automatically clean up objects
  return 0;
}
