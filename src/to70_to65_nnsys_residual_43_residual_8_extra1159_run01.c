#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 flags;u8 p[0x14];fx32 sx,sy;s16 sin,cos;fx32 tx,ty,pad,kx,ky;} Work;
extern void (*_02110C5C[])(MtxFx44*,Work*);
void NNSi_G3dSendTexSRTXsi(Work*);
void NNSi_G3dSendTexSRTXsi(Work *w){struct {u32 cmd,mode;MtxFx44 m;u32 restore;} c;
if(w->flags&8)c.cmd=0x101610;else c.cmd=0x101810;c.mode=3;c.restore=2;c.m._33=4096;c.m._32=0;c.m._23=0;c.m._22=0;c.m._21=0;c.m._20=0;c.m._13=0;c.m._12=0;c.m._03=0;c.m._02=0;
if(w->flags&1){w->sy=4096;w->sx=4096;}if(w->flags&2){w->cos=4096;w->sin=0;}if(w->flags&4){w->ty=0;w->tx=0;}_02110C5C[w->flags&7](&c.m,w);
if(w->kx!=4096){c.m._00=(s64)w->kx*c.m._00>>12;c.m._01=(s64)w->kx*c.m._01>>12;c.m._30=(s64)w->kx*c.m._30>>12;}if(w->ky!=4096){c.m._10=(s64)w->ky*c.m._10>>12;c.m._11=(s64)w->ky*c.m._11>>12;c.m._31=(s64)w->ky*c.m._31>>12;}NNS_G3dGeBufferOP_N(c.cmd,((u32*)&c)+1,18);}
