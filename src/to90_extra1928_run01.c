#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 a,b;} Ov48Pair;
typedef struct{u8 p[0x144];int value;} Config;
typedef struct{u8 p[0xc004];int sx,sy,sz;u8 sprites[3][0x78];u8 params[3][16];} Work;
extern int ov48_0225B178[];
extern void ov48_02258A80(Work *,int),sub_02018030(void *,int,int,int),sub_020181B0(void *,void *);
void ov48_022598EC(Work *,int,Config *,Ov48Pair,u32);
typedef struct{u32 parity:1,rest:31;} ModeBits;
void ov48_022598EC(Work*w,int arg,Config*c,Ov48Pair pair,volatile u32 layer){int i;layer=layer;ov48_02258A80(w,arg);for(i=0;i<3;i++){sub_02018030(w->params[i],c->value,ov48_0225B178[i],layer);sub_020181B0(w->sprites[i],w->params[i]);}if(((ModeBits*)&pair.a)->parity==0){w->sx=768;w->sy=768;w->sz=4096;}else{w->sx=4096;w->sy=4096;w->sz=4096;}}
