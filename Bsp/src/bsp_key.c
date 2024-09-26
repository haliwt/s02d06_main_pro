#include "bsp_key.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


key_fun_t gkey_t;

static void  key_mode_be_pressed_send_data_wifi(void);


/*********************************************************************************
*
*	函 数 名:static void mode_long_short_key_fun(void)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   
*********************************************************************************/
void power_long_short_key_fun(void)
{

  
    if(KEY_POWER_VALUE() == 1 && gkey_t.power_key_long_counter > 0 && gkey_t.power_key_long_counter < 60){


        gkey_t.power_key_long_counter++;
        if( gkey_t.power_key_long_counter > 15   && KEY_POWER_VALUE() == 1){
             gkey_t.power_key_long_counter = 200;
        
          
             	//WIFI CONNCETOR process
			 gkey_t.wifi_led_fast_blink_flag=1;
             gpro_t.link_net_step=0;
			 //WIFI CONNCETOR process
			gpro_t.tencent_link_success =0;
			gpro_t.get_beijing_time_success =0;
			
		     gpro_t.disp_link_wifi_comd_flag=1;
             gpro_t.disp_set_wifi_link_cmd = 0;
            
		
			wifi_t.gTimer_linking_tencent_duration=0; //120s

            Buzzer_KeySound();
            Disip_Wifi_Icon_State();
          

        }

    }
    else if(KEY_POWER_VALUE() == 0 && gkey_t.power_key_long_counter >0 && gkey_t.power_key_long_counter<15){ //short key of function

        gkey_t.power_key_long_counter=0;

      
           if(gkey_t.key_power==power_off){
              gkey_t.key_power=power_on;
              gkey_t.key_mode = disp_timer_timing;
               gctl_t.ai_flag = 1;
               gctl_t.ptc_warning =0;
               gctl_t.fan_warning =0;
              
              gctl_t.step_process=0;
              gpro_t.power_off_flag =1;
              gpro_t.send_data_power_on_flag = power_on;
            //  SendData_Set_Command(0x01, 0x01); // power on ->to second display 
            }
           else{
              gkey_t.key_power=power_off;
              gctl_t.step_process=0;
            // SendData_Set_Command(0x01, 0x0); // power off ->to second display 
              gpro_t.send_data_power_on_flag = power_off;

           }
           Buzzer_KeySound();
       
      

        }
}
/*********************************************************************************
*
*	函 数 名:void smartphone_power_on_handler(uint8_t phone_on)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   
*********************************************************************************/
void smartphone_power_on_handler(void)
{

      
      gkey_t.power_key_long_counter=0;

      gkey_t.key_power=power_on;
      gkey_t.key_mode = disp_timer_timing;
       gctl_t.ai_flag = 1;
       gctl_t.ptc_warning =0;
       gctl_t.fan_warning =0;
      
        gctl_t.step_process=0;
        gpro_t.power_off_flag =1;
            
          
    //   Buzzer_KeySound();

}
/*********************************************************************************
*
*	函 数 名:static void mode_long_short_key_fun(void)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   
*********************************************************************************/
void mode_long_short_key_fun(void)
{
    if(KEY_MODE_VALUE() == 1 && gkey_t.key_mode_long_counter < 100 && gkey_t.key_mode_long_counter >0){


        gkey_t.key_mode_long_counter++;
        if(gkey_t.key_mode_long_counter >  15  && KEY_MODE_VALUE() == 1){
            gkey_t.key_mode_long_counter = 150;

            gkey_t.key_mode = mode_set_timer;
           gkey_t.key_add_dec_mode = mode_set_timer;
           gctl_t.ai_flag = 0; //timer tiiming model
           gkey_t.gTimer_disp_set_timer = 0;       //counter exit timing this "mode_set_timer"
          
           buzzer_sound();
           Set_Timer_Timing_Lcd_Blink();
           

        }

    }
    else if(KEY_MODE_VALUE() == 0 && gkey_t.key_mode_long_counter >0 && gkey_t.key_mode_long_counter<15){ //short key of function

        gkey_t.key_mode_long_counter=0;

     
         if(gkey_t.key_mode  == disp_works_timing){
             gkey_t.key_mode  = disp_timer_timing;
           
               gctl_t.ai_flag = 0; // DON'T DISP AI ICON
               //counter exit timing this "mode_set_timer"
            gkey_t.key_mode_switch_flag = 1;
            gkey_t.key_add_dec_mode = set_temp_value_item;
            
            LCD_Disp_Timer_Timing_Init();
             disp_ai_iocn();
             buzzer_sound();
             SendData_Set_Command(0x27,0x02); //timer timing.
             HAL_Delay(10);
             
            gkey_t.key_mode_be_pressed = 2;
            gpro_t.gTimer_disp_short_time=0;
           

        }
        else{
            gkey_t.key_mode_switch_flag = 1;
            gkey_t.key_mode  = disp_works_timing;
            gkey_t.key_add_dec_mode = set_temp_value_item;
            gctl_t.ai_flag = 1; // AI DISPLAY AI ICON
           
            LCD_Disp_Works_Timing_Init();
             disp_ai_iocn();
             buzzer_sound();
             SendData_Set_Command(0x27,0x01); //works time .
             HAL_Delay(10);
            
           
            gkey_t.key_mode_be_pressed = 1;
            gpro_t.gTimer_disp_short_time=0;
             
         }


     }

    key_mode_be_pressed_send_data_wifi();

 }


