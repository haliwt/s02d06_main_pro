#include "bsp.h"
#include "queue.h"

uint8_t usart2_dataBuf[1];


uint8_t recieve_flag;
uint8_t receive_key_message;
uint8_t receive_task_start;
uint8_t key_long_counter;
uint16_t power_key_long_counter ;
uint8_t  dc_power_on;
uint8_t check_code;


/*
**********************************************************************************************************
											宏定义
**********************************************************************************************************
*/
#define POWER_KEY_0	        (1 << 0)
#define MODE_KEY_1	        (1 << 1)
#define DEC_KEY_2           (1 << 2)
#define ADD_KEY_3           (1 << 3)

#define RUN_POWER_4         (1 << 4)
#define RUN_MODE_5          (1 << 5)
#define RUN_DEC_6           (1 << 6)
#define DECODER_BIT_7           (1 << 7)

#define PHONE_POWER_ON_RX_8       (1<<8)
#define PHONE_POWER_ON_9         (1<<9)
#define DECODER_BIT_10            (1<<10)



typedef struct Msg
{
	uint8_t  ucMessageID;
    uint8_t  uid;
    uint8_t  bcc_check_code;
	uint8_t usData[12];
	
}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */


uint8_t rx_data_counter,rx_end_flag;

uint8_t  rx_end_counter,uid;


uint8_t power_off_flag_recoder ;

uint8_t add_flag,dec_flag,smart_phone_sound;


/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
//static void vTaskTaskUserIF(void *pvParameters);
static void vTaskUsartPro(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskUsartPro = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

/**********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
	/* 创建任务 */
	AppTaskCreate();

	/* 创建任务通信机制 */
//	AppObjCreate();
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
}


/**********************************************************************************************************
	*
	*Funtion Name:static void vTaskUsartPro(void *pvParameters)
	*Funtion:送的事件标志位设置
	*Input Ref:的形参
	*	返 回 
	*   优 先  
	*
**********************************************************************************************************/
static void vTaskUsartPro(void *pvParameters)//static void vTaskMsgPro(void *pvParameters)
{

    BaseType_t xResult;
	//const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为100ms */
	uint32_t ulValue;
	
	while(1)
	{
		xResult = xTaskNotifyWait(0x00000000,	   
							   		0xFFFFFFFF,	  
							    	&ulValue,		  /* 保存ulNotifiedValue到变量ulValue中 */
									portMAX_DELAY);  /* 最大允许延迟时间 */
		   
		if( xResult == pdPASS )
		{
            if((ulValue & DECODER_BIT_7) != 0){

	        gpro_t.disp_rx_cmd_done_flag = 0;


            check_code =  bcc_check(gl_tMsg.usData,gl_tMsg.uid);

           if(check_code == gl_tMsg.bcc_check_code ){
           
              receive_data_fromm_display(gl_tMsg.usData);
              if(gpro_t.buzzer_sound_flag == 1){
                  gpro_t.buzzer_sound_flag++ ;
                  buzzer_sound();


              }
           }

          // gl_tMsg.usData[0]=0;
          
            
         }

     //    clear_rx_copy_data();


		}
	}
}
/**********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
**********************************************************************************************************/
static void vTaskMsgPro(void *pvParameters)//static void vTaskStart(void *pvParameters)
{
   //BaseType_t xResult;
  // const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* 设置最大等待时间为30ms */
 
  // uint32_t ulValue;
   static uint8_t dc_power_on_flag;
  
    while(1)
    {


       //DC the first power on run prcess once times.      
        if(dc_power_on_flag==0){
         dc_power_on_flag++;
          
          LED_Mode_Off();
          Backlight_Off();
          
          buzzer_sound();
          gpro_t.shut_Off_backlight_flag == turn_on;
        }


        
         if(smart_phone_sound == 1){
            smart_phone_sound++;
           
            smartphone_power_on_handler();
            

          }
          else if(gkey_t.power_on_flag ==1){
            power_long_short_key_fun();

          }
          else if(gkey_t.key_mode_flag== 1){
      
               mode_long_short_key_fun();
          }
          else if(add_flag==1 ||dec_flag ==1){

                if(add_flag ==1){
                     add_flag ++;
                     Buzzer_KeySound();
                  

                 }
                 else if(dec_flag ==1){
                     dec_flag ++;
                     Buzzer_KeySound();
                    

                 }

                 
                 if(add_flag ==2){
                    add_flag ++;
                    Add_Key_Fun(gkey_t.key_add_dec_mode);
                 

                 }
                 else if(dec_flag ==2){

                       dec_flag ++;
                       Dec_Key_Fun(gkey_t.key_add_dec_mode);
                 }
         }


        if(gpro_t.send_data_power_on_flag == power_on){

               gpro_t.send_data_power_on_flag =0xff;
               
               SendData_Set_Command(0X01,0X01);
               osDelay(5);
                

          }
          else if(gpro_t.send_data_power_on_flag == power_off){
               power_off_flag_recoder ++ ;
              gpro_t.send_data_power_on_flag =0xff;
          
               SendData_Set_Command(0X01,0X00);
               osDelay(5);
          }
       

       if(gkey_t.key_power==power_on){
            
          backlight_on_off_state();
          power_on_run_handler();
          record_time_or_time_handler();
          key_add_dec_set_temp_value_fun();

          key_mode_be_pressed_send_data_wifi();
          
          Display_WorksTimingr_Handler(gkey_t.key_mode);
          
          mainboard_active_handler();
         
          LCD_Timer_Colon_Flicker();
          
          LCD_Wind_Run_Icon(wifi_t.set_wind_speed_value);
          link_wifi_net_handler(gkey_t.wifi_led_fast_blink_flag);
          Disip_Wifi_Icon_State();
          if(gkey_t.gTimer_disp_set_timer  > 1 && gkey_t.key_mode_long_counter > 100 ){

             gkey_t.key_mode_long_counter =0;
             gkey_t.key_mode_flag=0;

          }
          
         
        }
        else{
            
          power_off_run_handler();

       }

		
         if(gkey_t.wifi_led_fast_blink_flag==0 ){
                 wifi_get_beijing_time_handler();
                 wifi_auto_detected_link_state();

         }
        
       receive_message_displaybord_handler();
        vTaskDelay(20);
       }

    }
