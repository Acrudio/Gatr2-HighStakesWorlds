#pragma once

void default_constants();

void DrivePID_Tune();

void DrivePID_Tune_20();
void DrivePID_Tune_10();
void DrivePID_Tune_5();

void TurnPID_Tune_90();
void TurnPID_Tune_180();

void TurnBiasPID_Tune();
void SwingPID_Tune();

void TurnPID_Tune_LeftPoint();

void OuterRush();
void BlueOuterRush();

void drive_example();
void turn_example();
void drive_and_turn();
void wait_until_change_speed();
void swing_example();
void motion_chaining();
void combining_movements();
void interfered_example();
void odom_drive_example();
void odom_pure_pursuit_example();
void odom_pure_pursuit_wait_until_example();
void odom_boomerang_example();
void odom_boomerang_injected_pure_pursuit_example();
void measure_offsets();