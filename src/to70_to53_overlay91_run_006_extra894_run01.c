#pragma force_active on
#include "global.h"
extern int ov90_0225886C(void *,int);
extern void ov91_0225D3B4(void *,int,int);
extern void Comm_SendCommand(int,int,int);
typedef struct{u8 p[8];u8 data[0x2c];void *obj;u8 p2[12];u16 flag;u8 p3[0x2e];u32 count,unknown,level;u8 p4[4];u8 used[4];} Work;
void ov91_0225CAEC(Work *w,int arg,int value);
void ov91_0225CAEC(Work *w,int arg,int value){int index=ov90_0225886C(w->data,value);u32 i;int all;ov91_0225D3B4(w->obj,arg,index);w->used[index]++;if(w->flag==0){all=1;for(i=0;i<w->count;i++){if(w->level>w->used[i])all=0;}if(all==1){w->level++;Comm_SendCommand(29,0,0);}}}
