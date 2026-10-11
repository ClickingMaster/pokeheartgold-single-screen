#pragma force_active on
#include "global.h"
#pragma force_active on
void BgCellManager_UpdateCellMove(void*,int),ov14_021F29E4(void*,int,int),ManagedSprite_GetPositionXY(void*,s16*,s16*),ManagedSprite_SetPositionXY(void*,s16,s16);int ov14_021EA254(u8*);
#define H(w,o) (*(u16*)((w)+(o)))
#define P(w,o) (*(void**)((w)+(o)))
int ov14_021EA254(u8*w){s16 x,y;u8*state=P(w,0x34);BgCellManager_UpdateCellMove(P(state,0x2f0),15);switch(H(state,16)){case 0:ov14_021F29E4(P(w,0x34),9,9);H(state,16)++;case 1:if(H(state,18)==4){H(state,18)=0;H(state,16)++;}else{ManagedSprite_GetPositionXY(P((u8*)P(w,0x34),0x320),&x,&y);ManagedSprite_SetPositionXY(P((u8*)P(w,0x34),0x320),x,(s16)(y+2));H(state,18)++;}break;case 2:ov14_021F29E4(P(w,0x34),9,10);H(state,16)++;case 3:if(H(state,18)==4){H(state,18)=0;H(state,16)=0;return 0;}else{int index;ManagedSprite_GetPositionXY(P((u8*)P(w,0x34),0x320),&x,&y);ManagedSprite_SetPositionXY(P((u8*)P(w,0x34),0x320),x,(s16)(y-2));index=(((u8*)P(w,0x34))+w[0x21])[0x4094];ManagedSprite_GetPositionXY(P((u8*)P(w,0x34)+index*4,0x2fc),&x,&y);ManagedSprite_SetPositionXY(P((u8*)P(w,0x34)+index*4,0x2fc),x,(s16)(y-2));H(state,18)++;}break;}return 1;}
