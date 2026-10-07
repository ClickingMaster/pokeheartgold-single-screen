#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov18_021F3494(void*),ManagedSprite_SetPositionXY(void*,int,int),ov18_021F11C0(void*,int,int);
void ov18_021F34EC(u8*,int);
void ov18_021F34EC(u8 *s,int mode){u32 i;ov18_021F3494(s);if(mode==1){ManagedSprite_SetPositionXY(*(void**)(s+0x6e0),224,72);for(i=29;i<=42;i++)ov18_021F11C0(s,i,0);}else{ManagedSprite_SetPositionXY(*(void**)(s+0x6e0),152,20);for(i=29;i<=42;i++)ov18_021F11C0(s,i,1);}}
