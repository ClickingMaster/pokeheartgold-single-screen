#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x40c8];u8 images[1][1024];}Data;typedef struct {u8 p[0x34];Data*data;}Work;typedef NNSG2dCharacterData Image;
extern void*AllocAtEndAndReadWholeNarcMemberByIdPair(int,int,int);extern int ov14_021E7930(Work*,int);extern void Heap_Free(void*),ov14_021F46B0(Work*,void*,int,int,int),ov14_021F4A64(Work*,int,void*);
void ov14_021F4958(Work*,int);
void ov14_021F4958(Work*w,int n){Image*img;void*allocation=AllocAtEndAndReadWholeNarcMemberByIdPair(19,70,10);NNS_G2dGetUnpackedCharacterData(allocation,&img);MI_CpuCopy8(img->pRawData,w->data->images[n],1024);Heap_Free(allocation);ov14_021F46B0(w,w->data->images[n],ov14_021E7930(w,n),8,1024);ov14_021F4A64(w,n,w->data->images[n]);}
