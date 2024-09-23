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
		wifi_t.link_wifi_net_login_tencent_success =0;
		wifi_t.power_on_login_tencent_cloud_flag=0;
		
		wifi_t.gTimer_linking_tencent_duration=0; //120s
         
        Buzzer_KeySound();

}

/**********************************************************************
    *
    *Function Name:
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/

