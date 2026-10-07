#pragma force_active on
#include "global.h"
#pragma force_active on
typedef union {u16 raw;struct{u16 first:1,second:1,index:2,rest:12;} b;} Flags;
typedef struct{u8 pad[0x180];u32 value;u8 pad2[0x1d0-0x184];Flags flags;u16 score;} Field;
typedef struct{u8 pad[0x8c];void*table;u8 pad2[12];union{u32 raw;struct{u32 low:24,id:4,hi:4;} b;} bits;u8 pad3[12];u8 values[4];} Work;
extern int ov96_021E5F24(void*);extern Work*PokeathlonCourse_GetHeapAllocPtr4(void*);extern Field*PokeathlonCourse_GetFieldData(void*);extern int ov96_021ECC4C(void*,u8),ov96_021EE264(void*,u8);
void ov96_021EDE64(void*,void*);
void ov96_021EDE64(void*course,void*table){int id;Work*w;Field*f;int i,smallest,result;id=ov96_021E5F24(course);w=PokeathlonCourse_GetHeapAllocPtr4(course);f=PokeathlonCourse_GetFieldData(course);if(w->bits.b.id==id)f->flags.b.first=1;i=0;smallest=999;for(;i<4;i++){int value=ov96_021ECC4C(table,(u8)i);if(smallest>=value)smallest=value;}{int value=ov96_021ECC4C(table,(u8)id);if(value==smallest)f->flags.raw|=2;}f->value=w->values[id];f->score=ov96_021ECC4C(w->table,(u8)id);result=ov96_021EE264(&w->bits,(u8)id);if(result==-1)GF_AssertFail();f->flags.b.index=(u16)result;}
