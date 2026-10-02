#pragma force_active on
#include "global.h"
#pragma force_active on
void ManagedSprite_SetAffineOverwriteMode(void *,int);
void ManagedSprite_SetDrawFlag(void *,int);
void ManagedSprite_GetPositionXY(void *,s16 *,s16 *);
void ov07_02222268(void *,int,int,int,int,int);
void ov07_02222508(void *,int,int,int,int);
void ov07_02230F0C(void *,void *,void *,int);
void ov07_02230F0C(void *sprite,void *a,void *b,int i){
 s16 x,y;
 ManagedSprite_SetAffineOverwriteMode(sprite,2);ManagedSprite_SetDrawFlag(sprite,1);
 ManagedSprite_GetPositionXY(sprite,&x,&y);
 ov07_02222268(a,x,(s16)(x+i*16),y,(s16)(y-32),32);
 ov07_02222508(b,2,10,10,32);
}
