#pragma force_active on
#include "global.h"
#include "sprite.h"
#pragma force_active on
extern void ov70_02238B54(SpriteTemplate*,void*,void*,int),sub_0203A880(void);
void ov70_0223A06C(u8*);
void ov70_0223A06C(u8 *s){SpriteTemplate tmpl;ov70_02238B54(&tmpl,s,s+0xd60,1);*(int*)((u8*)&tmpl+8)=0xd0000;*(int*)((u8*)&tmpl+12)=0x3a000;*(Sprite**)(s+0xee0)=Sprite_CreateAffine(&tmpl);Sprite_SetAnimActiveFlag(*(Sprite**)(s+0xee0),1);Sprite_SetAnimCtrlSeq(*(Sprite**)(s+0xee0),37);Sprite_SetPriority(*(Sprite**)(s+0xee0),1);sub_0203A880();}
