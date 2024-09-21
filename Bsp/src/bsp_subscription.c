#include "bsp_subscription.h"
#include <string.h>
#include <stdlib.h>
#include "bsp.h"


static void smartphone_app_timer_power_on_handler(void);

static void Tencent_Rx_App_PowerOn_Handler(void);



uint8_t TCMQTTRCVPUB[40];
uint8_t rx_tencent_num[1];


uint8_t recoder_tx_net_data_counter;

uint8_t temp_num;

uint8_t  rx_data_array[11];

uint8_t rx_tencent_num_buffer[40] ;

uint8_t wind_total_value;





//处理腾讯云下发的数据
/*******************************************************************************
   **
   *Function Name:void Receive_Data_FromCloud_Data(int type, char *str)
   *Function: receive data from tencent cloud-subscription of topic
   *Input Ref: module , str ->data
   *Return Ref:NO
   *
********************************************************************************/
//void Receive_Data_FromCloud_Data(int type, char *str)
//{
//   uint8_t   iNameLen = 0;
//    char  *p_cName = 0, *p_cPos = str;

//       wifi_t.rx_data_len=strlen(str);

//      if (type == JSOBJECT) {
//        /* Get Key */
//        p_cPos = strchr(p_cPos, '"');
//        if (!p_cPos) {
//            return ;
//        }
//        p_cName = ++p_cPos;
//        p_cPos = strchr(p_cPos, '"');
//        if (!p_cPos) {
//            return ;
//        }
//        iNameLen = p_cPos - p_cName;
//      wifi_t.rx_data_name_len=iNameLen;

//        /* Get Value */
//        p_cPos = strchr(p_cPos, ':');
//      
//      
//       }

