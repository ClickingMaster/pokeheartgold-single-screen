#include "global.h"
#include "sprite_system.h"
#include "math_util.h"
typedef struct {ManagedSprite *sprites[3];fx32 angle;u8 middle[32];void *outputs[2];u8 tail[20];} Record;
typedef struct {Record rows[3];fx32 angle;} Group;
extern const ManagedSpriteTemplate ov93_02262E9C,ov93_02262ED0,ov93_02262F04;
extern const u16 ov93_02262C7A[];
extern void ov93_02262444(void *,void **);
void ov93_02261164(void *,void *);
#pragma force_active on
void ov93_02261164(void *work,void *groupInput){Group *group=groupInput;ManagedSpriteTemplate temp=ov93_02262E9C;int i;for(i=0;i<3;i++){Record *r=&group->rows[i];int x=128+FX_Mul(GF_SinDegFX32(group->angle+r->angle),76);int y=98-FX_Mul(GF_CosDegFX32(group->angle+r->angle),68);
r->sprites[0]=SpriteSystem_NewSprite(*(void **)((u8 *)work+36),*(void **)((u8 *)work+40),&temp);ManagedSprite_SetPositionXYWithSubscreenOffset(r->sprites[0],(s16)x,(s16)(y-24),0x160000);ManagedSprite_SetAnim(r->sprites[0],ov93_02262C7A[i+1]);Sprite_TickFrame(r->sprites[0]->sprite);ManagedSprite_SetDrawFlag(r->sprites[0],0);
r->sprites[1]=SpriteSystem_NewSprite(*(void **)((u8 *)work+36),*(void **)((u8 *)work+40),&ov93_02262ED0);ManagedSprite_SetPositionXYWithSubscreenOffset(r->sprites[1],0,0,0x160000);ManagedSprite_SetAnim(r->sprites[1],33);Sprite_TickFrame(r->sprites[1]->sprite);ManagedSprite_SetDrawFlag(r->sprites[1],0);
r->sprites[2]=SpriteSystem_NewSprite(*(void **)((u8 *)work+36),*(void **)((u8 *)work+40),&ov93_02262F04);ManagedSprite_SetPositionXYWithSubscreenOffset(r->sprites[2],(s16)x,(s16)y,0x160000);ManagedSprite_SetOamMode(r->sprites[2],GX_OAM_MODE_XLU);ManagedSprite_SetAffineOverwriteMode(r->sprites[2],1);ManagedSprite_SetAffineScale(r->sprites[2],1.0f,1.0f);ManagedSprite_SetAnim(r->sprites[2],i+34);Sprite_TickFrame(r->sprites[2]->sprite);ManagedSprite_SetDrawFlag(r->sprites[2],0);ov93_02262444(work,r->outputs);
}ManagedSprite_SetDrawFlag(group->rows[0].sprites[0],1);ManagedSprite_SetDrawFlag(group->rows[0].sprites[2],1);}
