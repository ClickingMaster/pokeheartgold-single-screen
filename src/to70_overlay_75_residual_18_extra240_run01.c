#pragma force_active on
#include "global.h"
#pragma force_active on
void *Heap_Alloc(int,int);void Heap_Free(void *);void CopyStringToU16Array(void *,u16 *,int);
int ov75_02249838(void *,int);
int ov75_02249838(void *str,int heap){u16 *p,*q;int good=1;p=Heap_Alloc(heap,200);CopyStringToU16Array(str,p,100);if(*p!=65535){q=p;do{if(*q!=289){good=0;break;}q++;}while(*q!=65535);}Heap_Free(p);return good;}
