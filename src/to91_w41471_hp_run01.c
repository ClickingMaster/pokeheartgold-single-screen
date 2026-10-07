#pragma force_active on
#include "global.h"
#include "unk_0208805C.h"
#pragma force_active on
u32 ov05_0221E0A8(u8*,u32);
static inline u16 Hp(u8 *s,u32 i){return *(u16*)((u8*)s+0x21a+i*24);}
static inline u16 MaxHp(u8 *s,u32 i){return *(u16*)((u8*)s+0x21c+i*24);}
static inline u32 Status(u8 *s,u32 i){return *(u32*)((u8*)s+0x228+i*24);}
u32 ov05_0221E0A8(u8*s,u32 i){u16 hp=Hp(s,i);if(!hp)return 0;if(Status(s,i)!=7 && Status(s,i)!=0)return 5;switch(CalculateHpBarColor(hp,MaxHp(s,i),48)){case 4:return 1;case 3:return 2;case 2:return 3;case 1:return 4;default:return 0;}}
