#include "bsp.h"

disp_two_state gdisp_t;

static void disp_power_on(void);
static void disp_power_off(void);


/**********************************************************************
    *
    *Function Name:static void disp_power_on(void);
    *Function: 
    *Input Ref:NO
    *Return Ref:NO
    *
***********************************************************************/
static void disp_power_on(void)
{

    gkey_t.key_power=power_on;
    gkey_t.key_mode = disp_timer_timing;
    gctl_t.ai_flag = 1;
    gctl_t.ptc_warning =0;
    gctl_t.fan_warning =0;

    gctl_t.step_process=0;
    gpro_t.power_off_flag =1;



}
static void disp_power_off(void)
{

   gkey_t.key_power=power_off;
    gctl_t.step_process=0;

           
    Buzzer_KeySound();



}





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

        if(pdata[3] == 0x01 ){ //open
             buzzer_sound();
             disp_power_on();
          

        }
        else if(pdata[3] == 0x0){
           buzzer_sound();

           disp_power_off();


        }
       

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){

        buzzer_sound();
        wake_up_backlight_on();
        if(gctl_t.interval_stop_run_flag  ==0){
          gctl_t.ptc_flag = 1;
          Ptc_On();
          Disp_Dry_Icon();
          
          if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
       }
       else if(pdata[3] == 0x0){
        
          buzzer_sound();
          wake_up_backlight_on();
          
       
          gctl_t.ptc_flag = 0;
          Ptc_Off();
          Disp_Dry_Icon();
          
         if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
          }
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
          
         

           }
        }
        else if(pdata[3] == 0x0){
       
           gctl_t.plasma_flag  = 0;
           Plasma_Off();
           Disp_Kill_Icon();
         

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



          }

        }
        else if(pdata[3] == 0x0){ //close 

             gctl_t.ultrasonic_flag =0;
            Ultrasonic_Pwm_Stop();
             Disp_Ultrsonic_Icon();

        }


     break;

      case 0x05: // link wifi command

          wake_up_backlight_on();

       if(pdata[3] == 0x01){  // link wifi 

             //WIFI CONNCETOR process
			 gkey_t.wifi_led_fast_blink_flag=1;
			 //WIFI CONNCETOR process
			wifi_t.link_wifi_net_login_tencent_success =0;
			wifi_t.runCommand_order_lable=wifi_link_tencent_cloud;
			wifi_t.wifi_config_net_lable= wifi_set_restor;
			wifi_t.power_on_login_tencent_cloud_flag=0;
			wifi_t.link_tencent_step_counter=0;
			wifi_t.gTimer_linking_tencent_duration=0; //120s ->
         
          //  Buzzer_KeySound();
            

        }
        else if(pdata[3] == 0x0){ //don't link wifi 

        }


     break;

     case 0x06: //buzzer sound done
         wake_up_backlight_on();
        if(pdata[3] == 0x01){  //buzzer sound 
            buzzer_sound();
           // gpro_t.buzzer_sound_flag = 1;

        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .



        }


     break;


      case 0x1A: //温度数据
          wake_up_backlight_on();
        if(pdata[3] == 0x0F){ //数据
          gdisp_t.disp_set_temp_value_flag =1;
          gpro_t.set_temperature_value_success=1;
          gkey_t.set_temp_value_be_pressed = 1;     //send data to tencent flag.
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

     case 0x22: //PTC打开关闭指令,没有蜂鸣器声音。

      if(pdata[3] == 0x01){

          wake_up_backlight_on();
        
        if(gctl_t.interval_stop_run_flag  ==0){
            
          gctl_t.ptc_flag = 1;
          Ptc_On();
          Disp_Dry_Icon();
         if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
          
         }
       }
       else if(pdata[3] == 0x0){
        
          gctl_t.ptc_flag = 0;
          Ptc_Off();
          Disp_Dry_Icon();
          if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
           }

       }

     break;

     case 0x27: //AI mode 

      if(pdata[3] == 0x02){
       
        // gctl_t.gModel=2;
         MqttData_Publish_SetState(2);
	     osDelay(100);//HAL_Delay(350);
        
        
          
       }
       else if(pdata[3] == 0x01){ //AI mode 
       
        // gctl_t.gModel=1;
         MqttData_Publish_SetState(1);
	     osDelay(100);//HAL_Delay(350);
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


