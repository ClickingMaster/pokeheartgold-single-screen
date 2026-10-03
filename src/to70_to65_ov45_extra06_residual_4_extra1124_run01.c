#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {void*ptr;u32 size,capacity;} Blob;typedef struct {u32 lo,hi;} Pair;
typedef struct {u8 p[0x134];int state;u8 q[0x144-0x138];u32 stored,flag;Pair time;} Work;
extern Work*_022577C4;extern u8 ov45_022550E4[],ov45_022550F4[];
extern int ov45_02233FE4(void*,Blob*),ov45_02238C74(u32*,void*,u32);extern void*ov45_02231C78(Blob*);extern void ov45_022379D4(Blob*),ov45_02233434(int,int,void*,void*,int),ov45_02232D8C(int*,int*),ov45_02238C1C(Blob*,u32*),ov00_021ECB94(RTCDate*,RTCTime*);
static inline int appendBlob(Blob*blob){Work*w=_022577C4;return ov45_02238C74(&w->stored,ov45_02231C78(blob),blob->size);}
void ov45_0223683C(int,int,int,int,void*),ov45_0223694C(int,int,int,int,void*);
void ov45_0223694C(int ok,int a,int b,int c,void*data){Blob blob,copy;int state;if(ok==0)return;blob.ptr=0;blob.size=0;blob.capacity=0;if(!ov45_02233FE4(data,&blob)||blob.size==0||!appendBlob(&blob)){ov45_02238C1C(&copy,&_022577C4->stored);ov45_02233434(-1,0,ov45_022550F4,ov45_02231C78(&copy),copy.size);ov45_022379D4(&copy);ov45_022379D4(&blob);return;}_022577C4->stored=4;state=6;ov45_02232D8C(&_022577C4->state,&state);ov45_022379D4(&blob);}
