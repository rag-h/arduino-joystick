/**
 * This is a code stub for BLUEsat's orientation day. It provides a basis for code that runs on an arduino to take
 * inputs from sensors and use them to drive the BLUEtounge Rover.
 * @date: 24/02/2017
 * @author: Harry J.E Day
 */
#include <Arduino.h>

const int MAGIC_1 = 0xBEEF;
const int MAGIC_2 = 0xF00D;

/**
 * Struct that represents the commands given to all the rover's joints
 */
typedef struct _cmd {

    int MAGIC_1;

    // the direction we want the rover to drive as a vector
    // the units are in m/s
    struct _drive_vector {
        double x;
        double y;
    } drive_vector;

    // the arm's rotation (rads)
    double arm_rotation;
    // the arm's lower extension (m)
    double arm_lower_extension;
    // the arm's upper extension (m)
    double arm_upper_extension;

    // the claw's rotation speed (rad/s)
    double claw_rot_speed;
    //the claw's grip position (in pwm - get someone to explain this to you)
    double claw_grip_pwm;

    int MAGIC_2;
} Cmd;

typedef struct _to_arduino {
   int magic;
} To_Arduino;

/**
 * This is used as part of the serialisation process
 */
typedef struct _msg_adaptor {
    union {
        Cmd cmd;
        byte buffer[sizeof(Cmd)];
    };
} Msg_Adaptor;

Cmd joystick_loop();
void send_msg(Cmd msg);

/**
 * Code run when the arduino boots.
 */
void setup() {
    // start the serial driver
    Serial.begin(19200);
}

/**
 * Main loop of the arduino. Handles communication and calling joystick_loop.
 */
void loop() {
    byte MAGIC[2] = {0xBE, 0xEF};
    bool found_first = false;
    int bytes_read = 0;
    while(Serial.available() > 0) {
        char val = Serial.read();
        if(val == MAGIC[0]) {
          bytes_read = 0;
          found_first = true;
        }

        if(found_first) {
          if(bytes_read >=  sizeof(To_Arduino))  {
              break;
          }
        }
    }
    Cmd cmd;
    cmd = joystick_loop();
    cmd.MAGIC_1 = MAGIC_1;
    cmd.MAGIC_2 = MAGIC_2;
    send_msg(cmd);
}

/**
 * Implement your code in this function. Does all the sensor processing and provides commands for the rover
 * @return the commands for the rover's joints
 */
Cmd joystick_loop() {
    Cmd command;

    // Add you code here to control the rover
    command.drive_vector.x = 1.0;
    command.drive_vector.y = 1.0;

    return command;
}

/**
 * Don't modify this, uses the serial connection to send the command to the rover
 * @param msg the struct to send
 */
void send_msg(Cmd msg) {
    Msg_Adaptor adaptor;
    adaptor.cmd = msg;
    Serial.write(adaptor.buffer, sizeof(Cmd));
}