static void  key_mode_be_pressed_send_data_wifi(void)
{
   
   if(gkey_t.key_mode_be_pressed == 1 && gpro_t.tencent_link_success==1){

         gkey_t.key_mode_be_pressed= 0xff;
    
        MqttData_Publish_SetState(1); //timer model  = 2, works model = 1
        osDelay(20);
     }
     else if(gkey_t.key_mode_be_pressed == 2  && gpro_t.tencent_link_success==1){
            gkey_t.key_mode_be_pressed= 0xff;

          MqttData_Publish_SetState(2); //timer model  = 2, works model = 1
           osDelay(20);
       }


}


/***************************************************************************
    *
    *Function Name:void Dec_Key_Fun(uint8_t cmd)
    *Function : key down function 
    *Input Ref: run cmd
    *Return Ref : NO
    *
***************************************************************************/
void Dec_Key_Fun(uint8_t cmd)
{

 

    switch(cmd){

         case set_temp_value_item: //set temperature 

             gpro_t.gTimer_run_main_fun =0;
             gpro_t.gTimer_run_dht11=0; //不显示，实际的温度值，显示设置的温度
         
             gctl_t.gSet_temperature_value  --;
			if( gctl_t.gSet_temperature_value  <20)  gctl_t.gSet_temperature_value  =40;
	        else if( gctl_t.gSet_temperature_value   >40) gctl_t.gSet_temperature_value  =40;

           if( gctl_t.gSet_temperature_value   > 40) gctl_t.gSet_temperature_value  = 20;
				
			   glcd_t.number3_low =  gctl_t.gSet_temperature_value   / 10 ;
            glcd_t.number3_high =  gctl_t.gSet_temperature_value   / 10 ;
			   glcd_t.number4_low  = gctl_t.gSet_temperature_value   % 10; //
            glcd_t.number4_high =  gctl_t.gSet_temperature_value   % 10; //

        
          
            gctl_t.send_ptc_state_data_flag =0;  //send data to tencent to tell ptc on or off state .
         
            gkey_t.set_temp_value_be_pressed =1;
           
     
            gpro_t.gTimer_set_temp_temp=0;
           gpro_t.app_ptc_flag = 0;
         break;

         case mode_set_timer: //set timer timing value 

         //   gkey_t.key_sound_flag = key_sound;
              gkey_t.gTimer_disp_set_timer = 0; 

              gpro_t.set_timer_timing_minutes =0;

              gpro_t.set_timer_timing_hours -- ;//run_t.dispTime_minutes = run_t.dispTime_minutes - 1;
				if(gpro_t.set_timer_timing_hours < 0){//if(run_t.dispTime_minutes < 0){

				    gpro_t.set_timer_timing_hours =24;//run_t.dispTime_hours --;
					
					
				}
            

         
         Set_Timer_Timing_Lcd_Blink();
         SendData_Tx_Data(0x4C, gpro_t.set_timer_timing_hours );

         break;
 
         }
}

