
#include "/Users/SeanHome/Projects/OdriveMbed/src/ODriveMbed.h"

static const int kMotorOffsetFloat = 2;
static const int kMotorStrideFloat = 28;
static const int kMotorOffsetInt32 = 0;
static const int kMotorStrideInt32 = 4;
static const int kMotorOffsetBool = 0;
static const int kMotorStrideBool = 4;
static const int kMotorOffsetUint16 = 0;
static const int kMotorStrideUint16 = 2;


ODriveMbed::ODriveMbed(Stream& serial)
    : serial_(serial) {}

void ODriveMbed::SetPosition(int motor_number, float position) {
    SetPosition(motor_number, position, 0.0f, 0.0f);
}

void ODriveMbed::SetPosition(int motor_number, float position, float velocity_feedforward) {
    SetPosition(motor_number, position, velocity_feedforward, 0.0f);
}

void ODriveMbed::SetPosition(int motor_number, float position, float velocity_feedforward, float current_feedforward) {
    //serial_ << "p " << motor_number  << " " << position << " " << velocity_feedforward << " " << current_feedforward << "\n";
    serial_.printf("p %d %f %f %f\n", motor_number, position, velocity_feedforward, current_feedforward);
}

void ODriveMbed::SetVelocity(int motor_number, float velocity) {
    SetVelocity(motor_number, velocity, 0.0f);
}

void ODriveMbed::SetVelocity(int motor_number, float velocity, float current_feedforward) {
    //serial_ << "v " << motor_number  << " " << velocity << " " << current_feedforward << "\n";
    serial_.printf("v%d %f %f\n", motor_number, velocity, current_feedforward);
}

float ODriveMbed::readFloat() {
    return atof(readString().c_str());
}

int32_t ODriveMbed::readInt() {
    return atoi(readString().c_str());
}

bool ODriveMbed::run_state(int axis, int requested_state, bool read_) {
    int timeout_ctr = 100;
    //serial_ << "w axis" << axis << ".requested_state " << requested_state << '\n';
    serial_.printf("w axis%d.requested_state %d\n", axis, requested_state);
    if (read_) {
        do {
            wait_ms(100);
            //serial_ << "r axis" << axis << ".current_state\n";
            serial_.printf("r axis%d.current_state\n",axis);
        } while (readInt() != AXIS_STATE_IDLE && --timeout_ctr > 0);
    }

    return timeout_ctr > 0;
}

string ODriveMbed::readString() {
    string str = "";
    static const unsigned int timeout = 1000;
    Timer t;
    t.start();
    unsigned int timeout_start = t.read_ms();
    for (;;) {
        while (!serial_.readable()) {
            if (t.read_ms() - timeout_start >= timeout) {
                return str;
            }
        }
        char c = serial_.getc();
        if (c == '\n')
            break;
        str += c;
    }
    t.stop();
    return str;
}
