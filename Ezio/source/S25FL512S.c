#include <stdint.h>
#include "S25FL512S.h"
#include "common.h"

void SSI2_Init(void)
{
	volatile uint32_t delay;
	SYSCTL_RCGCSSI_R |= 0x04;
	delay = 0;

	SSI2_CR1_R &= ~0x02;
	SSI2_CR0_R = 0x0007;
	SSI2_CPSR_R = 0x02;
	SSI2_CR1_R = 0x02; //LB

	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R & 0x00000002) == 0){};

	//DebugPort("Finish SSI3 Init");
	GPIO_PORTB_AFSEL_R = (GPIO_PORTB_AFSEL_R & 0x0F) + 0xD0;
	GPIO_PORTB_AFSEL_R &= ~0x20;
	GPIO_PORTB_DIR_R |= 0x20;
	GPIO_PORTB_DEN_R |= 0xF0;
	GPIO_PORTB_AMSEL_R &= ~0xF0;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0x0000FFFF) + 0x22020000;
	GPIO_PORTB_PUR_R |= 0x20;
}

extern void Flash_Init()
{
	SSI2_Init();
	//Flash_Write_SR();
}

void SSI2_Write(uint8_t command)
{
	while((SSI2_SR_R & 0x00000001) == 0){};
	CHIP_SELECT_SSI2 = CS_LOW;
	SSI2_DR_R = command;
	while((SSI2_SR_R & 0x00000010) == 0x00000010){};
	CHIP_SELECT_SSI2 = CS_HIGH;
}

void SSI2_Stream_Write(uint8_t *commandLine, uint8_t numberOfCMD)
{
	uint8_t num = numberOfCMD;
	CHIP_SELECT_SSI2 = CS_HIGH;
	delay1ms(1);
	CHIP_SELECT_SSI2 = CS_LOW;
	while(num-- > 0)
	{
		while((SSI2_SR_R & 0x00000001) == 0){};
		SSI2_DR_R = *(commandLine++);
	}
}

uint8_t SSI2_Read()
{
	uint8_t n = 0X55;
	uint8_t loop = 20;

	while(loop >5)
	{
		loop --;
		while((SSI2_SR_R & 0x00000001) == 0){};
		SSI2_DR_R = 0x00;
		while((SSI2_SR_R & 0x00000004) == 0){};
		delay1ms(1);
		n = SSI2_DR_R;
		delay1ms(1);// 330 us
	}
	return n;
}

extern void Flash_ReadID(uint16_t * Mfg_ID, uint16_t * DeviceID)
{
	uint8_t loc_mfg_Id, loc_device_Id;
	uint8_t cmd[4] = {0x90, 0x00, 0x00, 0x00};
	SSI2_Stream_Write(cmd, 4);
	SPI_FIFO_Cleanup();

	while((SSI2_SR_R & 0x00000001) == 0){};
	SSI2_DR_R = 0XFF;
	loc_device_Id = SSI2_DR_R;

	while((SSI2_SR_R & 0x00000001) == 0){};
	SSI2_DR_R = 0XFF;
	loc_mfg_Id = SSI2_DR_R;

	while((SSI2_SR_R & 0x00000010) == 0x00000010){};
	CHIP_SELECT_SSI2 = CS_HIGH;
	*Mfg_ID = loc_mfg_Id;
	*DeviceID = loc_device_Id;
}

uint8_t Flash_Bulk_Erase()
{
	uint16_t SR1[20];
	uint16_t old_buffer[20];
	uint8_t count = 20;
	uint8_t c = 0x10;
	uint8_t cmd = 0x05;

	for(c = 0; c < 20; c++)
	{
		SR1[c] = 0x0000;
	}
	for(c = 0; c < 20; c++)
	{
		old_buffer[c] = 0xFFFF;
	}

	Flash_WREN();
	SSI2_Write(0x60);
	SSI2_Stream_Write(&cmd, 1);

	c = 0;
	while(c < 9)
	{
		while((SSI2_SR_R & 0x00000001) == 0){};
		SSI2_DR_R = 0xFF;
		c++;
		old_buffer[c] = SSI2_DR_R;
	} //clean up the old buffer from receive FIFO

	while(count>0)
	{
		count--;
		c = 0;
		while(c < 8)
		{
			while((SSI2_SR_R & 0x00000001) == 0){};
			SSI2_DR_R = 0xFF;
			old_buffer[c] = SSI2_DR_R;
			c++;
		}
		SR1[19 - count] = SSI2_DR_R;
		if(SR1[19-count] == 0x0000)
		{
			break;
		}
		delay1ms(5000);
	}
	while((SSI2_SR_R & 0x00000010) == 0x00000010){};
	CHIP_SELECT_SSI2 = CS_HIGH;
	return count;
}

void Flash_Page_Program(uint32_t start_addr, uint16_t pattern)
{
	uint16_t count = 256;
	uint8_t pattern_L = (uint8_t)(pattern & 0x00FF);
	uint8_t pattern_H = (uint8_t)((pattern & 0xFF00) >> 8);
	Flash_WREN();
	uint8_t cmd[5] = {0x12, 0x00, 0x00, 0x00, 0x00};
	SSI2_Stream_Write(cmd, 5);
	while(count > 0)
	{
		count--;
		while((SSI2_SR_R & 0x00000001) == 0){};
		SSI2_DR_R = pattern_L;
		while((SSI2_SR_R & 0x00000001) == 0){};
		SSI2_DR_R = pattern_H;
	}
	while((SSI2_SR_R & 0x00000010) == 0x00000010){};
	CHIP_SELECT_SSI2 = CS_HIGH;
}

