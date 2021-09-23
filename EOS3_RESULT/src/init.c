#include "eos.h"

#define ROOD_1 24
#define ROOD_2 7
#define ROOD_3 21
#define ROOD_4 6
#define GROEN_1 23
#define GROEN_2 8
#define GROEN_3 20
#define GROEN_4 5

#define EOS_PI3ID                           0x0D03
#define EOS_PI4ID                           0x0D08
#define EOS_PI3BASE                         0x3F000000
#define EOS_PI4BASE                         0xFE000000

#define EOS_CORE1_PTR                       0x000000E0
#define EOS_CORE2_PTR                       0x000000E8
#define EOS_CORE3_PTR                       0x000000F0

#define EOS_GPIO_BASE                       eosBase + 0x00200000
#define EOS_GPFSEL0                         (UI32 *) (EOS_GPIO_BASE)
#define EOS_GPFSEL1                         (UI32 *) (EOS_GPIO_BASE + 0x04)
#define EOS_GPFSEL2                         (UI32 *) (EOS_GPIO_BASE + 0x08)
#define EOS_GPSET0                          (UI32 *) (EOS_GPIO_BASE + 0x1C)
#define EOS_GPSET1                          (UI32 *) (EOS_GPIO_BASE + 0x20)
#define EOS_GPCLR0                          (UI32 *) (EOS_GPIO_BASE + 0x28)
#define EOS_GPCLR1                          (UI32 *) (EOS_GPIO_BASE + 0x2C)
#define EOS_GPLEV0                          (UI32 *) (EOS_GPIO_BASE + 0x34)


#define EOS_UART0_BASE                      eosBase + 0x00201000
#define EOS_UART0_DR                        (EOS_UART0_BASE + 0x00)
#define EOS_UART0_FR                        (EOS_UART0_BASE + 0x18)

#define EOS_ASCII_ESC                       0x1B
#define EOS_TERM_CLS                        "[2J"
#define EOS_TERM_HOME                       "[H"

UI64 eosBase;
UI32 active;

volatile UI32 * const GPFSEL0 = (UI32 *)0xFE200000;
volatile UI32 * const GPFSEL1 = (UI32 *)0xFE200004;
volatile UI32 * const GPFSEL2 = (UI32 *)0xFE200008;
volatile UI32 * const GPSET0  = (UI32 *)0xFE20001C;
volatile UI32 * const GPCLR0  = (UI32 *)0xFE200028;
volatile UI32 * const GPLEV0  = (UI32 *)0xFE200034;
volatile UI32 * const GPEDS0  = (UI32 *)0xFE200040;           

int getBit(int n, int bitnr);
void printBin(volatile UI32 bits);
void setGPSET(volatile UI32 *address, UI32 value, UI32 offset);
void setFSEL(volatile UI32 *address, UI32 value, UI32 offset);
void stage2(UI32 piId, UI32 exceptionLevel, UI64 sp);
void hang();
void core1_start();
void core2_start();
void core3_start();
void core1_stage2();
void core2_stage2();
void core3_stage2();
void buttonCheck(UI32 button);
void blinkMultiv2(UI32 rood, UI32 groen);
//void blinkMulti(UI32 bit1, UI32 bit2, UI32 bit3, UI32 bit4, UI32 bit5, UI32 bit6, UI32 bit7, UI32 bit8, UI32 Button);
//void blinkMultiCore_2(UI32 bit1, UI32 bit2, UI32 bit3, UI32 bit4, UI32 bit5, UI32 bit6, UI32 bit7, UI32 bit8);
void blink(UI32 bit, UI32 delay);
void putcs(char c);
char getcs();
void clearsScreen();
void prints(char* str);
void printsln(char* str);

/* entry point
   DO NOT USE ANY VARIABLES OR FUNCTION CALLS IN THIS FUNCTION!!!
   If you do, gcc will add a stack adjustment at the top of the function,
   before the stack is initialized. This will result in a hang at boot!!!
*/
void start() {
  asm volatile (
    " mov  x0, #0x00003000     \n " // set stack pointer
    " mov  sp, x0              \n "
    " mrs  x0, midr_el1        \n " // get pi version id
    " and  x0, x0, #0x0000FFF0 \n " // mask out other bits
    " lsr  x0, x0, #4          \n " // shift bits to end
    " mrs  x1, CurrentEL       \n " // get current exception level
    " and  x1, x1, #0x0000000C \n " // mask out other bits
    " lsr  x1, x1, #2          \n " // shift bits to end
    " mov  x2, sp              \n "
    " bl   stage2              \n " // go to stage 2
  );

  // Execution should never reach this line, but just to be sure
  for(;;) asm volatile ( " wfe \n "); // hang
}