/***************************************************************************
    *
    *Function Name:void Dec_Key_Fun(uint8_t cmd)
    *Function : key down function 
    *Input Ref: run cmd
    *Return Ref : NO
    *
***************************************************************************/
void Add_Key_Fun(uint8_t cmd)
{

  //static uint8_t add_key;
   switch(cmd){
        
    case set_temp_value_item:  //set temperature value 

         gpro_t.gTimer_run_main_fun=0;
         gpro_t.gTimer_run_dht11=0;
        gctl_t.gSet_temperature_value   ++;
        if(gctl_t.gSet_temperature_value   < 20){
            gctl_t.gSet_temperature_value  =20;
        }

        if(gctl_t.gSet_temperature_value   > 40)gctl_t.gSet_temperature_value  = 20;

        glcd_t.number3_low = gctl_t.gSet_temperature_value   / 10 ;
        glcd_t.number3_high = gctl_t.gSet_temperature_value   / 10 ;
        glcd_t.number4_low  =gctl_t.gSet_temperature_value   % 10; //
        glcd_t.number4_high = gctl_t.gSet_temperature_value  % 10; //

     
    
      
        gctl_t.send_ptc_state_data_flag =0; //send data to tencent to tell ptc on or off state .
    
     //   Disp_SetTemp_Value(gctl_t.gSet_temperature_value );

        //add_key = 1;
        gkey_t.set_temp_value_be_pressed = 1;
       

       gpro_t.gTimer_set_temp_temp=0;
       gpro_t.app_ptc_flag = 0;
    break;

    case mode_set_timer: //set timer timing value 

        // gkey_t.key_sound_flag = key_sound;

         gkey_t.gTimer_disp_set_timer = 0; 

         gpro_t.set_timer_timing_minutes=0;


        	gpro_t.set_timer_timing_hours++ ;//run_t.dispTime_minutes = run_t.dispTime_minutes + 60;
		   if(gpro_t.set_timer_timing_hours > 24){ //if(run_t.dispTime_minutes > 59){

		          gpro_t.set_timer_timing_hours=0;//run_t.dispTime_hours =0;
		                
             }
	
        Set_Timer_Timing_Lcd_Blink();

        SendData_Tx_Data(0x4C, gpro_t.set_timer_timing_hours );
        
     break;
        
    }

}


void key_add_dec_set_temp_value_fun(void)
{

    if((gkey_t.set_temp_value_be_pressed == 1 || g_tDisp.disp_set_temp_value_flag==1)&& gpro_t.gTimer_set_temp_temp < 2){

        gpro_t.gTimer_run_dht11=0; 
        Disp_SetTemp_Value(gctl_t.gSet_temperature_value );
     
    }
    else if((gkey_t.set_temp_value_be_pressed == 1 || g_tDisp.disp_set_temp_value_flag==1)&& gpro_t.gTimer_set_temp_temp > 1){
      
       gpro_t.gTimer_run_dht11=0; 
      if(gkey_t.set_temp_value_be_pressed ==1){

          gkey_t.set_temp_value_be_pressed ++;
           lcd_donot_disp_number_34_temperature();
            osDelay(200);
            Disp_SetTemp_Value(gctl_t.gSet_temperature_value );
            
            gpro_t.set_temperature_value_success =1;


        }
       else if(g_tDisp.disp_set_temp_value_flag == 1){

          g_tDisp.disp_set_temp_value_flag++;

           lcd_donot_disp_number_34_temperature();
            osDelay(200);
            Disp_SetTemp_Value(gctl_t.gSet_temperature_value );
            gpro_t.set_temperature_value_success =1;

           
      }
       set_temp_value_compare_dht11_temp_value();

     if(gpro_t.tencent_link_success==1){
        
        MqttData_Publis_SetTemp(gctl_t.gSet_temperature_value);
        osDelay(50);

        MqttData_Publish_SetPtc(gctl_t.ptc_flag);
        osDelay(50);
        
        }

    }
}
    
 
/********************************************************************
 * 
 * Function Name:void set_temp_value_compare_dht11_temp_value(void)
 * 
 * 
 * 
 ********************************************************************/
