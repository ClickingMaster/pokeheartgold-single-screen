#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma dollar_identifiers on
extern volatile u32 skipCheck$3668[],nextCount$3668[];
extern int CARD_CompareCardID(void),CARD_IsCardIreqLo(void);
int CARD_IsPulledOut(void);
// @fn CARD_IsPulledOut
int CARD_IsPulledOut(void){if(!skipCheck$3668[3]){if(*(volatile u8*)0x027FFE1F&128)CARD_CompareCardID();else CARD_IsCardIreqLo();}return skipCheck$3668[3];}
