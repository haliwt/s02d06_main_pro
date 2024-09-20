#include "bsp.h"


BSP_process_t gpro_t;
static uint8_t Works_Time_Out(void);
static void Mainboard_Action_Fun(void);

static void Process_Dynamical_Action(void);
static void power_off_function(void);
static void power_on_init_function(void);

static void interval_continuce_works_fun(void);

static void interval_two_hours_stop_action(void);

static void Detected_Fan_Error(void);
static void Detected_Ptc_Error(void);



uint8_t power_off_flag;

uint8_t  fan_continue_flag;



/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
//	/* --- 喂狗 8s input reset */
//    if(gctl_t.gTimer_prcoess_iwdg > 5){
//		gctl_t.gTimer_prcoess_iwdg =0;
//    	iwdg_feed();
//
//    }

	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();

    if(gkey_t.key_power == power_on){

        

         LCD_Wind_Run_Icon(wifi_t.set_wind_speed_value);

         Disip_Wifi_Icon_State();

 

    }
	
}


/*
*********************************************************************************************************
*	函 数 名: void mainboard_process_handler(void)
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//void mainboard_process_handler(void)
void power_off_run_handler(void)
{

  
  if( gkey_t.key_sound_flag == key_sound){
		gkey_t.key_sound_flag =0;
		Buzzer_KeySound();

    }

   power_off_function();
		
   Breath_Led();

}




void power_on_run_handler(void)
{

   static uint8_t power_on_run_dht11_times ;
     switch(gctl_t.step_process){


		  case 0:

            power_on_init_function();

             gctl_t.ai_flag = 1; // AI DISPLAY AI ICON
             gkey_t.key_mode  = disp_works_timing;

           gpro_t.gTimer_timer_Counter =0;
		   gpro_t.set_timer_timing_hours=0;
		   gpro_t.set_timer_timing_minutes=0;

        
           gpro_t.first_disp_work_time=0;
            
		    gctl_t.step_process = 1;


		  break;


       
          case 1:   //run dht11 display 

             if(gpro_t.gTimer_run_dht11 > 12  || power_on_run_dht11_times < 10){
                gpro_t.gTimer_run_dht11=0;
                power_on_run_dht11_times++;
                  Update_DHT11_Value();
                  Disp_HumidityTemp_Value();

                  if(gctl_t.interval_stop_run_flag==0){
                     SetTemp_Compare_SensoTemp();

                  }

                    

              }



          gctl_t.step_process=3;
		  
          break;

        

	  case 3: //7

	  
	   if(wifi_link_net_state()==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.link_net_tencent_data_flag ==1){ //after send publish datat to tencent .){
             wifi_t.link_net_tencent_data_flag ++;
		     gpro_t.gTimer_pro_action_publis =0;
		     MqttData_Publish_SetOpen(0x01);
		     HAL_Delay(200);
            

		}
		if(wifi_link_net_state()==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.link_net_tencent_data_flag ==2 ){
             wifi_t.link_net_tencent_data_flag ++;
		    gpro_t.gTimer_pro_action_publis =0;
           gpro_t.gTimer_publish_tencent_dht11 =20;
		    MqttData_Publish_Update_Data();
		     HAL_Delay(200);

		}

        if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3 &&   gpro_t.gTimer_publish_tencent_dht11 > 12){
             
            gpro_t.gTimer_publish_tencent_dht11=0;
            Update_Dht11_Totencent_Value();
		    

        }
	 
	   
	      gctl_t.step_process=5;

	 break;



	case 5: //check works times 
			  if(gpro_t.gTimer_run_total > 119){//119 //120 minutes
			       gpro_t.gTimer_run_total =0;
				   gpro_t.gTimer_run_time_out=0;  //time out recoder start 10 minutes
				   gpro_t.gTimer_run_one_mintue =0;
				   fan_continue_flag=0;
                   gctl_t.step_process=7;
			       gctl_t.interval_stop_run_flag  =1 ;
		         
			    }
                else if(gctl_t.interval_stop_run_flag  ==1){
                 gctl_t.step_process=7;
                
                }
			    else{
				 gctl_t.step_process=1;

                 }

		  break;

		  case 7: //works have a rest ten minutes
              if(gctl_t.interval_stop_run_flag  ==1){

		            Works_Time_Out();

                
                  gctl_t.step_process=1;

			   }
               else 
                   gctl_t.step_process=1;
                   
			 


		  break;

		  


	     }

 }      


/**********************************************************************************************************
*
*	函 数 名: void mainboard_active_handler(void)
*	功能说明: 
*			 
*	形    参: 无
*	返 回 值: 无
*
**********************************************************************************************************/
void mainboard_active_handler(void)
{

   
    if(gpro_t.gTimer_run_main_fun > 0){
                    gpro_t.gTimer_run_main_fun =0;
                  if(gctl_t.interval_stop_run_flag  ==0){
                       Process_Dynamical_Action();
                  }
                  else{
                      interval_two_hours_stop_action();
                   
    
                  }
               }
    

}

