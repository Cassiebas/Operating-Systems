#ifndef EOS_H
#define EOS_H

#include <stdint.h>
#include <inttypes.h>

typedef       int32_t  I32;
typedef      uint32_t UI32;
typedef       int64_t  I64;
typedef      uint64_t UI64;
typedef unsigned char   UC;
UI64 eosBase;

extern void blink();
extern void setFSEL(volatile UI32 *address, UI32 value, UI32 offset);
extern void setGPSET(volatile UI32 *address, UI32 value, UI32 offset);
extern void setGPCLR(volatile UI32 *address, UI32 value, UI32 offset);
extern int getBit(int n, int bitnr);
extern void printsln(char* str);;
extern void printBin(volatile uint32_t bits);


#define BUSY_WAIT asm volatile("")
#define BUSY_WAIT_N 0x100000

//Base Registers
#define EOS_PI3ID                           0x0D03
#define EOS_PI4ID                           0x0D08
#define EOS_PI3BASE                         0x3F000000
#define EOS_PI4BASE                         0xFE000000

//GPIO Functional bitfield
#define INPUT                             (UI32)  0b000
#define OUTPUT                            (UI32)  0b001
#define ALT0                              (UI32)  0b100
#define ALT1                              (UI32)  0b101
#define ALT2                              (UI32)  0b110
#define ALT3                              (UI32)  0b111
#define ALT4                              (UI32)  0b011
#define ALT5                              (UI32)  0b010

//GPIO Registers
#define EOS_GPIO_BASE                       eosBase + 0x00200000
#define EOS_GPFSEL0                         (UI32 *) (EOS_GPIO_BASE + 0x00)
#define EOS_GPFSEL1                         (UI32 *) (EOS_GPIO_BASE + 0x04)
#define EOS_GPFSEL2                         (UI32 *) (EOS_GPIO_BASE + 0x08)
#define EOS_GPFSEL3                         (UI32 *) (EOS_GPIO_BASE + 0x0C)
#define EOS_GPFSEL4                         (UI32 *) (EOS_GPIO_BASE + 0x10)
#define EOS_GPFSEL5                         (UI32 *) (EOS_GPIO_BASE + 0x14)
#define EOS_GPSET0                          (UI32 *) (EOS_GPIO_BASE + 0x1C)
#define EOS_GPSET1                          (UI32 *) (EOS_GPIO_BASE + 0x20)
#define EOS_GPCLR0                          (UI32 *) (EOS_GPIO_BASE + 0x28)
#define EOS_GPCLR1                          (UI32 *) (EOS_GPIO_BASE + 0x2C)
#define EOS_GPLEV0                          (UI32 *) (EOS_GPIO_BASE + 0x34)
#define EOS_GPLEV1                          (UI32 *) (EOS_GPIO_BASE + 0x38)
#define EOS_GPEDS0                          (UI32 *) (EOS_GPIO_BASE + 0x40)
#define EOS_GPEDS1                          (UI32 *) (EOS_GPIO_BASE + 0x44)
#define EOS_GPREN0                          (UI32 *) (EOS_GPIO_BASE + 0x4C)
#define EOS_GPREN1                          (UI32 *) (EOS_GPIO_BASE + 0x50)
#define EOS_GPFEN0                          (UI32 *) (EOS_GPIO_BASE + 0x58)
#define EOS_GPFEN1                          (UI32 *) (EOS_GPIO_BASE + 0x5C)
#define EOS_GPHEN0                          (UI32 *) (EOS_GPIO_BASE + 0x64)
#define EOS_GPHEN1                          (UI32 *) (EOS_GPIO_BASE + 0x68)
#define EOS_GPLEN0                          (UI32 *) (EOS_GPIO_BASE + 0x70)
#define EOS_GPLEN1                          (UI32 *) (EOS_GPIO_BASE + 0x74)
#define EOS_GPAREN0                          (UI32 *) (EOS_GPIO_BASE + 0x7C)
#define EOS_GPAREN1                          (UI32 *) (EOS_GPIO_BASE + 0x80)
#define EOS_GPAFEN0                          (UI32 *) (EOS_GPIO_BASE + 0x88)
#define EOS_GPAFEN1                          (UI32 *) (EOS_GPIO_BASE + 0x8C)
#define EOS_PUP_PDN_REG0                          (UI32 *) (EOS_GPIO_BASE + 0xE4)
#define EOS_PUP_PDN_REG1                          (UI32 *) (EOS_GPIO_BASE + 0xE8)
#define EOS_PUP_PDN_REG2                          (UI32 *) (EOS_GPIO_BASE + 0xEC)
#define EOS_PUP_PDN_REG3                          (UI32 *) (EOS_GPIO_BASE + 0xF0))

//UART Base Registers
#define EOS_UART0_BASE                      eosBase + 0x00201000
#define EOS_UART1_BASE                      eosBase + 0x00201400
#define EOS_UART2_BASE                      eosBase + 0x00201600
#define EOS_UART3_BASE                      eosBase + 0x00201800
#define EOS_UART4_BASE                      eosBase + 0x00201A00

//UART Registers ; Please Adjust the base register to used UARTX BASE!
#define EOS_UART_DR                        (EOS_UART0_BASE + 0x00)
#define EOS_UART_RSRECR                        (EOS_UART0_BASE + 0x04)
#define EOS_UART_FR                        (EOS_UART0_BASE + 0x18)
#define EOS_UART_ILPR                        (EOS_UART0_BASE + 0x20)
#define EOS_UART_IBRD                        (EOS_UART0_BASE + 0x24)
#define EOS_UART_FBRD                        (EOS_UART0_BASE + 0x28)
#define EOS_UART_FCRH                        (EOS_UART0_BASE + 0x2C)
#define EOS_UART_CR                        (EOS_UART0_BASE + 0x30)
#define EOS_UART_IFLS                        (EOS_UART0_BASE + 0x34)
#define EOS_UART_IMSC                        (EOS_UART0_BASE + 0x38)
#define EOS_UART_RIS                        (EOS_UART0_BASE + 0x3C)
#define EOS_UART_MIS                        (EOS_UART0_BASE + 0x40)    
#define EOS_UART_ICR                        (EOS_UART0_BASE + 0x44)
#define EOS_UART_DMACR                        (EOS_UART0_BASE + 0x48)
#define EOS_UART_ITCR                        (EOS_UART0_BASE + 0x80)
#define EOS_UART_ITIP                        (EOS_UART0_BASE + 0x84)
#define EOS_UART_ITOP                        (EOS_UART0_BASE + 0x88)
#define EOS_UART_TDR                        (EOS_UART0_BASE + 0x8C)

//Ascii codes
#define EOS_ASCII_ESC                       0x1B

//Terminal codes (ansi)
#define EOS_TERM_CLS                        "[2J"
#define EOS_TERM_HOME                       "[H"

//Macros
#define put8( reg, data) (*((UC   *)(reg)) = ((UC)(data)))
#define get8( reg)       (*((UC   *)(reg)))
#define put32(reg, data) (*((UI32 *)(reg)) = ((UI32)(data)))
#define get32(reg)       (*((UI32 *)(reg)))
#define put64(reg, data) (*((UI64 *)(reg)) = ((UI64)(data)))
#define get64(reg)       (*((UI64 *)(reg)))

#define BIT                         (UI32) 0b1
#define SELECT_BIT(x)               (1 << (x))
#define HOLD_1S                     for (i = 0; i < 2000000; ++i) { BUSY_WAIT; }

#endif