/**********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)//static void vTaskMsgPro(void *pvParameters)
{
   // MSG_T *ptMsg;
    BaseType_t xResult;
	//const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为100ms */
	uint32_t ulValue;
   
   
	
    while(1)
    {
	
	   xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          portMAX_DELAY);  /* 最大允许延迟时间 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
			if((ulValue & POWER_KEY_0) != 0)
			{
  
			   if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                }
                else{
                    gkey_t.power_on_flag =1;//gkey_t.power_key_long_counter =1;
                    
                    gpro_t.gTimer_shut_off_backlight =0;
                }
            
                gpro_t.gTimer_run_dht11=0;
				                                    
			}
            else if((ulValue & PHONE_POWER_ON_RX_8 ) != 0)
            {
                  gpro_t.gTimer_shut_off_backlight =0;
                  wake_up_backlight_on();
                  buzzer_sound();

               
                 smart_phone_sound = 1;
                 gpro_t.gTimer_shut_off_backlight =0;
                 gpro_t.gTimer_run_dht11=0;
               
            }
            else if((ulValue & MODE_KEY_1) != 0){

              if(gkey_t.key_power == power_on ){

                  if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                  }
                  else{

                  if(gctl_t.ptc_warning ==0 && gctl_t.fan_warning == 0){
                    
                     gkey_t.key_mode_flag=1;
                     gpro_t.gTimer_shut_off_backlight =0;

                    }

                 }

                }
               
             
                 gpro_t.gTimer_run_dht11=0;
               
            }   
            else if((ulValue & DEC_KEY_2) != 0){

                 if(gkey_t.key_power==power_on){

                   if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                  }
                  else{
                    if(gctl_t.ptc_warning ==0 && gctl_t.fan_warning == 0){
                        dec_flag =1;
                        gpro_t.gTimer_shut_off_backlight =0;
                    }

                   }
                  }
                
                  gpro_t.gTimer_run_dht11=0;
               

               
            }
            else if((ulValue & ADD_KEY_3) != 0){

              if(gkey_t.key_power==power_on){

                 if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                  }
                  else{
                   if(gctl_t.ptc_warning ==0 && gctl_t.fan_warning == 0){
                       add_flag =1;
                       gpro_t.gTimer_shut_off_backlight =0;

                   }

                }
                
              }
                
                gpro_t.gTimer_run_dht11=0;
            }

              
                    
          }
   
             
      
       }
   }


 
/**********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
static void AppTaskCreate (void)
{


    xTaskCreate( vTaskUsartPro,     		/* 任务函数  */
                 "vTaskUsartPro",   		/* 任务名    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,               		/* 任务优先级*/
                 &xHandleTaskUsartPro );  /* 任务句柄  */

	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{

 
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    __HAL_GPIO_EXTI_CLEAR_RISING_IT(GPIO_Pin);
 
   switch(GPIO_Pin){

   case KEY_POWER_Pin:
       // DISABLE_INT(); //WT.EDIT 2024.08.15 modify.
        if(KEY_POWER_VALUE()==KEY_DOWN){

       
      
        xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
        POWER_KEY_0,      /* 设置目标任务事件标志位bit0  */
        eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
        &xHigherPriorityTaskWoken);

        /* Èç¹ûxHigherPriorityTaskWoken = pdTRUE£¬ÄÇÃ´ÍË³öÖÐ¶ÏºóÇÐµ½µ±Ç°×î¸ßÓÅÏÈ¼¶ÈÎÎñÖ´ÐÐ */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


        }
            
     //  ENABLE_INT();
   break;

   case KEY_MODE_Pin:
     // DISABLE_INT();
      if(KEY_MODE_VALUE() == KEY_DOWN){
        
        gkey_t.power_on_flag =0;
        xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
               MODE_KEY_1,     /* 设置目标任务事件标志位bit0  */
               eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
               &xHigherPriorityTaskWoken);

        /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


       }
     // ENABLE_INT();
   
   break;


   case KEY_UP_Pin:
      // DISABLE_INT();
       if(KEY_ADD_VALUE() == KEY_DOWN){
       
         xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
                ADD_KEY_3,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);
   
         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
     ///  ENABLE_INT();
   break;

   case KEY_DOWN_Pin:
      ///   DISABLE_INT();
        if(KEY_DEC_VALUE() == KEY_DOWN){
             
        xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
                DEC_KEY_2,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);
   
         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
    ///    ENABLE_INT();
   break;
    }
}