/**********************************************************************************************************
*
*	函 数 名: void disp_works_or_timer_timing_fun(void)
*	功能说明: 
*			 
*	形    参: 无
*	返 回 值: 无
*
**********************************************************************************************************/
void disp_works_or_timer_timing_fun(void)
{

   Display_WorksTimingr_Handler(gkey_t.key_mode);

}
/**********************************************************************************************************
*	函 数 名: static uint8_t Works_Time_Out(void)
*	功能说明: 主板工作2小时，停止工作10分钟
*			 
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
static uint8_t Works_Time_Out(void)
{
	if(gpro_t.gTimer_run_time_out < 11){
		
		interval_two_hours_stop_action();//Mainboard_Fun_Stop();
		 
    }

	if(gpro_t.gTimer_run_one_mintue < 60 && ( fan_continue_flag ==0)){

		Fan_Run();

	}

	if(gpro_t.gTimer_run_one_mintue > 60){

	     fan_continue_flag=1;

         Fan_Stop();
	 }

	if(gpro_t.gTimer_run_time_out > 10){ //10 minutes
		gpro_t.gTimer_run_time_out=0;
		gpro_t.gTimer_run_total=0;

        gctl_t.interval_stop_run_flag= 0;

		//Continuce_main_action_Fun();
		
        interval_continuce_works_fun();
		

		return 1;


	}
	else{
      
	  return 0;

	}

}


/**********************************************************************************************************
*	函 数 名: static void Mainboard_Action_Fun(void)
*	功能说明: 主板工作：功能动作输出
*			 
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
static void Mainboard_Action_Fun(void)
{
    Ptc_On();
	Ultrasonic_Pwm_Output();
	Fan_Run();
	Plasma_On();
	
	
}

/*
*********************************************************************************************************
*
*	函 数 名: static void interval_two_hours_stop_action(void)
*	功能说明: 主板工作：功能动作输出			 
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
static void interval_two_hours_stop_action(void)
{
   Ptc_Off();

   Ultrasonic_Pwm_Stop();
  // Fan_Stop();
   Plasma_Off();

   if(ptc_state() ==1){


      
       Disp_Dry_Icon();


     }
    else{

           
 
     Disp_Dry_Icon();
    }
            

    if(plasma_state() ==1){
		
 
       Disp_Kill_Icon();

    }
	else{

	 
       Disp_Kill_Icon();

	}

    if(ultrasonic_state() ==1){


         Disp_Ultrsonic_Icon();

    }
    else{

 
    Disp_Ultrsonic_Icon();


    }

  
 
 }


/**********************************************************************************************************
*
*	函 数 名: void Process_Dynamical_Action(void)
*	功能说明: 主板工作：功能动作输出			 
*	形    参: 无
*	返 回 值: 无
*
**********************************************************************************************************/
static void Process_Dynamical_Action(void)
{


    if(gctl_t.ai_flag == 1){

            disp_ai_symbol();

     }
     else{
       donot_disp_ai_symbol();

     }

   if(ptc_state() ==1){


       Ptc_On();
       Disp_Dry_Icon();


     }
    else{

           
     Ptc_Off();
     Disp_Dry_Icon();
    }
            

    if(plasma_state() ==1){
		
       Plasma_On();
       Disp_Kill_Icon();

    }
	else{

	   Plasma_Off();
       Disp_Kill_Icon();

	}

    if(ultrasonic_state() ==1){

         Ultrasonic_Pwm_Output();
         Disp_Ultrsonic_Icon();

    }
    else{

    Ultrasonic_Pwm_Stop();
    Disp_Ultrsonic_Icon();


    }

  
    switch(wifi_t.set_wind_speed_value){
    
         case 0: //full speed
    
    
           Fan_Run();
    
         break;
    
         case 1 : //middle speed
          Fan_Run_Middle();
    
         break;
    
         case 2: //lower speed
          Fan_Run_Lower();
         break;
    
    
       }

}


