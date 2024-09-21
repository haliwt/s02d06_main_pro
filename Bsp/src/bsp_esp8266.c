#include "bsp_esp8266.h"
#include "bsp.h"


#if 0
/*********************************************************
 *
 *pdata: pointer of data for send
 *len:  len of data to be sent
 *return: the len of data send success
 * @brief hal api for at data send
***********************************************************/
uint8_t at_send_data(uint8_t* pdata, uint16_t len)
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, pdata, len, 0xffff))
	{
		return len;
	}
	else
	{
		return 0;
	}	
}

#endif 







