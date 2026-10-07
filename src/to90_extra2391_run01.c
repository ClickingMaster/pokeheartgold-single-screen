#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

void*OverlayManager_GetData(void*);void ov83_022412A0(void*),ov83_0224753C(void*),ov83_02247944(void*,int),ov80_0222A7CC(void*,int),ov83_02240DA8(void*,int*,int),ov83_02241B30(void*),SpriteList_RenderAndAnimateSprites(void*);int Options_GetFrame(void*),ov83_0223FD14(void*,int,int),ov83_0223E008(void*),ov83_0223E14C(void*),ov80_02237D8C(int),ov83_0223EEA0(void*),ov83_0223EFA4(void*),ov83_0223F010(void*);int ov83_0223DE60(void*,int*);
typedef struct {u8 zero:1;u8 one:1;u8 rest:6;} Bits;
int ov83_0223DE60(void*m,int*state){void*s=OverlayManager_GetData(m);if(B(s,0x7fe)==1){if(*state==1){B(s,0x7fe)=0;ov83_022412A0(s);if(P(s,0x75c)){ov83_0224753C(P(s,0x75c));B(s,0xe)&=~4;}ov83_02247944((u8*)s+0xb0,Options_GetFrame(P(s,0x508)));ov80_0222A7CC(P(s,0x24),0);B(s,0xa)=ov83_0223FD14(s,8,1);ov83_02240DA8(s,state,3);}}else if(B(s,0x12)!=255&&(*state==1||*state==3)){B(s,0x7fe)=0;ov83_022412A0(s);ov83_02240DA8(s,state,2);}switch(*state){case 0:if(ov83_0223E008(s)==1)ov83_02240DA8(s,state,1);break;case 1:if(ov83_0223E14C(s)==1){if(((Bits*)((u8*)s+0xe))->one==1)ov83_02240DA8(s,state,2);else if(ov80_02237D8C(B(s,9))==1)ov83_02240DA8(s,state,3);else ov83_02240DA8(s,state,4);}break;case 2:if(ov83_0223EEA0(s)==1)ov83_02240DA8(s,state,1);break;case 3:if(ov83_0223EFA4(s)==1)ov83_02240DA8(s,state,4);break;case 4:if(ov83_0223F010(s)==1)return 1;break;}ov83_02241B30(s);SpriteList_RenderAndAnimateSprites(P(s,0x518));return 0;}
