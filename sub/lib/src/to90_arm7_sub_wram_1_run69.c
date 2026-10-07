#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void CARD_Init(void);
void FS_Init(void);
// @fn FS_Init
void FS_Init(void){CARD_Init();}