void stage2(UI32 piId, UI32 exceptionLevel, UI64 sp) {
  // if not pi 3 or pi4 -> hang
  if((piId != EOS_PI3ID) && (piId != EOS_PI4ID)) hang();

  if(piId == EOS_PI3ID) {
    eosBase = EOS_PI3BASE;
  } else {
    eosBase = EOS_PI4BASE;
  }

  //buttons
  setFSEL(GPFSEL1, INPUT, 0); //GPIO10
  setFSEL(GPFSEL0, INPUT, 9); //GPIO9
  setFSEL(GPFSEL1, INPUT, 1); //GPIO11
  setFSEL(GPFSEL2, INPUT, 2); //GPIO22

  //leds
  setFSEL(GPFSEL2, OUTPUT, 3); //GPIO23 rood
  setFSEL(GPFSEL2, OUTPUT, 4); //GPIO24 groen
  setFSEL(GPFSEL0, OUTPUT, 8); //GPIO8 rood
  setFSEL(GPFSEL0, OUTPUT, 7); //GPIO7 groen
  setFSEL(GPFSEL2, OUTPUT, 0); //GPIO20 rood
  setFSEL(GPFSEL2, OUTPUT, 1); //GPIO21 groen
  setFSEL(GPFSEL0, OUTPUT, 5); //GPIO5 rood
  setFSEL(GPFSEL0, OUTPUT, 6); //GPIO6 groen

  clearsScreen();
  printsln("EOS 1.2 Casper Stoplichtjes Multi-core");
  UI32 gpset0, gpclr0;

  //gpset0 = get32(GPSET0);
  put32(GPSET0, 0x00000000 | BIT(24) | BIT(7) | BIT(21) | BIT(6));

  active =0;
  put32(EOS_CORE1_PTR, (UI32)((UI64)core1_start));
  put32(EOS_CORE2_PTR, (UI32)((UI64)core2_start));
  put32(EOS_CORE3_PTR, (UI32)((UI64)core3_start));
  
  asm volatile ( " sev \n ");

  buttonCheck(22);

  hang(); 
}

void hang() {
  for(;;) asm volatile ( " wfe \n "); // hang
}

void core1_start() {
  asm volatile (
    " mov  x0, #0x00004000     \n " // set stack pointer
    " mov  sp, x0              \n "
    " bl   core1_stage2        \n " // go to stage 2
  );
}

void core2_start() {
  asm volatile (
    " mov  x0, #0x00005000     \n " // set stack pointer
    " mov  sp, x0              \n "
    " bl   core2_stage2        \n " // go to stage 2
  );
}

void core3_start() {
  asm volatile (
    " mov  x0, #0x00006000     \n " // set stack pointer
    " mov  sp, x0              \n "
    " bl   core3_stage2        \n " // go to stage 2
  );
}

void core1_stage2() {
  buttonCheck(10);
  hang();
}

void core2_stage2() {
  buttonCheck(9);
  hang();
}

void core3_stage2() {
  buttonCheck(11);
  hang();
}

int getBit(int n, int bitnr)
{
  int mask = BIT(bitnr);
  int masked_n = n & mask;
  int thebit = masked_n >> bitnr;
  return thebit;
}

void printBin(volatile UI32 bits)
{
  if(bits)
  {
    for(int i = 31; i >= 0; i--)
    {
      if (getBit(bits, i) == 1)
      {
        putcs('1');
      }
      else
        putcs('0');
    }

    putcs('\r');
    putcs('\n');
  }
}

void setFSEL(volatile UI32 *address, UI32 value, UI32 offset)
{
  value <<= offset * 3;
  UI32 mask = 0b111 << offset * 3;
  UI32 temp = get32(address);
  temp &= (~mask);
  put32(address, (temp | value));
}

void setGPSET(volatile UI32 *address, UI32 value, UI32 offset)
{
  value <<= offset * 1;
  UI32 mask = 0b1 << offset * 1;
  UI32 temp = get32(address);
  temp &= (~mask);
  put32(address, (temp | value));
}


void blink(UI32 bit, UI32 delay) {
  for(;;) {
    put32(EOS_GPCLR0, 0x00000000 | BIT(bit));
    for(UI32 i = 0; i < delay; i++);
    put32(EOS_GPSET0, 0x00000000 | BIT(bit));
    for(UI32 i = 0; i < delay; i++);
  }
}

void buttonCheck(UI32 button)
{
  while(1)
  {
  
    UI32 gplev0, gpset0;
      gplev0 = get32(EOS_GPLEV0);
      if ((getBit(gplev0, button)) && (active == 0))
      {
        if (button == 11)
        {
          printsln("1");
          blinkMultiv2(ROOD_1, GROEN_1);
        }

        if (button == 9)
        {
          printsln("2");
          blinkMultiv2(ROOD_2, GROEN_2);
        }
        
        if (button == 10)
        {
          printsln("3");
          blinkMultiv2(ROOD_3, GROEN_3);
        }
        
        if (button == 22)
        {
          printsln("4");
          blinkMultiv2(ROOD_4, GROEN_4);
        }

      }

  }
}

