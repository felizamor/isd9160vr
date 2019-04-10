/** ���SPI */

#include "stdint.h"
#include "ISD9100.h"
#include "gpio.h"
#include "gpio_spi.h"




void delay(uint32_t time)
{
	while(time)	time--;
}



/**
  * @brief :SPI��ʼ��(���)
  * @param :��
  * @note  :��
  * @retval:��
  */
void drv_spi_init( void )
{
//	 GPIO_SetMode(PA, 9, GPIO_MODE_OUTPUT );
//	 GPIO_SetMode(PA, 11, GPIO_MODE_OUTPUT );
//	 GPIO_SetMode(PA, 15, GPIO_MODE_OUTPUT );
//	 //GPIO_SetMode(PA, 13, GPIO_MODE_INPUT );
	
	
	PA->MODE|=(1<<9);
	PA->MODE|=(1<<11);
	PA->MODE|=(1<<15);
	PA->MODE&=(~(1<<13));
	// PA->MODE &= ~(3<<26);
	spi_set_nss_high();	
	spi_set_clk_high();
}

/**
  * @brief :SPI�շ�һ���ֽ�
  * @param :
  *			@TxByte: ���͵������ֽ�
  * @note  :�Ƕ���ʽ��һ���ȴ���ʱ���������Զ��˳�
  * @retval:���յ����ֽ�
*/
uint8_t drv_spi_read_write_byte( uint8_t TxByte )
{
	uint8_t i = 0, Data = 0;
	
	spi_set_clk_low( );
	delay(2500)	;
	for( i = 0; i < 8; i++ )			//һ���ֽ�8byte��Ҫѭ��8��
	{
		/** ���� */
		if( 0x80 == ( TxByte & 0x80 ))
		{
			spi_set_mosi_hight( );		//�������Ҫ���͵�λΪ 1 ���ø�IO����
			delay(40)	;
		}
		else
		{
			spi_set_mosi_low( );		//�������Ҫ���͵�λΪ 0 ���õ�IO����
			delay(40)	;
		}
		TxByte <<= 1;					//��������һλ���ȷ��͵������λ
		
		spi_set_clk_high( );			//ʱ�����ø�
		//__nop( );
		//__nop( ); __nop( );__nop( );__nop( );
		delay(5000)	;
		/** ���� */
		Data <<= 1;						//������������һλ���Ƚ��յ��������λ
		if(spi_get_miso( ))
		{
			Data |= 0x01;				//�������ʱIO����Ϊ������Ϊ���յ� 1
		}
		
		spi_set_clk_low( );				//ʱ�����õ�
		//__nop( );__nop( ); __nop( );__nop( );__nop( );
		//__nop( ); __nop( );__nop( );__nop( );__nop( );
		delay(5000)	;
	}
	delay(2000)	;
	return Data;		//���ؽ��յ����ֽ�
}

/**
  * @brief :SPI�շ��ַ���
  * @param :
  *			@ReadBuffer: �������ݻ�������ַ
  *			@WriteBuffer:�����ֽڻ�������ַ
  *			@Length:�ֽڳ���
  * @note  :�Ƕ���ʽ��һ���ȴ���ʱ���������Զ��˳�
  * @retval:��
  */
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
{
	spi_set_nss_low( );			//Ƭѡ����
	spi_set_clk_low();
	while( Length-- )
	{
		*ReadBuffer = drv_spi_read_write_byte( *WriteBuffer );		//�շ�����
		ReadBuffer++;
		WriteBuffer++;			//��д��ַ��1
	}
	
	spi_set_nss_high( );		//Ƭѡ����
	spi_set_clk_high();
}


/** ���SPI */

