#ifndef __BSP_WIFI_FUN_
#define __BSP_WIFI_FUN_
#include "main.h"



#define SSID                    "UYIKIA"
#define PASSWD                  "20329263"


#define TOPIC                  "open"

#define TOPIC_VALUE            1
//#define SMARTCONFIG      			0


#define PRODUCT_ID              "EHQB1P53IH" //production ID 
//#define DEVUICE_NAME            "UYIJIA01-afcc8"      //device Name:
#define DEVICE_SECRET           "5xdQMgx7ZHznYyQK6wvdmolc"//"5xdQMgx7ZHznYyQK6wvdmolc"  //Secret key 
#define PRODUCT_REGION          "ap-guangzhou" //filed name 

#define WIFI_IC_ENABLE()         HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_SET)
#define WIFI_IC_DISABLE()        HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_RESET)







typedef enum{
  
   wifi_AI=0x08 ,wifi_notAI=0x18,wifi_kill=0x04,wifi_notkill=0x14,
   wifi_heat= 0x02,wifi_notheat=0x12


}wifi_mode_t;

typedef enum _wifi_state_t{  
	
    
	wifi_link_tencent_cloud=0x01, //1
	wifi_tencent_publish_init_data, //2
	wifi_subscriber_form_tencent_data,//3
	wifi_publish_dht11_delay,//4
	wifi_subscriber_form_tencent_cloud_cmd,//5
	wifi_publish_update_tencent_cloud_data,//6
	wifi_auto_to_link_cloud,//7
	wifi_atuo_link_cloud_ref,//8
	wifi_auto_repeat_link_cloud,//9
	wifi_tencent_publish_dht11_data,//10
	wifi_get_beijing_time,//11
	wifi_disconnect,//12
	wifi_null//13


}wifi_state_t;


/* USER CODE BEGIN Prototypes */
typedef struct usart
{
    unsigned char UART_Data[150]; 
    unsigned char UART_Flag;
    unsigned int wifi_uart_rx_counter;
    unsigned char wifi_inputBuf[1];
}USART_DataBuf;






typedef struct _WIFI_FUN{
	

    uint8_t data[150];

    uint8_t  data_size;
    uint8_t flag;
	uint8_t wifi_RunState;
	uint8_t esp8266_smartphone_flag;
	uint8_t esp8266_link_cloud_flag;
	uint8_t soft_ap_config_success;
	uint8_t link_wifi_net_login_tencent_success;

	uint8_t subscribe_cloud_success;
	uint8_t power_off_step;
    uint8_t rx_net_counter;
    uint8_t link_net_tencent_data_flag; //after send publish datat to tencent .
    uint8_t tencent_rx_data_success ;
    uint8_t recoder_tempearture_rx_data;

    //
    uint8_t fin_speed_full_flag;
    uint8_t rx_data_state;
    uint8_t wifi_rx_counter;
    uint8_t wifi_rx_data_array[150];
   
	
	
	uint8_t esp8266_dynamic_reg_flag;
    uint8_t subscribe_flag;
	uint8_t subscribe_rx_flag;
    uint8_t subscribe_rxCloud_flag;
	uint8_t esp8266_data_rx_success;
	uint8_t rx_data_success ;
	uint8_t  rx_counter ;

	uint8_t getCloudValue_unit ;
	uint8_t getCloudValue_decade;
	uint8_t rx_data_len;
	uint8_t rx_data_name_len;
	uint8_t linking_tencent_cloud_doing;
    uint8_t cmp_flag;
	uint8_t send_tencent_cloud_data_lable;
    uint8_t subscrible_receive_data_label;
	uint8_t link_tencent_step_counter;
	uint8_t link_beijing_times_flag;
	uint8_t auto_linknet_flag;
	uint8_t three_times_link_beijing;
    //esp8266 up ref

	uint8_t runCommand_order_lable;
	uint8_t has_been_login_flag;
    uint8_t soft_ap_config_flag;
    uint8_t get_rx_beijing_time_enable;
	uint8_t auto_link_login_tencent_cloud_flag;
	uint8_t receive_beijing_time;
	
	//wifi
	unsigned char wifi_data[150]; 
	uint8_t  wifi_temp_data[150]; 
 
	uint8_t usart1_dataBuf[1];
  
    //uint8_t usart2_rx_flag;
	
	
	uint8_t wifi_uart_rx_counter;
	uint8_t tencent_cloud_command_power_on;
	uint8_t get_rx_auto_repeat_net_enable;
    uint8_t once_rx_data_done;
	
	//
	uint8_t response_wifi_signal_label;
	uint8_t reconnect_tencent_cloud_flag;

	
    //smart phone 
    uint8_t smartphone_app_power_on_flag;
   
	uint8_t power_on_login_tencent_cloud_flag;
	uint8_t repeat_login_tencent_cloud_init_ref;
    

	//fan
	uint8_t set_wind_speed_value;

	//config


    //real time 
    uint8_t real_hours;
	uint8_t real_minutes;
	uint8_t real_seconds;
	uint8_t restart_link_tencent_cloud ;
	uint8_t received_data_from_tencent_cloud ;
	//beijing time
	uint8_t set_beijing_time_flag;
	

	//auto link net
	uint8_t rx_error_codes_flag;
	uint8_t link_tencent_thefirst_times;
	uint8_t rx_setup_hardware_counter;
    uint8_t tencent_rx_data_flag;
    uint8_t get_beijing_timing_success;

    //wifi gtimer

	uint8_t gTimer_linking_tencent_duration;
	
    uint8_t gTimer_beijing_time;
	uint8_t gTimer_reconnect_wifi;
	uint8_t gTimer_power_off_run_times;
	uint8_t gTimer_subscriber_send ;
	
	
	uint8_t gTimer_login_tencent_times;
	
	uint8_t gTimer_wifi_power_on_detect ;

	uint8_t gTimer_main_gpro_times;
	uint8_t gTimer_wifi_sub_power_off;
	uint8_t gTimer_wifi_rx_error;

    uint8_t gTimer_normal_send_dat_tencent ;
     uint8_t gTimer_auto_link_net_time;
     uint8_t gTimer_power_first_link_tencent;
	
    uint16_t gTimer_get_beijing_time;
	uint16_t gTimer_auto_detected_net_state_times;
    
	

	uint32_t randomName[1];
	
	
		
	
}WIFI_FUN;

extern WIFI_FUN   wifi_t;

extern uint8_t (*wifi_link_net_state)(void); //函数指针


void Wifi_Init(void);

void PowerOn_Host(void (*poweronHandler)(void));
void PowerOff_Host(void (*poweroffHandler)(void));




void wifiDisplayTemperature_Humidity(void);

void Wifi_Link_Net_Handler(uint8_t (*wifi_link_handler)(void));








void link_wifi_net_state_handler(void);


uint8_t at_send_data(uint8_t* pdata, uint16_t len);



#endif 

