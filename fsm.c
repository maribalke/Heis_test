#include "fsm.h"

state current_state = INIT;
Dir current_direction;

void fsm_transistion(state state){
    current_state = state;
}

int get_floor_request(){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b] == 1){
                return f;
            }
        }
    }
    return 0;
}

int get_btn_request(){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(queue[f][b] == 1){
                return b;
            }
        }
    }
    return 0;
}

int should_stop(int current_floor, Dir current_direction){
    
    switch (current_direction)
    {
    case D_UP:
        for(int f = current_floor; f < N_FLOORS-1; f++){
            if(queue[f][2] == 1 || queue[f][0]==1){ //dersom knapp er cab, 1 pri
                return 1;
            }
            else if((queue[f][1] == 1) && (queue[f][0] = 0) && (queue[f][2] = 0)){ //dersom knapp er ned og hverken up eller cab er trykket
                current_direction = D_DOWN; //d_down
                printf("bytter retning\n");
                return 1;
            }
            else if(queue[f][1] == 1){
                if(current_floor == N_FLOORS-1){
                    return 1;
                }
            }

            else{
                return 0;
                }
        }
        break;
    
    case D_DOWN:
        for(int f = 0; f < current_floor; f++){
            if((queue[f][2] == 1) || (queue[f][1] == 1)){ //dersom knapp er ned eller cab
                printf("dir = down,  knapp = ned eller knapp = cab\n");
                return 1;
            }
            else if(queue[f][0] == 1){
                if(current_floor == 0){
                    return -1;
                }
                else if ((queue[f][0] == 1) && (queue[f][1] = 0) && (queue[f][2] = 0)){
                    current_direction = D_UP; //d_down
                    printf("bytter retning\n");
                    return 1;
                }

            }
            if((queue[f][0] == 1) && (queue[f][1] = 0) && (queue[f][2] = 0)){ //dersom knapp er ned og hverken up eller cab er trykket
                current_direction = D_UP; //d_down
                return 1;
            }
            
            else{
                return 0;
                }
        }
        break;
    default: 
        break;
    }
    return 0;
}

int next_stop(int current_floor, MotorDirection dir){

    if(elevio_floorSensor() == -1){
        return 0;
    }

    int b = get_btn_request();
    int f = get_floor_request();

    if(b == 2 && elevio_floorSensor() == f){ //cab
        return 1;
    }

    else if ((dir == DIRN_UP && b == 0) || (dir == DIRN_DOWN && b == 1)){
        return 1;
    }

    else if (dir == DIRN_UP && b == 1){
        if(current_floor == N_FLOORS - 1){
            return 1;
        }
        for(int f = current_floor+1; f < N_FLOORS; f++){
            if(queue[f][0] == 0 && queue[f][2] == 0){
                return 1;
            } 
            else{
                return 0;
            }
        }
        
    } 
    else if (dir == DIRN_DOWN && b == 0){
        if (current_floor == 0){
            return 1;
        }
        else{
            for(int f = 0; f < current_floor; f++){
                if(queue[f][1] == 0 && queue[f][2] == 0){
                    return 1;
                } 
                else{
                    return 0;
                }   
            }   

        }
    }

    else if(!get_btn_request()){
        return 0;
    }
    return 0;
}


void fsm_run(int current_floor){

    int floor_request = get_floor_request();
    //MotorDirection dir = get_current_direction();
    
    switch(current_state){

        case(INIT):
            set_initial_condition();
            add_order();
            current_state = DOOR_OPEN;
            printf("init\n");
            break;

        case(IDLE):
            add_order();
            order_light(); 
            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
            }

            // if(current_direction == DIRN_STOP && queue_any_orders()){
            //     current_state = MOVING;
            // }

            // if(should_stop(current_floor, current_direction) && timer_expired()){
            //     current_state = MOVING;
            // }

            if(queue_any_orders() && timer_expired()){
                current_state = MOVING;
            }
            

            if(timer_expired()){
                elevio_doorOpenLamp(0);
                delete_order(current_floor);
            }

            break;


        case(MOVING):
            add_order();
            order_light(); 

            
            if(should_stop(current_floor, current_direction) == 1){
                printf("next stop\n");
                current_state = DOOR_OPEN;
            }

            // if(should_stop(current_floor, current_direction) == 0){
            //     printf("next stop\n");
            //     current_state = IDLE;
            // }
            
            MotorDirection current_motor_direction = get_current_direction();

            if(current_motor_direction == DIRN_STOP && !should_stop(current_floor, current_direction)){
                elevator_direction(floor_request, current_floor); 
                if(elevio_floorSensor() == floor_request){
                    current_state = DOOR_OPEN;
                }
            }

        
            // if(elevio_floorSensor() == floor_request){
            //     current_state = DOOR_OPEN;
            // }

            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
                current_direction = D_STOP;
            }

            

            break;


        case (DOOR_OPEN):
            add_order();
            
            if(elevio_stopButton()){
                current_state = EMERGENCY_STOP;
            }

            
            elevio_motorDirection(DIRN_STOP);
            elevio_doorOpenLamp(0);
            printf("Door open\n");


            delete_light(current_floor);
            delete_order(current_floor);

            current_state = IDLE;
            timer_start(); 

            
            
            break;


        case(EMERGENCY_STOP):
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);
            delete_all_orders();
            

            if(current_floor == floor_request){
                if(elevio_stopButton() == 0){
                    elevio_stopLamp(0);
                    current_state = DOOR_OPEN;
                }
            } else{
                while(elevio_stopButton() == 1){
                    current_state = IDLE;
                    elevio_stopLamp(0);

                }
            }
            break;
    }
}