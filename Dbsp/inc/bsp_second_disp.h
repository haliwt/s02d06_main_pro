#ifndef __BSP_SECOND_DISP_H
#define __BSP_SECOND_DISP_H
#include "main.h"



typedef struct{

    uint8_t disp_power_on;
    uint8_t disp_power_off;
    uint8_t disp_cmd_ptc ;
    uint8_t disp_cmd_plasma;
    uint8_t disp_cmd_ultrasonic;
    uint8_t disp_set_temp_value;
    uint8_t disp_set_timer_value;
    uint8_t disp_set_temp_value_flag;



}second_disp_ref;


extern second_disp_ref  g_tDisp;












#endif 

