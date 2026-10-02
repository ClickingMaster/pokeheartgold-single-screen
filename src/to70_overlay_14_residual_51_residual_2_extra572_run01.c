#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x14];void*raw;}Image;
typedef struct {u8 pad[0x458];u8 images[30][512];u8 q[0x4058-0x458-30*512];u8 active[30];u8 palettes[30];}Detail;
typedef struct {u32 a;void*storage;u8 pad[0x34-8];Detail*detail;}Work;
extern int PCStorage_GetMonDataByIndexPair(void*,int,int,int,void*),Boxmon_GetIconPalette(void*);
extern void*ov14_021E60C0(Work*,int,int),*ov14_021F2DC4(Detail*,void*,Image**);
extern void Heap_Free(void*);
void ov14_021F2DE8(Work*,int);
void ov14_021F2DE8(Work*w,int box){u32 i;Image*img;void*allocation,*mon;for(i=0;i<30;i++){if(!PCStorage_GetMonDataByIndexPair(w->storage,box,i,172,0))w->detail->active[i]=0;else{mon=ov14_021E60C0(w,box,i);allocation=ov14_021F2DC4(w->detail,mon,&img);MIi_CpuCopy32((const u32*)img->raw,(u32*)w->detail->images[i],512);Heap_Free(allocation);w->detail->palettes[i]=Boxmon_GetIconPalette(mon);w->detail->active[i]=1;}}}
