#include "OdriveMbed.h"
#include <mbed.h>

Serial pc(USBTX,USBRX);

ODriveMbed odriveLaptopTest(pc); 

int main()
{
    wait(5);
    pc.baud(9600);
    pc.printf("ODriveArduino\n");
    pc.printf("Setting parameters...\n");

    odriveLaptopTest.SetPosition(0,2000.0f);
    odriveLaptopTest.SetVelocity(1,45.0f);

    odriveLaptopTest.run_state(0,odriveLaptopTest.AXIS_STATE_FULL_CALIBRATION_SEQUENCE,1);

}
