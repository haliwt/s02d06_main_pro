#include "bsp.h"


uint8_t power_off_flag;

/*******************************************************************************************************
*	函 数 名: void mainboard_process_handler(void)
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
//void mainboard_process_handler(void)
void power_off_run_handler(void)
{

  gkey_t.power_key_long_counter=0;
  if( gkey_t.key_sound_flag == key_sound){
		gkey_t.key_sound_flag =0;
		Buzzer_KeySound();

    }

  
  
   if(gpro_t.disp_power_on_flag ==2 || gpro_t.wifi_power_on_flag==2){
      power_off_init_function();
	  if(gpro_t.disp_power_on_flag ==2)gpro_t.disp_power_on_flag++;
      if(gpro_t.wifi_power_on_flag==2)gpro_t.wifi_power_on_flag++;

   }
		
   Breath_Led();

}




void power_on_run_handler(void)
{

    
     switch(gctl_t.step_process){


		  case 0:
           if(gpro_t.disp_power_on_flag == 1 || gpro_t.wifi_power_on_flag==2){
                power_on_init_function();

           }

             power_off_flag=0;
          

            gctl_t.step_process = 1;


		  break;


       
          case 1:   //run dht11 display 

            read_senson_dht11_data();
			//LCD_Fan_Run_Icon(wifi_t.set_wind_speed_value);

          gctl_t.step_process=2;
		  
          break;

        

	  case 2: //7

       
         
        if(gctl_t.ptc_warning == 0 && gctl_t.fan_warning ==0){
        if(g_tDisp.ai_mode_flag ==1 ){
            g_tDisp.ai_mode_flag=3;

             LCD_Disp_Works_Timing_Init();
             disp_ai_iocn();

             second_disp_ai_time_fun();

         if(gpro_t.tencent_link_success==1 ){
             MqttData_Publish_SetState(1);
	         osDelay(50);//HAL_Delay(350);
           }
        }
        else if(g_tDisp.ai_mode_flag ==2 ){
            g_tDisp.ai_mode_flag =3;

            LCD_Disp_Timer_Timing_Init();
            disp_ai_iocn();
            second_disp_not_ai_timer_fun();
  

        if(gpro_t.tencent_link_success==1){
         MqttData_Publish_SetState(2);
	     osDelay(50);//HAL_Delay(350);

         }
        }

       }
         gctl_t.step_process=3;
	   
      case 3: 
	   
	   if(gpro_t.tencent_link_success==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.link_net_tencent_data_flag ==1){ //after send publish datat to tencent .){
             wifi_t.link_net_tencent_data_flag ++;
		  
		      MqttData_Publish_Update_Data();//MqttData_Publish_SetOpen(0x01);
		     osDelay(30);//HAL_Delay(20);
          
            

		}
		else if(gpro_t.tencent_link_success==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.link_net_tencent_data_flag ==2 ){
             wifi_t.link_net_tencent_data_flag ++;

           
		    //MqttData_Publish_Update_Data();
		    /// osDelay(20);//HAL_Delay(20);
		    Subscriber_Data_FromCloud_Handler();
            osDelay(30);
		     

		}
        else if(gpro_t.tencent_link_success==1 && wifi_t.link_net_tencent_data_flag ==3 ){
             
         
            wifi_t.link_net_tencent_data_flag ++;
            Update_Dht11_Totencent_Value();
            osDelay(100);
       }
       else if(gpro_t.tencent_link_success==1 && wifi_t.link_net_tencent_data_flag ==4 ){
                 wifi_t.link_net_tencent_data_flag++ ;
                   Publish_Data_Warning(fan_warning,no_warning);
                      osDelay(30);
                
                      Publish_Data_Warning(ptc_temp_warning,no_warning);
                      osDelay(30);
                
       }
		gctl_t.step_process=4;

	 break;

      case 4:

    	mainboard_active_handler();
    	gctl_t.step_process=5;
     break;

      case 5:

    	 detected_ptc_fan_error_handler();
    	 gctl_t.step_process=6;
       break;

      case 6:
	  	  
           Display_WorksTimingr_Handler(gkey_t.key_mode);
	  	  
    	  gctl_t.step_process=7;
      break;



	case 7: //check works times

	       works_two_hours_handler();
		  // LCD_Fan_Run_Icon(wifi_t.set_wind_speed_value);
		   gctl_t.step_process=1;
    break;

	default:

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
void power_off_init_function(void)
{

  if(gpro_t.power_off_flag == 1){
    		gpro_t.power_off_flag ++;
    	    
           //key set ref 
           gpro_t.disp_power_on_flag = 1;
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
    	
   
  
            wifi_t.gTimer_linking_tencent_duration=0; //120s

         
    	
      
    		wifi_t.smartphone_app_power_on_flag=0;

            gpro_t.gTimer_run_dht11=20;
            gpro_t.set_temperature_value_success=0;
           
    	    //stop main board function ref.
    	    PowerOff_Off_Led();
            SendData_Set_Command(0X01,0X00);
            osDelay(30);
    	  
		
	  }

     if(gpro_t.tencent_link_success ==1 && power_off_flag==0 ){
	
		power_off_flag++;
		MqttData_Publish_PowerOff_Ref();
	
	     
		 
		  
	}
	if(gpro_t.tencent_link_success ==1   && power_off_flag==1){
		power_off_flag++;
		
        Subscriber_Data_FromCloud_Handler();
	  
	
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
void power_on_init_function(void)
{
     

	if(gpro_t.get_beijing_time_success==0 ){

		gpro_t.disp_works_minutes_value=0;
		gpro_t.disp_works_hours_value =0;
		gpro_t.gTimer_works_counter_sencods =0;

	}


	if(wifi_t.smartphone_app_power_on_flag==0){
	  main_fun_init();

	}
	else{
		MqttData_Publis_App_PowerOn_Ref();
	    osDelay(50);//HAL_Delay(50);//

		SendWifiData_Ref_three(gctl_t.ptc_flag,gctl_t.plasma_flag,gctl_t.ultrasonic_flag);
		osDelay(10);//HAL_Delay(50);//
    }



	LED_Mode_On();
	LED_POWER_ON();
	Backlight_On();


	LCD_Numbers1234_Init();
	Display_Wind_Icon_Inint();

	Update_DHT11_Value();


	sendData_Real_TimeHum(gctl_t.dht11_humidity_value,gctl_t.dht11_temp_value);
	osDelay(5);

	LCD_Fan_Run_Icon(0);
	Disp_HumidityTemp_Init();


	if(gpro_t.tencent_link_success==1){
		MqttData_Publish_SetOpen(1);  
		osDelay(30);//300

		Subscriber_Data_FromCloud_Handler();
		HAL_Delay(30);

	}

    gpro_t.power_off_flag=1;


	//temperature value inti
	gpro_t.set_temperature_value_success=0;

	wifi_t.set_wind_speed_value=0; //init 
	gctl_t.ptc_warning = 0;
	gctl_t.fan_warning = 0;

	//timig init
	gpro_t.gTimer_run_total=0;

	gpro_t.set_timer_timing_hours =0 ;
	gpro_t.set_timer_timing_minutes =0;

	gpro_t.gTimer_timer_Counter =0;

	gpro_t.gTimer_run_dht11=100;
	wifi_t.link_net_tencent_data_flag=1;

	gkey_t.set_timer_timing_success = 0;
	gkey_t.key_add_dec_mode = disp_works_timing;
	gctl_t.ai_flag = 1; // AI DISPLAY AI ICON
	gkey_t.key_mode = disp_works_timing;
	gctl_t.gSet_temperature_value =40;


	if(gpro_t.tencent_link_success==1){

		Publish_Data_Warning(fan_warning,no_warning);
		osDelay(100);

		Publish_Data_Warning(ptc_temp_warning,no_warning);
		osDelay(100);

	}
	 TIM1723_Write_Cmd(LUM_VALUE);//(0x97);//(0x94);//(0x9B);

}

