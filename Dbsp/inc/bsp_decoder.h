#ifndef __BSP_DECODER_H
#define __BSP_DECODER_H
#include "main.h"




void receive_data_fromm_display(uint8_t *pdata);




uint8_t bcc_check(const unsigned char *data, int len) ;



#endif 

