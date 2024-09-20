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

        if(pdata[3] == 0x01){ //open
          buzzer_sound()
           

           gpro_t.gpower_on = power_on;

        }
        else if(pdata[3] == 0x0){ //close 
           buzzer_sound();
           gpro_t.gpower_on = power_off;


        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){
          buzzer_sound();
        

       if(gctl_t.interval_time_two_hours_stop_flag ==0){
          gctl_t.gDry = 1;
          Dry_Function(0);
          if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
       }
       }
       else if(pdata[3] == 0x0){
          buzzer_sound();
          
       
         gctl_t.gDry =0;
         Dry_Function(1);
         if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
          }

       }

     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
           
            buzzer_sound();
           
           gctl_t.gPlasma = 1;
          
           PLASMA_SetHigh();
        }
        else if(pdata[3] == 0x0){
           buzzer_sound();
           
          gctl_t.gPlasma = 0;
        
          PLASMA_SetLow();

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[3] == 0x01){  //open 
          
           gctl_t.gUlransonic =1;

        }
        else if(pdata[3] == 0x0){ //close 

           gctl_t.gUlransonic = 0;

        }


     break;

      case 0x05: // link wifi command

       if(pdata[3] == 0x01){  // link wifi 
         //  buzzer_sound();
           gpro_t.link_net_step =0;
	      net_t.wifi_link_net_success=0;
          gpro_t.wifi_led_fast_blink_flag =1;
          gctl_t.wifi_config_net_lable=wifi_set_restor;
		  wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;//2 
		  
          gctl_t.gTimer_linking_tencen_total_counter=0; //total times is 120s

        }
        else if(pdata[3] == 0x0){ //don't link wifi 

        }


     break;

     case 0x06: //buzzer sound done

        if(pdata[3] == 0x01){  //buzzer sound 
             //buzzer_sound();
            gpro_t.buzzer_sound_flag = 1;

        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .



        }


     break;


      case 0x1A: //温度数据

        if(pdata[3] == 0x0F){ //数据

            gctl_t.set_temperature_value = pdata[5] ;

          MqttData_Publis_SetTemp(gctl_t.set_temperature_value);
		  osDelay(20);//HAL_Delay(350);

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
        
        if(gctl_t.interval_time_two_hours_stop_flag ==0){
          gctl_t.gDry = 1;
          Dry_Function(0);
         if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
          
         }
       }
       else if(pdata[3] == 0x0){
        
         gctl_t.gDry =0;
         Dry_Function(1);
          if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
           }

       }

     break;

     case 0x27:

      if(pdata[3] == 0x02){
       
         gctl_t.gModel=2;
         MqttData_Publish_SetState(2);
	     osDelay(100);//HAL_Delay(350);
        
        
          
       }
       else if(pdata[3] == 0x01){ //AI mode 
       
         gctl_t.gModel=1;
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


