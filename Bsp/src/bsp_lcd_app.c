#include "bsp.h"



//static void Display_Timer_Timing(int8_t hours,int8_t minutes);

//static void Display_LCD_Works_Timing(void);
static void disp_speical_works_timing_value(void);

/**********************************************************************************************************
*	函 数 名: void mainboard_process_handler(void)
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/

void PowerOn_Init(void)
{
  
    LED_Mode_On();
    LED_POWER_ON();
    Backlight_On();
    Mainboard_Action_Fun();

}


void PowerOff_freeFun(void)
{
       
		Ultrasonic_Pwm_Stop();
		Plasma_Off();

        Fan_Stop();
		Backlight_Off();
		Lcd_Display_Off();
}

void PowerOff_Off_Led(void)
{
    LED_Mode_Off();
    LED_POWER_OFF();
    Ptc_Off();
    Ultrasonic_Pwm_Stop();
    Plasma_Off();



}



/*
*********************************************************************************************************
*	函 数 名: static void Mainboard_Action_Fun(void)
*	功能说明: 主板工作：功能动作输出
*			 
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
 void Mainboard_Action_Fun(void)
{
    Ptc_On();
	Ultrasonic_Pwm_Output();
	Fan_Run();
	Plasma_On();
}



/*
*********************************************************************************************************
*
*	函 数 名: void Display_Works_Timing(void)
*	功能说明: 显示设备工作的时间，时间最大值是 99个小时
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
void Display_Works_Timing(void)
{
    static uint8_t minutes_change_flag = 0xff;
    if(gpro_t.gTimer_works_counter_sencods > 59){

	  gpro_t.gTimer_works_counter_sencods=0;

	  gpro_t.disp_works_minutes_value++;


	if( gpro_t.disp_works_minutes_value > 59){ //1 hours
		gpro_t.disp_works_minutes_value=0;

	     gpro_t.disp_works_hours_value++;


		glcd_t.number7_low = gpro_t.disp_works_minutes_value / 10;
		glcd_t.number7_high = gpro_t.disp_works_minutes_value / 10;


		glcd_t.number8_low = gpro_t.disp_works_minutes_value  % 10;
		glcd_t.number8_high = gpro_t.disp_works_minutes_value % 10;

		//display hours works
        if(gpro_t.disp_works_hours_value > 99){
               gpro_t.disp_works_hours_value=0;
        
        
          }


        glcd_t.number5_low = gpro_t.disp_works_hours_value / 10;
		glcd_t.number5_high = gpro_t.disp_works_hours_value / 10;


		glcd_t.number6_low = gpro_t.disp_works_hours_value  % 10;
		glcd_t.number6_high = gpro_t.disp_works_hours_value % 10;

       if(gpro_t.get_beijing_time_success==0 && gpro_t.tencent_link_success==0){

          SendWifiData_To_PanelTime(gpro_t.disp_works_hours_value,gpro_t.disp_works_minutes_value, gpro_t.gTimer_works_counter_sencods);
		
       }
	}
    

   
  

   }

    if(( minutes_change_flag != gpro_t.disp_works_minutes_value) || gpro_t.disp_timer_switch_time_flag >0){
 
         if(minutes_change_flag != gpro_t.disp_works_minutes_value)minutes_change_flag = gpro_t.disp_works_minutes_value;

         if(gpro_t.disp_timer_switch_time_flag >0) gpro_t.disp_timer_switch_time_flag=0;
         
        glcd_t.number5_low = gpro_t.disp_works_hours_value / 10;
		glcd_t.number5_high = gpro_t.disp_works_hours_value / 10;


		glcd_t.number6_low = gpro_t.disp_works_hours_value  % 10;
		glcd_t.number6_high = gpro_t.disp_works_hours_value % 10;


        glcd_t.number7_low = gpro_t.disp_works_minutes_value / 10;
		glcd_t.number7_high = gpro_t.disp_works_minutes_value / 10;


		glcd_t.number8_low = gpro_t.disp_works_minutes_value  % 10;
		glcd_t.number8_high = gpro_t.disp_works_minutes_value % 10;
       // Display_LCD_Works_Timing();
 

       display_works_times_handler();




   }

   //Display_LCD_Works_Timing();
   

}


static void disp_speical_works_timing_value(void)
{

    glcd_t.number5_low = gpro_t.disp_works_hours_value / 10;
    glcd_t.number5_high = glcd_t.number5_low;//gpro_t.disp_works_hours_value / 10;
    
    
   glcd_t.number6_low = gpro_t.disp_works_hours_value  % 10;
   glcd_t.number6_high = glcd_t.number6_low;//gpro_t.disp_works_hours_value % 10;


   glcd_t.number7_low = gpro_t.disp_works_minutes_value / 10;
   glcd_t.number7_high = glcd_t.number7_low;//gpro_t.disp_works_minutes_value / 10;


   glcd_t.number8_low = gpro_t.disp_works_minutes_value  % 10;
   glcd_t.number8_high = glcd_t.number8_low ;//gpro_t.disp_works_minutes_value % 10;
   //Display_LCD_Works_Timing();

   display_works_times_handler();



}


//static void Display_LCD_Works_Timing(void)
//{


//    LCD_Number_FiveSix_Hours();
//    LCD_Number_SevenEight_Minutes();


//}



void LCD_Disp_Works_Timing_Init(void)
{

       gctl_t.ai_flag = 1;

       LCD_Number_Ai_OneTwo_Humidity();
       
       glcd_t.number5_low = gpro_t.disp_works_hours_value / 10;
		glcd_t.number5_high = gpro_t.disp_works_hours_value / 10;


		glcd_t.number6_low = gpro_t.disp_works_hours_value  % 10;
		glcd_t.number6_high = gpro_t.disp_works_hours_value % 10;
		

	
	 
		  glcd_t.number7_low = gpro_t.disp_works_minutes_value / 10;
		  glcd_t.number7_high = gpro_t.disp_works_minutes_value / 10;
		
						   
		  glcd_t.number8_low = gpro_t.disp_works_minutes_value	% 10;
		  glcd_t.number8_high = gpro_t.disp_works_minutes_value % 10;

	    
           display_works_times_handler();

	 
    //Display_LCD_Works_Timing();


}

/*
*********************************************************************************************************
*
*	函 数 名: void Dissplay_Timer_Timing(uint8_t hours,uint8_t minutes)
*	功能说明: 到记时功能
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
void Display_Timer_Timing(void)
{

    static uint8_t send_timer_times_flag;
     if(gpro_t.gTimer_timer_Counter > 59){
	    gpro_t.gTimer_timer_Counter =0;
		
		gpro_t.set_timer_timing_minutes -- ;

	
	    if(gpro_t.set_timer_timing_minutes <  0 ){
			 
		   gpro_t.set_timer_timing_hours -- ;
		   gpro_t.set_timer_timing_minutes =59;
         }

		
		
		 if(gpro_t.set_timer_timing_hours < 0 ){
		 
			
    			gpro_t.set_timer_timing_hours=0;
    			gpro_t.set_timer_timing_minutes=0;
    			gkey_t.key_power = power_off;
                gpro_t.power_off_flag = 1;
    		
             
           
			
			
	      }

         //display hours timing
	     glcd_t.number5_low = gpro_t.set_timer_timing_hours / 10;
		 glcd_t.number5_high = gpro_t.set_timer_timing_hours / 10;
	 
	 
		 glcd_t.number6_low = gpro_t.set_timer_timing_hours   % 10;
		 glcd_t.number6_high = gpro_t.set_timer_timing_hours % 10;
		 
	      //display minutes 
		 glcd_t.number7_low = gpro_t.set_timer_timing_minutes / 10;
		 glcd_t.number7_high = gpro_t.set_timer_timing_minutes / 10;
		 
							
		 glcd_t.number8_low = gpro_t.set_timer_timing_minutes   % 10;
		 glcd_t.number8_high = gpro_t.set_timer_timing_minutes % 10;

        // LCD_Disp_Timer_Timing();
         display_works_times_handler();
         send_timer_times_flag= 1;
		    
     }

     if(send_timer_times_flag==1){
         send_timer_times_flag++;
         
        SendWifiData_To_SynTimerTime(gpro_t.set_timer_timing_hours,gpro_t.set_timer_timing_minutes,gpro_t.gTimer_timer_Counter);
        osDelay(5);
     }
     
 

}



void LCD_Disp_Timer_Timing(void)
{

   LCD_Number_FiveSix_Hours();
   LCD_Number_SevenEight_Minutes();

}

void LCD_Disp_Timer_Timing_Init(void)
{

     gctl_t.ai_flag = 0;

     LCD_Number_Ai_OneTwo_Humidity();
//display hours timing
    glcd_t.number5_low = gpro_t.set_timer_timing_hours / 10;
    glcd_t.number5_high = glcd_t.number5_low; //gpro_t.set_timer_timing_hours / 10;


    glcd_t.number6_low = gpro_t.set_timer_timing_hours   % 10;
    glcd_t.number6_high =  glcd_t.number6_low;//gpro_t.set_timer_timing_hours % 10;

    //display minutes 
    glcd_t.number7_low = gpro_t.set_timer_timing_minutes / 10;
    glcd_t.number7_high = glcd_t.number7_low;//gpro_t.set_timer_timing_minutes / 10;

    				
    glcd_t.number8_low = gpro_t.set_timer_timing_minutes   % 10;
    glcd_t.number8_high =  glcd_t.number8_low ;//gpro_t.set_timer_timing_minutes % 10;

    //LCD_Disp_Timer_Timing();
    display_works_times_handler();

}
/*********************************************************************************
*
*	函 数 名:static void mode_long_short_key_fun(void)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   

*********************************************************************************/
void Display_WorksTimingr_Handler(uint8_t sel_item)
{

  // static uint8_t switch_counter;
 
    switch(sel_item){

    case disp_works_timing :
        if(gkey_t.set_timer_timing_success ==0){
            gctl_t.ai_flag = 1; //  DISPLAY AI ICON
            
		   Display_Works_Timing();


        }
		else if(gpro_t.gTimer_disp_short_time <6 && gkey_t.set_timer_timing_success ==1){

		     gctl_t.ai_flag = 1; //  DISPLAY AI ICON
		     disp_ai_symbol();
             Display_Works_Timing();

		}
		else if(gpro_t.gTimer_disp_short_time > 5 && gkey_t.set_timer_timing_success ==1){
                gpro_t.gTimer_disp_short_time=0;
                gkey_t.key_mode = disp_timer_timing;
                gctl_t.ai_flag =0;
				donot_disp_ai_symbol();
                Display_Timer_Timing();
        }
        
            
    break;
    
    case disp_timer_timing:
   
        if(gkey_t.set_timer_timing_success ==1){
            gctl_t.ai_flag = 0; // don't  DISPLAY AI ICON
            Display_Timer_Timing();


        }
        else if(gkey_t.set_timer_timing_success == 0 && gpro_t.gTimer_disp_short_time < 6){ //&& gkey_t.gTimer_disp_switch_disp_mode > 3){

             gctl_t.ai_flag = 0; // don't  DISPLAY AI ICON
             donot_disp_ai_symbol();
             display_timer_times_handler();
          
        }
        else if(gpro_t.gTimer_disp_short_time > 5 && gkey_t.set_timer_timing_success == 0){
                gpro_t.gTimer_disp_short_time=0;
                gkey_t.key_mode = disp_works_timing;
                gctl_t.ai_flag =1;
				disp_ai_symbol();
                display_works_times_handler();


        }

    break;


    case mode_set_timer:

	    #if 0
    
        Set_Timer_Timing_Lcd_Blink();//(gpro_t.set_timer_timing_hours,gpro_t.set_timer_timing_minutes);
       
        if(gkey_t.gTimer_disp_set_timer > 1){

            if(gpro_t.set_timer_timing_hours == 0 && gpro_t.set_timer_timing_minutes==0){

                gkey_t.set_timer_timing_success = 0;

                gctl_t.ai_flag = 1;
                gkey_t.key_mode =disp_works_timing;
            
                gkey_t.key_add_dec_mode = set_temp_value_item;
                LCD_Disp_Works_Timing_Init();

                if(g_tDisp.second_disp_set_temp_flag == 0){
                    
                    SendData_Tx_Data(0x4C,0x0);
				    osDelay(5);

                 }
                 
            
            }
            else{
                gkey_t.set_timer_timing_success = 1;
                gpro_t.gTimer_timer_Counter =0; //start recoder timer timing is "0",from "0" start

                gctl_t.ai_flag = 0;
              
                gkey_t.key_mode = disp_timer_timing;
                gkey_t.key_add_dec_mode = set_temp_value_item;
                
                LCD_Disp_Timer_Timing_Init();

                if(gpro_t.tencent_link_success==1){
                    MqttData_Publish_SetState(2); //timer model  = 2, works model = 1
                    osDelay(50);//HAL_Delay(200);
                }
                if(g_tDisp.second_disp_set_temp_flag == 0){
                    
                    SendData_Tx_Data(0x4C, gpro_t.set_timer_timing_hours);
                    osDelay(5);
                 }
             
            }
        }
           #endif 
       break;

       case disp_fan_error_number:
          fan_error_code_number();
          if(gctl_t.ptc_warning == 1){
             gkey_t.key_mode = disp_ptc_error_number;

          }

       break;

       case disp_ptc_error_number:

          ptc_error_code_number();
          if(gctl_t.fan_warning == 1){
             gkey_t.key_mode = disp_fan_error_number ;

          }
       break;

     
    }
}



