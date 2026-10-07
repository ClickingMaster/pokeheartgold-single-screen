#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma dollar_identifiers on
extern volatile u32 skipCheck$3668[],nextCount$3668[];
int CARD_IsCardIreqLo(void);
// @fn CARD_IsCardIreqLo
int CARD_IsCardIreqLo(void){int low=1;if(*(volatile u32*)0x04000214&0x100000){low=0;skipCheck$3668[3]=1;}return low;}
