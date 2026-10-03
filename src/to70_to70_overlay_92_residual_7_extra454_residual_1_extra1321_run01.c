#pragma force_active on
#include "global.h"
extern void ov92_0225DDD8(void*,s16,int,int,int,int),ManagedSprite_SetPaletteOverrideOffset(void*,int),ManagedSprite_SetAnim(void*,int),ov92_0225DF0C(void*,int,int);extern void*SpriteSystem_NewSprite(void*,void*,void*);
void ov92_0225DE08(u8*w);
void ov92_0225DE08(u8*w){u8 template[52];u8*p=*(void**)(w+20);void*system=*(void**)(p+8);void*manager=*(void**)(p+12);int x,i;u8*cursor;i=0;cursor=w;x=i;do{ov92_0225DDD8(template,(s16)x,20,1,3,9006);*(void**)(cursor+64)=SpriteSystem_NewSprite(system,manager,template);ManagedSprite_SetPaletteOverrideOffset(*(void**)(cursor+64),3);ManagedSprite_SetAnim(*(void**)(cursor+64),i%11+1);x+=8;cursor+=4;i++;}while(i<36);ov92_0225DF0C(w,0,0);}
