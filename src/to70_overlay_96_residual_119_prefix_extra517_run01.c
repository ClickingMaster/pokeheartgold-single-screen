#pragma force_active on
#include "global.h"
extern u16 CalcAngleBetweenVecs(VecFx32 *,VecFx32 *);
extern VecFx32 ov96_0221D6B8;
typedef struct {u8 pad[0x14];int kind;u8 pad2[0x48];u32 unused:26;u32 flag:1;u32 unused2:5;} Work;
int ov96_022186CC(int *);BOOL ov96_02218744(Work *);
int ov96_022186CC(int *p){VecFx32 a=ov96_0221D6B8;VecFx32 b={0,0,0};u16 angle;b.x=p[1];b.z=p[0];b.y=0;angle=CalcAngleBetweenVecs(&a,&b);if(angle<=0x2000||angle>=0xe000)return 4;if(angle>0x2000&&angle<0x6000)return 2;if(angle>=0x6000&&angle<=0xa000)return 3;return 1;}
BOOL ov96_02218744(Work *w){GF_ASSERT(w);GF_ASSERT(w->kind);return w->kind!=10 && !w->flag && w->kind!=4 && w->kind!=3;}
