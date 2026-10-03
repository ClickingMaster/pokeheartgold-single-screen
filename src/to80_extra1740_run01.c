#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u8 unused;} Tag;
typedef struct{u32 a,b;} Entry;
typedef struct{Entry *p;} Iter;
static inline int Equal(const Iter *first,Entry *last){return first->p==last;}
static inline long Distance(Iter first,Iter last){Iter *a=&first,*b=&last;return b->p-a->p;}
typedef struct{u32 start,size,field8,fieldC;int capacity;} Vector;
static inline u32 End(Vector *v){if(v->capacity)return (v->capacity-1)<<7;return 0;}
typedef struct{u32 x[5];} Position;
void ov45_0223B2DC(Vector*,u32);
Position ov45_0223B250(Vector*,u32);
Position ov45_02238F18(Iter,Iter,Position*,u32*,u32*);
void ov45_02238E60(Vector*,Iter*,Iter*,Tag);
void ov45_02238E60(Vector*v,Iter*first,Iter*last,Tag tag){Position result,pos;u32 count,available;if(Equal(first,last->p))return;count=Distance(*first,*last);if(v->capacity==0)available=0;else available=(v->capacity-1)<<7;available-=v->start+v->size;if(count>available)ov45_0223B2DC(v,count-available);pos=ov45_0223B250(v,v->size);result=ov45_02238F18(*first,*last,&pos,&v->field8,&v->size);}
