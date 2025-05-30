#include "main.h"

#include "../../PID_Tuner/GATR_Tuner.cpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 90;
const int SWING_SPEED = 110;

void DrivePID_Tune();

// pros::Task Pid_LoggingTask([]() -> void{
//   Pid_LoggingOperation_TASK(chassis.pid_drive_constants_get(), []() -> double {
//     return chassis.odom_pose_get().y;
//   },20);
// });


///
// Constants
///
void default_constants() {
  // P, I, D, and Start I
  chassis.pid_drive_constants_set(10, 0, 5);         // Fwd/rev constants, used for odom and non odom motions
  chassis.pid_heading_constants_set(11.0, 0.0, 20.0);        // Holds the robot straight while going forward without odom
  chassis.pid_turn_constants_set(3.0, 0.025, 20.0, 15);     // 3.0, 0.025, 20.0, 15
  chassis.pid_swing_constants_set(6.0, 0.0, 65.0);           // Swing constants
  chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular control for odom motions | 6.5, 0.0, 52.5
  chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

  // Exit conditions
  chassis.pid_turn_exit_condition_set(90_ms, 1_deg, 250_ms, 4_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(90_ms*2, 0.5_in, 250_ms, .25_in, 500_ms, 500_ms);// DONT EDIT THIS
  chassis.pid_odom_turn_exit_condition_set(90_ms, 1_deg, 250_ms, 7_deg, 500_ms, 750_ms); // 90_ms, 1_deg, 250_ms, 7_deg, 500_ms, 750_ms
  chassis.pid_odom_drive_exit_condition_set(90_ms*2, 0.5_in, 500_ms, 1_in, 500_ms, 500_ms);
  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  // Slew constants
  chassis.slew_turn_constants_set(3_deg, 70);
  chassis.slew_drive_constants_set(3_in, 70);
  chassis.slew_swing_constants_set(3_in, 80);

  // The amount that turns are prioritized over driving in odom motions
  // - if you have tracking wheels, you can run this higher.  1.0 is the max
  chassis.odom_turn_bias_set(0.9);

  chassis.odom_look_ahead_set(7_in);           // This is how far ahead in the path the robot looks at
  chassis.odom_boomerang_distance_set(16_in);  // This sets the maximum distance away from target that the carrot point can be
  chassis.odom_boomerang_dlead_set(0.625);     // This handles how aggressive the end of boomerang motions are

  chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}

void GATR_TurnAndMove(ez::pose _pose, ez::drive_directions _dir, bool _waitLast = true, int _turnSpeed=TURN_SPEED/2, int _driveSpeed=DRIVE_SPEED){
  chassis.pid_turn_set(_pose, _dir,_turnSpeed);
  chassis.pid_wait();
  chassis.pid_odom_set({_pose, _dir, _driveSpeed},true);
  if(_waitLast) chassis.pid_wait();
}

int SMALLDIST_SPEED = 110/4; // Originally for 5 inch movements
int TURN_PRECISION_SPEED = 110/2; // Useful for 180 degree movements or when precision is needed

void DrivePID_Tune_20(){
  chassis.pid_odom_set({{0, 60}, fwd, DRIVE_SPEED}, true);
  chassis.pid_wait();
}
void DrivePID_Tune_10(){
  chassis.pid_odom_set({{0, 10}, fwd, DRIVE_SPEED}, true);
  chassis.pid_wait();
}
void DrivePID_Tune_5(){
  chassis.pid_odom_set({{0, 5}, fwd, DRIVE_SPEED}, true);
  chassis.pid_wait();
}

void TurnPID_Tune_90(){
  chassis.pid_turn_set(90, TURN_SPEED);
  chassis.pid_wait();
}
void TurnPID_Tune_LeftPoint(){
  double angle = atan2(-24,0)*180/M_PI;
  
  chassis.pid_turn_set(angle, TURN_SPEED);
  chassis.pid_wait();
}
void TurnPID_Tune_180(){
  chassis.pid_turn_set(180, TURN_PRECISION_SPEED);
  chassis.pid_wait();
}

void OuterRush(){
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);    // Set the current position, you can start at a specific position with this
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);  // Set motors to hold.  This helps autonomous consistency


  // GATR_TurnAndMove({.02, 12}, fwd); // Drive to close
  chassis.pid_drive_constants_set(30, 0, 5);         // Fwd/rev constants, used for odom and non odom motions 
  chassis.pid_drive_set(15,DRIVE_SPEED); // Run away with our goal
  chassis.pid_wait_until(13); // 9.25

  default_constants();

  doinker_pistion.set_value(true);
  chassis.pid_swing_set(LEFT_SWING,25,SWING_SPEED); // Swing and grab the goal
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency
  chassis.pid_wait_quick();
  
  chassis.pid_drive_set(-10.5,DRIVE_SPEED); // Run away with our goal
  chassis.pid_wait();
  

  // chassis.pid_odom_set(3.7, 50); // bump the goal a little initial 4, second 3.5
  // chassis.pid_wait();
  // chassis.pid_swing_set(RIGHT_SWING, -17.20, 35); // Move it with intake
  // chassis.pid_wait();
  // doinker_pistion.set_value(false);
  // chassis.pid_odom_set(3, 50); // push forward a bit
  // chassis.pid_wait();


  // chassis.pid_odom_set(-7, 50); // Back up from goal to get away
  // chassis.pid_wait_quick();


  // chassis.pid_turn_relative_set(90,50); // turn 90 to goal
  // chassis.pid_wait();
  // chassis.pid_odom_set(3, 127); // move forward to allign a tiny bit
  // chassis.pid_wait();
  // chassis.pid_turn_relative_set(90,50); // finish 180 turn
  // chassis.pid_wait();
  // chassis.pid_drive_constants_set(30, 0, 5);
  // chassis.pid_odom_set(-10, 100);
  // chassis.pid_wait();
  // default_constants();

  // pros::delay(250);
  // clamp_piston.set_value(true); // Clamp our goal!
  // pros::delay(250);
  // chassis.pid_odom_set(5, 127); // push forward a bit
  // chassis.pid_wait();

  // // Give the clamp a chance!
  // conveyor_motor.move(127);
  // intake_motor.move(127);

  // pros::delay(5000);
  // conveyor_motor.move(0);
  // intake_motor.move(0);





  // clamp_piston.set_value(false); // release goal

  // doinker_pistion.set_value(true);

  // // corner clear
  // chassis.pid_turn_set(103.34, fwd, 50);
  // chassis.pid_wait();
  // pros::delay(2000);
  // chassis.pid_odom_set(30,75);
  // chassis.pid_wait();
  // pros::delay(2000);
  // chassis.pid_turn_set(200.4,50); // x:160.4
  // chassis.pid_wait();
  // pros::delay(2000);

  // Touch the ladder
  GATR_TurnAndMove({-12,10.21}, fwd);
  chassis.pid_drive_set(100,DRIVE_SPEED); // Run away with our goal
  chassis.pid_wait();
  ladybrown_motors.move(50);
  pros::delay(1000);


  // chassis.pid_drive_set(15,75);
  // chassis.pid_wait();

  //GATR_TurnAndMove({17,10.5}, fwd); // get out da way

  // chassis.pid_drive_set(5,75);
  // chassis.pid_wait();
  // chassis.pid_turn_relative_set(90,50);
  // chassis.pid_wait();
  // chassis.pid_turn_relative_set(90,50);
  // chassis.pid_wait();
  // chassis.pid_drive_set(10,50);
  // chassis.pid_wait();
  pros::delay(250);
}


