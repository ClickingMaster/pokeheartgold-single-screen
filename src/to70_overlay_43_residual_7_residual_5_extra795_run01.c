#pragma force_active on
#include "global.h"
extern void *sub_0202C6F4(void *);
extern int sub_0202C090(void *,int,int);
extern void BufferIntegerAsString(void *,int,int,int,int,int);
extern void BufferMonthNameAbbr(void *,int,int);
typedef struct{u8 p[0x50];void *format;} Work;
int ov43_0222AB94(Work *w,void *save,int index);
int ov43_0222AB94(Work *w,void *save,int index){void *data=sub_0202C6F4(save);int ok=1,value=sub_0202C090(data,index,6);if(!value)ok=0;BufferIntegerAsString(w->format,2,value,2,0,1);BufferIntegerAsString(w->format,0,sub_0202C090(data,index,4),4,0,1);BufferMonthNameAbbr(w->format,1,sub_0202C090(data,index,5));return ok;}
