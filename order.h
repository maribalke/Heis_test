#pragma once

void undefined_floor();

void set_initial_condition(); //definerer start tilstand
void set_current_floor(int* current_floor); 

void add_order(); 
int get_floor_request();
int get_btn_request();


void cab_direction(int floor, int current_floor); //heispanel
void elevator_direction(int floor, int current_floor);
void delete_order(int f);



void delete_all_orders();