#include "elev.h"
#include <stdio.h>

int orders[12];
int button_pressed() {
    int count = 0; 
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 4; j++){
            if(elev_get_button_signal(i, j)) {
                order_place_order(i, j, orders);
                count++;
            }
        }
    }
    return count;
}


int main() {
    
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);


    while (1) {
        
        int floor = elev_get_floor_sensor_signal();

        if(timer_timeout()) {
            fsm_timeout();
        }
        if(button_pressed()){
            fsm_register_button();
        }
        if(elev_get_floor_sensor_signal()){
            fsm_in_floor();
        }
        if(elev_get_stop_signal()){
            fsm_emergency();
        }
    


        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }    

    }

    return 0;
}
