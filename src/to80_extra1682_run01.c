#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u32 pad:6;u32 axis:2;u32 tail:24; } Header;
typedef struct { Header *header; } Res;
typedef struct {u8 pad0[0x20];Res *res;u8 pad24[0x2c];VecFx16 axis;u8 pad56[0x2e];VecFx16 cross,other;} Work;
extern const VecFx16 gSplAxisY;
extern fx32 VEC_Fx16DotProduct(const VecFx16 *,const VecFx16 *);
extern void VEC_Fx16Normalize(const VecFx16 *,VecFx16 *);
void spl_set_cross_to_axis(Work *);
void spl_set_cross_to_axis(Work *w) {
 VecFx16 base=gSplAxisY,axis; fx32 d;
 switch(w->res->header->axis) {
 case 2:axis.x=FX32_ONE;axis.y=0;axis.z=0;break;
 case 1:axis.x=0;axis.y=FX32_ONE;axis.z=0;break;
 case 0:axis.x=0;axis.y=0;axis.z=FX32_ONE;break;
 default:VEC_Fx16Normalize(&w->axis,&axis);break;
 }
 d=VEC_Fx16DotProduct(&base,&axis);
 if(d==FX32_ONE||d==-FX32_ONE){base.x=FX32_ONE;base.y=0;base.z=0;}
 w->cross.x=FX_Mul(axis.y,base.z)-FX_Mul(axis.z,base.y);
 w->cross.y=FX_Mul(axis.z,base.x)-FX_Mul(axis.x,base.z);
 w->cross.z=FX_Mul(axis.x,base.y)-FX_Mul(axis.y,base.x);
 w->other.x=FX_Mul(axis.y,w->cross.z)-FX_Mul(axis.z,w->cross.y);
 w->other.y=FX_Mul(axis.z,w->cross.x)-FX_Mul(axis.x,w->cross.z);
 w->other.z=FX_Mul(axis.x,w->cross.y)-FX_Mul(axis.y,w->cross.x);
 VEC_Fx16Normalize(&w->cross,&w->cross);VEC_Fx16Normalize(&w->other,&w->other);
}