void BlueOuterRush(){
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);    // Set the current position, you can start at a specific position with this
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);  // Set motors to hold.  This helps autonomous consistency


  // GATR_TurnAndMove({.02, 12}, fwd); // Drive to close
  chassis.pid_drive_constants_set(30, 0, 5);         // Fwd/rev constants, used for odom and non odom motions 
  chassis.pid_odom_set({{.015, 15.5}, fwd, 127});
  chassis.pid_wait_until(13.2); 

  default_constants();

  doinker_pistion.set_value(true);
  chassis.pid_swing_set(LEFT_SWING,30,SWING_SPEED); // Swing and grab the goal
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency
  chassis.pid_wait_quick();
  
  chassis.pid_drive_set(-18,DRIVE_SPEED); // Run away with our goal
  chassis.pid_wait();


  chassis.pid_odom_set(3.5, 50); // bump the goal a little initial 4, second 3.5
  chassis.pid_wait();
  chassis.pid_turn_relative_set(-40,50); // turn to remove
  chassis.pid_wait();
  doinker_pistion.set_value(false);
  chassis.pid_odom_set(-8.5, 127); // back up
  chassis.pid_wait();

  // Touch the ladder
  GATR_TurnAndMove({10.5,12.8}, fwd);

  ladybrown_motors.move(50);
  pros::delay(1000);
 
  // chassis.pid_odom_set(-7, 50); // Back up from goal to get away
  // chassis.pid_wait_quick();


  // chassis.pid_turn_relative_set(90,50); // turn 90 to goal
  // chassis.pid_wait();
  // chassis.pid_odom_set(3, 127); // move forward to allign a tiny bit
  // chassis.pid_wait();
  // chassis.pid_turn_relative_set(90,50); // finish 180 turn
  // chassis.pid_wait();
  // chassis.pid_drive_constants_set(30, 0, 5);
  // chassis.pid_odom_set(-10, 100);
  // chassis.pid_wait();
  // default_constants();

  // pros::delay(250);
  // clamp_piston.set_value(true); // Clamp our goal!
  // pros::delay(250);
  // chassis.pid_odom_set(5, 127); // push forward a bit
  // chassis.pid_wait();

  // // Give the clamp a chance!
  // conveyor_motor.move(127);
  // intake_motor.move(127);

  // pros::delay(5000);
  // conveyor_motor.move(0);
  // intake_motor.move(0);
  // // clamp_piston.set_value(false); // release goal




  pros::delay(1500); 
}

