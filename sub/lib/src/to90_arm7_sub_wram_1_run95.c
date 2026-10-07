#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma dollar_identifiers on
extern volatile u32 skipCheck$3668[],nextCount$3668[];
extern int OS_GetLockID(void),OS_TryLockCard(u16);extern void OS_UnlockCard(u16),OS_ReleaseLockID(u16);extern u32 CARDi_ReadRomID(void);
int CARD_CompareCardID(void);
// @fn CARD_CompareCardID
int CARD_CompareCardID(void){int equal=1;int lock=OS_GetLockID();if(lock!=-3){if(OS_TryLockCard((u16)lock)==0){volatile u32 *address;volatile u32 id;u32 actual;if(*(volatile u16*)0x027FFC10==0)address=(void*)0x027FF800;else address=(void*)0x027FFC00;id=*address;actual=CARDi_ReadRomID();equal=(actual==id);OS_UnlockCard((u16)lock);}OS_ReleaseLockID((u16)lock);}skipCheck$3668[3]=!equal;return equal;}