void blinkMultiv2(UI32 rood, UI32 groen) {

  UI32 gpset0, gpclr0, i;

      put32(GPCLR0, 0x00000000 | BIT(rood));
      put32(GPSET0, 0x00000000 | BIT(groen));
      active = 1;
      HOLD_1S;
      HOLD_1S;
      HOLD_1S;
      HOLD_1S;
      HOLD_1S;
      put32(GPCLR0, 0x00000000 | BIT(groen));
      put32(GPSET0, 0x00000000 | BIT(rood));
      active = 0; 
  
}
// void blinkMulti(UI32 bit1, UI32 bit2, UI32 bit3, UI32 bit4, UI32 bit5, UI32 bit6, UI32 bit7, UI32 bit8, UI32 Button) {

//   UI32 gpset0, gpclr0, gplev0, gpeds0, i;

  
//   for(;;){

//     gplev0 = get32(EOS_GPLEV0);

//             if (getBit(gplev0, Button) == 1)
//             {
//               gpclr0 = get32(GPCLR0); 
//               put32(get32(GPCLR0, 0x00000000 | BIT(bit1));
//               HOLD_1S;
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | /*BIT(bit1)+*/BIT(bit2)+BIT(bit3)+BIT(bit4)+BIT(bit5)+BIT(bit6)+BIT(bit7)+BIT(bit8));
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//               gpclr0 = get32(GPCLR0); 
//               put32(GPCLR0, 0x00000000 | BIT(bit2));
//               HOLD_1S;
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | BIT(bit1)/*+BIT(bit2)*/+BIT(bit3)+BIT(bit4)+BIT(bit5)+BIT(bit6)+BIT(bit7)+BIT(bit8));
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;

//             }

//             else if (getBit(gplev0, Button) == 1)
//             {
//               gpclr0 = get32(GPCLR0); 
//               put32(GPCLR0, 0x00000000 | BIT(bit3));
//               HOLD_1S;
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | BIT(bit1)+BIT(bit2)+/*BIT(bit3)+*/BIT(bit4)+BIT(bit5)+BIT(bit6)+BIT(bit7)+BIT(bit8));
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//               gpclr0 = get32(GPCLR0); 
//               put32(GPCLR0, 0x00000000 | BIT(bit4));
//               HOLD_1S;
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | BIT(bit1)+BIT(bit2)+BIT(bit3)/*+BIT(bit4)*/+BIT(bit5)+BIT(bit6)+BIT(bit7)+BIT(bit8));
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//             }

//             else if (getBit(gplev0, 10) == 1)
//             {
//               gpclr0 = get32(GPCLR0); 
//               put32(GPCLR0, 0x00000000 | BIT(bit5));
//               HOLD_1S;
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | BIT(bit1)+BIT(bit2)+BIT(bit3)+BIT(bit4)/*+BIT(bit5)*/+BIT(bit6)+BIT(bit7)+BIT(bit8));
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//               gpclr0 = get32(GPCLR0); 
//               put32(GPCLR0, 0x00000000 | BIT(bit6));
//               HOLD_1S;
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | BIT(bit1)+BIT(bit2)+BIT(bit3)+BIT(bit4)+BIT(bit5)/*+BIT(bit6)*/+BIT(bit7)+BIT(bit8));
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//             }

//             else if (getBit(gplev0, 22) == 1)
//             {
//               gpclr0 = get32(GPCLR0); 
//               put32(GPCLR0, 0x00000000 | BIT(bit7));
//               HOLD_1S;
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | BIT(bit1)+BIT(bit2)+BIT(bit3)+BIT(bit4)+BIT(bit5)+BIT(bit6)+/*BIT(bit7)+*/BIT(bit8));
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//               gpclr0 = get32(GPCLR0); 
//               put32(GPCLR0, 0x00000000 | BIT(bit8));
//               HOLD_1S;
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | BIT(bit1)+BIT(bit2)+BIT(bit3)+BIT(bit4)+BIT(bit5)+BIT(bit6)+BIT(bit7)/*+BIT(bit8)*/);
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//               HOLD_1S;
//             }
//             else
//               gpset0 = get32(GPSET0);
//               put32(GPSET0, 0x00000000 | 0xFFFFFFFFF);
      
//   }
// }

void putcs(char c) {
  while(get32(EOS_UART0_FR) & BIT(5));
  put32(EOS_UART0_DR, c);
}

char getcs() {
  while(get32(EOS_UART0_FR) & BIT(4));
  UI32 data = get32(EOS_UART0_DR);

  return data;
}

void clearsScreen() {
  // Use VT100 commands to clear screen and move cursor to position 0, 0
  putcs((char)EOS_ASCII_ESC);
  prints(EOS_TERM_CLS);
  putcs((char)EOS_ASCII_ESC);
  prints(EOS_TERM_HOME);
}

void prints(char* str) {
  for (UI32 i = (UI32)0; str[i] != '\0'; i ++) {
    putcs((char)str[i]);
  }
}

void printsln(char* str) {
  prints(str);
  putcs('\r');
  putcs('\n');
}