static void interval_continuce_works_fun(void)
{
  
    if(ptc_state() ==1){
    
    
          Ptc_On();
          Disp_Dry_Icon();
    
    
        }
       else{
    
              
        Ptc_Off();
        Disp_Dry_Icon();
       }
               
    
       
    
       
    
       if(plasma_state() ==1){
           
          Plasma_On();
    
       }
       else{
    
          Plasma_Off();
    
       }
    
       if(ultrasonic_state() ==1){
    
            Ultrasonic_Pwm_Output();
    
    
       }
       else{
    
       Ultrasonic_Pwm_Stop();
    
    
       }
    
     
       switch(wifi_t.set_wind_speed_value){
       
            case 0: //full speed
       
       
              Fan_Run();
       
            break;
       
            case 1 : //middle speed
             Fan_Run_Middle();
       
            break;
       
            case 2: //lower speed
             Fan_Run_Lower();
            break;
       
       
          }


}


/***********************************************************************
*
*Function Name:static void power_off_function(void)
*Function: by key be pressed power off run process
*Input Ref:NO
*Return Ref:NO
*
************************************************************************/
static void power_off_function(void)
{

  if(gpro_t.power_off_flag == 1){
    		gpro_t.power_off_flag ++;
    	    
           //key set ref 

           gkey_t.gTimer_power_off_run_times=0;
           gkey_t.wifi_led_fast_blink_flag=0;
            gctl_t.ptc_flag =0;
            gctl_t.plasma_flag =0;
            
            gctl_t.fan_warning = 0;
            gctl_t.ptc_warning = 0;
            gkey_t.set_timer_timing_success =0;
    	   
    	  
    	    //control set ref
    	    wifi_t.link_net_tencent_data_flag =1;

             gctl_t.step_process=0;
             //cotrol displ dht11 value 
            

            //wifi set ref
    	
    		wifi_t.link_tencent_thefirst_times=0;
    	
    		wifi_t.gTimer_wifi_pub_power_off=0;	
            wifi_t.gTimer_linking_tencent_duration=0; //120s

            wifi_t.repeat_login_tencent_cloud_init_ref=0;
    	    wifi_t.runCommand_order_lable= 0xff;
            wifi_t.three_times_link_beijing=0;
    		wifi_t.smartphone_app_power_on_flag=0;

            gpro_t.gTimer_run_dht11=20;
            gpro_t.set_temperature_value_success=0;
           
    	    //stop main board function ref.
    	    PowerOff_Off_Led();
    	  
		
	  }

     if(wifi_link_net_state() ==1 && power_off_flag==0 ){
		wifi_t.gTimer_wifi_pub_power_off=0;
		power_off_flag++;
		MqttData_Publish_PowerOff_Ref();
		wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	     
		 
		  
	}
	if(wifi_link_net_state() ==1  && wifi_t.gTimer_wifi_sub_power_off > 4 && power_off_flag==1){
		power_off_flag++;
		wifi_t.gTimer_wifi_sub_power_off=0;
        Subscriber_Data_FromCloud_Handler();
	  
	
    }
    if(wifi_link_net_state() ==1){
       // Record_WorksTime_DonotDisp_Handler();
       wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;

    }
	
    if(	gpro_t.power_off_flag ==2){
           if(gkey_t.gTimer_power_off_run_times < 61){
                Fan_Run();
				OnlyDisp_Wind_Icon_Handler();
				

		   }
		   else{
              gpro_t.power_off_flag++;
			   Fan_Stop();
		       Backlight_Off();
               Lcd_Display_Off();
		     
			   
		  }

	}

}

