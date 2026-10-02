#pragma force_active on
#include "global.h"
#include "system.h"
#include "sprite.h"
#include "sound.h"
void PlaySE(u16);
#pragma force_active on
void sub_0208AB58(void *,int);int sub_0208AEC4(void *),sub_0208B0F4(void *,int),sub_0208931C(u8 *);
#define S(o) (*(Sprite **)(w+(o)))
int sub_0208931C(u8 *w){int k;
 if(gSystem.newAndRepeatedKeys & 32){sub_0208AB58(w,-1);return 12;}
 if(gSystem.newAndRepeatedKeys & 16){sub_0208AB58(w,1);return 12;}
 if(gSystem.newAndRepeatedKeys & 64){sub_0208AB58(w,-3);return 12;}
 if(gSystem.newAndRepeatedKeys &128){sub_0208AB58(w,3);return 12;}
 if(gSystem.newKeys&3){PlaySE(2368);return sub_0208B0F4(w,11);}
 k=sub_0208AEC4(w);switch(k){case -1:break;case 9:if(Sprite_GetDrawFlag(S(1284))==1){PlaySE(1500);Sprite_SetAnimCtrlSeq(S(1284),2);return 13;}break;case 10:if(Sprite_GetDrawFlag(S(1288))==1){PlaySE(1500);Sprite_SetAnimCtrlSeq(S(1288),3);return 14;}break;case 11:PlaySE(2368);return sub_0208B0F4(w,11);default:if(w[1988]!=k){w[1988]=k;PlaySE(1500);sub_0208AB58(w,0);}return 12;}return 12;}