//   
//   }

 /*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler(void)
{
        
	     uint8_t *device_massage;

         device_massage = (uint8_t *)malloc(128);
         wifi_t.randomName[0]=HAL_GetUIDw0();
      
         sprintf((char *)device_massage,"AT+TCMQTTSUB=\"$thing/down/property/%s/UYIJIA01-%d\",0\r\n", PRODUCT_ID, wifi_t.randomName[0]);
         HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 0xffff); 
         free(device_massage);
   }
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_Interrupt_Handler(void)
{

     switch(wifi_t.rx_data_state)
      {
      case 0: //#0

	  	if((wifi_t.usart1_dataBuf[0] == 'p') ||wifi_t.usart1_dataBuf[0]=='E' || wifi_t.usart1_dataBuf[0] =='T') //hex :54 - "T" -fixed
            wifi_t.rx_data_state=1; //=1
          else{
               wifi_t.wifi_rx_counter=0;
               wifi_t.rx_data_state=0; 
            }
        break;

      case 1:
      
         if((wifi_t.usart1_dataBuf[0] == 'a')  ||wifi_t.usart1_dataBuf[0]=='R' || wifi_t.usart1_dataBuf[0] =='C')//hex :54 - "T" -fixed
            wifi_t.rx_data_state=2; //=1
          else{
               wifi_t.wifi_rx_counter=0;
               wifi_t.rx_data_state=1;
            }
            
         break;
      case 2: //#1
             if((wifi_t.usart1_dataBuf[0] == 'r')||wifi_t.usart1_dataBuf[0]=='R'|| wifi_t.usart1_dataBuf[0] =='M')  //hex :4B - "K" -fixed
            wifi_t.rx_data_state=3; //=1
         else{
            wifi_t.rx_data_state =0;
             wifi_t.wifi_rx_counter=0;
         }
         break;
            
        case 3:
            if((wifi_t.usart1_dataBuf[0] == 'a')||wifi_t.usart1_dataBuf[0]=='o'|| wifi_t.usart1_dataBuf[0] =='Q')    //hex :4B - "K" -fixed
            wifi_t.rx_data_state=4; //=1
         else{
           wifi_t.rx_data_state =0;
             wifi_t.wifi_rx_counter=0;
         }
        
        break;
        
        case 4:
            if((wifi_t.usart1_dataBuf[0] == 'm')  ||wifi_t.usart1_dataBuf[0]=='R'|| wifi_t.usart1_dataBuf[0] =='T')  //hex :4B - "K" -fixed
            wifi_t.rx_data_state=5; //=1
         else{
            wifi_t.rx_data_state =0;
             wifi_t.wifi_rx_counter=0;
         }
        
        break;

      case 5:
       if((wifi_t.usart1_dataBuf[0] == 's') ||wifi_t.usart1_dataBuf[0]==':'|| wifi_t.usart1_dataBuf[0] =='T')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=6; //=1
         else{
           wifi_t.rx_data_state=0;
            wifi_t.wifi_rx_counter=0;
         }
            
      break;

      
      case 6:
       if((wifi_t.usart1_dataBuf[0] == '"')||wifi_t.usart1_dataBuf[0]=='2' || wifi_t.usart1_dataBuf[0] =='S')    //hex :4B - "K" -fixed
         wifi_t.rx_data_state=7; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.wifi_rx_counter=0;
         }
            
      break;

      case 7:
       if((wifi_t.usart1_dataBuf[0] == ':')||wifi_t.usart1_dataBuf[0]=='0' ||wifi_t.usart1_dataBuf[0]=='T' ){  //hex :4B - "K" -fixed
         wifi_t.rx_data_state=8; //=1
    	}
		else{
           wifi_t.rx_data_state =0;
            wifi_t.wifi_rx_counter=0;
         }
           
      break;

       case 8:
       if((wifi_t.usart1_dataBuf[0] == '{') ||wifi_t.usart1_dataBuf[0]=='8' ||wifi_t.usart1_dataBuf[0]=='A' ){ //hex :4B - "K" -fixed
         if(wifi_t.usart1_dataBuf[0]=='8')  wifi_t.rx_data_state =11;
         else if(wifi_t.usart1_dataBuf[0]=='A')  wifi_t.rx_data_state =12;
         else
             wifi_t.rx_data_state =9; //=1

        }
         else{
           wifi_t.rx_data_state =0;
            wifi_t.wifi_rx_counter=0;
         }

		
            
      break;


      case 9:

       if(wifi_t.rx_data_success ==0){

            wifi_t.wifi_rx_data_array[wifi_t.wifi_rx_counter] = wifi_t.usart1_dataBuf[0];
            wifi_t.wifi_rx_counter++ ;

            if(wifi_t.usart1_dataBuf[0]=='}' || wifi_t.usart1_dataBuf[0]==0x0A) //0x7D='}', 0x0A = line feed // end
            {
                wifi_t.rx_data_success=1;
                wifi_t.rx_data_state=0;
                wifi_t.received_data_from_tencent_cloud = wifi_t.wifi_rx_counter;
                wifi_t.wifi_rx_counter=0;


            }


       }
       else{
            wifi_t.rx_data_success=0;
            wifi_t.rx_data_state =0;
            wifi_t.wifi_rx_counter=0;
            wifi_t.received_data_from_tencent_cloud =0;


       }

     break;

     
      case 11:
		 wifi_t.link_wifi_net_login_tencent_success =0; //wifi_t.esp8266_login_cloud_success =0;
         gpro_t.get_beijing_time_success = 0;
         wifi_t.rx_data_state =0;
         wifi_t.wifi_rx_counter=0;

      break;

      case 12:
		if(wifi_t.usart1_dataBuf[0]== 'T'){   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=13; //=1
        }
         else{
            wifi_t.rx_data_state =0;
            wifi_t.wifi_rx_counter=0;
         }

      break;

	  case 13:

       if(wifi_t.usart1_dataBuf[0]== 'E'){   //hex :4B - "K" -fixed
          wifi_t.rx_data_state=14; //=1
        }
        else{
             wifi_t.rx_data_state =0;
             wifi_t.wifi_rx_counter=0;
         }

      break;

	  case 14:
		if(wifi_t.usart1_dataBuf[0]== ':'){   //hex :4B - "K" -fixed
          wifi_t.rx_data_state=15; //=1
         }
         else{
             wifi_t.rx_data_state =0;
             wifi_t.wifi_rx_counter=0;
         }

      break;

	  case 15:
		if(wifi_t.usart1_dataBuf[0]== '0'){   //hex :4B - "K" -fixed
            wifi_t.link_wifi_net_login_tencent_success = 0;//wifi_t.esp8266_login_cloud_success =0;
            gpro_t.get_beijing_time_success = 0;
            wifi_t.rx_data_state =0;
            wifi_t.wifi_rx_counter=0;
        }
        else if(wifi_t.usart1_dataBuf[0]== '1'){

            wifi_t.link_wifi_net_login_tencent_success = 1;//net_t.esp8266_login_cloud_success =1;
       
             wifi_t.rx_data_state =0;
             wifi_t.wifi_rx_counter=0;
        }
        else{

           wifi_t.rx_data_state =0;
           wifi_t.wifi_rx_counter=0;


        }

      break;

	

	  default:
      break;
      }
    
    
  
}
	
   

  
/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/
static void Tencent_Rx_App_PowerOn_Handler(void)
{

  if(wifi_t.rx_data_success==1){
         wifi_t.rx_data_success=0;
         wifi_t.set_beijing_time_flag =0; //WT.EDIT 2023.06.12
		
		 wifi_t.gTimer_auto_detected_net_state_times=0;
	
    // if(wifi_t.received_data_from_tencent_cloud ==0x25){ //36
	  //  wifi_t.received_data_from_tencent_cloud=0;
		
		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
		wifi_t.response_wifi_signal_label = APP_TIMER_POWER_ON_REF;
	    __HAL_UART_CLEAR_OREFLAG(&huart2);
		strcpy((char*)TCMQTTRCVPUB,(char *) rx_tencent_num_buffer);
	    
	
	//}
    }

}


 

/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/
void Tencent_Cloud_Rx_Handler(void)
{



    if(wifi_t.tencent_rx_data_success==1){
        
        wifi_t.tencent_rx_data_success=0;


  
         wifi_t.set_beijing_time_flag =0; //WT.EDIT 2023.06.12
		
		 wifi_t.gTimer_auto_detected_net_state_times=0;
	
     if(wifi_t.received_data_from_tencent_cloud > 22){ //36
	    wifi_t.received_data_from_tencent_cloud=0;
		
		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
		wifi_t.response_wifi_signal_label = APP_TIMER_POWER_ON_REF;
	    __HAL_UART_CLEAR_OREFLAG(&huart2);
		strcpy((char*)TCMQTTRCVPUB,(char *) rx_tencent_num_buffer);
   
     }
     else{
   switch(rx_tencent_num[0]){


   case 8: //if(rx_tencent_num[0]   > 6 && rx_tencent_num[0]   <8){
   
       if(strstr((char *) rx_tencent_num_buffer,"open\":0")){
    		  wifi_t.response_wifi_signal_label = OPEN_OFF_ITEM;
            
    		 
    	}
    	else if(strstr((char *)rx_tencent_num_buffer,"open\":1")){
    	   
    	   wifi_t.response_wifi_signal_label = OPEN_ON_ITEM;
             
    	}

    break;

    case 7:
 
    	
    	if(strstr((char *) rx_tencent_num_buffer,"ptc\":0")){
                if(power_on_state() ==power_on){
    			   
    	           wifi_t.response_wifi_signal_label = PTC_OFF_ITEM;
    	         
                 }
             
    			
        }
        else if(strstr((char *) rx_tencent_num_buffer,"ptc\":1")){
                if(power_on_state() ==power_on){
    	     
    			  wifi_t.response_wifi_signal_label = PTC_ON_ITEM;
    				
                }
             
        }

    break;

    case 9: //if(rx_tencent_num[0]  > 7 && rx_tencent_num[0]   < 9){

    if(strstr((char *) rx_tencent_num_buffer,"Anion\":0")){
          if(power_on_state() ==power_on){
	      //   gctl_t.plasma_flag=0;
			wifi_t.response_wifi_signal_label = ANION_OFF_ITEM;
		    
             }
        
		 
    }
    else if(strstr((char *)rx_tencent_num_buffer,"Anion\":1")){
            if(power_on_state() ==power_on){
         //   gctl_t.plasma_flag=1;
			wifi_t.response_wifi_signal_label = ANION_ON_ITEM;
		
            }
      }
	
    if(strstr((char *)rx_tencent_num_buffer,"sonic\":0")){
            if(power_on_state() ==power_on){
           // gctl_t.ultrasonic_flag=0;
			wifi_t.response_wifi_signal_label = SONIC_OFF_ITEM;
        
                
            }
    }
    else if(strstr((char *) rx_tencent_num_buffer,"sonic\":1")){
            if(power_on_state() ==power_on){
          //  gctl_t.ultrasonic_flag=1;
			wifi_t.response_wifi_signal_label = SONIC_ON_ITEM;
       
           }
  }
  if(strstr((char *)rx_tencent_num_buffer,"state\":1")){
           if(power_on_state() ==power_on){
           
            
			wifi_t.response_wifi_signal_label = STATE_AI_MODEL_ITEM;
        	}

         
		  
    }
    else if(strstr((char *)rx_tencent_num_buffer,"state\":2")){
            if(power_on_state() ==power_on){
        
			wifi_t.response_wifi_signal_label = STATE_TIMER_MODEL_ITEM;
            }


        
			
    }

       if(strstr((char *)rx_tencent_num_buffer,"find")){

		 if(power_on_state()==power_on){
            wifi_t.fin_speed_full_flag = 0;
			wifi_t.response_wifi_signal_label= FAN_ITEM;
		}

        }


    break;


    case 10:

       
      if(strstr((char *)rx_tencent_num_buffer,"find")){

		 if(power_on_state()==power_on){
             wifi_t.fin_speed_full_flag = 1;
			wifi_t.response_wifi_signal_label= FAN_ITEM;
		}

        }


    break;


    case 16://15:  //if(rx_tencent_num[0]   > 13){
	
   
   if(strstr((char *)rx_tencent_num_buffer,"temperature")){

	        if(power_on_state() ==power_on){

			wifi_t.response_wifi_signal_label = TEMPERATURE_ITEM;
            
	        }
      

      
    }
    break;

   
  
     }
  
 
    }
    }
}




/****************************************************************************
**
   *Function Name:void Json_Parse_Command_Fun(void)
   *Function: parse setment of wifi receive data from tencent 
   *Input Ref: NO
   *Return Ref:NO
   *

*****************************************************************************/
void Json_Parse_Command_Fun(void)
{

     uint8_t i;
    static uint8_t wind_hundred, wind_decade,wind_unit,temp_decade,temp_unit;
	
  while(wifi_t.response_wifi_signal_label != 0xff && wifi_t.response_wifi_signal_label !=0 && wifi_t.response_wifi_signal_label !=0xf0){

   switch(wifi_t.response_wifi_signal_label){
       
	case OPEN_OFF_ITEM:

       buzzer_sound();
        MqttData_Publish_SetOpen(0);  
        osDelay(200);//HAL_Delay(350);//350

        wifi_t.link_wifi_net_login_tencent_success=1;
      
          gkey_t.power_key_long_counter=0;
         wifi_t.smartphone_app_power_on_flag=0;
        gkey_t.key_power = power_off; //WT.EDIT 2024.02.20
         gpro_t.power_off_flag =1;
       
        wifi_t.gTimer_auto_detected_net_state_times=0; //don't need check wifi if has or not
        wifi_t.response_wifi_signal_label = 0xff;
        
	  break;

	  case OPEN_ON_ITEM:
      	buzzer_sound();
		  
	 
	   //  gkey_t.power_key_long_counter=1;
		wifi_t.link_wifi_net_login_tencent_success=1;
		MqttData_Publish_SetOpen(1);  
		osDelay(200);//HAL_Delay(350);//300

        // gkey_t.key_power = power_on;
        smartphone_turn_on_handler();

		gctl_t.ptc_warning =0;
		gctl_t.fan_warning =0;
		
		wifi_t.gTimer_auto_detected_net_state_times=0;//don't need check wifi if has or not
		wifi_t.response_wifi_signal_label = 0xff;

	  break;

	  case PTC_ON_ITEM:
	  if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
	    if(ptc_error_state() ==0){
			
	         Ptc_On();
             gctl_t.ptc_flag=1;
             Disp_Dry_Icon();
             MqttData_Publish_SetPtc(0x01);
             osDelay(200);//HAL_Delay(350);//350ms
             buzzer_sound()	;
		
	     }
		
          
		 wifi_t.gTimer_auto_detected_net_state_times=0;//don't need check wifi if has or not
         wifi_t.response_wifi_signal_label=0xff;
	  	}
	    
	   break;

	  case PTC_OFF_ITEM:
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
		
		 
         MqttData_Publish_SetPtc(0);
		 HAL_Delay(350);

		
		 Ptc_Off();
		 gctl_t.ptc_flag=0;
         Disp_Dry_Icon();
         buzzer_sound()	;
	
	    

		wifi_t.response_wifi_signal_label = 0xff;
		wifi_t.gTimer_auto_detected_net_state_times=0;
		wifi_t.linking_tencent_cloud_doing =0;


	  	}
	  	break;

	  case ANION_OFF_ITEM: //"杀菌" //5
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
		
		    Plasma_Off();
	    
			
            MqttData_Publish_SetPlasma(0);
			HAL_Delay(200);
           gctl_t.plasma_flag=0;
           Disp_Kill_Icon();
           	buzzer_sound();
           
		  
		}
        wifi_t.gTimer_auto_detected_net_state_times=0;
		wifi_t.linking_tencent_cloud_doing =0;
       wifi_t.response_wifi_signal_label = 0xff;
		
	   break;
		
	  case ANION_ON_ITEM: //plasma 
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			
			Plasma_On();
	  
            MqttData_Publish_SetPlasma(1);
		    HAL_Delay(200);
           gctl_t.plasma_flag=1;
            Disp_Kill_Icon();
            buzzer_sound();
		   
			}
        wifi_t.gTimer_auto_detected_net_state_times=0;
       wifi_t.linking_tencent_cloud_doing =0;
	   wifi_t.response_wifi_signal_label=0xff;
	    break;

	  case SONIC_OFF_ITEM://ultransonic off
        if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
          
			Ultrasonic_Pwm_Stop();
	    
            MqttData_Publish_SetUltrasonic(0);
			HAL_Delay(200);
            gctl_t.ultrasonic_flag=0;
             Disp_Ultrsonic_Icon();
            buzzer_sound();
            
		}
        wifi_t.gTimer_auto_detected_net_state_times=0;
	     wifi_t.response_wifi_signal_label=0xff;
		
		 wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case SONIC_ON_ITEM://ultransonic on
	    if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			
			
			Ultrasonic_Pwm_Output();
            MqttData_Publish_SetUltrasonic(1);
			HAL_Delay(200);
          
            gctl_t.ultrasonic_flag=1;
            Disp_Ultrsonic_Icon();
             buzzer_sound();
            //send data to the second display board
           
		}
      wifi_t.gTimer_auto_detected_net_state_times=0;
	   wifi_t.response_wifi_signal_label=0xff;
	  
        wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case STATE_TIMER_MODEL_ITEM: //display timer timing of value  
	  if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
	  	    buzzer_sound();
	       
	        gkey_t.key_mode=disp_timer_timing;
            gctl_t.ai_flag = 0 ; //timer model

            //gkey_t.key_mode_switch_flag = 1;
          //  gkey_t.key_add_dec_mode = set_temp_value_item;
            LCD_Disp_Timer_Timing_Init();

            
       
         
            MqttData_Publish_SetState(2); //timer model  = 2, works model = 1
			HAL_Delay(200);
            //do someting disp timer timing value 

            //send data to the second display board
            
		
			
        }
       wifi_t.gTimer_auto_detected_net_state_times=0;
	   wifi_t.response_wifi_signal_label = 0xff;
	
	   
	  break;
		
	  case STATE_AI_MODEL_ITEM: //display works timing 
	  	 if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
		    buzzer_sound();
		    gkey_t.key_mode=disp_works_timing;
            gctl_t.ai_flag = 1;//AI mode

          //   gkey_t.key_mode_switch_flag = 1;
           
         //   gkey_t.key_add_dec_mode = set_temp_value_item;
         
            LCD_Disp_Works_Timing_Init();

           

           
            MqttData_Publish_SetState(1); //beijing timing = 1
			HAL_Delay(200);

            //send data to the second display board
			
          
        }
       wifi_t.gTimer_auto_detected_net_state_times=0;

	   wifi_t.response_wifi_signal_label = 0xff;
		
		  
			 wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case TEMPERATURE_ITEM:
	   if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){

           
			 gpro_t.gTimer_run_dht11=0;  // don't display sensor of temperature value 
             temp_decade=rx_tencent_num_buffer[13]-0x30; //
             temp_unit=rx_tencent_num_buffer[14]-0x30;
            gctl_t.gSet_temperature_value = temp_decade*10 +  temp_unit;
            if(gctl_t.gSet_temperature_value > 40)   gctl_t.gSet_temperature_value=40;
            if(gctl_t.gSet_temperature_value <20 )   gctl_t.gSet_temperature_value=20;
            
            
            MqttData_Publis_SetTemp(gctl_t.gSet_temperature_value);
			osDelay(20);//HAL_Delay(200);//350
		

           
            gkey_t.key_add_dec_mode = set_temp_value_item; //set_temp_value_item;
            gkey_t.gTimer_set_temp_value  = 0;
            gpro_t.set_temperature_value_success =1;
        
           gpro_t.gTimer_run_dht11=0; 
           wifi_t.gTimer_auto_detected_net_state_times=0;
         
            Disp_SetTemp_Value(gctl_t.gSet_temperature_value );
            //send data to the second display board
            
			
			if(gctl_t.gSet_temperature_value > gctl_t.dht11_temp_value){

		    	
					gctl_t.ptc_flag =1;
					Ptc_On();
				    Disp_Dry_Icon();
                    
                     MqttData_Publish_SetPtc(1);
                     osDelay(20);//HAL_Delay(200);

                     gpro_t.gTimer_run_dht11=10;  //at once display sensor of temperature value 
                      //send data to the second display board
		    	

			}
			else if(gctl_t.gSet_temperature_value <   gctl_t.dht11_temp_value || gctl_t.gSet_temperature_value ==   gctl_t.dht11_temp_value ){
		   		
                    gctl_t.ptc_flag = 0;
			   		Ptc_Off();
                    Disp_Dry_Icon();
                    
                     MqttData_Publish_SetPtc(0);
                      osDelay(20);//HAL_Delay(200);

                     gpro_t.gTimer_run_dht11=10;  //at once display sensor of temperature value 
                      //send data to the second display board
			   		
				
			}

//          for(i=0;i< recoder_tx_net_data_counter;i++){
//
//            rx_tencent_num_buffer[i]=0;
//
//
//          }
		    buzzer_sound();
		}
      wifi_t.response_wifi_signal_label = 0xff;
	 
	  wifi_t.gTimer_auto_detected_net_state_times=0;	
	  wifi_t.linking_tencent_cloud_doing =0;
	  break;

	  case FAN_ITEM:
	    if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			buzzer_sound();

		     if(gctl_t.fan_warning ==0){
                 if(wifi_t.fin_speed_full_flag ==1){
           		      wind_hundred =rx_tencent_num_buffer[6]-0x30;
                 
                     wind_decade=rx_tencent_num_buffer[7]-0x30;
                     wind_unit = rx_tencent_num_buffer[8]-0x30;
                     wind_total_value= 100;;

                 }
                 else{
                    wind_hundred =rx_tencent_num_buffer[5] =0;
                    wind_decade=rx_tencent_num_buffer[6]-0x30;
                    wind_unit = rx_tencent_num_buffer[7]-0x30;

                   wind_total_value =wind_decade*10 + wind_unit   ;
                   
                  



                 }

                if(wind_hundred ==1 && wind_decade==0 && wind_unit==0){
                    
                    wifi_t.set_wind_speed_value=0;
                    

                 }
                else{
                     
                     
                     if(wind_total_value < 34 ){

                         wifi_t.set_wind_speed_value=2;
                          //send data to the second display board

                     }
                     else if(wind_total_value > 33 && wind_total_value < 67){
                         wifi_t.set_wind_speed_value=1;
                          //send data to the second display board


                     }
                     else{

                        wifi_t.set_wind_speed_value=0;
                         //send data to the second display board

                     }
                     
                 }

                
			     MqttData_Publis_SetFan(wind_total_value);
        	     HAL_Delay(350);//
    	   
          
		       }
			   else{
				 wifi_t.set_wind_speed_value=0;

			    MqttData_Publis_SetFan(wifi_t.set_wind_speed_value);
				HAL_Delay(350);
				//do seomthing 


			    }

              
               
            
		}
        
	   wifi_t.gTimer_auto_detected_net_state_times=0;
	    wifi_t.response_wifi_signal_label = 0xff;
	
		 	
			 wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case APP_TIMER_POWER_ON_REF :

		//   gpro_t.gTimer_gpro_temp_delay =0;
		   wifi_t.set_beijing_time_flag=0;
		   wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
          
		   smartphone_app_timer_power_on_handler();

	      wifi_t.response_wifi_signal_label=0xff;
	
		 wifi_t.gTimer_auto_detected_net_state_times=0;
		  wifi_t.linking_tencent_cloud_doing =0;

	  break;


   }


   if(wifi_t.response_wifi_signal_label==0xff){
        
        wifi_t.response_wifi_signal_label=0xf0;
        wifi_t.tencent_rx_data_success=0;

		for(i=0;i<15;i++){
		   rx_tencent_num_buffer[i]=0;
		}
    }
}
}


