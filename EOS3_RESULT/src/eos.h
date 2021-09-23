#ifndef EOS_H
#define EOS_H

#include <stdint.h>

typedef       int32_t  I32;
typedef      uint32_t UI32;
typedef       int64_t  I64;
typedef      uint64_t UI64;
typedef unsigned char   UC;

//GPIO Functional bitfield
#define INPUT                             (UI32)  0b000
#define OUTPUT                            (UI32)  0b001
#define ALT0                              (UI32)  0b100
#define ALT1                              (UI32)  0b101
#define ALT2                              (UI32)  0b110
#define ALT3                              (UI32)  0b111
#define ALT4                              (UI32)  0b011
#define ALT5                              (UI32)  0b010

#define BUSY_WAIT asm volatile("")

#define put8( reg, data) (*((UC   *)(reg)) = ((UC)(data)))
#define get8( reg)       (*((UC   *)(reg)))
#define put32(reg, data) (*((UI32 *)(reg)) = ((UI32)(data)))
#define get32(reg)       (*((UI32 *)(reg)))
#define put64(reg, data) (*((UI64 *)(reg)) = ((UI64)(data)))
#define get64(reg)       (*((UI64 *)(reg)))

#define BIT(x)           (1 << (x))
#define HOLD_1S                     for (i = 0; i < 2000000; ++i) { BUSY_WAIT; }

#endif