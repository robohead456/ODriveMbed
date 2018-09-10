#include "OdriveMbed.h"
#include <mbed.h>

Serial pc(USBTX, USBRX);
Serial odrive_serial(odriveTx,odriveRx);
ODriveMbed odrive(odrive_serial);

int main()
{
    wait(5);
    pc.baud(115200);
    pc.printf("ODriveArduino\n");
    pc.printf("Setting parameters...\n");

    /* Already verified that these commands print out to serial if
       odrive takes in pc serial object
    
    odrive.SetPosition(0,2000.0f);
    odrive.SetVelocity(1,45.0f);

    odrive.run_state(0,odrive.AXIS_STATE_FULL_CALIBRATION_SEQUENCE,1);
    odrive.run_state(0,odrive.AXIS_STATE_ENCODER_INDEX_SEARCH,0);
    
    */

    pc.printf("ODriveArduino\n");
    pc.printf("Setting parameters...\n");

    // In this example we set the same parameters to both motors.
    // You can of course set them different if you want.
    // See the documentation or play around in odrivetool to see the available parameters
    for (int axis = 0; axis < 2; ++axis)
    {
        // TODO: rewrite the library to include all these states in the enumeration instead of manually sending them over serial
        odrive_serial.printf("w axis%d.controller.config.vel_limit %f\n",axis, 22000.0f;
        odrive_serial.printf("w axis%d.motor.config.current_lim %f\n",axis, 11.0f;
        // odrive_serial << "w axis" << axis << ".motor.config.current_lim " << 11.0f << '\n';
        // This ends up writing something like "w axis0.motor.config.current_lim 10.0\n"
    }

    pc.printf("Ready!");
    pc.printf("Send the character '0' or '1' to calibrate respective motor (you must do this before you can command movement)");
    pc.printf("Send the character 's' to exectue test move");
    pc.printf("Send the character 'b' to read bus voltage");
    pc.printf("Send the character 'p' to read motor positions in a 10s loop");

    // Ported arduino example code up to here, start here upon return
    if (pc.available())
    {
        char c = pc.read();

        // Run calibration sequence
        if (c == '0' || c == '1')
        {
            int motornum = c - '0';
            int requested_state;

            requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            odrive.run_state(motornum, requested_state, true);

            requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            odrive.run_state(motornum, requested_state, true);

            requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            odrive.run_state(motornum, requested_state, false); // don't wait
        }

        // Sinusoidal test move
        if (c == 's')
        {
            pc.printf("Executing test move");
            for (float ph = 0.0f; ph < 6.28318530718f; ph += 0.01f)
            {
                float pos_m0 = 20000.0f * cos(ph);
                float pos_m1 = 20000.0f * sin(ph);
                odrive.SetPosition(0, pos_m0);
                odrive.SetPosition(1, pos_m1);
                delay(5);
            }
        }

        // Read bus voltage
        if (c == 'b')
        {
            odrive_serial << "r vbus_voltage\n";
            Serial << "Vbus voltage: " << odrive.readFloat() << '\n';
        }

        // print motor positions in a 10s loop
        if (c == 'p')
        {
            static const unsigned long duration = 10000;
            unsigned long start = millis();
            while (millis() - start < duration)
            {
                for (int motor = 0; motor < 2; ++motor)
                {
                    odrive_serial << "r axis" << motor << ".encoder.pos_estimate\n";
                    Serial << odrive.readFloat() << '\t';
                }
                Serial << '\n';
            }
        }
    }
}
