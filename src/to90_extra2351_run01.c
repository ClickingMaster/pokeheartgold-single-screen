#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 ov07_022366F5[],ov07_022366F6[],ov07_022366F7[],ov07_022366F8[],ov07_022366F9[],ov07_022366D8[],ov07_022366D9[],ov07_022366DA[];
extern void ov07_02222590(void*,int,int,int,int,int,int),ov07_02222268(void*,int,int,s16,s16,int),Pokepic_SetAttr(void*,int,int),ov07_022226C4(void*,int,int,int,int),ov07_0221C448(void*,void*),Heap_Free(void*);extern int ov07_0222260C(void*),ov07_022222B4(void*);
void ov07_02228510(void*,u8*);
#pragma opt_common_subs off
void ov07_02228510(void*task,u8*w){switch(w[0]){case 0:{int off=w[1]*5;ov07_02222590(w+16,ov07_022366F5[off],ov07_022366F6[off],ov07_022366F7[off],ov07_022366F8[off],100,ov07_022366F9[off]);off=w[1]*3;{int x=*(s16*)(w+4);ov07_02222268(w+52,0,0,(s16)(x+ov07_022366D8[off]),(s16)(x+ov07_022366D9[off]),ov07_022366DA[off]);}w[1]++;w[0]++;break;}case 1:{int done=0;if(!ov07_0222260C(w+16))done++;if(!ov07_022222B4(w+52))done++;if(done>=2){if(w[1]>=3){w[2]++;w[1]=0;if(w[2]>=3)w[0]++;}else w[0]=0;}Pokepic_SetAttr(*(void**)(w+12),12,*(s16*)(w+16));Pokepic_SetAttr(*(void**)(w+12),13,*(s16*)(w+18));ov07_022226C4(*(void**)(w+12),*(s16*)(w+54),*(s16*)(w+6),*(int*)(w+36),0);break;}default:ov07_0221C448(*(void**)(w+8),task);Heap_Free(w);break;}}
