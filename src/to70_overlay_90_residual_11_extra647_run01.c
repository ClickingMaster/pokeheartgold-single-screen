#pragma force_active on
#include "global.h"
typedef struct {u32 count,a,b,heap;} Params;
typedef struct {void*list;u8 renderer[1];} Work;
extern Params ov90_0225C294;extern void NNS_G2dInitOamManagerModule(void);extern void OamManager_Create(int,int,int,int,int,int,int,int,int);extern void ObjCharTransfer_InitEx(Params*,int,int);extern void ObjPlttTransfer_Init(int,int);extern void ObjCharTransfer_ClearBuffers(void);extern void ObjPlttTransfer_Reset(void);extern void G2dRenderer_SetObjCharTransferReservedRegion(int,int);extern void G2dRenderer_SetPlttTransferReservedRegion(int);extern void *G2dRenderer_Init(int,void*,int);extern void WirelessStatusIcon_Create(void);extern void GfGfx_EngineATogglePlanes(int,int);extern void GfGfx_EngineBTogglePlanes(int,int);

void ov90_022590CC(Work*w,int n,int count,int pltt,int heap);

void ov90_022590CC(Work*w,int n,int count,int pltt,int heap){Params p;int h;NNS_G2dInitOamManagerModule();h=*(volatile int*)&heap;OamManager_Create(0,126,0,31,0,126,0,31,h);p=ov90_0225C294;p.count=count;p.heap=h;ObjCharTransfer_InitEx(&p,16,16);ObjPlttTransfer_Init(pltt,h);ObjCharTransfer_ClearBuffers();ObjPlttTransfer_Reset();G2dRenderer_SetObjCharTransferReservedRegion(1,16);G2dRenderer_SetPlttTransferReservedRegion(1);w->list=G2dRenderer_Init(n,w->renderer,h);WirelessStatusIcon_Create();GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);}