void set_temp_value_compare_dht11_temp_value(void)
{

   static uint8_t first_turn_off_flag;
   static uint8_t  send_1_off =0xff,send_1_on=0xff,send_2_on=0xff,send_2_off=0xff;
   static uint8_t send_2_on_flag,send_2_off_flag,send_1_on_flag,send_1_off_flag;
    switch(gpro_t.set_temperature_value_success){

    case 1:

        if(gctl_t.gSet_temperature_value > gctl_t.dht11_temp_value){

            if(gctl_t.manual_turn_off_ptc_flag ==0){
                gctl_t.ptc_flag = 1;

                Ptc_On();
                Disp_Dry_Icon();

                gpro_t.gTimer_run_dht11=2;  //at once display sensor of temperature value 

                
                  if(send_1_on !=send_1_on_flag){
                       send_1_on = send_1_on_flag;
                       send_1_off_flag ++;
                    if(gpro_t.tencent_link_success==1){
                       MqttData_Publish_SetPtc(0x01);
                       osDelay(10);
                     }
                  }
            }


        }
        else if(gctl_t.gSet_temperature_value <   gctl_t.dht11_temp_value || gctl_t.gSet_temperature_value ==   gctl_t.dht11_temp_value){

            gctl_t.ptc_flag = 0;
            Ptc_Off();
            Disp_Dry_Icon();
             if(send_1_off !=send_1_off_flag ){
                 send_1_off = send_1_off_flag;
                 send_1_on_flag ++;
                 if(gpro_t.tencent_link_success==1){
                     MqttData_Publish_SetPtc(0x0);
                     osDelay(10);
                   }
                  

              }
            gpro_t.gTimer_run_dht11=2;  //at once display sensor of temperature value 


        }
    break;


    case 0: //default of compare real temperature value 


    if(gctl_t.dht11_temp_value > 39){

          first_turn_off_flag=1;
          gctl_t.ptc_flag = 0;
          Ptc_Off();
          Disp_Dry_Icon();

          if(send_2_off !=send_2_off_flag ){
             send_2_off = send_2_off_flag;
             send_2_on_flag ++;
    	     if(gpro_t.tencent_link_success==1){
               MqttData_Publish_SetPtc(0x0);
               osDelay(10);

               }
          }


    }
    else if(gctl_t.dht11_temp_value < 38 && first_turn_off_flag==1){

        if(gctl_t.manual_turn_off_ptc_flag ==0){
         gctl_t.ptc_flag = 1;

         Ptc_On();
         Disp_Dry_Icon();

         if(send_2_on !=send_2_on_flag ){
             send_2_on = send_2_on_flag;
             send_2_off_flag ++;
    	     if(gpro_t.tencent_link_success==1){
               MqttData_Publish_SetPtc(0x01);
               osDelay(10);
               }
          }

         gpro_t.gTimer_run_dht11=0;  //at once display sensor of temperature value 
        }


    }
    else if(gctl_t.dht11_temp_value < 40 && first_turn_off_flag==0){

          if(gctl_t.manual_turn_off_ptc_flag ==0){
             gctl_t.ptc_flag = 1;
    
             Ptc_On();
             Disp_Dry_Icon();

            if(send_2_on !=send_2_on_flag ){
                 send_2_on = send_2_on_flag;
                 send_2_off_flag ++;
        	     if(gpro_t.tencent_link_success==1){
                   MqttData_Publish_SetPtc(0x01);
                   osDelay(10);
                  }
              }
    
             gpro_t.gTimer_run_dht11=0;  //at once display sensor of temperature value 
            }



    }


    break;

    }
}