/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  
     static uint8_t state;
     BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(huart->Instance==USART1)//if(huart==&huart1) // Motor Board receive data (filter)
	{
      // DISABLE_INT();
		switch(state)
		{
		case 0:  //#0
			if(disp_inputBuf[0] == 0xA5){  // 0xA5 --didplay command head
               rx_data_counter=0;
               mess_t.mesData[rx_data_counter] = disp_inputBuf[0];
			   state=1; //=1

             }
            else
                state=0;
		break;

       
		case 1: //#1

            if(gpro_t.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               rx_data_counter++;
		     
	          mess_t.mesData[rx_data_counter] = disp_inputBuf[0];
              

              if(rx_end_flag == 1){

                state = 0;
            
                mess_t.mesLength = rx_data_counter;
                rx_end_flag=0;

                rx_data_counter =0;

                gpro_t.disp_rx_cmd_done_flag = 1 ;

          

                 mess_t.bcc_check_code=disp_inputBuf[0];

          
                #if 0

                 xTaskNotifyFromISR(xHandleTaskUsartPro,  /* 目标任务 */
                                    DECODER_BIT_7,     /* 设置目标任务事件标志位bit0  */
                                    eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                                    &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                #endif 
                  
              }

              }

              if(mess_t.mesData[rx_data_counter] ==0xFE && rx_end_flag == 0 &&  rx_data_counter > 4){
                     
                       rx_end_flag = 1 ;
               }

        break;


			
		}

      //  ENABLE_INT();
	    __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,disp_inputBuf,1);//UART receive data interrupt 1 byte
		
	}
    else if(huart->Instance==USART2)  //wifi usart1 --wifi 
    {
      // DISABLE_INT();    
	  if(gpro_t.linking_tencent_cloud_doing  ==1){ //link tencent netware of URL

			wifi_t.wifi_data[wifi_t.wifi_uart_rx_counter] = usart2_dataBuf[0];
			wifi_t.wifi_uart_rx_counter++;

			if(*usart2_dataBuf==0X0A) // 0x0A = "\n"
			{
				//wifi_t.usart2_rx_flag = 1;
				Wifi_Rx_Link_Net_InputInfo_Handler();
				wifi_t.wifi_uart_rx_counter=0;
			}

	      } 
		  else{

		         if(wifi_t.get_rx_beijing_time_enable==1){
					wifi_t.wifi_data[wifi_t.wifi_uart_rx_counter] =usart2_dataBuf[0];
					wifi_t.wifi_uart_rx_counter++;
				}
			    else{
					
                   // wifi_t.wifi_data_rx[wifi_t.wifi_uart_rx_counter] =usart2_dataBuf[0];
					///wifi_t.wifi_uart_rx_counter++;
                    Subscribe_Rx_Interrupt_Handler();

				}
	      }
	 
	//  ENABLE_INT();

	__HAL_UART_CLEAR_OREFLAG(&huart2);
	
	 HAL_UART_Receive_IT(&huart2,usart2_dataBuf,1);
     
	}


 }

  
 

//	__HAL_UART_CLEAR_NEFLAG(&huart2);
//	__HAL_UART_CLEAR_FEFLAG(&huart2);
//	__HAL_UART_CLEAR_OREFLAG(&huart2);
//	__HAL_UART_CLEAR_TXFECF(&huart2);


/*********************************************************************
*
*Function Name: 
*Function :
*Input Ref:
*Return Ref:
*
*********************************************************************/
void smartphone_turn_on_handler(void)
{
   
    xTaskNotify(xHandleTaskMsgPro,  /* 目标任务 */
    PHONE_POWER_ON_RX_8,      /* 设置目标任务事件标志位bit0  */
    eSetBits);  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */

}


void display_board_commnunication_handler(void)
{

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
         DECODER_BIT_10,     /* 设置目标任务事件标志位bit0  */
         eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
            &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
           portYIELD_FROM_ISR(xHigherPriorityTaskWoken);



}