/*****************************************************************************
	*
	*Function Name:static void smartphone_app_timer_power_on_handler(void)
	*Function:
	*Inpur Ref:
	*Retern Ref:
	*
*****************************************************************************/
static void smartphone_app_timer_power_on_handler(void)
{

   static uint8_t app_step;


    if(app_step==0 ){

		app_step=1;
		if(strstr((char *)TCMQTTRCVPUB,"open\":1")){
		wifi_t.smartphone_app_power_on_flag=1;
		}

		if(wifi_t.smartphone_app_power_on_flag==1){

		if(strstr((char *)TCMQTTRCVPUB,"ptc\":1")){

		gctl_t.ptc_flag=1;
		}
		else{ //if(strstr((char *)TCMQTTRCVPUB,"ptc\":0")){

		gctl_t.ptc_flag=0;
	//	gpro_t.gTimer_gpro_temp_delay =0;

		}

		if(strstr((char *)TCMQTTRCVPUB,"sonic\":1")){

		gctl_t.ultrasonic_flag=1;

		}
		else{// if(strstr((char *)TCMQTTRCVPUB,"sonic\":0")){

		gctl_t.ultrasonic_flag=0;

		}

		if(strstr((char *)TCMQTTRCVPUB,"Anion\":1")){
		gctl_t.plasma_flag=1;
		}
		else{ // if(strstr((char *)TCMQTTRCVPUB,"Anion\":0")){
		gctl_t.plasma_flag=0;
		}



		}
    }
   

   

	if(app_step==1){
	  
		buzzer_sound();

		//Device_Action_Publish_Handler();
		if(gkey_t.key_power == power_off ){
			//gkey_t.key_power = power_on;   
		
		//	gpro_t.run_process_step=0;
			wifi_t.smartphone_app_power_on_flag=1;
            wifi_t.link_tencent_thefirst_times=0;
            wifi_t.set_wind_speed_value=0;
            smartphone_turn_on_handler();
		}
        
		
		if(gctl_t.ptc_flag ==0)gpro_t.app_ptc_flag = 1;

        MqttData_Publis_App_PowerOn_Ref();
  
		
		HAL_Delay(200);//
		app_step=0;

	

	}


    
}
      
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_Link_Net_InputInfo_Handler(void)
{
    
          strcpy((char *)wifi_t.data, (const char *)wifi_t.wifi_data);
          wifi_t.data_size = wifi_t.wifi_uart_rx_counter;


		   if(wifi_t.soft_ap_config_flag==1){

               if(strstr((const char*)wifi_t.data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              		wifi_t.soft_ap_config_success=1;
					

               	}

			
            else{
				  if(strstr((const char*)wifi_t.data,"+TCMQTTCONN:OK")){

				     wifi_t.repeat_login_tencent_cloud_init_ref=0;
	                 wifi_t.link_wifi_net_login_tencent_success=1;
				     wifi_t.auto_link_login_tencent_cloud_flag=1;
				  //link to tencent cloud is success .
	              wifi_t.linking_tencent_cloud_doing=0; //release this flag. usart
				
				  wifi_t.soft_ap_config_flag=0;
			
				  
			  }

				  
		   		if(strstr((char*)wifi_t.wifi_temp_data,"+CME ERROR:208")){
						
					   
						   wifi_t.link_wifi_net_login_tencent_success =0;
						   wifi_t.rx_error_codes_flag= 1;
						   wifi_t.rx_setup_hardware_counter=0;
						
						   wifi_t.gTimer_wifi_rx_error = 0;
						   wifi_t.get_rx_beijing_time_enable=0;
					   
		   
				 }
           
           }
		  }
		  else{

		     if(strstr((const char*)wifi_t.data,"+TCMQTTCONN:OK")){
			 	
                 
				  wifi_t.link_wifi_net_login_tencent_success=1;
			      wifi_t.repeat_login_tencent_cloud_init_ref=0;
                  gkey_t.wifi_led_fast_blink_flag =0;
				
	              wifi_t.linking_tencent_cloud_doing=0;
				  wifi_t.auto_link_login_tencent_cloud_flag=1;
		
				  wifi_t.soft_ap_config_flag=0;
				  
			  }

			  if(strstr((char*)wifi_t.wifi_temp_data,"+CME ERROR:208")){
				
			
				wifi_t.link_wifi_net_login_tencent_success =0;
				wifi_t.rx_error_codes_flag= 1;
				wifi_t.rx_setup_hardware_counter=0;
				
				wifi_t.gTimer_wifi_rx_error = 0;
				wifi_t.get_rx_beijing_time_enable=0;
                gkey_t.wifi_led_fast_blink_flag =0;
			

			  }

			 


		  }
       
         wifi_t.wifi_uart_rx_counter=0;

         //
         
        
        if(strstr((const char*)wifi_t.data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              		wifi_t.soft_ap_config_success=1;
					
            }
           
            if(strstr((const char*)wifi_t.data,"+TCMQTTCONN:OK")){
	              wifi_t.link_wifi_net_login_tencent_success =1;
	              wifi_t.linking_tencent_cloud_doing=0;
                  gkey_t.wifi_led_fast_blink_flag =0;
				  
	              wifi_t.gTimer_auto_detected_net_state_times=0;
				  wifi_t.soft_ap_config_flag=0;
			  }
              else if(strstr((char*)wifi_t.data,"+CME ERROR:208")){
						
			       wifi_t.linking_tencent_cloud_doing=0; //release this flag. usart
				   //wifi_t.esp8266_login_cloud_success =0;
                   wifi_t.link_wifi_net_login_tencent_success=0;
				
			
				
				   wifi_t.get_rx_beijing_time_enable=0;
                    gkey_t.wifi_led_fast_blink_flag =0;
		    }
            else if(strstr((char*)wifi_t.data,"+TCPRDINFOSET:1,\"EHQB1P53IH\"")){ //WT.EDIT 2024.07.22

                    // wifi_t.serch_for_wifi_flag=1;
                   //  wifi_t.gTime_link_time_start =0;

              }
              else  if(strstr((char*)wifi_t.data,"+TCMQTTCONN:FAIL,202")){

                      //  wifi_t.esp8266_login_cloud_success =0;
                       wifi_t.link_wifi_net_login_tencent_success=0;
                       wifi_t.link_wifi_net_login_tencent_success=0;   //WT.EDIT .2024.07.31
                        
                      //wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                      wifi_t.linking_tencent_cloud_doing=0;
                      wifi_t.get_rx_beijing_time_enable=0;
                      gkey_t.wifi_led_fast_blink_flag =0;
              }
		  else if(strstr((const char*)wifi_t.data,"+TCMQTTCONN:OK")){
	              wifi_t.link_wifi_net_login_tencent_success=1;
	              wifi_t.linking_tencent_cloud_doing=0;
		           wifi_t.gTimer_auto_detected_net_state_times=0;
                   gkey_t.wifi_led_fast_blink_flag =0;
				
				  wifi_t.soft_ap_config_flag=0;
			  }
             else if(strstr((char*)wifi_t.data,"+CME ERROR:208")){
				
			
				 wifi_t.link_wifi_net_login_tencent_success=0;//wifi_t.esp8266_login_cloud_success =0;
			     wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
			     wifi_t.get_rx_beijing_time_enable=0;
                  gkey_t.wifi_led_fast_blink_flag =0;
			
             }
              else if(strstr((char*)wifi_t.data,"+TCMQTTCONN:FAIL,202")){

                       wifi_t.link_wifi_net_login_tencent_success=0; //wifi_t.esp8266_login_cloud_success =0;
                       
                        
                      wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                          
                      wifi_t.get_rx_beijing_time_enable=0;
                       gkey_t.wifi_led_fast_blink_flag =0;
              }
              else if(strstr((char*)wifi_t.data,"+TCMQTTDISCON")){
          
                      wifi_t.link_wifi_net_login_tencent_success=0;//wifi_t.esp8266_login_cloud_success =0;
                       wifi_t.get_rx_beijing_time_enable=0;
                       wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                       gkey_t.wifi_led_fast_blink_flag =0;
              }
              else if(strstr((char*)wifi_t.data,"+TCSAP:WIFI_CONNECT_FAILED")){
          
                      wifi_t.link_wifi_net_login_tencent_success=0;//wifi_t.esp8266_login_cloud_success =0;
                      wifi_t.get_rx_beijing_time_enable=0;
                      wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                      gkey_t.wifi_led_fast_blink_flag =0;
              

              }
  
    wifi_t.wifi_uart_rx_counter=0;
    wifi_t.once_rx_data_done = 1;    
}



void clear_rx_copy_data(void)
{
   //uint8_t i;
   #if 0
   if( wifi_t.once_rx_data_done == 1 && wifi_link_net_state() == 0){

      wifi_t.once_rx_data_done ++;

      for(i=0;i<150;i++){
        
       wifi_t.wifi_data[i]=0;
      // wifi_t.auto_det_data[i]=0;
      }
    }
   #endif 
   if( wifi_t.once_rx_data_done == 1){
      wifi_t.once_rx_data_done ++; //WT.EDIT 2024.08.17
      memset(wifi_t.data,'\0',150);
    }
}

