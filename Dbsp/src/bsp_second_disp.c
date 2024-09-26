#include "bsp.h"

second_disp_ref  g_tDisp;
/**********************************************************************
    *
    *Function Name:
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void second_disp_power_on_fun(void)
{
    gkey_t.key_power=power_on;
    gkey_t.key_mode = disp_timer_timing;
    gctl_t.ai_flag = 1;
    gctl_t.ptc_warning =0;
    gctl_t.fan_warning =0;

    gctl_t.step_process=0;
    gpro_t.power_off_flag =1;

}
/**********************************************************************
    *
    *Function Name:
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void second_disp_power_off_fun(void)
{
     gkey_t.key_power=power_off;
     gctl_t.step_process=0;
}

/**********************************************************************
    *
    *Function Name:
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void second_disp_set_link_wifi_fun(void)
{

     	//WIFI CONNCETOR process
	    gkey_t.wifi_led_fast_blink_flag=1;
			 //WIFI CONNCETOR process
		gpro_t.tencent_link_success =0;
	
		
		wifi_t.gTimer_linking_tencent_duration=0; //120s
         
       

}

/**********************************************************************
    *
    *Function Name:
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void second_disp_ai_time_fun(void)
{
     
    gkey_t.key_mode_switch_flag = 1;
    gkey_t.key_mode  = disp_works_timing;
    gkey_t.key_add_dec_mode = set_temp_value_item;
    gctl_t.ai_flag = 1; // AI DISPLAY AI ICON

    LCD_Disp_Works_Timing_Init();
    disp_ai_iocn();
 

    gkey_t.key_mode_be_pressed = 1;
             
        
}

void second_disp_not_ai_timer_fun(void)

{

    
    gkey_t.key_mode  = disp_timer_timing;

    gctl_t.ai_flag = 0; // DON'T DISP AI ICON
    //counter exit timing this "mode_set_timer"
    gkey_t.key_mode_switch_flag = 1;
    gkey_t.key_add_dec_mode = set_temp_value_item;

    LCD_Disp_Timer_Timing_Init();
    disp_ai_iocn();
  


    gkey_t.key_mode_be_pressed = 2;
    
              
}

