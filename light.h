#include <stdlib.h>
#include <stdio.h>

#include "driver/elevio.h"


void order_light();
void floor_light(int floor); //etasjelys
void stop_lamp(void); //stopplys
void obstruction_light();
void delete_light(int f);
void delete_all_lights();