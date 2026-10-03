#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad[0x600];u8 pad2[0x4a];u16 value;u8 pad3[0x444];u8 state;u8 pad4[7];u32 counter;} State;
extern State *ov13_0224F5C8;
u32 ov13_0223DF28(void);
void ov13_0223DF3C(void);
u16 ov13_0223E1A8(void);
void ov13_0223E698(void);
void ov13_0223EEAC(void);
void ov13_0223EEAC(void){switch(ov13_0223DF28()){case 1:ov13_0223DF3C();break;case 3:break;case 7:ov13_0224F5C8->value=ov13_0223E1A8();ov13_0224F5C8->counter=0;ov13_0224F5C8->state=3;break;case 0:ov13_0224F5C8->counter=0;ov13_0224F5C8->state=3;break;case 9:ov13_0223E698();break;default:OS_Terminate();break;}}
