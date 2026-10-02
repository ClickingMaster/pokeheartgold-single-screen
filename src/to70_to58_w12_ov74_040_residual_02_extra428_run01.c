#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x208];void *resource;u8 pad2[8];u8 temp[16];u8 pad3[0xc70];u8 image[16];} Work;
typedef struct {u8 pad[8];u8 pokemon[1];} Adapter;
extern void ov74_02235728(int,int,int,int,int,int);extern void ov74_0223589C(int,int);extern void *ov74_02235930(int,void *,int,int,int);extern u32 GetMonData(void *,int,void *);extern void ov74_02235B14(void *,void *,u32,u32,void *,void *);
void ov74_02235C10(Work *,int,Adapter *);
void ov74_02235C10(Work *w,int type,Adapter *adapter){void *mon;ov74_02235728(52,10,0,11,12,1);ov74_0223589C(0,1<<20);w->resource=ov74_02235930(1,w->resource,128,0,1);mon=adapter->pokemon;switch(type){case 1:case 13:{u32 species=GetMonData(mon,5,0);u32 personality=GetMonData(mon,112,0);ov74_02235B14(w->resource,mon,species,personality,w->temp,w->image);break;}case 2:ov74_02235B14(w->resource,mon,0x1ee,0,w->temp,w->image);break;case 7:ov74_02235B14(w->resource,mon,0x1ee,1,w->temp,w->image);break;}}
