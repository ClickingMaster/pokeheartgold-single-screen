#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "options.h"
#include "unk_02030A98.h"
#pragma force_active on
typedef struct {u8 pad[4];void *data;u8 pad2;u8 mode,result;u8 pad3[4];union {u8 flags;struct {u8 lower:3,updates:5;};};u8 pad4[5];u8 threshold;u8 pad5[14];void *format;u8 pad6[0x28];u8 work[0x70];u8 window[0x1f8];void *options,*save;u8 pad7[0x2f7];u8 counts[3];u16 points;} Work;
typedef char WorkPrefixExtent[(sizeof(Work)==0x5bc)?1:-1];
typedef char CountsOffset[(offsetof(Work,counts)==0x5b7)?1:-1];
typedef char PointsOffset[(offsetof(Work,points)==0x5ba)?1:-1];
extern int ov80_02237B24(int,int),ov80_02237D8C(int),Comm_GetLocalNetId(void),ov83_022448AC(Work *,int,int);
extern u8 ov83_0224776C(u8,u8),ov83_0224777C(void *,int,int);
extern void ov83_02244ABC(Work *,int),ov80_02237FA4(void *,int,int),ov83_022477C4(u32,int),ov83_02245390(Work *),ov83_022453DC(Work *,void *);
extern void ov83_02247944(Window *,int);
extern u32 sub_0205C174(u32,u32),sub_0205C268(u32);
extern const u16 ov83_02248054[][3];
void ov83_02245ACC(void *,u8,int);
void ov83_02245ACC(void *owner,u8 index,int kind){Work *w=owner;
int category;u32 threshold;ov80_02237B24(w->mode,0);if(kind==5)category=2;else {GF_AssertFail();}threshold=w->threshold;ov83_0224776C(threshold,index);
if(Comm_GetLocalNetId()==0){if(index<threshold){FrontierSave *save;int stat;ov83_02244ABC(w,5);ov83_0224777C(w->save,w->mode,category);ov80_02237FA4(w->data,w->mode,50);index=ov83_0224777C(w->save,w->mode,category);save=Save_Frontier_GetStatic(w->save);stat=sub_0205C174(w->mode,category);sub_02031108(save,stat,sub_0205C268(sub_0205C174(w->mode,category)),(u16)(index+1));if(ov80_02237D8C(w->mode)==1)w->updates=2;
}else{ov83_022477C4((u32)w->format,5);index=w->counts[category];w->points-=50;w->counts[category]++;}}
else{if(index<threshold){ov83_022477C4((u32)w->format,5);index=w->counts[category];w->points-=50;w->counts[category]++;}
else{FrontierSave *save;int stat;ov83_02244ABC(w,5);ov83_0224777C(w->save,w->mode,category);ov80_02237FA4(w->data,w->mode,50);index=ov83_0224777C(w->save,w->mode,category);save=Save_Frontier_GetStatic(w->save);stat=sub_0205C174(w->mode,category);sub_02031108(save,stat,sub_0205C268(sub_0205C174(w->mode,category)),(u16)(index+1));if(ov80_02237D8C(w->mode)==1)w->updates=2;}}
ov83_02245390(w);ov83_022453DC(w,w->work);ov83_02247944((Window *)w->window,Options_GetFrame(w->options));w->result=ov83_022448AC(w,ov83_02248054[category][index],1);
}
