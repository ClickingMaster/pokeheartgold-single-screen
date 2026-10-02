#pragma force_active on
#include "global.h"
extern void SysTask_Destroy(void*);extern void Heap_Free(void*);extern void GF_DestroyVramTransferManager(void);
extern void ov90_0225A108(void*),ov90_02259EAC(void*),ov90_0225B380(void*),ov90_0225A960(void*),ov90_02259434(void*),ov90_022591D4(void*),ov90_02258E10(void*),ov90_02259158(void*),ov90_02259084(void*);
typedef struct {u8 p[0x14];u8 count;u8 q[0x644-0x15];void *task1,*task2;} Work;
void ov90_02258A04(Work*);
void ov90_02258A04(Work*w){int i;u8 *cursor;SysTask_Destroy(w->task1);SysTask_Destroy(w->task2);ov90_0225A108((u8*)w+0x33c);i=0;if(i<w->count){cursor=(u8*)w+0x25c;do{ov90_02259EAC(cursor);i++;cursor+=0x38;}while(i<w->count);}ov90_0225B380((u8*)w+0x1e8);ov90_0225A960(w);ov90_02259434((u8*)w+0x68);ov90_022591D4((u8*)w+0x58);ov90_02258E10((u8*)w+0x1cc);ov90_02259158((u8*)w+0xa0);ov90_02259084((u8*)w+0x4c);*(volatile u16*)0x04000050=0;*(volatile u16*)0x04001050=0;GF_DestroyVramTransferManager();Heap_Free(w);}
