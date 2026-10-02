#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct W{u8 pad[16];void*sprite;u8 gap[5];u8 choice;}W;extern const s16 ov95_021E7818[2][2];extern const u8 ov95_021E7820[];extern void ManagedSprite_SetPositionXY(void*,int,int),ManagedSprite_SetAnim(void*,int),PlaySE(int);extern int TouchscreenHitbox_FindRectAtTouchNew(const void*);void ov95_021E7410(W*);int ov95_021E7450(W*);
void ov95_021E7410(W*w){int index;if(!w->sprite)GF_AssertFail();switch(w->choice){case 1:index=0;break;case 2:index=1;break;default:GF_AssertFail();break;}ManagedSprite_SetPositionXY(w->sprite,ov95_021E7818[index][0],ov95_021E7818[index][1]);}
int ov95_021E7450(W*w){int changed=0;int hit=TouchscreenHitbox_FindRectAtTouchNew(ov95_021E7820);if(hit!=-1){PlaySE(1500);switch(hit){case 0:w->choice=1;changed=1;break;case 1:w->choice=2;changed=1;break;default:GF_AssertFail();break;}}else{u32 keys=gSystem.newKeys;if(keys&64){if(w->choice!=1){w->choice=1;ov95_021E7410(w);PlaySE(1500);}}else if(keys&128){if(w->choice!=2){w->choice=2;ov95_021E7410(w);PlaySE(1500);}}else if(keys&1){if(w->choice==0){GF_AssertFail();w->choice=2;}changed=1;}else if(keys&2){w->choice=2;changed=1;}}if(changed){ov95_021E7410(w);ManagedSprite_SetAnim(w->sprite,3);PlaySE(1500);}return changed;}