void oldRoute(){
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);    // Set the current position, you can start at a specific position with this
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);  // Set motors to hold.  This helps autonomous consistency


  // GATR_TurnAndMove({.02, 12}, fwd); // Drive to close
  chassis.pid_drive_constants_set(30, 0, 5);         // Fwd/rev constants, used for odom and non odom motions 
  chassis.pid_odom_set({{.02, 12}, fwd, 127});
  chassis.pid_wait_until(9.25);

  default_constants();

  doinker_pistion.set_value(true);
  chassis.pid_swing_set(LEFT_SWING,25,SWING_SPEED); // Swing and grab the goal
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency
  chassis.pid_wait_quick();
  
  chassis.pid_drive_set(-15,DRIVE_SPEED); // Run away with our goal
  chassis.pid_wait();

  chassis.pid_odom_set(3, DRIVE_SPEED); // Move a bit to unlock
  chassis.pid_wait();
  chassis.pid_turn_set(-7.5, TURN_SPEED); // Turn the key to unlock
  chassis.pid_wait();
  chassis.pid_turn_set(45, TURN_SPEED); // Turn the key to unlock
  chassis.pid_wait();
  // chassis.pid_turn_set(-15, TURN_SPEED); // Turn the key to unlock
  // chassis.pid_wait();
  doinker_pistion.set_value(false);
  chassis.pid_odom_set(-5, DRIVE_SPEED, true);
  chassis.pid_wait();

  // GATR_TurnAndMove({-7.74, 4.16}, rev);
  // chassis.pid_odom_set(-5, DRIVE_SPEED); // Move a bit to unlock
  // chassis.pid_wait();
  // clamp_piston.set_value(true); // Clamp our goal!
  // chassis.pid_odom_set(5, DRIVE_SPEED); // Move a bit to unlock
  // chassis.pid_wait();
  // conveyor_motor.move(127); // Run our conveyor
  // pros::delay(2000);
  // clamp_piston.set_value(false);
}


void SwingPID_Tune(){
  chassis.pid_swing_set(LEFT_SWING, 90_deg, SWING_SPEED);
  chassis.pid_wait();
}

void Odom_TurnAndDrive(ez::odom movement, int turnSpeed, int driveSpeed, bool turnSlew = true, bool driveSlew = true){
  // Turn
  vector target = {movement.target.x, movement.target.y};
  printf("X GATR target is {%.3f,%.3f}\n",target[0], target[1]);
  vector currentPos = {chassis.odom_pose_get().x, chassis.odom_pose_get().y};
  printf("X GATR current pos is {%.3f,%.3f}\n",currentPos[0], currentPos[1]);

  vector point = {target[0]-currentPos[0], target[1]-currentPos[1]};
  printf("X GATR calculated vector is {%.3f,%.3f}\n",point[0], point[1]);


  double angle = atan2(point[0], point[1])*180/M_PI;
  printf("X GATR turning to %.3f\n",angle);
  chassis.pid_turn_set(angle, turnSpeed, turnSlew);
  chassis.pid_wait();

  // Drive
  printf("X GATR moving to {%.3f, %.3f}\n",movement.target.x,movement.target.y);
  chassis.pid_drive_set(sqrt(pow(point[0],2)+pow(point[0],2)), driveSpeed);
  chassis.pid_wait();
}

void TurnBiasPID_Tune(){
  Odom_TurnAndDrive({{24, 24}, fwd}, TURN_SPEED, DRIVE_SPEED);

  // chassis.pid_turn_set({-24,0}, fwd, TURN_SPEED);
  // chassis.pid_wait();
}

void DrivePID_Tune(){
  chassis.pid_odom_set({{0, 5}, fwd, SMALLDIST_SPEED}, true);
  chassis.pid_wait();
}

