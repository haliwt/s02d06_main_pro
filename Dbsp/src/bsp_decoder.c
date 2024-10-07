#include "bsp.h"




/**********************************************************************
    *
    *Function Name:void receive_data_fromm_display(uint8_t *pdata,uint8_t len)
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void receive_data_fromm_display(uint8_t *pdata)
{

   if(pdata[1] == 0x01){

    switch(pdata[2]){

     case 0:


     break;

     case 0x01: //表示开机指令
        wake_up_backlight_on();

        if(pdata[3] == 0x01 ){ //open
             buzzer_sound();
             
            second_disp_power_on_fun();


        }
        else if(pdata[3] == 0x0){
           buzzer_sound();
           second_disp_power_off_fun();

          
        }
       

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){

        buzzer_sound();
        wake_up_backlight_on();
        if(gctl_t.interval_stop_run_flag  ==0){
          gctl_t.manual_turn_off_ptc_flag = 0;
          gctl_t.ptc_flag = 1;
          
          Ptc_On();
          Disp_Dry_Icon();
          
          if(gpro_t.tencent_link_success==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
       }
       }
       else if(pdata[3] == 0x0){
        
          buzzer_sound();
          wake_up_backlight_on();

          gctl_t.manual_turn_off_ptc_flag = 1;
       
          gctl_t.ptc_flag = 0;
          Ptc_Off();
          Disp_Dry_Icon();
          
         if(gpro_t.tencent_link_success==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
          }
        }
     
      
     break;

     case 0x22: //compare set temp value ->PTC打开关闭指令,没有蜂鸣器声音。
      wake_up_backlight_on();

      if(pdata[3] == 0x01){

         
        
        if(gctl_t.interval_stop_run_flag  ==0){

         gctl_t.manual_turn_off_ptc_flag =0;  //at last set is active.
            
          gctl_t.ptc_flag = 1;
          Ptc_On();
          Disp_Dry_Icon();
         if(gpro_t.tencent_link_success==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
          
         
         }
       }
       else if(pdata[3] == 0x0){

       
          gctl_t.manual_turn_off_ptc_flag =0;
          gctl_t.ptc_flag = 0;
          Ptc_Off();
          Disp_Dry_Icon();
          if(gpro_t.tencent_link_success==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
           }

       }

     break;

     case 0x03: //PLASMA 打开关闭指令

     
         wake_up_backlight_on();
         
         buzzer_sound();

        if(pdata[3] == 0x01){
           
          if( gctl_t.interval_stop_run_flag ==0){
            
           
           gctl_t.plasma_flag  = 1;
           Plasma_On();
           Disp_Kill_Icon();

          if(gpro_t.tencent_link_success==1){
              MqttData_Publish_SetPlasma(0x01);
	  	      osDelay(100);//HAL_Delay(350);
          }
          
         

           }
        }
        else if(pdata[3] == 0x0){
       
           gctl_t.plasma_flag  = 0;
           Plasma_Off();
           Disp_Kill_Icon();

          if(gpro_t.tencent_link_success==1){
              MqttData_Publish_SetPlasma(0x0);
	  	      osDelay(100);//HAL_Delay(350);
          }
         

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

         buzzer_sound();

        wake_up_backlight_on();
            
       if(pdata[3] == 0x01){  //open 

         if( gctl_t.interval_stop_run_flag ==0){
          
           gctl_t.ultrasonic_flag =1;
            Ultrasonic_Pwm_Output();
             Disp_Ultrsonic_Icon();

             if(gpro_t.tencent_link_success==1){
              MqttData_Publish_SetUltrasonic(0x01);
	  	      osDelay(100);//HAL_Delay(350);
             }



          }

        }
        else if(pdata[3] == 0x0){ //close 

             gctl_t.ultrasonic_flag =0;
            Ultrasonic_Pwm_Stop();
             Disp_Ultrsonic_Icon();

              if(gpro_t.tencent_link_success==1){
              MqttData_Publish_SetUltrasonic(0x0);
	  	      osDelay(100);//HAL_Delay(350);
             }

        }


     break;

      case 0x05: // link wifi command

          wake_up_backlight_on();
         

       if(pdata[3] == 0x01){  // link wifi 
        

        second_disp_set_link_wifi_fun();

           
            

        }
        else if(pdata[3] == 0x0){ //don't link wifi 

        }


     break;

     case 0x06: //buzzer sound done
         wake_up_backlight_on();
        if(pdata[3] == 0x01){  //
            buzzer_sound();
           

        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .



        }


     break;



     break;


     case 0x0A: //the second display board link state. dc power on the first link state

        wake_up_backlight_on();
        if(pdata[3] == 0x01){  //
          if(gkey_t.key_power==power_on){
            g_tDisp.disp_second_link_state_flag =1;

          }
          else{
              g_tDisp.disp_second_link_state_flag =0;


          }
           

        }
       

     break;


      case 0x1A: //读取传感的温度数据
          wake_up_backlight_on();
        if(pdata[3] == 0x0F){ //数据
          
          gpro_t.set_temperature_value_success=1;
          gkey_t.set_temp_value_be_pressed = 1;     //send data to tencent flag.
          gctl_t.gSet_temperature_value  = pdata[5] ;

        }
      break;

      case 0x2A:   //按键设置的温度值

         wake_up_backlight_on();
        if(pdata[4] == 0x01){ //数据
           gpro_t.gTimer_set_temp_temp=0;
          g_tDisp.disp_set_temp_value_flag =1;
        //  gpro_t.set_temperature_value_success=1;
          gctl_t.gSet_temperature_value  = pdata[5] ;

        }


      break;

      case 0x1B: //湿度数据

        if(pdata[3] == 0x0F){ //数据
            

        }
      break;

      case 0x1C: //表示时间：小时，分，秒

        if(pdata[3] == 0x0F){ //数据

          


        }
      break;

        case 0x1D: //表示日期： 年，月，日

        if(pdata[3] == 0x0F){ //数据

             
            

        }
      break;


     

     case 0x27: //AI mode 
      wake_up_backlight_on();

      if(pdata[3] == 0x01){ //AI mode ,don't buzzer sound .
        
        g_tDisp.ai_mode_flag =1;
        gkey_t.key_mode = disp_works_timing;
      //   second_disp_ai_time_fun();
      }
       else{
      
        gkey_t.key_mode = disp_timer_timing;
        g_tDisp.ai_mode_flag =2 ;
        gpro_t.gTimer_disp_short_time =0;
       // second_disp_not_ai_timer_fun();
       
            

        }
        
     break;


     case 0x4C: //display #1 or display #2  timer timing .
         wake_up_backlight_on();
         if(pdata[4]==0x01){

            gkey_t.key_mode = mode_set_timer;
        
            g_tDisp.second_disp_set_temp_flag=1; //send data to the second display board .
            gkey_t.gTimer_disp_set_timer = 0; 
            gpro_t.set_timer_timing_minutes =0;
          
            gpro_t.set_timer_timing_hours = pdata[5];
           // Set_Timer_Timing_Lcd_Blink();
         }


     break;
        
     
     }

   }

}

/**********************************************************************
    *
    *Function Name:uint8_t bcc_check(const unsigned char *data, int len) 
    *Function: BCC校验函数
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
uint8_t bcc_check(const unsigned char *data, int len) 
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}


