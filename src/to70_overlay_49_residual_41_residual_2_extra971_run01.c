#pragma force_active on
#include "global.h"
typedef struct {u16 pad,index;u8 pad2[0x78];u8 obj[3][20];u8 pad3[4];u8 modes[3];u8 pad4;int values[3];} Entry;
typedef struct {u8 pad[0x3bc];void*items[18][3];} Work;extern void ModelAnimation_SetFrame(void*,int);

void ov49_0225D3F8(Work*w,Entry*e,int idx,int value);

void ov49_0225D3F8(Work*w,Entry*e,int idx,int value){if(idx>=3)GF_AssertFail();if(e->index>=18)GF_AssertFail();if(w->items[e->index][idx]==0)GF_AssertFail();if(e->modes[idx]==2){e->values[idx]=value;ModelAnimation_SetFrame(e->obj[idx],e->values[idx]);}}