void Flash_Read(uint32_t start_addr)
{
	uint8_t loop = 1;
	uint16_t increment;
	uint16_t data [200];
	//uint16_t data2 [200];
	//uint16_t data3 [200];
	uint8_t cmd[5];
	*cmd = 0x13;
	*(cmd+1) = start_addr >> 24 & 0xFF;
	*(cmd+2) = start_addr >> 16 & 0xFF;
	*(cmd+3) = start_addr >> 8 & 0xFF;
	*(cmd+4) = start_addr & 0xFF;
	SSI2_Stream_Write(cmd, 5);
	SPI_FIFO_Cleanup();

	while(loop > 0)
	{
		loop--;
		increment = 0;
		while(increment < 200)
		{
			while((SSI2_SR_R & 0x00000001) == 0){};
			SSI2_DR_R = 0XFF;
			data[increment] = SSI2_DR_R;
			increment++;
		}
	}

	while((SSI2_SR_R & 0x00000010) == 0x00000010){};
	CHIP_SELECT_SSI2 = CS_HIGH;
}

void Flash_WREN()
{
	SSI2_Write(0x06);
}

void Flash_SR_Read(uint8_t * SR1, uint8_t * SR2)
{
	uint8_t c = 0x10;
	uint8_t cmd = 0x05;
	CHIP_SELECT_SSI2 = CS_HIGH;
	SSI2_Stream_Write(&cmd, 1);

	while((SSI2_SR_R & 0x00000001) == 0){};
	SSI2_DR_R = 0xFF;
	while((SSI2_SR_R & 0x00000004) == 0){}
	//while((SSI2_DR_R == 0x00)&&(c>0))
	{
		//c--;
		//delay1ms(5);
	};
	*SR1 = SSI2_DR_R;
	*SR1 = SSI2_DR_R;
	*SR1 = SSI2_DR_R;
	//while((SSI2_SR_R & 0x00000001) == 0){};
	//SSI2_DR_R = 0xFF;
	//while((SSI2_SR_R & 0x00000004) == 0){}
	//while((SSI2_DR_R == 0x00)&&(c>0))
	{
		//c--;
		//delay1ms(5);
	};
	*SR2 = SSI2_DR_R;
	while((SSI2_SR_R & 0x00000010) == 0x00000010){};
	CHIP_SELECT_SSI2 = CS_HIGH;
}

void Flash_SR_Write()
{

}

void SPI_FIFO_Cleanup()
{
	uint8_t c = 0x10;
	uint16_t old_buffer[20];
	for(c = 0; c < 20; c++)
	{
		old_buffer[c] = 0xFFFF;
	}
	c = 0;
	while(c < 9)
	{
		while((SSI2_SR_R & 0x00000001) == 0){};
		SSI2_DR_R = 0xFF;
		c++;
		old_buffer[c] = SSI2_DR_R;
	} //clean up the old buffer from receive FIFO
	c = 0x00; //dummy
}






/*
 *
 * The following dead code is for studying the receive FIFO.
int Flash_Bulk_Erase()
{
	uint8_t SR1, SR2;
	uint16_t test1[20];
	uint16_t test2[20];
	uint16_t test3[20];
	uint16_t test4[20];
	uint16_t test5[20];
	uint16_t test6[20];
	uint16_t test7[20];
	uint16_t test8[20];
	uint16_t test9[20];
	uint8_t count = 20;
	uint8_t c = 0x10;
	uint8_t cmd = 0x05;

	for(c = 0x00; c < 20; c++)
	{
		test1[c] = 0x0000;
		test2[c] = 0x0000;
		test3[c] = 0x0000;
		test4[c] = 0x0000;
		test5[c] = 0x0000;
		test6[c] = 0x0000;
		test7[c] = 0x0000;
		test8[c] = 0x0000;
	}
	for(c = 0x00; c < 20; c++)
	{
		test9[c] = 0xFFFF;
	}


	Flash_WREN();
	SSI2_Write(0x60);
	//delay1ms(5);
	CHIP_SELECT_SSI2 = CS_HIGH;
	SSI2_Stream_Write(&cmd, 1);

	c = 0;
	while(c < 19)
	{
		while((SSI2_SR_R & 0x00000001) == 0){};
		SSI2_DR_R = 0xFF;
		c++;
		test9[c] = SSI2_DR_R;
	} //keep sending 0xFF until FIFO is full

	while(count>0)
	{
		count--;
		while((SSI2_SR_R & 0x00000001) == 0){};
		SSI2_DR_R = 0xFF;

		//while((SSI2_DR_R == 0x00)&&(c>0))
		{
			//c--;
			//delay1ms(5);
		};

		//delay1ms(100);
		test1[19 - count] = SSI2_DR_R;
		//delay1ms(100);
		test2[19 - count] = SSI2_DR_R;

		//while((SSI2_SR_R & 0x00000001) == 0){};
		//SSI2_DR_R = 0xFF;
		//while((SSI2_SR_R & 0x00000004) == 0){}
		//while((SSI2_DR_R == 0x00)&&(c>0))
		{
			//c--;
			//delay1ms(5);
		};

		test3[19 - count] = SSI2_DR_R;
		//delay1ms(1);
		test4[19 - count] = SSI2_DR_R;
		//delay1ms(1);

		test5[19 - count] = SSI2_DR_R;
		//delay1ms(1);
		test6[19 - count] = SSI2_DR_R;
		test7[19 - count] = SSI2_DR_R;
		test8[19 - count] = SSI2_DR_R;
		//test9[19 - count] = SSI2_DR_R;

		//delay1ms(100);
	}
	while((SSI2_SR_R & 0x00000010) == 0x00000010){};
	CHIP_SELECT_SSI2 = CS_HIGH;
	return 0;
}
*/
