/* This code is to not be adjust for it is a backbone (functional) for the GPIO-driver API!
Please Do not adjust any code, however adding further tactility for low-level to high-level programming is allowed!
Copyrighted by Casper Leitner
*/
#include "eos.h"


void printBin(volatile uint32_t bits);
int getBit(int n, int bitnr);
void setFSEL(volatile UI32 *address, UI32 value, UI32 offset);
void setGPSET(volatile UI32 *address, UI32 value, UI32 offset);
void setGPCLR(volatile UI32 *address, UI32 value, UI32 offset);
void stage2(UI32 piId, UI32 exceptionLevel, UI64 sp);
void hang();
void blink();
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

  clearsScreen();
  printsln("GPIO Driver Casper , Stoplichtjes");

  blink();

  hang();
}

void hang() {
  for(;;) asm volatile ( " wfe \n "); // hang
}

void putcs(char c) {
  while(get32(EOS_UART_FR) & SELECT_BIT(5));
  put32(EOS_UART_DR, c);
}

char getcs() {
  while(get32(EOS_UART_FR) & SELECT_BIT(4));
  UI32 data = get32(EOS_UART_DR);

  return data;
}

void clearsScreen() {
  // Uses VT100 Terminal commands to clear screen and move cursor to position 0, 0
  putcs((char)EOS_ASCII_ESC);
  prints(EOS_TERM_CLS);
  putcs((char)EOS_ASCII_ESC);
  prints(EOS_TERM_HOME);
}

void prints(char* str) {
  for (UI32 i =  (UI32) 0; str[i] != '\0'; i ++) {
    putcs((char)str[i]);
  }
}

int getBit(int n, int bitnr)
{
  int mask = SELECT_BIT(bitnr);
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

void setGPCLR(volatile UI32 *address, UI32 value, UI32 offset)
{
  value <<= offset * 1;
  UI32 mask = 0b1 << offset * 1;
  UI32 temp = get32(address);
  temp &= (~mask);
  put32(address, (temp | value));
}


void printsln(char* str) {
  prints(str);
  putcs('\r');
  putcs('\n');
}