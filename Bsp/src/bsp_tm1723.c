#include "bsp.h"

// 引脚定义 - 根据实际连接修改
#define TM1723_SCK_PORT     GPIOB
#define TM1723_SCK_PIN      GPIO_PIN_6

#define TM1723_MOSI_PORT    GPIOB
#define TM1723_MOSI_PIN     GPIO_PIN_5

#define TM1723_CS_PORT      GPIOB
#define TM1723_CS_PIN       GPIO_PIN_7

#define TM1723_STB_PORT    GPIOB
#define TM1723_STB_PIN    GPIO_PIN_7

// TM1723命令定义
#define TM1723_CMD_DATA     0x40
#define TM1723_CMD_DISPLAY  0x80
#define TM1723_CMD_ADDRESS  0xC0

// 显示控制选项
#define TM1723_DISPLAY_ON   0x08
#define TM1723_DISPLAY_OFF  0x00
#define TM1723_PULSE_1_16   0x00
#define TM1723_PULSE_2_16   0x01
#define TM1723_PULSE_4_16   0x02
#define TM1723_PULSE_10_16  0x03

// TM1723 命令定义
#define TM1723_CMD_DATA_SET       0x40  // 数据设置命令
#define TM1723_CMD_ADDR_FIXED     0x42  // 固定地址模式
#define TM1723_CMD_ADDR_AUTO_INC  0x40  // 自动递增地址模式
#define TM1723_CMD_DISPLAY_START  0xC0  // 显示起始地址


// 延时宏定义（根据主频调整）
#define SOFT_SPI_DELAY()    do { \
    asm volatile ("nop"); asm volatile ("nop"); \
    asm volatile ("nop"); asm volatile ("nop"); \
} while(0)



// 延时函数
void Delay_us(uint16_t us) {
    HAL_Delay(us / 1000 + 1);  // 简单实现，至少保证 1ms 的延时
}

  /**
 * @brief  TM1723写入一个字节
 * @param  byte: 要写入的字节
 * @retval None
 */
static void SoftSPI_WriteByte(uint8_t data)
{

    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        TM1723_CLK_SetLow();
        delay_us(2);
        
        if(data & 0x01)
            TM1723_DIO_SetHigh(); //写入数据 ‘1’
        else
            TM1723_DIO_SetLow(); //写入数据 ‘0’
            
        delay_us(2);
        TM1723_CLK_SetHigh();
        delay_us(2);
        data >>= 1;
    }
}


// 向指定地址写入数据
void TM1723_WriteData(uint8_t addr, uint8_t data)
{

   #if 0
// 拉低CS
    HAL_GPIO_WritePin(TM1723_CS_PORT, TM1723_CS_PIN, GPIO_PIN_RESET);
    
    // 发送地址命令 (固定地址模式)
    SoftSPI_WriteByte(TM1723_CMD_ADDRESS | (addr & 0x0F));
    
    // 发送数据命令 + 写数据
    SoftSPI_WriteByte(TM1723_CMD_DATA | 0x00);
    SoftSPI_WriteByte(data);
    
    // 拉高CS
    HAL_GPIO_WritePin(TM1723_CS_PORT, TM1723_CS_PIN, GPIO_PIN_SET);
	#else
	// 发送地址和数据
    SoftSPI_WriteByte(TM1723_CMD_DISPLAY_START | (addr & 0x0F));
   SoftSPI_WriteByte(data);




	#endif 
	
}





// 设置显示参数
void TM1723_SetDisplay(uint8_t on_off, uint8_t pulse_width)
{
    uint8_t cmd = TM1723_CMD_DISPLAY | on_off | (pulse_width & 0x03);
    
    // 拉低CS
    HAL_GPIO_WritePin(TM1723_CS_PORT, TM1723_CS_PIN, GPIO_PIN_RESET);
    
    // 发送命令
    SoftSPI_WriteByte(cmd);
    
    // 拉高CS
    HAL_GPIO_WritePin(TM1723_CS_PORT, TM1723_CS_PIN, GPIO_PIN_SET);
}

// 清除所有显示
//void TM1723_ClearAll(void)
//{
//    uint8_t clear_data[16] = {0};
//    TM1723_WriteMultipleData(0, clear_data, 16);
//}

