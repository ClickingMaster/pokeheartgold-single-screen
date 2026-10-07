#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct{void*a,*sprite;u8 p[0x25];u8 state;u8 p2[5];u8 choice;}W;
extern const u8 ov72_0223B4C8[];extern int TouchscreenHitbox_FindRectAtTouchNew(const void*);extern void PlaySE(int),Sprite_SetAnimCtrlSeq(void*,int),ov72_0223B2FC(W*,int);int ov72_0223AF7C(W*);
int ov72_0223AF7C(W*w){int hit=TouchscreenHitbox_FindRectAtTouchNew(ov72_0223B4C8);if(hit!=-1){PlaySE(1500);if(hit==0){w->choice=0;ov72_0223B2FC(w,w->choice);Sprite_SetAnimCtrlSeq(w->sprite,3);w->state=3;}else if(hit==1){w->choice=1;ov72_0223B2FC(w,w->choice);Sprite_SetAnimCtrlSeq(w->sprite,3);w->state=3;}}else{if(gSystem.newKeys&64){w->choice=0;ov72_0223B2FC(w,w->choice);PlaySE(1500);}else if(gSystem.newKeys&128){w->choice=1;ov72_0223B2FC(w,w->choice);PlaySE(1500);}else if(gSystem.newKeys&1){Sprite_SetAnimCtrlSeq(w->sprite,3);w->state=3;PlaySE(1500);}else if(gSystem.newKeys&2){w->choice=1;ov72_0223B2FC(w,w->choice);w->state=3;Sprite_SetAnimCtrlSeq(w->sprite,3);PlaySE(1500);}}return 0;}
