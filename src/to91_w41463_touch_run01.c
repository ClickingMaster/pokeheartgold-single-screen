#pragma force_active on
#include "global.h"
#include "touch_sampling_internal.h"
#pragma force_active on
typedef struct Work {void *manager;u8 prefix[0x3b8-4];TouchOutput touch;u8 scratch[0x9600];} Work;
typedef struct Bounds {u8 minY,maxY,minX,maxX;} Bounds;
typedef char WorkSize[(sizeof(Work)==0x99fc)?1:-1];
typedef char TouchOffset[(offsetof(Work,touch)==0x3b8)?1:-1];
typedef char SampleSize[(sizeof(TPData)==8)?1:-1];
extern const Bounds ov87_021E829C[9];
void ov87_021E7A44(u8*,int,int);
void ov87_021E74F4(void*,int);
void ov87_021E74F4(void *parent,int kind) {
 Work *w=parent;
 int i=0;
 if(i<w->touch.count) {
  u8 *cursor=(u8*)w;
  const Bounds *b=&ov87_021E829C[kind];
  int minX=b->minX;
  do {
   int x=*(const u16*)(cursor+offsetof(Work,touch)+offsetof(TouchOutput,samples)+offsetof(TPData,x));
   int y=*(const u16*)(cursor+offsetof(Work,touch)+offsetof(TouchOutput,samples)+offsetof(TPData,y));
   if(minX<=x&&x<=b->maxX&&b->minY<=y&&y<=b->maxY)ov87_021E7A44((u8*)w,x,y);
   cursor+=sizeof(TPData);i++;
  }while(i<w->touch.count);
 }
}
