#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 yes;u8 pad[7];}Item;typedef struct{u8 pad[16];u8*data;int selected;u8 gap[0x458];Item items[7];}Work;
int ov27_0225C1AC(Work*w,int index);
void ov27_0225C1EC(Work*w);
int ov27_0225C1AC(Work*w,int index){int i,result,count;result=-1;count=0;i=count;u8*p=(u8*)w;for(;i<7;i++,p+=8){if(p[0x470]){if(index==count){result=i;break;}count++;}}if(result==-1){w->data[0xd3]=0;result=0;}return result;}
