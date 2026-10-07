#pragma force_active on
#include "global.h"
#pragma force_active on
typedef union{u32 raw;struct{u32 species:8,form:8,active:1,pad:15;}b;} Identity;
typedef struct{Identity id;u32 words[6];union{u32 raw;struct{u32 lower:24,time:6,upper:2;}b;}timer;union{u32 raw;struct{u32 lower:31,done:1;}b;}status;} Sample;
typedef struct{u16 species,form;union{u32 raw;struct{u32 counter:14,entered:1,previous:1,old:13,pad:3;}b;}flags;u8 tail[0xdc];} Record;
void*PokeathlonCourse_GetHeapAllocPtr4(void*);u8*PokeathlonCourse_GetDataCopyArea(void*);BOOL ov96_021E5F24(void*);u8 *ov96_021E8A20(void *ptr);void ov96_0220FA18(void*,void*);void ov96_02210858(void*);void ov96_02210030(void*,void*,int);void ov96_0220F8C8(void*,void*);void ov96_0220F03C(void*);
#define S32(p,o) (*(s32*)((u8*)(p)+(o)))
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
void ov96_0220F03C(void*course){u8*heap=PokeathlonCourse_GetHeapAllocPtr4(course);u8*area=PokeathlonCourse_GetDataCopyArea(course);int remaining,i;Sample*current;u8*source;Record*record;
 if(ov96_021E5F24(course))return;remaining=0;current=(Sample *)ov96_021E8A20(area+0x28);if(current->status.b.done){ov96_0220FA18(heap+0xcc,course);return;}
 if(S32(heap,0x50c)>0){S32(heap,0x50c)--;remaining=(S32(heap,0x50c)+30)/30;current->timer.b.time=remaining;}
 {Sample*dest=(Sample *)ov96_021E8A20(area+0x50);Sample*src=(Sample *)ov96_021E8A20(area);*dest=*src;}
 i=0;source=area+0x50;record=(Record*)(heap+0x15c);for(;i<4;i++){Sample*s=(Sample *)ov96_021E8A20(source);if(s->id.b.active){if(record->flags.b.entered&&record->flags.b.previous)record->flags.b.entered=0;else if(!record->flags.b.entered&&!record->flags.b.previous){record->flags.b.entered=1;record->flags.b.previous=1;}record->species=s->id.b.species;record->form=s->id.b.form;record->flags.b.counter++;}else{record->flags.b.old=record->flags.b.counter;record->flags.b.counter=0;record->flags.b.entered=0;record->flags.b.previous=0;}source+=0x28;record++;}
 ov96_02210858(PTR(heap,0xc8));ov96_02210030(heap+0x4ec,area,remaining);ov96_0220F8C8(heap+0x15c,course);if(S32(heap,0x50c)<=0)current->status.b.done=1;ov96_0220FA18(heap+0xcc,course);
}
