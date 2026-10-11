#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x14];void *object;u8 pad2[0x74];void *manager;u8 pad3[0x1c];u8 counts[4];u8 pad4[2];u8 team,slot,total;} Work;
typedef struct {u16 species,form;} Cry;
extern Work *PokeathlonCourse_GetHeapAllocPtr4(void *);extern int ov96_021EDD64(Work *);
extern void ov96_021ED5AC(void *,int,int),ov96_021ECC38(void *,int,u16),ov96_021EC298(Work *,u8),sub_02006E3C(int),PlayCry(u16,u8);
extern void *PokeathlonCourseMonObjects_Get(void *,u8),*PokeathlonCourseMonObject_GetGfx(void *);extern Cry *PokeathlonCourseMonGfx_GetInfo(void *);
int ov96_021EDDA4(void *,u32 (*)(void *,int,int),int);
int ov96_021EDDA4(void *data,u32 (*test)(void *,int,int),int value){
Work *w=PokeathlonCourse_GetHeapAllocPtr4(data);int index;Cry *cry;
do{if(test(data,w->team,w->slot))break;}while(ov96_021EDD64(w)==0);
if(w->team<4){index=w->team*3+w->slot;w->counts[w->team]++;w->total++;ov96_021ED5AC(data,w->team,w->slot);ov96_021ECC38(w->manager,w->team,(u16)value);ov96_021EC298(w,(u8)index);cry=PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(PokeathlonCourseMonObjects_Get(w->object,(u8)index)));sub_02006E3C(1);PlayCry(cry->species,(u8)cry->form);ov96_021EDD64(w);return 1;}return 0;
}
