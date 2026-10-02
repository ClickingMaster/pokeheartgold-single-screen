#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x14];void *data;} CharData;
typedef struct {void *obj;u8 pad[0x14];u16 big;} Work;
extern CharData *GF2DGfxResObj_GetCharDataPtr(void *);extern u8 ov96_021E9180(void *,int);
u8 ov96_021E9104(Work *);
u8 ov96_021E9104(Work *w){CharData *data=GF2DGfxResObj_GetCharDataPtr(w->obj);int size=w->big?8:4;u8 result=255;int y,base;void *pixels=data->data;y=0;if(size>0){base=0;do{u8 min=255;int x=0;for(;x<size;x++){u8 value=ov96_021E9180(pixels,(x+base)*32);if(value!=255&&min>value)min=value;}if(min!=255){result=(u8)(min+y*8);break;}base+=size;y++;}while(y<size);}GF_ASSERT(result!=255);return result;}
