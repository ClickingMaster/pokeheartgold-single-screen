#pragma force_active on
#include "global.h"
extern int Sprite_GetAnimationFrame(void *);
extern void Sprite_SetAnimActiveFlag(void *,int);
extern int Sprite_GetAnimActiveFlag(void *);
extern void Sprite_SetAnimationFrame(void *,int);
void ov34_0225E630(void *arg);
void ov34_0225E630(void *arg){u8 *sprites=arg,*flags=arg;int i;for(i=0;i<2;i++){Sprite_GetAnimationFrame(*(void**)(sprites+0x198));if(*(u16*)(flags+0x2a0)==1){Sprite_SetAnimActiveFlag(*(void**)(sprites+0x198),0);*(u16*)(flags+0x2a0)=0;}else if(!Sprite_GetAnimActiveFlag(*(void**)(sprites+0x198))){Sprite_SetAnimActiveFlag(*(void**)(sprites+0x198),1);Sprite_SetAnimationFrame(*(void**)(sprites+0x198),1);}sprites+=4;flags+=2;}}
