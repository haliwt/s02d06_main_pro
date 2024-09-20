#ifndef __BSP_DECODER_H
#define __BSP_DECODER_H
#include "main.h"


typedef struct{

   uint8_t disp_set_temp_value_flag;



}disp_two_state;


extern disp_two_state gdisp_t;




void receive_data_fromm_display(uint8_t *pdata);




uint8_t bcc_check(const unsigned char *data, int len) ;



#endif 

