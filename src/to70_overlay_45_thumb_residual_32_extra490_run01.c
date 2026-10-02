#pragma force_active on
#include "global.h"
#include "heap.h"
#include <string.h>
typedef struct {u32 a[5];} Params;
typedef struct {void *save,*first,*history;Params params;u32 mode;u8 pad[0x6a0-0x24];u8 area[0x984-0x6a0];int state;} Work;
extern Work *_022577C0;extern void *sub_0202C6F4(void *);extern void *Save_WiFiHistory_Get(void *);extern void ov45_0222F848(Work *,u32,u32);extern void ov45_0222F9B8(Work *,int,int,u32);extern void ov45_02230144(Work *);extern void ov45_022303E4(void *,int,u32);
void ov45_0222E5D4(u32,void *,u32,Params *,u32);
void ov45_0222E5D4(u32 heap,void *save,u32 count,Params *params,u32 mode){GF_ASSERT(_022577C0==NULL);GF_ASSERT(count<300);_022577C0=Heap_Alloc((enum HeapID)heap,0x988);memset(_022577C0,0,0x988);_022577C0->save=save;_022577C0->first=sub_0202C6F4(save);_022577C0->history=Save_WiFiHistory_Get(save);_022577C0->params=*params;_022577C0->mode=mode;ov45_0222F848(_022577C0,count,heap);ov45_0222F9B8(_022577C0,20,8,heap);ov45_02230144(_022577C0);ov45_022303E4(_022577C0->area,128,heap);_022577C0->state=0;}
