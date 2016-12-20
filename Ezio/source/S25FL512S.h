/*
 * S25FL512S.h
 *
 *  Created on: Apr 25, 2016
 *      Author: csxwin
 */

#ifndef S25FL512S_H_
#define S25FL512S_H_

//*****************************************************************************
//
// SYSCTL registers
//
//*****************************************************************************
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
#define SYSCTL_RCGCSSI_R        (*((volatile unsigned long *)0x400FE61C))
//*****************************************************************************
//
// GPIO registers (PORTB)
//
//*****************************************************************************
#define GPIO_PORTB_DATA_BITS_R  ((volatile unsigned long *)0x40005000)
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_IS_R         (*((volatile unsigned long *)0x40005404))
#define GPIO_PORTB_IBE_R        (*((volatile unsigned long *)0x40005408))
#define GPIO_PORTB_IEV_R        (*((volatile unsigned long *)0x4000540C))
#define GPIO_PORTB_IM_R         (*((volatile unsigned long *)0x40005410))
#define GPIO_PORTB_RIS_R        (*((volatile unsigned long *)0x40005414))
#define GPIO_PORTB_MIS_R        (*((volatile unsigned long *)0x40005418))
#define GPIO_PORTB_ICR_R        (*((volatile unsigned long *)0x4000541C))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DR2R_R       (*((volatile unsigned long *)0x40005500))
#define GPIO_PORTB_DR4R_R       (*((volatile unsigned long *)0x40005504))
#define GPIO_PORTB_DR8R_R       (*((volatile unsigned long *)0x40005508))
#define GPIO_PORTB_ODR_R        (*((volatile unsigned long *)0x4000550C))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_PDR_R        (*((volatile unsigned long *)0x40005514))
#define GPIO_PORTB_SLR_R        (*((volatile unsigned long *)0x40005518))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define GPIO_PORTB_ADCCTL_R     (*((volatile unsigned long *)0x40005530))
#define GPIO_PORTB_DMACTL_R     (*((volatile unsigned long *)0x40005534))


//*****************************************************************************
//
// SSI registers (SSI2)
//
//*****************************************************************************
#define SSI2_CR0_R              (*((volatile unsigned long *)0x4000A000))
#define SSI2_CR1_R              (*((volatile unsigned long *)0x4000A004))
#define SSI2_DR_R               (*((volatile unsigned long *)0x4000A008))
#define SSI2_SR_R               (*((volatile unsigned long *)0x4000A00C))
#define SSI2_CPSR_R             (*((volatile unsigned long *)0x4000A010))
#define SSI2_IM_R               (*((volatile unsigned long *)0x4000A014))
#define SSI2_RIS_R              (*((volatile unsigned long *)0x4000A018))
#define SSI2_MIS_R              (*((volatile unsigned long *)0x4000A01C))
#define SSI2_ICR_R              (*((volatile unsigned long *)0x4000A020))
#define SSI2_DMACTL_R           (*((volatile unsigned long *)0x4000A024))
#define SSI2_CC_R               (*((volatile unsigned long *)0x4000AFC8))

#define CHIP_SELECT_SSI2        (*((volatile unsigned long *)0x40005080)) //PB5
#define CS_LOW 					0x00
#define CS_HIGH					0x20

#define READ 					0x13
#define Bulk_Erase              0x60
#define WREN                    0x06
#define WRR 					0x01
#define EWSR					0x50
#define Read_ID 				0x90
#define RDID					0x9F

void SSI2_Init(void);
extern void Flash_Init();
void SSI2_Write(uint8_t command);
uint8_t SSI2_Read();
void Flash_WREN();
void Flash_SR_Write(void);
void Flash_SR_Read(uint8_t * SR1, uint8_t * SR2);
extern void Flash_ReadID(uint16_t * Mfg_ID, uint16_t * DeviceID);
extern uint8_t Flash_Bulk_Erase(void);
void Flash_Page_Program(uint32_t start_addr, uint16_t pattern);
void Flash_Read(uint32_t start_addr);
void SPI_FIFO_Cleanup(void);
#endif /* S25FL512S_H_ */
