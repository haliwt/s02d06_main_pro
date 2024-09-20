#include "interrupt_manager.h"
#include "bsp.h"

uint8_t inputBuf[1];


//typedef struct Msg
//{
//	uint8_t  ucMessageID;
//	uint8_t usData[12];
//	//uint8_t ulData[1];
//}MSG_T;
//
//MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */

uint8_t rx_data_counter,rx_end_flag;

uint8_t  rx_end_counter,uid;



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
    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(huart->Instance==USART1)//if(huart==&huart1) // Motor Board receive data (filter)
	{
      // DISABLE_INT();
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 0xA5){  // 0xA5 --didplay command head
               rx_data_counter=0;
               gpro_t.usData[rx_data_counter] = inputBuf[0];
			   state=1; //=1

             }
            else
                state=0;
		break;

       
		case 1: //#1

            if(gpro_t.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               rx_data_counter++;
		     
	         gpro_t.usData[rx_data_counter] = inputBuf[0];
              

              if(rx_end_flag == 1){

                state = 0;
            
                gpro_t.uid = rx_data_counter;
                rx_end_flag=0;

                rx_data_counter =0;

                gpro_t.disp_rx_cmd_done_flag = 1 ;

                state=0;

                 gpro_t.bcc_check_code=inputBuf[0];

                display_board_commnunication_handler();
                 

                #if 0

                xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
                DECODER_BIT_10,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                #endif 
                  
              }

              }

              if(gpro_t.usData[rx_data_counter] ==0xFE && rx_end_flag == 0 &&  rx_data_counter > 4){
                     
                       rx_end_flag = 1 ;
                          
                        
               }

        break;


			
		}

      //  ENABLE_INT();
	    __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
		
	 }
     else if(huart->Instance==USART2)  //wifi usart1 --wifi 
    {
           
	  if(wifi_t.linking_tencent_cloud_doing  ==1){ //link tencent netware of URL

			wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
			wifi_t.wifi_uart_counter++;

			if(*wifi_t.usart1_dataBuf==0X0A) // 0x0A = "\n"
			{
				//wifi_t.usart2_rx_flag = 1;
				Wifi_Rx_Link_Net_InputInfo_Handler();
				wifi_t.wifi_uart_counter=0;
			}

	      } 
		  else{

		         if(wifi_t.get_rx_beijing_time_enable==1){
					wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
					wifi_t.wifi_uart_counter++;
				}
				else if(wifi_t.get_rx_auto_repeat_net_enable ==1){

					wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
					wifi_t.wifi_uart_counter++;

					if(*wifi_t.usart1_dataBuf==0X0A) // 0x0A = "\n"
					{
						
						Wifi_Rx_Auto_Link_Net_Handler();
						wifi_t.wifi_uart_counter=0;
					}


				}
				else{
					Subscribe_Rx_Interrupt_Handler();

				}
	      }
	 
	  
//	__HAL_UART_CLEAR_NEFLAG(&huart2);
	//__HAL_UART_CLEAR_FEFLAG(&huart2);
	__HAL_UART_CLEAR_OREFLAG(&huart2);
	//__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	//__HAL_UART_CLEAR_TXFECF(&huart2);
	 HAL_UART_Receive_IT(&huart2,wifi_t.usart1_dataBuf,1);
     
	}


 }

  
 

//	__HAL_UART_CLEAR_NEFLAG(&huart2);
//	__HAL_UART_CLEAR_FEFLAG(&huart2);
//	__HAL_UART_CLEAR_OREFLAG(&huart2);
//	__HAL_UART_CLEAR_TXFECF(&huart2);


/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 10ms interrupt call back function call back function
	*timer 1ms 
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint8_t tm1,tm2,tm3;
  static uint16_t tm0;

 
    
   if(htim->Instance==TIM17){
    
    tm0++;  //1ms
	tm1++;
    tm3++;

    if(tm1>9){ //10ms
        tm1=0;
        
    gctl_t.gTimer_wifi_fast_blink++;
  
    
	glcd_t.gTimer_fan_blink++;

   
       
    	
    }
	
    if(tm3>99){ //1*100 = 100ms 
       tm3=0;
     
            //key timer
       gkey_t.gTimer_set_temp_blink++;

       //lcd timer 
       glcd_t.gTimer_colon_blink++ ;
       
      
       
      
    }

    if(tm0>999){ //100 *10ms = 1000ms = 1s
		tm0=0;
		tm2++;
      
          //control timer
        gctl_t.gTimer_wifi_blink++;
       glcd_t.gTimer_error_times++;
    
  
	   
       //key timer
	
        gkey_t.gTimer_power_off_run_times++;
        gkey_t.gTimer_set_temp_value++;
        gkey_t.gTimer_disp_set_timer++;
      //  gctl_t.gTimer_feed_dog_times++ ;
       

        //lcd timer
     

        //process gtimer
        gpro_t.gTimer_ptc_detected++;
        gpro_t.gTimer_works_counter_sencods ++;
        gpro_t.gTimer_timer_Counter++;
		gpro_t.gTimer_run_adc++ ;
		gpro_t.gTimer_run_dht11++;
        gpro_t.gTimer_run_one_mintue ++;
        gpro_t.gTimer_pro_action_publis++;
        gpro_t.gTimer_run_main_fun++;
        gpro_t.gTimer_publish_tencent_dht11++;
        gpro_t.gTimer_compare_temp++;
        
       
        //wifi
		wifi_t.gTimer_wifi_pub_power_off++;
		wifi_t.gTimer_power_first_link_tencent++;
		wifi_t.power_on_login_tencent_cloud_flag++;
		wifi_t.gTimer_power_first_link_tencent++;
        wifi_t.gTimer_linking_tencent_duration++;

        wifi_t.gTimer_login_tencent_times ++;
		wifi_t.gTimer_publish_dht11++;
		wifi_t.gTimer_get_beijing_time++;
		wifi_t.gTimer_auto_detected_net_state_times++;
		wifi_t.gTimer_read_beijing_time ++;
		wifi_t.gTimer_wifi_counter_link_beijing_times++;
        wifi_t.gTimer_counter_repeat_link_net++;
        

        

       if(tm2 > 59){ //60s = 1 minutes
			tm2 =0;
			gpro_t.gTimer_run_total++;
			gpro_t.gTimer_run_time_out ++ ;
            gpro_t.gTimer_shut_off_backlight++;
			

		}
	}
	
   }


}







