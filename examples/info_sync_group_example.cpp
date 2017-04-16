/**
 * This file demonstrates the ability to get info from a group.
 */

#include <stdio.h>
#include <math.h>

#include "lookup.hpp"
#include "info.hpp"
#include "mac_address.hpp"
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

  // How many modules are in the group?
  int num_modules = group->size();

  // Create a group info object; this will be filled in during the request.
  hebi::GroupInfo info(num_modules);

  // In a loop, send requests for info to the group and wait for responses.
  long timeout_ms = 1000;
  float period_s = 0.25f;
  for (int i = 0; i < 20; i++)
  {
    if (group->requestInfo(&info, timeout_ms))
    {
      for (int mod_idx = 0; mod_idx < num_modules; mod_idx++)
      {
        std::cout << "Module index: " << mod_idx << std::endl;
        std::cout
          << "Name "
          << (info[mod_idx].settings().name().has() ? info[mod_idx].settings().name().get() : "n/a")
          << "| Position k_p "
          << (info[mod_idx].settings().actuator().positionGains().positionKp().has() ? info[mod_idx].settings().actuator().positionGains().positionKp().get() : NAN)
          << "| Velocity k_p "
          << (info[mod_idx].settings().actuator().velocityGains().velocityKp().has() ? info[mod_idx].settings().actuator().velocityGains().velocityKp().get() : NAN)
          << "| Control Strategy "
          << (info[mod_idx].settings().actuator().controlStrategy().has() ? info[mod_idx].settings().actuator().controlStrategy().get() : -1) << std::endl;
      }
    }
    else
    {
      std::cout << "Received no info from group!" << std::endl;
    }
    hebi_sleep_ms(period_s * 1000);
  }

  // NOTE: destructors automatically clean up remaining objects
  return 0;
}
