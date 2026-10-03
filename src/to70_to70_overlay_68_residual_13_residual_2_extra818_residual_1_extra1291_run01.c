#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x11c];void*a,*b,*sprites[12];} Work;
extern u8 ov68_021E7E74[][52];void ov68_021E6DDC(Work*,int);void ov68_021E6EB8(Work*,int);void *SpriteSystem_NewSprite(void*,void*,void*);void ManagedSprite_SetAnimateFlag(void*,int);void ov68_021E7178(Work*,int);
void ov68_021E7178(Work*w,int a){u32 i;ov68_021E6DDC(w,a);ov68_021E6EB8(w,a);for(i=0;i<12;i++){w->sprites[i]=SpriteSystem_NewSprite(w->a,w->b,ov68_021E7E74[i]);ManagedSprite_SetAnimateFlag(w->sprites[i],0);}}
