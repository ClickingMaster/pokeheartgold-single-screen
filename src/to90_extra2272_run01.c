#pragma force_active on
#include "global.h"
extern void ov08_022235D4(void *,int,int,int,int,int),ov08_02223678(void *,int,int,int,int,int,int);
void ov08_0222370C(void *,int);
#pragma force_active on
void ov08_0222370C(void *argument,int index){
 u8 *work=argument;int base=(*(u8 **)work+work[0x114d])[0x2c]*6;int shift;if(work[0x31]==0)shift=0;else shift=12;
 ov08_022235D4(work,index+base,index,shift+index*2,4,0x30201);ov08_02223678(work,index+base,index,shift+1+index*2,0,4,0x10200);
}
