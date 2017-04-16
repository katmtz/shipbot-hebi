/**
 * This file demonstrates the ability to command a module.
 */

#include <stdio.h>
#include <math.h>

#include "lookup.hpp"
#include "command.hpp"
#include "mac_address.hpp"
#include "lookup_helpers.cpp"
#include "hebi_util.h"

int main(int argc, char* argv[])
{
  // Try and get the requested module.
  std::unique_ptr<hebi::Module> module = getModuleFromArgs(argc, argv);
  if (!module)
  {
    printf("No module found!\n");
    return -1;
  }

  // Create a command object; this can be sent to the module
  hebi::Command command;

  // Send commands to the module in a loop.
  // Note that these packets may be dropped if network traffic is too high, so
  // be sure to close a feedback loop at the high level!
  float period = 0.5f;
  float t;
  for (t = 0.0f; t < 10.0f; t += period)
  {
    command.actuator().position().set(sin(t * 0.5f));
    module->sendCommand(command);
    hebi_sleep_ms(period * 1000);
  }

  // For critical packets, we can verify that they were sent by requesting
  // confirmation from the module.  If the acknowledgement function returns
  // 'true', it got positive confirmation. If it returns 'false', EITHER:
  // - the sent command was dropped
  // - the sent command was received by the module, but its response was either
  //   dropped or the timeout period expired before receipt of the module.
  // Again, a high-level process should intelligently handle these conditions!
  // Note that this is a blocking call, and so for high-frequency applications,
  // SendCommand should be used instead.
  period = 0.5f;
  long timeout_ms = 100;
  for (t = 0.0f; t < 10.0f; t += period)
  {
    command.actuator().position().set(sin(t * 0.5f));
    if (module->sendCommandWithAcknowledgement(command, timeout_ms))
    {
      printf("Got acknowledgement.\n");
    }
    else
    {
      printf("Did not receive acknowledgement!\n");
    }
    hebi_sleep_ms(period * 1000);
  }

  // NOTE: destructors automatically clean up group command and group
  return 0;
}
