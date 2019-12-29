#ifndef CPU_COMMON_H
#define CPU_COMMON_H

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// @TODO: Move to arch/cpu.h . This file shall be needed again when qlib is
//        ported to another arch.
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

class CPU_Common
{
 public:
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: Check if integer types are really architecture independent.
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    typedef unsigned long Reg;
    typedef unsigned char Reg8;
    typedef unsigned short Reg16;
    typedef unsigned long Reg32;
    typedef unsigned long long Reg64;
    typedef unsigned long LogicalAddr;
};

#endif  // CPU_COMMON_H