#include <stdint.h>

#define DR                      (*((volatile unsigned long *)0x40005010))
#define RE                      0x00;
#define DE                      0x04;

void UART1_Init(void);

void UART1_TX(uint8_t data_out);

void UART1_Out_String(char *ptr);

uint8_t UART1_RX(void);

void RS485_DR(void);

