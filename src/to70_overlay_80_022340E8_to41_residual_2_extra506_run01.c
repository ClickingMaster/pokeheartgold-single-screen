#pragma force_active on
#include "global.h"
typedef struct{u32 d[14];} MonData;typedef struct{u8 a[8];void*save;u8 b[0x74-12];void*party;u16 values[14];u8 c[0x314-0x94];u16 more[4];} Work;
extern void ov80_022344D4(Work*);extern u16 sub_02030F34(void*,int,u8,int,int);extern void FrontierMon_GenerateList(MonData*,u16*,u32*,int,u32*,int,int,int);extern void*AllocMonZeroed(int);extern int ov80_02238370(Work*);extern void FrontierMon_ToPokemon(MonData*,void*,int);extern void ov80_022383A8(Work*,void*,void*);extern void Heap_Free(void*);

void ov80_02234424(Work*w);

void ov80_02234424(Work*w){MonData mons[6];u16 choices[6];u32 info[2],other[6];int i;void*mon;ov80_022344D4(w);for(i=0;i<14;i++)w->values[i]=sub_02030F34(w->save,5,(u8)i,0,0);for(i=0;i<4;i++){choices[i]=sub_02030F34(w->save,7,(u8)i,0,0);w->more[i]=choices[i];}FrontierMon_GenerateList(mons,choices,info,0,other,4,11,205);mon=AllocMonZeroed(11);for(i=0;i<4;i++){FrontierMon_ToPokemon(&mons[i],mon,ov80_02238370(w));ov80_022383A8(w,w->party,mon);}Heap_Free(mon);}
