#ifndef  __BSP_SENSOR_H_
#define __BSP_SENSOR_H_
#include "main.h"

#define DHT11_DQ_GPIO_PORT                  GPIOB
#define DHT11_DQ_GPIO_PIN                   GPIO_PIN_13
#define DHT11_DQ_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* GPIOAå£æ—¶é’Ÿä½¿èƒ½ */

/******************************************************************************************/

/* IOæ“ä½œå‡½æ•° */
#define DHT11_DQ_OUT(x)     do{ x ? \
                                HAL_GPIO_WritePin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN, GPIO_PIN_RESET); \
                            }while(0)                                                /* æ•°æ®ç«¯å£è¾“å‡º */
#define DHT11_DQ_IN         HAL_GPIO_ReadPin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN)  /* æ•°æ®ç«¯å£è¾“å…¥ */



/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
//void DHT11_Init( void );
//uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef * DHT11_Data);
void Update_DHT11_Value(void);


void Update_Dht11_Totencent_Value(void);



#endif 

















