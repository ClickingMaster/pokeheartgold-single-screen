#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u16 ov93_02262CEC[],ov93_02262CEE[];typedef struct {u32 v[3];} Row;extern Row ov93_02262FD4[];
extern void *NARC_New(int,int);extern void NARC_Delete(void*);extern void GfGfxLoader_LoadCharDataFromOpenNarc(void*,int,void*,int,int,int,int,int);extern void GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,int);extern void ov93_0225FCA4(void*,void*,int);extern void PlaySE(int);
typedef struct {u32 state,selected,x,data;u8 p[0x2c-0x10];u8 flag;} Work;static inline u16 scrnFile(int selected){return ov93_02262CEE[selected*4];}
void ov93_0225FBF0(void*,int,int,Work*);
void ov93_0225FBF0(void*bg,int kind,int selected,Work*w){void*narc=NARC_New(201,117);GfGfxLoader_LoadCharDataFromOpenNarc(narc,ov93_02262CEC[selected*4],bg,7,0,0,0,117);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,scrnFile(selected),bg,7,0,0,0,117);NARC_Delete(narc);MI_CpuFill8(w,0,48);w->data=ov93_02262FD4[kind].v[selected];w->selected=selected;w->state=1;w->flag=1;ov93_0225FCA4(bg,w,kind);PlaySE(1515);}
