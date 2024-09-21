#include "bsp.h"

uint8_t sub_send_power_on_times;
uint8_t get_rx_beijing_time_enable;


uint8_t beijing_step;
uint8_t auto_det_flag;
uint8_t auto_link_tencent_step;


static void auto_link_tencent_cloud_fun(void);

static void  wifi_link_net_check_cmd(void);



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
    static uint8_t alternate_flag,flag_switch,auto_link_net_flag;
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
        else if(flag_switch >  1 && gkey_t.key_power == power_off){
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
    	
    
            
            beijing_step =1;

           

         break;

         case 1:
          
    		
        
                 //disable publish data to tencent cloud.
                gpro_t.gTimer_get_data_from_tencent_data=0;
                wifi_t.get_rx_beijing_time_enable=1;
        		wifi_t.wifi_uart_rx_counter =0;
        		Get_Beijing_Time();
              
        	     osDelay(100);//HAL_Delay(20); //WT.EDIT .2024.08.10
                
        
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

                //    SendWifiData_To_PanelTime(gpro_t.disp_works_hours,gpro_t.disp_works_minutes,gpro_t.disp_works_time_seconds);
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

  
       wifi_link_net_check_cmd();
    

   

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
         if(wifi_link_net_state()==0 && gkey_t.wifi_led_fast_blink_flag==0){

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
            wifi_t.wifi_uart_rx_counter=0;
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
       
         

        wifi_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
	    wifi_t.wifi_uart_rx_counter=0; //clear USART2 counter is zero
		wifi_t.soft_ap_config_flag =0; 
	

          if(gkey_t.key_power == power_on){
                MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
                HAL_Delay(200);

            }
            else if(gkey_t.key_power == power_off){

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
	*Functin Name: void wifi_auto_detected_link_state(void)
	*Function :
	*Input Ref:  NO
	*Return Ref: NO
	*
**********************************************************************/
void wifi_auto_detected_link_state(void)
{
    static uint8_t power_on_dc_power;
	if(auto_link_tencent_step!=0xff && wifi_link_net_state()==0 && power_on_dc_power ==0){
	
       gpro_t.gTimer_get_data_from_tencent_data=0;
       wifi_t.linking_tencent_cloud_doing = 1;

      auto_link_tencent_cloud_fun();
     
    }
    if(wifi_link_net_state()==1    && power_on_dc_power ==0){
              power_on_dc_power++;
   
           gpro_t.gTimer_get_data_from_tencent_data=0; //don't send dato to tencent .waiting .
       
             
           wifi_t.linking_tencent_cloud_doing = 0;

           auto_link_tencent_step =0;
           wifi_t.link_net_tencent_data_flag=1;
  
        
          if(gkey_t.key_power == power_off){
		     MqttData_Publish_PowerOff_Ref();
               HAL_Delay(200);

          }
          else{
             MqttData_Publish_Update_Data();
		     HAL_Delay(100);


          }
          Subscriber_Data_FromCloud_Handler();
          HAL_Delay(200);
		
	}
   

   
   
}


/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
static void auto_link_tencent_cloud_fun(void)
{


   switch(auto_link_tencent_step){

   case 0:

	WIFI_IC_ENABLE();
   
	  // at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
	   at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
       HAL_Delay(1000);

	  auto_link_tencent_step=1;
      wifi_t.gTimer_power_first_link_tencent=0;

    break;

    case 1:
    
	if(wifi_t.gTimer_power_first_link_tencent > 1 ){
	   wifi_t.gTimer_power_first_link_tencent=0;
	    auto_link_tencent_step=2;
	   
	   

	}
    break;
    
    case 2:

    wifi_t.linking_tencent_cloud_doing =1; //enable usart2 receive wifi  data
    wifi_t.wifi_uart_rx_counter=0;
	wifi_t.soft_ap_config_flag =0;
  
     auto_link_tencent_step =3;

   break;

   case 3:
  
        HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接
       HAL_Delay(1000);
       wifi_t.gTimer_power_first_link_tencent=0;
	
        auto_link_tencent_step=4; //4

   break;

   case 4:
	if(wifi_t.gTimer_power_first_link_tencent >4 ){
		wifi_t.gTimer_power_first_link_tencent=0;
		  auto_link_tencent_step=0xff; //5
	

		if(wifi_link_net_state()==1){
			wifi_t.linking_tencent_cloud_doing =0;
	    }

	}
    break;

    }

}



void Get_BeiJing_Time_Cmd(void)
{

 // HAL_UART_Transmit(&huart2, "AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n", strlen("AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n"), 5000);//开始连接
    HAL_UART_Transmit(&huart2, "AT+CIPSNTPCFG=1,8\r\n", strlen("AT+CIPSNTPCFG=1,8\r\n"), 5000);//开始连接

}

void Get_Beijing_Time(void)
{

   HAL_UART_Transmit(&huart2, "AT+CIPSNTPTIME?\r\n", strlen("AT+CIPSNTPTIME?\r\n"), 5000);//开始连接


}



static void  wifi_link_net_check_cmd(void)
{

   HAL_UART_Transmit(&huart2, "AT+TCMQTTSTATE?\r\n", strlen("AT+TCMQTTSTATE?\r\n"), 0xffff);

  
 
  
}

