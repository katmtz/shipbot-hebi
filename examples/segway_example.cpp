/**
 * This file demonstrates a simple segway robot control.
 */

#include <stdio.h>
#include <math.h>
#include <signal.h> // For signal catching in Linux
#include <stdlib.h> // For signal catching in Linux

#include "lookup.hpp"
#include "group_command.hpp"
#include "joystick.cpp"
#include "hebi_util.h"

bool handled_ctrl_c = false;

void my_handler(int s)
{
  std::cout << "Caught signal " << s << std::endl;
  handled_ctrl_c = true;
}

int main(int argc, char* argv[])
{
  Joystick joystick;

  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = my_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);

  // Get the groups:
  hebi::Lookup lookup;
  std::unique_ptr<hebi::Group> sense = lookup.getConnectedGroupFromName("Sensor", "MattTest");
  std::vector<std::string> wheel_names;
  wheel_names.push_back("Left");
  wheel_names.push_back("Right");
  std::vector<std::string> wheel_families;
  wheel_families.push_back("MattTest");
  std::unique_ptr<hebi::Group> wheels = lookup.getGroupFromNames(wheel_names, wheel_families);

  if (!sense || !wheels)
  {
    std::cout << "Modules not found:";
    if (!sense)
      std::cout << " (sensing module)";
    if (!wheels)
      std::cout << " (wheel modules)";
    std::cout << std::endl;
    return -1;
  }

  // Create a command object; this can be sent to the group
  hebi::GroupCommand wheel_commands(2);

  // reasonable starting points: -20, 0, -1, 0, -.1 for torque // and maybe -.01 for torque?
  float k1 = -15;
  float k1i = -0.01;//-.01;
  float k2 = -3; // -10 is WAY too big.
  float k3 = .05;
  float k4 = .3;
  float tilt_offset = -0.04;

  float tilt = 0;
  float tilt_vel = 0;
  float wheel = 0;
  float wheel_vel = 0;

  bool has_bias = false;
  int bias_samples = 0;
  int total_bias_samples = 100;
  float gyro_bias = 0;

  bool has_wheel_start = false;
  float init_wheel = 0;

  // Set up feedback
  float feedback_rate_hz = 100;
  float dt = 1.0 / feedback_rate_hz;
  // Alpha for complementary filter
  float comp_alpha = 0.005;

  sense->addFeedbackHandler(
    [&tilt, &tilt_vel, comp_alpha, dt, &has_bias, &bias_samples, &total_bias_samples, &gyro_bias](const hebi::GroupFeedback* feedback)->void
      {
        hebi::Vector3f accel = (*feedback)[0].imu().accelerometer().get();
        float raw_tilt = atan2(-accel.getX(), -accel.getY());
        hebi::Vector3f gyro = ((*feedback)[0]).imu().gyro().get();
        tilt = (1 - comp_alpha) * (tilt + gyro.getZ() * dt) + comp_alpha * raw_tilt;
        // Low pass tilt velocity.
        if (!has_bias)
        {
          if (bias_samples < total_bias_samples)
          {
            bias_samples++;
            gyro_bias += gyro.getZ();
          }
          else
          {
            gyro_bias /= (float)(bias_samples);
            has_bias = true;
          }
        }
        else
        {
          tilt_vel = (tilt_vel * 0.9 + gyro.getZ() * 0.1);
        }
      });
  wheels->addFeedbackHandler(
    [&wheel, &wheel_vel, &init_wheel, &has_wheel_start](const hebi::GroupFeedback* const feedback)->void
      {
        float tmp_wheel = ((*feedback)[0]).actuator().position().get() * 0.5 -
                          ((*feedback)[1]).actuator().position().get() * 0.5;
        wheel = wheel * 0.9 + tmp_wheel * 0.1;
        wheel_vel = wheel_vel * 0.9 + 0.1 * (
                    ((*feedback)[0]).actuator().velocity().get() * 0.5 -
                    ((*feedback)[1]).actuator().velocity().get() * 0.5);
        if (!has_wheel_start)
        {
          has_wheel_start = true;
          init_wheel = tmp_wheel;
        }
      });

  sense->setFeedbackFrequencyHz(feedback_rate_hz);
  wheels->setFeedbackFrequencyHz(feedback_rate_hz);
  sleep(2);

  float control_dt_s = 0.01; // 100 Hz

  float tilt_i = 0;
  float tilt_cap = 0;//40;

  // Joystick control values
  // Axis 1: up/down, left joystick
  // Axis 2: left/right, right joystick
  JoyEvent joy_cmd;
  float turn_cmd = 0;
  float position_change_cmd = 0;
  float position_set_point = init_wheel;

  while(!handled_ctrl_c)
  {
    if (joystick.isConnected())
    {
      if (joystick.getEvent(&joy_cmd))
      {
        if (joy_cmd.isAxis(0))
        {
          float joy_value = joy_cmd.scaledAxisValue();
          if (fabs(joy_value) > 0.5)
          {
            turn_cmd = -.4 * joy_cmd.scaledAxisValue();
          }
          else
          {
            turn_cmd = 0;
          }
        }
        else if (joy_cmd.isAxis(1))
        {
          float joy_value = joy_cmd.scaledAxisValue();
          if (fabs(joy_value) > 0.5)
          {
            position_change_cmd = -.1 * joy_value;
          }
          else
          {
            position_change_cmd = 0;
          }
        }
      }
      position_set_point += position_change_cmd;
    }

    // Accumulate an i term.
    tilt_i += (tilt + tilt_offset);
    tilt_i = std::min(tilt_cap, tilt_i);
    tilt_i = std::max(-tilt_cap, tilt_i);
    //std::cout << "tilt_i: " << tilt_i << std::endl;

//    std::cout << "pos_error: " << (tilt + tilt_offset) << std::endl;
//    std::cout << "gyro: " << (tilt_vel - gyro_bias) << std::endl;
//    std::cout << "wheel: " << (wheel - init_wheel) << std::endl;
//    std::cout << "wheel_vel: " << (wheel_vel) << std::endl;
    float input = k1 * (tilt + tilt_offset) +
                  k1i * tilt_i + 
                  k2 * (tilt_vel - gyro_bias) +
                  k3 * (wheel - position_set_point) +
                  k4 * wheel_vel;
    float scale = .3;
    wheel_commands[0].actuator().torque().set((input - turn_cmd) * scale);
    wheel_commands[1].actuator().torque().set(-(input + turn_cmd) * scale);
    wheels->sendCommand(wheel_commands);
    hebi_sleep_ms(control_dt_s * 1000);
  }

  sense->setFeedbackFrequencyHz(0);
  wheels->setFeedbackFrequencyHz(0);
  sense->clearFeedbackHandlers();
  wheels->clearFeedbackHandlers();

  wheel_commands[0].actuator().torque().set(0);
  wheel_commands[1].actuator().torque().set(0);
  if (!(wheels->sendCommandWithAcknowledgement(wheel_commands)))
    std::cout << "Could not stop wheels! We've gone off the rails on a crazy train!" << std::endl;

  std::cout << "Cleaned up." << std::endl;

  return 0;    
}
