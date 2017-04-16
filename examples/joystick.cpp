// A simple class for handling joystick input

#include <fcntl.h>
#include "unistd.h"

struct JoyEvent
{
  private:
    static const char button_mask = 0x01;
    static const char axis_mask = 0x02;
  public:
    unsigned int timestamp;
    /** Axis value or 1/0 button state **/
    short value;
    /** Axis or Button **/
    unsigned char type;
    /** Axis or Button Number **/
    unsigned char number;
    bool isButton()
    {
      return (type & button_mask);
    }
    bool isAxis()
    {
      return (type & axis_mask);
    }
    bool isButton(int button_number)
    {
      return isButton() && number == button_number;
    }
    bool isAxis(int axis_number)
    {
      return isAxis() && number == axis_number;
    }
    /** Axis value from -1 to 1; only valid for axis types. */
    float scaledAxisValue()
    {
      return (float)value / (float)32768;
    }
};

class Joystick
{
  private:
    int joy_file_ = -1;

  public:
    Joystick()
    {
      joy_file_ = open("/dev/input/js0", O_RDONLY | O_NONBLOCK );
    }

    ~Joystick()
    {
      close(joy_file_);
    }

    bool isConnected()
    {
      return (joy_file_ > 0);
    }

    /**
     * Try and get a joystick event; return false if no event (or invalid event) is found.
     */
    bool getEvent(JoyEvent* evt)
    {
       int num_read = read(joy_file_, evt, sizeof(*evt));
       return (num_read == sizeof(*evt));
    }
};