/*
*********************************************************************************************************
*
*	函 数 名: void Set_Timer_Timing_Lcd_Blink(void )
*	功能说明: 设置的定时时间闪烁
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
void Set_Timer_Timing_Lcd_Blink(void)
{

   // static uint8_t switch_timer_flag;
   //  if(gpro_t.gTimer_set_timer_value > 9){ //90x10ms= 90ms
	//   gpro_t.gTimer_set_timer_value=0;

	// switch_timer_flag = switch_timer_flag ^ 0x01;

	// if(switch_timer_flag ==1){
      glcd_t.number5_low =  0x0A ;
      glcd_t.number5_high =  0x0A ;

      glcd_t.number6_low  =  0x0A; //
      glcd_t.number6_high =  0x0A; //
      
       //dispaly minutes 
      glcd_t.number7_low =   0x0A;
      glcd_t.number7_high =   0x0A;

      glcd_t.number8_low =   0x0A;
      glcd_t.number8_high =   0x0A;
      //LCD_Disp_Timer_Timing();
      display_works_times_handler();
      osDelay(100);//HAL_Delay(100);

	
      glcd_t.number5_low =  gpro_t.set_timer_timing_hours  / 10 ;
      glcd_t.number5_high =  gpro_t.set_timer_timing_hours  / 10 ;

      glcd_t.number6_low  = gpro_t.set_timer_timing_hours% 10; //
      glcd_t.number6_high = gpro_t.set_timer_timing_hours % 10; //
      
       //dispaly minutes 
      glcd_t.number7_low =  0;
      glcd_t.number7_high =  0;

      glcd_t.number8_low = 0;
      glcd_t.number8_high =   0;
      LCD_Disp_Timer_Timing();
      osDelay(100);//HAL_Delay(100);

 }
    



/*************************************************************************************
	*
	*Function Name: void Lcd_Display_Off(void)
	*Function : don't display any one
	*Input Ref:NO
	*Return Ref:NO
	*
*************************************************************************************/
void Disp_HumidityTemp_Value(void)
{

    glcd_t.number1_high = gctl_t.dht11_humidity_value / 10;//gctl_t.dht11_temp_value / 10;
    glcd_t.number1_low =  glcd_t.number1_high;///gctl_t.dht11_temp_value / 10;
    
    glcd_t.number2_high = gctl_t.dht11_humidity_value % 10;//gctl_t.dht11_temp_value % 10;
    glcd_t.number2_low = glcd_t.number2_high;//gctl_t.dht11_temp_value % 10;

    glcd_t.number3_high = gctl_t.dht11_temp_value / 10;//gctl_t.dht11_humidity_value / 10;
    glcd_t.number3_low = glcd_t.number3_high;///gctl_t.dht11_humidity_value / 10;

    glcd_t.number4_high = gctl_t.dht11_temp_value % 10;//gctl_t.dht11_humidity_value % 10;
    glcd_t.number4_low =  glcd_t.number4_high ;//gctl_t.dht11_humidity_value % 10;
    
    LCD_Number_ThreeFour_Temperature();
    LCD_Number_Ai_OneTwo_Humidity();
}




