#pragma once
#include "driver/elevio.h"

int queue[4][3];

typedef enum { 
    ORDER_DOWN = 2,
    ORDER_CAB = 0,
    ORDER_UP = 1,
    ORDER_NONE = -1
} OrderType;

OrderType queue_floor[4];

void set_current_direction(MotorDirection direction);
MotorDirection get_current_direction();

int queue_any_orders();

int queue_any_orders_below(int current_floor);
int queue_any_orders_above(int current_floor);

void queue_prio(int current_floor);