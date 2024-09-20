#include "bsp.h"

uint8_t sub_send_power_on_times;
uint8_t get_rx_beijing_time_enable;


uint8_t beijing_step;
uint8_t auto_det_flag;



static void RunWifi_Command_Handler(void);
static void auto_repeat_link_netware_fun(void);
static void auto_repeat_init_link_net(void);
static void auto_repeat_tencnet_net(void);

///********************************************************************************
//	*
//	*Functin Name:void WIFI_Process_Handler(void)
//	*Functin :
//	*Input Ref: NO
//	*Return Ref:NO
//	*
//********************************************************************************/
void  wifi_get_beijing_time_handler(void)
{
    static uint8_t alternate_flag,flag_switch;
    uint8_t real_hours ,real_minutes,real_seconds;

    if(wifi_t.get_rx_beijing_time_enable==0){
     Tencent_Cloud_Rx_Handler();
	 Json_Parse_Command_Fun();
    }


     switch(gpro_t.get_beijing_step){

     case 0: //WT.EDIT .2024.08.10

      if(wifi_link_net_state()==1 && gpro_t.gTimer_get_data_from_tencent_data > 9){
       
          gpro_t.gTimer_get_data_from_tencent_data =0;
           flag_switch++;

       

         if(flag_switch == 1){
            wifi_t.get_rx_beijing_time_enable=0;
            Subscriber_Data_FromCloud_Handler();
            osDelay(100);//HAL_Delay(200)
            gpro_t.get_beijing_step = 1;

            
         }
        else if(flag_switch >  1 && gpro_t.gpower_on == power_off){
            flag_switch=0;
            wifi_t.get_rx_beijing_time_enable=0;
            Update_Dht11_Totencent_Value();
            osDelay(50);//HAL_Delay(200) //WT.EDIT 2024.08.10
            
             gpro_t.get_beijing_step = 1;

         }
         else{ //WT.EDIT 2024.08.10 ADD ITEM
             if(flag_switch > 1){
                flag_switch=0;
                wifi_t.get_rx_beijing_time_enable=0; 
               gpro_t.get_beijing_step = 1;
             }

         }
       
    }
    else
     gpro_t.get_beijing_step = 1;

   break;


   case 1:

        if(wifi_t.gTimer_get_beijing_time > 100){

         wifi_t.gTimer_get_beijing_time=0;
        
          if(wifi_link_net_state()==1){

     
    		    gpro_t.get_beijing_step = 2;
           
                alternate_flag++;
                wifi_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
                SendWifiData_To_Data(0x1F,0x01);
                //gpro_t.gTimer_pro_update_dht11_data =0; //disable publish to data to tencent .

            }
            else{
              
               SendWifiData_To_Data(0x1F,0x0); //0x1F: 0x1=wifi link net is succes ,0x0 = wifi link net is fail
               gpro_t.get_beijing_step = 10;
               wifi_t.linking_tencent_cloud_doing  =1; //receive from tencent command state .
             }
	
       }
       else{

            gpro_t.get_beijing_step = 0;
       }

      break;


      case 2:


	    if(gpro_t.get_beijing_time_success == 0){
		

         gpro_t.get_beijing_step = 3;
         beijing_step =0; //WT.EDIT 2024.08.10
	      
		wifi_t.gTimer_auto_detected_net_state_times =0;
	
		wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
	
       }
	   else{

          
         if(alternate_flag == 1){
		  
	        wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		    gpro_t.get_beijing_step = 3;
			beijing_step =0; //WT.EDIT 2024.08.10

        }
		else{
              alternate_flag=0;
             
			  gpro_t.get_beijing_step = 6;

		}

	   }

     
    break;


     case 3:

         switch(beijing_step){

         case 0:
            //disable publish data to tencent cloud.
            gpro_t.gTimer_get_data_from_tencent_data=0;

    		
    		Get_BeiJing_Time_Cmd();
    	    osDelay(100);//HAL_Delay(20); //WT.EDIT .2024.08.10//HAL_Delay(20);
    		//wifi_t.gTimer_read_beijing_time=0;
    
            
            beijing_step =1;

           

         break;

         case 1:
             //if(wifi_t.gTimer_read_beijing_time > 0 ){//
    		
                wifi_t.gTimer_read_beijing_time=0;
                 //disable publish data to tencent cloud.
                gpro_t.gTimer_get_data_from_tencent_data=0;
                wifi_t.get_rx_beijing_time_enable=1;
        		gpro_t.wifi_rx_data_counter =0;
        		Get_Beijing_Time();
              
        	     osDelay(100);//HAL_Delay(20); //WT.EDIT .2024.08.10
                
        		wifi_t.gTimer_read_beijing_time=0;
                beijing_step =2;
          
          //  }

          break; 


          case 2:

            if(wifi_t.wifi_data[50] > 0x31 ){
           
                gpro_t.disp_works_hours_value  = (wifi_t.wifi_data[41]-0x30)*10 + wifi_t.wifi_data[42]-0x30;
                gpro_t.disp_works_minutes_value  =(wifi_t.wifi_data[44]-0x30)*10 + wifi_t.wifi_data[45]-0x30;
                gpro_t.gTimer_works_counter_sencods = (wifi_t.wifi_data[47]-0x30)*10 + wifi_t.wifi_data[48]-0x30;

                wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
              

                gpro_t.get_beijing_time_success =1;

                    SendWifiData_To_PanelTime(gpro_t.disp_works_hours,gpro_t.disp_works_minutes,gpro_t.disp_works_time_seconds);
                    osDelay(50);

                   
                
                }
                else if(wifi_t.wifi_data[50] == 0x31){  //"0x31" ASCII = '1'

                   wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
                   if(wifi_link_net_state()==1){
                       gpro_t.get_beijing_step = 0;
                       gpro_t.gTimer_get_data_from_tencent_data=0;
                       wifi_t.gTimer_get_beijing_time = 50;
                      

                    }
                    else 
                       gpro_t.get_beijing_step = 0;
            }
            else{

              gpro_t.get_beijing_step = 0;
              

            }
            break;
         }
    break;


    case 6:

  
       Wifi_Link_Tencent_Net_State();
    

       wifi_t.gTimer_auto_detected_net_state_times=0;  

       gpro_t.get_beijing_step = 0;
    
    break;

    //auto link net 

    case 10:

     if(wifi_t.gTimer_auto_detected_net_state_times > 100){

		      wifi_t.gTimer_auto_detected_net_state_times=0;

    

         if(wifi_link_net_state()==0){
            gpro_t.get_beijing_step = 11;
            wifi_t.linking_tencent_cloud_doing  =1; //receive from tencent command state .
            wifi_t.wifi_uart_rx_counter=0;//gpro_t.wifi_rx_data_counter=0;
           
      
           
          }
          else{
             wifi_t.soft_ap_config_flag =1; //WE.EIDT 
             wifi_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
             gpro_t.get_beijing_step = 0;

          }
        
       }

     
     if(wifi_link_net_state()==1){
          gpro_t.get_beijing_step = 0;

       }
       
     break;


     case 11:
         if(wifi_link_net_state()==0 && gpro_t.wifi_led_fast_blink_flag==0){

           wifi_t.linking_tencent_cloud_doing =1;
        

            WIFI_IC_ENABLE();
       
    		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
            HAL_Delay(1000);
            wifi_t.gTimer_auto_link_net_time =0;
            auto_link_net_flag=1;

            gpro_t.get_beijing_step = 12;

         }
         else {

             gpro_t.get_beijing_step = 10;


         }
        
               

     break;


     case 12:

        if(wifi_t.gTimer_auto_link_net_time > 2 && auto_link_net_flag==1){


            wifi_t.gTimer_auto_link_net_time=0;
            gpro_t.wifi_rx_data_counter=0;
	        wifi_t.soft_ap_config_flag =1;
             auto_link_net_flag =2;
           // DISABLE_INT();
	        HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接
            HAL_Delay(1000);
           // HAL_Delay(1000);
           // HAL_Delay(1000);
           
            //HAL_Delay(1000);
           
		
	    }

        if(wifi_t.gTimer_auto_link_net_time > 4 && auto_link_net_flag==2){

            auto_link_net_flag=0 ;
            gpro_t.get_beijing_step = 13;
           
        }


     break;


     case 13:
       if(wifi_link_net_state()==1){
       
         

        net_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
	    gpro_t.wifi_rx_data_counter=0; //clear USART2 counter is zero
		wifi_t.soft_ap_config_flag =0; 
	

          if(gpro_t.gpower_on == power_on){
                MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
                HAL_Delay(200);

            }
            else if(gpro_t.gpower_on == power_off){

               MqttData_Publish_PowerOff_Ref();
               HAL_Delay(200);


            }
            Subscriber_Data_FromCloud_Handler();
            HAL_Delay(200);

            SendWifiData_To_Data(0x1F,0x01); //0x1F: wifi link net is succes 

             gpro_t.get_beijing_step = 0;
		
         }
         else{

             gpro_t.get_beijing_step = 10;
         }

       break;
	

	
        }

     
}
/**********************************************************************
	*
	*Functin Name: void MainBoard_Itself_PowerOn_Fun(void)
	*Function :
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
 void wifi_auto_detected_link_state(void)
{
   static uint8_t dc_power_on;
	if(power_on_login_tencent_cloud_flag     <  5 && wifi_link_net_state()==0){
		
      net_t.linking_tencent_cloud_doing = 1;
      gpro_t.gTimer_dc_power_on_auto_link_net = 0;

      Auto_InitWifiModule_Hardware();//InitWifiModule();
      Auto_SmartPhone_TryToLink_TencentCloud();
	 

	
       
    }
    if(wifi_link_net_state()==1    && gpro_t.gTimer_dc_power_on_auto_link_net > 1 && dc_power_on ==0 ){
              
             dc_power_on ++ ;
           //wifi_t.linking_tencent_cloud_doing = 0;
           net_t.linking_tencent_cloud_doing  =0;
           gpro_t.process_run_step=0;
        
          if(gpro_t.gpower_on == power_off){
		     MqttData_Publish_PowerOff_Ref();
               HAL_Delay(200);

          }
          
          Subscriber_Data_FromCloud_Handler();
          HAL_Delay(200);

          SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
   }
   
	
}
/********************************************************************************
   *
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : separately update value 
   *Input Ref:NO
   *Return Ref:NO
   *
*********************************************************************************/
#if 0
static void RunWifi_Command_Handler(void)
{
  
  
   static uint8_t link_net_flag; 

    
 	switch(wifi_t.runCommand_order_lable){

   case wifi_link_tencent_cloud: //01

	if(power_on_state() == power_on){
     
		//Wifi_Fast_Led_Blink(); 
		Wifi_SoftAP_Config_Handler();
		//Wifi_Fast_Led_Blink();
		SmartPhone_LinkTencent_Cloud();
		//Wifi_Fast_Led_Blink();
		//wifi_t.auto_linknet_flag =0;

	}
	
	if(wifi_link_net_state()==0){
		if(wifi_t.gTimer_linking_tencent_duration < 120 ){
			//Wifi_Fast_Led_Blink();
		   wifi_t.runCommand_order_lable = wifi_link_tencent_cloud;
		}
		else if(wifi_link_net_state()==0 && wifi_t.gTimer_linking_tencent_duration >120){
		  //auto link wifi net 
		  	wifi_t.gTimer_linking_tencent_duration=0;
		    gkey_t.wifi_led_fast_blink_flag=0;
			wifi_t.link_wifi_net_login_tencent_success =0;
			wifi_t.rx_error_codes_flag= 1;
			wifi_t.rx_setup_hardware_counter=0;
			wifi_t.gTimer_auto_detected_net_state_times=0;
			wifi_t.gTimer_wifi_rx_error = 0;
			wifi_t.get_rx_beijing_time_enable=0;

			 wifi_t.runCommand_order_lable = wifi_auto_to_link_cloud;
		   
		 }

		
		

		}


	if(wifi_link_net_state()==1   && power_on_state() == power_on){
        gkey_t.wifi_led_fast_blink_flag=0;
        wifi_t.get_rx_beijing_time_enable=0;
        wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;

        wifi_t.gTimer_auto_detected_net_state_times =0;
        wifi_t.gTimer_linking_tencent_duration=0;
        // MqttData_Publish_SetOpen(0x01);
        wifi_t.gTimer_get_beijing_time =0;
        wifi_t.gTimer_publish_dht11=0; 
        wifi_t.link_tencent_thefirst_times=0;
        MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
        HAL_Delay(200);
        Subscriber_Data_FromCloud_Handler();
        HAL_Delay(200);

        wifi_t.link_net_tencent_data_flag = 1;
    }
    
	break;


	case wifi_tencent_publish_init_data://02

       if(wifi_t.gTimer_publish_dht11 >10){
	 
	       MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
		   wifi_t.gTimer_publish_dht11=0;
		   wifi_t.gTimer_get_beijing_time =0;
	       wifi_t.runCommand_order_lable = wifi_subscriber_form_tencent_data;

       }


    break;

			
     case wifi_subscriber_form_tencent_data: //03

	   if( wifi_t.gTimer_publish_dht11 >12){

	      Subscriber_Data_FromCloud_Handler();
		 wifi_t.gTimer_publish_dht11=0;
	     wifi_t.gTimer_get_beijing_time =0;
	     wifi_t.get_rx_auto_repeat_net_enable=0;
		 wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	   }


	break;



	case wifi_publish_update_tencent_cloud_data://06

	if(wifi_t.gTimer_get_beijing_time > 180 && wifi_t.get_rx_auto_repeat_net_enable==0){
        
            if(wifi_link_net_state()==1){

                wifi_t.gTimer_get_beijing_time=0;
    		    wifi_t.link_beijing_times_flag =1;

            }
            else{
               wifi_t.runCommand_order_lable=  wifi_auto_to_link_cloud;

               wifi_t.gTimer_auto_detected_net_state_times =290;

            }
	
	 }

	if(wifi_t.link_beijing_times_flag ==1){
	   wifi_t.link_beijing_times_flag =0;
	   wifi_t.three_times_link_beijing++ ;
	  

	   if((wifi_t.three_times_link_beijing >0 && wifi_t.three_times_link_beijing <4) || gctl_t.beijing_time_flag == 0 ){
		

	   
	   	wifi_t.set_beijing_time_flag =1;
		wifi_t.gTimer_auto_detected_net_state_times =0;
	
		wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		wifi_t.runCommand_order_lable= wifi_get_beijing_time;
       }
	   else{
        if(wifi_t.three_times_link_beijing % 2 == 0 && gctl_t.beijing_time_flag == 1){
			
			wifi_t.set_beijing_time_flag =1;
			wifi_t.gTimer_auto_detected_net_state_times =0;
		
			wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		
			wifi_t.runCommand_order_lable= wifi_get_beijing_time;

        }
		else{
			wifi_t.runCommand_order_lable= wifi_auto_repeat_link_cloud;
             


		}

	}
	}
	else{

      if(wifi_link_net_state()==1){

         wifi_t.runCommand_order_lable= wifi_auto_repeat_link_cloud;//09
       }
       else{

        wifi_t.runCommand_order_lable=  wifi_auto_to_link_cloud;
       }

	}

	break;

	case wifi_auto_to_link_cloud: //0x07
	
	if(wifi_t.gTimer_auto_detected_net_state_times > 180){

		wifi_t.gTimer_auto_detected_net_state_times=0;

         if(wifi_link_net_state()==0){
           auto_repeat_link_netware_fun();


        }
        else{

    
		wifi_t.wifi_uart_rx_counter=0; //clear USART2 counter is zero
		if(wifi_t.auto_link_login_tencent_cloud_flag==1){ //link net is OK
			wifi_t.get_rx_auto_repeat_net_enable=1;
			Wifi_Link_Tencent_Net_State();
			wifi_t.gTimer_publish_dht11=0;

		}
		else{ //link net is NG
			//  Wifi_Reconnect_Tencent_Net();
			wifi_t.get_rx_auto_repeat_net_enable=0;
			wifi_t.power_on_login_tencent_cloud_flag =0;
			wifi_t.gTimer_publish_dht11=1;
			auto_det_flag=1 ;



		}

        }

		wifi_t.runCommand_order_lable= wifi_auto_repeat_link_cloud;

	}
	else{

		wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;

	}


	break;

	case wifi_auto_repeat_link_cloud://09

	if(wifi_t.gTimer_counter_repeat_link_net>180){

       wifi_t.gTimer_counter_repeat_link_net=0;

	switch(wifi_t.auto_link_login_tencent_cloud_flag){


	case 1:  //link is OK
	   // Wifi_Rx_Auto_Link_Net_Handler();
	     Wifi_Link_Tencent_Net_State();
		wifi_t.get_rx_auto_repeat_net_enable=0;
		wifi_t.gTimer_publish_dht11=0;
		wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;//wifi_tencent_publish_init_data;//wifi_tencent_publish_init_data;
		 wifi_t.gTimer_auto_detected_net_state_times=0;  

	break;

	case 0:  //don't link to tencet cloud net.
   
		auto_repeat_link_netware_fun();
		wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;//wifi_tencent_publish_init_data;//wifi_tencent_publish_init_data;
	    wifi_t.gTimer_auto_detected_net_state_times=0;  


	break;

	}

	}
	
    wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
	break;


	case wifi_get_beijing_time://8


	wifi_t.linking_tencent_cloud_doing =0;
    if(wifi_t.set_beijing_time_flag ==1){   //&& wifi_t.gTimer_beijing_time>1){
		wifi_t.set_beijing_time_flag ++;
		//wifi_t.get_rx_beijing_time_enable=1; //enable beijing times
		//wifi_t.wifi_uart_rx_counter=0; //wifi receive beijing of counter must be is zero
		Get_BeiJing_Time_Cmd();
		osDelay(50);////WT.2024.07.14
		wifi_t.gTimer_read_beijing_time=0;
		beijing_step =1;
		wifi_t.gTimer_wifi_counter_link_beijing_times = 0;

	}

	//if(wifi_t.gTimer_read_beijing_time > 2 && wifi_t.gTimer_read_beijing_time < 4){
	if(wifi_t.gTimer_read_beijing_time > 0 && beijing_step ==1){//
	   // wifi_t.get_rx_beijing_time_enable=1; //enable beijing times
	   //wifi_t.wifi_uart_rx_counter=0; //wifi receive beijing of counter must be is zero
		
		Get_Beijing_Time();
		//HAL_Delay(100);
		wifi_t.get_rx_beijing_time_enable=1; //enable beijing times
		wifi_t.wifi_uart_rx_counter=0; //wifi receive beijing of counter must be is zero
		osDelay(100);//WT.2024.07.14
		wifi_t.gTimer_read_beijing_time=0;
        beijing_step =2;


	}

	if(beijing_step ==2){

		wifi_t.real_hours = (wifi_t.wifi_data[41]-0x30)*10 + wifi_t.wifi_data[42]-0x30;
		wifi_t.real_minutes =(wifi_t.wifi_data[44]-0x30)*10 + wifi_t.wifi_data[45]-0x30;
		wifi_t.real_seconds = (wifi_t.wifi_data[47]-0x30)*10 + wifi_t.wifi_data[48]-0x30;

		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times

         wifi_t.get_beijing_timing_success = 1;

         if(wifi_t.real_hours < 25 && wifi_t.real_minutes < 61 ){

		     if(wifi_t.real_hours == 0x08 && (wifi_t.real_minutes   < 0x06)){
             
                                 
             }
            else{

             gpro_t.disp_works_hours_value = wifi_t.real_hours;

		     gpro_t.disp_works_minutes_value = wifi_t.real_minutes;

		     gpro_t.gTimer_works_counter_sencods = wifi_t.real_seconds;
            

			gctl_t.beijing_time_flag = 1; //WT.2024.04.25
			//display must is AI mode EDIT.WT.2024.07.14 for mass after edit.version:1.9 .
			if(gkey_t.key_mode==disp_works_timing && gctl_t.fan_warning ==0 && gctl_t.ptc_warning==0){
                 LCD_Disp_Works_Timing_Init();
             }
		

		   }
	     }

       wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
       wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
        
		beijing_step =3;
		
	}

	
	if(beijing_step== 3){
        beijing_step++;
	    wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
	}


   break;


   case wifi_null:

   if(wifi_link_net_state()==1){

	wifi_t.get_rx_beijing_time_enable=0;
	wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;

	wifi_t.gTimer_auto_detected_net_state_times =0;
	wifi_t.gTimer_linking_tencent_duration=0;
	// MqttData_Publish_SetOpen(0x01);
	   wifi_t.gTimer_get_beijing_time =0;
	  wifi_t.gTimer_publish_dht11=0;  
	  wifi_t.rx_error_codes_flag= 0;
    }


   break;

	   
	  default:

	   break;
	 
  }

  if(wifi_t.gTimer_wifi_counter_link_beijing_times > 20 && wifi_t.get_rx_beijing_time_enable==1){

      wifi_t.gTimer_wifi_counter_link_beijing_times=0;
	  wifi_t.get_rx_beijing_time_enable=0;


  }
  
}
#endif  
/********************************************************************************
	*
	*Functin Name:static void auto_repeat_link_netware_fun(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
static void auto_repeat_link_netware_fun(void)
{

  
  if(auto_det_flag==1   || (wifi_link_net_state()==0)){
        //Buzzer_KeySound();
      
		//InitWifiModule_Hardware();//InitWifiModule();
        auto_repeat_init_link_net();
        //SmartPhone_TryToLink_TencentCloud();
        auto_repeat_tencnet_net();
  }

    if(wifi_link_net_state()==1){

		  wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;//04
		  auto_det_flag=0;
          wifi_t.link_wifi_net_login_tencent_success =1;

            MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
            HAL_Delay(200);
            Subscriber_Data_FromCloud_Handler();
            HAL_Delay(200);
            
		  wifi_t.gTimer_auto_detected_net_state_times=0;  
	 }

	
	
   
}


void Wifi_Rx_Auto_Link_Net_Handler(void)
{

   
	   strncpy((char *)wifi_t.auto_det_data, (const char *)wifi_t.wifi_data,150);
	   wifi_t.get_rx_auto_repeat_net_enable =1;

       if(strstr((const char*)wifi_t.auto_det_data,"QTTSTATE:0")){

			 wifi_t.link_wifi_net_login_tencent_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =0;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"+TCMQTTRECONNECTING")){

			 wifi_t.link_wifi_net_login_tencent_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =0;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"ONNECTING")){

			 wifi_t.link_wifi_net_login_tencent_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =0;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"QTTSTATE:1")){

		   wifi_t.link_wifi_net_login_tencent_success= 1;
		   wifi_t.auto_link_login_tencent_cloud_flag =1;//link NET OK.


       }
       
         wifi_t.wifi_uart_rx_counter=0;
		
         wifi_t.gTimer_auto_detected_net_state_times=0;
		
    
}


static void auto_repeat_init_link_net(void)
{
	    WIFI_IC_DISABLE();
		HAL_Delay(1000);
		//HAL_Delay(1000);
		//HAL_Delay(1000);
		WIFI_IC_ENABLE();
		//at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
		HAL_Delay(1000);
}

static void auto_repeat_tencnet_net(void)
{
            wifi_t.linking_tencent_cloud_doing =1;
          
			wifi_t.wifi_uart_rx_counter=0;
	        wifi_t.soft_ap_config_flag =0;
	        HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
		    HAL_Delay(1000);
			HAL_Delay(1000);
			HAL_Delay(1000);
		
}