/***********************************************************************
*
*Function Name:static void power_off_function(void)
*Function: by key be pressed power off run process
*Input Ref:NO
*Return Ref:NO
*
************************************************************************/
static void power_on_init_function(void)
{
     //led on 

   
    power_off_flag=0;
    gpro_t.power_off_flag=1;
    
    gkey_t.set_timer_timing_success =0;
    //temperature value inti
    gpro_t.set_temperature_value_success=0;

    wifi_t.set_wind_speed_value=0; //init 

    //timig init
    gpro_t.gTimer_run_total=0;

    gpro_t.set_timer_timing_hours =0 ;
    gpro_t.set_timer_timing_minutes =0;

     if(wifi_t.get_beijing_timing_success ==0 || wifi_link_net_state()==0){
          gpro_t.disp_works_minutes_value=0;
          gpro_t.disp_works_hours_value =0;
          gpro_t.gTimer_works_counter_sencods =0;

     }

   
    if(wifi_t.smartphone_app_power_on_flag==0){
        main_fun_init();

    }
   


    LED_Mode_On();
    LED_Power_On();
    Backlight_On();


    LCD_Numbers1234_Init();
    Display_Wind_Icon_Inint();

    Update_DHT11_Value();
    



    LCD_Wind_Run_Icon(0);
    Disp_HumidityTemp_Init();


    //fan on
    Mainboard_Action_Fun();

    if(wifi_link_net_state()==1){
    MqttData_Publish_SetOpen(1);  
    osDelay(100);//300

    Publish_Data_Warning(fan_warning,no_warning);

    Publish_Data_Warning(ptc_temp_warning,no_warning);

    }

    


}
/***************************************************************************
    *
    *Function Name:vvoid detected_error_handler(void)
    *Function : 
    *Input Ref: NO
    *Return Ref : NO
    *
***************************************************************************/
static void Detected_Fan_Error(void)
{
    if(gpro_t.gTimer_run_adc > 13 && gctl_t.interval_stop_run_flag==0){ //2 minute 180s
				gpro_t.gTimer_run_adc=0;

                if( gctl_t.interval_stop_run_flag  ==0){
				   Get_Fan_Adc_Fun(ADC_CHANNEL_0,20);
                }
				
	               

		 }


}

static void Detected_Ptc_Error(void)
{

   if(gpro_t.gTimer_ptc_detected > 6 ){ //3 minutes 120s
			gpro_t.gTimer_ptc_detected =0;	
			  Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,20);

              
					
       }



}

/***************************************************************************
    *
    *Function Name:void link_wifi_net_handler(uint8_t link)
    *Function : 
    *Input Ref: link wifi flag :1 -> link wifi ,0-> don't link wifi
    *Return Ref : NO
    *
***************************************************************************/
void link_wifi_net_handler(uint8_t link)
{
    if(link == 1){

         if(wifi_t.gTimer_linking_tencent_duration > 119){

            gkey_t.wifi_led_fast_blink_flag =0;//gpro_t.wifi_led_fast_blink_flag =0;
           if(wifi_link_net_state()==0){

             gpro_t.get_beijing_step = 10;
             wifi_t.gTimer_auto_detected_net_state_times = 120;
             wifi_t.linking_tencent_cloud_doing =1;
             wifi_t.soft_ap_config_flag =1; 
           }

        }
        else
        link_wifi_net_handler();

    }
    else{
      
       key_mode_be_pressed_send_data_wifi();
       Detected_Fan_Error();
       Detected_Ptc_Error();

    }

}


