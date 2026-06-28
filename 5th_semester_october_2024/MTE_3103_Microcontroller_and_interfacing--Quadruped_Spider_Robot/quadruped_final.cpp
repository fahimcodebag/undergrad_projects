#include <Servo.h>
#include <FlexiTimer2.h>

Servo servo[4][3];  // Array to hold servo objects for 4 legs, each with 3 servos

const int servo_pin[4][3] = { {2, 3, 4}, {5, 6, 7}, {8, 9, 10}, {11, 12, 13} };

const float LENGTH_A = 55.0f;  // Arm lengths in mm
const float LENGTH_B = 77.5f;
const float LENGTH_C = 27.5f;
const float SIDE_LENGTH = 71.0f;
const float Z_ABSOLUTE = -28.0f;
/* Constants to define movement settings */
const float Z_DEFAULT = -50.0f, Z_UP = -30.0f, Z_BOOT = Z_ABSOLUTE;
const float X_DEFAULT = 62.0f, X_OFFSET = 0.0f;
const float Y_START = 0.0f, Y_STEP = 40.0f;
/* Variables to manage movement */
volatile float current_position[4][3];  // Actual position of each leg
volatile float target_position[4][3];   // Desired position of each leg
float temp_speed[4][3];
float move_speed;
float speed_factor = 1.0f;  // Movement speed scaling factor
const float TURN_SPEED = 4.0f;
const float LEG_MOVE_SPEED = 8.0f;
const float BODY_MOVE_SPEED = 3.0f;
const float STAND_SEAT_SPEED = 1.0f;
volatile int rest_timer;     // Increments every 0.02s for rest calculation
// Constants for angles and lengths
const float PI = 3.1415926f;

/* Constants for turning */
const float TEMP_A = sqrt(pow(2 * X_DEFAULT + SIDE_LENGTH, 2) + pow(Y_STEP, 2));
const float TEMP_B = 2 * (Y_START + Y_STEP) + SIDE_LENGTH;
const float TEMP_C = sqrt(pow(2 * X_DEFAULT + SIDE_LENGTH, 2) + pow(2 * Y_START + Y_STEP + SIDE_LENGTH, 2));
const float ALPHA_ANGLE = acos((pow(TEMP_A, 2) + pow(TEMP_B, 2) - pow(TEMP_C, 2)) / (2 * TEMP_A * TEMP_B));
const float TURN_X1 = (TEMP_A - SIDE_LENGTH) / 2;
const float TURN_Y1 = Y_START + Y_STEP / 2;
const float TURN_X0 = TURN_X1 - TEMP_B * cos(ALPHA_ANGLE);
const float TURN_Y0 = TEMP_B * sin(ALPHA_ANGLE) - TURN_Y1 - SIDE_LENGTH;

/* Setup function */
void setup() {
    set_site(0, X_DEFAULT - X_OFFSET, Y_START + Y_STEP, Z_BOOT);
    set_site(1, X_DEFAULT - X_OFFSET, Y_START + Y_STEP, Z_BOOT);
    set_site(2, X_DEFAULT + X_OFFSET, Y_START, Z_BOOT);
    set_site(3, X_DEFAULT + X_OFFSET, Y_START, Z_BOOT);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            current_position[i][j] = target_position[i][j];
        }
    }

    FlexiTimer2::set(20, servo_service);
    FlexiTimer2::start();
    Serial.println("Servo service initialized");

    attach_servos();
    Serial.println("Servos attached");
    Serial.println("Robot initialization complete");
}

void attach_servos() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            servo[i][j].attach(servo_pin[i][j]);
            delay(100);
        }
    }
}

void detach_servos() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            servo[i][j].detach();
            delay(100);
        }
    }
}

void loop() {
    stand();
    delay(2000);

    step_forward(5);
    delay(2000);

    step_backward(5);
    delay(2000);

    turn_left(5);
    delay(2000);

    turn_right(5);
    delay(2000);

    wave_hand(3);
    delay(2000);

    shake_hand(3);
    delay(2000);

    sit();
    delay(5000);
}

void sit() {
    move_speed = STAND_SEAT_SPEED;
    for (int leg = 0; leg < 4; leg++) {
        set_site(leg, KEEP, KEEP, Z_BOOT);
    }
    wait_all_reach();
}

void stand() {
    move_speed = STAND_SEAT_SPEED;
    for (int leg = 0; leg < 4; leg++) {
        set_site(leg, KEEP, KEEP, Z_DEFAULT);
    }
    wait_all_reach();
}

void turn_left(unsigned int steps) {
    move_speed = TURN_SPEED;
    while (steps-- > 0) {
        if (current_position[3][1] == Y_START) {
            perform_turn(3, 1, TURN_X1, TURN_Y1, TURN_X0, TURN_Y0);
        } else {
            perform_turn(0, 2, TURN_X0, TURN_Y0, TURN_X1, TURN_Y1);
        }
    }
}

void turn_right(unsigned int steps) {
    move_speed = TURN_SPEED;
    while (steps-- > 0) {
        if (current_position[2][1] == Y_START) {
            perform_turn(2, 0, TURN_X0, TURN_Y0, TURN_X1, TURN_Y1);
        } else {
            perform_turn(1, 3, TURN_X1, TURN_Y1, TURN_X0, TURN_Y0);
        }
    }
}

void step_forward(unsigned int steps) {
    move_speed = LEG_MOVE_SPEED;
    while (steps-- > 0) {
        if (current_position[2][1] == Y_START) {
            leg_move(2, 1);
        } else {
            leg_move(0, 3);
        }
    }
}

void step_backward(unsigned int steps) {
    move_speed = LEG_MOVE_SPEED;
    while (steps-- > 0) {
        if (current_position[3][1] == Y_START) {
            leg_move(3, 0);
        } else {
            leg_move(1, 2);
        }
    }
}

void move_body_left(int increment) {
    adjust_body_position(increment, -increment);
}

void move_body_right(int increment) {
    adjust_body_position(-increment, increment);
}

void wave_hand(int times) {
    execute_hand_movement(times, TURN_X1, TURN_Y1, TURN_X0, TURN_Y0, move_body_right, move_body_left);
}

void shake_hand(int times) {
    execute_hand_movement(times, X_DEFAULT - 30, Y_START + 2 * Y_STEP, 55, 10, move_body_left, move_body_right);
}

void adjust_body_position(int offset1, int offset2) {
    set_site(0, current_position[0][0] + offset1, KEEP, KEEP);
    set_site(1, current_position[1][0] + offset1, KEEP, KEEP);
    set_site(2, current_position[2][0] + offset2, KEEP, KEEP);
    set_site(3, current_position[3][0] + offset2, KEEP, KEEP);
    wait_all_reach();
}

void execute_hand_movement(int cycles, float pos_x1, float pos_y1, float pos_z1, float pos_z2, void (*shift_left)(int), void (*shift_right)(int)) {
    move_speed = 1;
    float temp_x, temp_y, temp_z;
    if (current_position[3][1] == Y_START) {
        shift_left(15);
        save_current_position(2, temp_x, temp_y, temp_z);
        run_hand_cycle(cycles, 2, pos_x1, pos_y1, pos_z1, pos_z2);
        restore_position(2, temp_x, temp_y, temp_z);
        shift_right(15);
    } else {
        shift_right(15);
        save_current_position(0, temp_x, temp_y, temp_z);
        run_hand_cycle(cycles, 0, pos_x1, pos_y1, pos_z1, pos_z2);
        restore_position(0, temp_x, temp_y, temp_z);
        shift_left(15);
    }
}
