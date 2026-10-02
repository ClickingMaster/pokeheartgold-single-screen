#pragma force_active on
#include "global.h"
extern s32 FX_Sqrt(s32);
extern u16 FX_Atan2Idx(s32,s32);
void ov07_02222DE4(s16 x,s16 y,s16 tx,s16 ty,s32 *out);
void ov07_02222E0C(s16 x,s16 y,s16 tx,s16 ty,u16 *out);
void ov07_02222DE4(s16 x,s16 y,s16 tx,s16 ty,s32 *out){s16 dy=-(y-ty),dx=x-tx;*out=FX_Sqrt((dy*dy+dx*dx)<<12);}
void ov07_02222E0C(s16 x,s16 y,s16 tx,s16 ty,u16 *out){s16 dy=-(y-ty),dx=x-tx;*out=FX_Atan2Idx(dy<<12,dx<<12);if(*out&&dy<0)*out=(*out-0x7fff)*0xffff;}
