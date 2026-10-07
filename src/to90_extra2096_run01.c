#pragma force_active on
#include "global.h"
extern void FillWindowPixelBuffer(void *,int),ScheduleWindowCopyToVram(void *),ov08_0221DDCC(void *,int,int,int,int,int),ov08_0221E244(void *,int,int,u32,int,int,int),ov08_0221F284(void *,void *,int),ov08_0221E3A4(void *,int,int);
extern const u32 ov08_02224FE0[];
void ov08_0221F7C0(void *);
#pragma force_active on
void ov08_0221F7C0(void *argument){
 u8 *work=argument;u16 base=work[0x2075]*6,i;int w1,w2,w3,w4;u8 *records;
 FillWindowPixelBuffer((*(u8 **)(work+0x2070))+base*16,0);w1=(base+1)*16;FillWindowPixelBuffer((*(u8 **)(work+0x2070))+w1,0);w2=(base+2)*16;FillWindowPixelBuffer((*(u8 **)(work+0x2070))+w2,0);w3=(base+3)*16;FillWindowPixelBuffer((*(u8 **)(work+0x2070))+w3,0);w4=(base+4)*16;FillWindowPixelBuffer((*(u8 **)(work+0x2070))+w4,0);FillWindowPixelBuffer((*(u8 **)(work+0x2070))+0x50,0);
 ov08_0221DDCC(work,base,0,(*(u8 **)work)[0x11],0,0);
 records=work;records+=0x34;for(i=0;i<4;i++){u8 *row=records+(*(u8 **)work)[0x11]*80,*slot=row+i*8;if(*(u16 *)slot){ov08_0221E244(work,*(u16 *)slot,base+1+i,ov08_02224FE0[i],4,7,0x70809);ov08_0221F284(work,slot,base+1+i);}}
 ov08_0221E3A4(work,5,18);ScheduleWindowCopyToVram((*(u8 **)(work+0x2070))+w1);ScheduleWindowCopyToVram((*(u8 **)(work+0x2070))+w2);ScheduleWindowCopyToVram((*(u8 **)(work+0x2070))+w3);ScheduleWindowCopyToVram((*(u8 **)(work+0x2070))+w4);work[0x2075]^=1;
}
