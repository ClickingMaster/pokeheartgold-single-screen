#pragma force_active on
#include "global.h"
extern void BufferIntegerAsString(void *,int,int,int,int,int);
typedef struct {u8 pad[0x50];void *format;} Work;
void ov43_0222AAA4(Work *,u64);
void ov43_0222AAA4(Work *w,u64 n){BufferIntegerAsString(w->format,1,n/100000000,4,2,1);BufferIntegerAsString(w->format,2,n/10000%10000,4,2,1);BufferIntegerAsString(w->format,3,n%10000,4,2,1);}