///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater than the slew distance + a few inches

  chassis.pid_drive_set(2_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-2_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Turn Example
///
void turn_example() {
  // The first parameter is the target in degrees
  // The second parameter is max speed the robot will drive at

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(24_in, 30, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(-24_in, 30, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is the target in degrees
  // The third parameter is the speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
  // Motion chaining is where motions all try to blend together instead of individual movements.
  // This works by exiting while the robot is still moving a little bit.
  // To use this, replace pid_wait with pid_wait_quick_chain.
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // Your final motion should still be a normal pid_wait
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    // Attempt to drive backward
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    }
    // If the robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, the robot will drive forward and turn 90 degrees.
// If interfered, the robot will drive forward and then attempt to drive backward.
void interfered_example() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered) {
    tug(3);
    return;
  }

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Odom Drive PID
///
void odom_drive_example() {
  // This works the same as pid_drive_set, but it uses odom instead!
  // You can replace pid_drive_set with pid_odom_set and your robot will
  // have better error correction.

  chassis.pid_odom_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_odom_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_odom_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Odom Pure Pursuit
///
void odom_pure_pursuit_example() {
  // Drive to 0, 30 and pass through 6, 10 and 0, 20 on the way, with slew
  chassis.pid_odom_set({{{6_in, 10_in}, fwd, DRIVE_SPEED},
                        {{0_in, 20_in}, fwd, DRIVE_SPEED},
                        {{0_in, 30_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait();

  // Drive to 0, 0 backwards
  chassis.pid_odom_set({{0_in, 0_in}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Odom Pure Pursuit Wait Until
///
void odom_pure_pursuit_wait_until_example() {
  chassis.pid_odom_set({{{0_in, 24_in}, fwd, DRIVE_SPEED},
                        {{12_in, 24_in}, fwd, DRIVE_SPEED},
                        {{24_in, 24_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait_until_index(1);  // Waits until the robot passes 12, 24
  // Intake.move(127);  // Set your intake to start moving once it passes through the second point in the index
  chassis.pid_wait();
  // Intake.move(0);  // Turn the intake off
}

///
// Odom Boomerang
///
void odom_boomerang_example() {
  chassis.pid_odom_set({{0_in, 24_in, 45_deg}, fwd, DRIVE_SPEED},
                       true);
  chassis.pid_wait();

  chassis.pid_odom_set({{0_in, 0_in, 0_deg}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Odom Boomerang Injected Pure Pursuit
///
void odom_boomerang_injected_pure_pursuit_example() {
  chassis.pid_odom_set({{{0_in, 24_in, 45_deg}, fwd, DRIVE_SPEED},
                        {{12_in, 24_in}, fwd, DRIVE_SPEED},
                        {{24_in, 24_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait();

  chassis.pid_odom_set({{0_in, 0_in, 0_deg}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Calculate the offsets of your tracking wheels
///
void measure_offsets() {
  // Number of times to test
  int iterations = 10;

  // Our final offsets
  double l_offset = 0.0, r_offset = 0.0, b_offset = 0.0, f_offset = 0.0;

  // Reset all trackers if they exist
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->reset();
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->reset();
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->reset();
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->reset();
  
  for (int i = 0; i < iterations; i++) {
    // Reset pid targets and get ready for running an auton
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    chassis.odom_xyt_set(0_in, 0_in, 0_deg);
    double imu_start = chassis.odom_theta_get();
    double target = i % 2 == 0 ? 90 : 270;  // Switch the turn target every run from 270 to 90

    // Turn to target at half power
    chassis.pid_turn_set(target, 63, ez::raw);
    chassis.pid_wait();
    pros::delay(250);

    // Calculate delta in angle
    double t_delta = util::to_rad(fabs(util::wrap_angle(chassis.odom_theta_get() - imu_start)));

    // Calculate delta in sensor values that exist
    double l_delta = chassis.odom_tracker_left != nullptr ? chassis.odom_tracker_left->get() : 0.0;
    double r_delta = chassis.odom_tracker_right != nullptr ? chassis.odom_tracker_right->get() : 0.0;
    double b_delta = chassis.odom_tracker_back != nullptr ? chassis.odom_tracker_back->get() : 0.0;
    double f_delta = chassis.odom_tracker_front != nullptr ? chassis.odom_tracker_front->get() : 0.0;

    // Calculate the radius that the robot traveled
    l_offset += l_delta / t_delta;
    r_offset += r_delta / t_delta;
    b_offset += b_delta / t_delta;
    f_offset += f_delta / t_delta;
  }

  // Average all offsets
  l_offset /= iterations;
  r_offset /= iterations;
  b_offset /= iterations;
  f_offset /= iterations;

  // Set new offsets to trackers that exist
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->distance_to_center_set(l_offset);
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->distance_to_center_set(r_offset);
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->distance_to_center_set(b_offset);
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->distance_to_center_set(f_offset);
}

// . . .
// Make your own autonomous functions here!
// . . .