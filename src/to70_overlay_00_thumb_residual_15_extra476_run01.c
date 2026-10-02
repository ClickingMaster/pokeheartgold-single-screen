#pragma force_active on
#include "global.h"
extern void Heap_Free(void*);
extern void*Heap_Alloc(int,u32);
extern void ov00_021E6638(int,void*,int);
extern void*ov00_021E65D4(int,int,int);
extern void ov00_021F9850(u8,void*,int);
typedef struct {u8 p[0xf80];void*buffers[1];u8 q[0x104];int heap;u8 r[0x55];u8 allocated[1];} Work;
extern Work *_0221A680;
void ov00_021E7234(int);
void ov00_021E727C(int);
void ov00_021E7234(int i){void*p=_0221A680->buffers[i];if(p){if(_0221A680->allocated[i]==1)Heap_Free(p);else ov00_021E6638(0,p,0);_0221A680->buffers[i]=0;}}
void ov00_021E727C(int i){ov00_021E7234(i);if(_0221A680->buffers[i]==0){if(_0221A680->heap==11){_0221A680->buffers[i]=Heap_Alloc(_0221A680->heap,0x1020);_0221A680->allocated[i]=1;}else{_0221A680->buffers[i]=ov00_021E65D4(0,0x1000,32);_0221A680->allocated[i]=0;}ov00_021F9850((u8)i,(void*)(((u32)_0221A680->buffers[i]+31)&~31),0x1000);}}
