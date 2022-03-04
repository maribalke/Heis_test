#pragma once
#include "order.h"
#include "driver/elevio.h"
#include "order.h"
#include "light.h"
#include "timer.h"
#include "queue.h"

typedef enum {INIT, IDLE, MOVING, DOOR_OPEN, EMERGENCY_STOP} state;

extern state current_state;

typedef enum {D_UP, D_DOWN, D_STOP} Dir;

extern Dir current_direction;


void fsm_transistion(state state);
int get_floor_request();
int should_stop(int current_floor, Dir current_direction);
int next_stop(int current_floor, MotorDirection dir);
int get_btn_request();


void fsm_run(int current_floor);