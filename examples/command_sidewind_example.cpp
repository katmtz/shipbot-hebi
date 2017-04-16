/*
  This file demonstrates sends group commands to perform sidewinding and 
  turn-in-place gaits.

  example run command:
  ./bin/examples/command_sidewind_example -nSA013 -fSEA-Snake

  Tested on modules:
  'SA017','SA046','SA027','SA048','SA043','SA045','SA041','SA033','SA050',
  'SA018','SA036','SA003','SA026','SA031','SA020','SA013','SA025','SA015'

  TODO:
  Accept keyword argument to implement specified gaits
  Accept keyword argument to implement specified direction
  Accept keyword args to manually set key gait params
  Provide help menu that describes args
  Move this out of the example folder (possibly to a demo folder)
  Write a function to read / write module and control params to / from file
 */

#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#include "lookup.hpp"
#include "group_command.hpp"
#include "command.hpp"
#include "mac_address.hpp"
#include "lookup_helpers.cpp"
#include "hebi_util.h"


int main(int argc, char* argv[])
{
  // Try and get the requested group.
  std::unique_ptr<hebi::Group> group = getGroupFromArgs(argc, argv);
  if (!group)
  {
    printf("No group found!\n");
    return -1;
  }

  int num_modules = group->size();

  // Create a command object; this can be sent to the group
  hebi::GroupCommand command(num_modules);

  // Send commands to the group in a loop.
  // Note that these packets may be dropped if network traffic is too high, so
  // be sure to close a feedback loop at the high level!
  float period = 0.05f;
  long timeout_ms = 100;
  for (float t = 0.0f; t < 30.0f; t += period)
  {
    // Set the actuator command position field for all the modules
    int sign_e = 0; // for SEA snake modules wind helically at pi/2 increm., 
    int sign_o = 0; // so switch the sign of the amplitude every other mod.
    for (int module_index = 0; module_index < num_modules; module_index++)
    {
      // TODO: add switch statement for gait cases below
      // Spatial frequency of the wave:
      /* case: sidewinding */
      double spat_freq_e = 2*3.14159/20.0;
      double spat_freq_o = spat_freq_e;
      /* case: turn in place */
      /* double spat_freq_e = 2*3.14159/20.0; */
      /* double spat_freq_o = 0.8*spat_freq_e; */
      /**/
      double temp_freq = 0.25;
      // Temporal phase offset between horizontal and vertical waves.
      /* case: sidewind left */ 
      double TPO = M_PI_2; // for sidewinding left
      /* case: sidewind right */ 
      /* double TPO = 3.0*M_PI_2+.2; // for sidewinding right */

      // First, do even modules
      double amplitude_e = 0.9;
      double amplitude_o = 0.4;
      int dir = 1;    // direction of wave propagation

      double position = 0;
      if (module_index%2 == 0)
      {
        position = pow(-1,sign_e) * amplitude_e * sin(2.0*M_PI*(dir*temp_freq*t + module_index*spat_freq_e));
        sign_e++;
      }
      else
      {
        position = pow(-1,sign_o) * amplitude_o * sin(2.0*M_PI*(dir*temp_freq*t + TPO + module_index*spat_freq_o));
        sign_o++;
      }
      command[module_index].actuator().position().set(position);
    }

    if (group->sendCommandWithAcknowledgement(command, timeout_ms))
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
