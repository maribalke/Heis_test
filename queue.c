#include "queue.h"
#include "driver/elevio.h"

#include <stdio.h>

static MotorDirection current_motor_direction; 

void set_current_direction(MotorDirection direction){
    current_motor_direction=direction;
}

MotorDirection get_current_direction(){
    return current_motor_direction;
}


int queue_any_orders(){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b] == 1 ){
                return 1;
            }    
        }
    }
    return 0;
}



int queue_any_orders_above(int current_floor){
    for(int f = current_floor; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b]==1){
                return 1;
            } 
        }
    }
    return 0;
}

int queue_any_orders_below(int current_floor){
    for(int f = 0; f < current_floor; f++){
        if(queue[f][1] == 1 || queue[f][2] == 1){
            return 1;
        }
    }  
    
    return 0;
}



void queue_prio(int current_floor){

    // if(elevio_floorSensor() != -1){
    //     switch (get_current_direction())
    //     {
    //     case DIRN_DOWN:
    //         for(int f = 0; f < current_floor; ++f){
    //             if((queue[f][1] == 1) || (queue[f][2] == 1)){
    //                 elevator_direction(f, current_floor);
                    
    //             }
            
    //         }
            
    //         break;

    //     case DIRN_UP:
            
    //         break;

    //     default:
    //         break;
        
    //     }
    // }
    
}