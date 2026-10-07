#include "global.h"
#include "heap.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "sprite_system.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "pokeathlon/pokeathlon.h"
#pragma force_active on
typedef struct Display260 {
 PokeathlonCourseData *course;enum HeapID heap;
 SpriteSystem *a;SpriteManager *b;void *c;BgConfig *d;void *e,*f;
 u16 palette;u8 flag,pad23;NARC *narc;void *screen;NNSG2dScreenData *screenData;
 Window windows[3];u8 remaining60[0x50];String *names[3];u8 tail[12];
}Display260;
typedef char Display260Size[sizeof(Display260)==200?1:-1];
typedef char Display260Names[offsetof(Display260,names)==0xb0?1:-1];
typedef char Display260Windows[offsetof(Display260,windows)==0x30?1:-1];
void ov96_02219A08(void *);
void *ov96_022194C4(void *,void *,void *,void *,void *,void *,u8,enum HeapID,void *);
// @fn ov96_022194C4
void *ov96_022194C4(void *a,void *b,void *c,void *d,void *e,void *f,u8 flag,enum HeapID heap,void *course){
 Display260 *w;u8 *member;int i;
 GF_ASSERT(a);GF_ASSERT(b);GF_ASSERT(c);GF_ASSERT(d);GF_ASSERT(e);GF_ASSERT(f);
 w=Heap_Alloc(heap,200);MI_CpuFill8(w,0,200);
 w->a=a;w->b=b;w->c=c;w->flag=flag;w->heap=heap;w->d=d;w->e=e;w->f=f;w->course=course;
 w->narc=NARC_New((NarcId)20,w->heap);
 w->screen=GfGfxLoader_GetScrnData((NarcId)242,7,TRUE,&w->screenData,w->heap);
 w->palette=SpriteSystem_LoadPlttResObj(a,b,(NarcId)20,sub_02074490(),FALSE,3,2,10004);
 member=PokeathlonCourse_GetParticipantUnk04(course,ov96_021E5F24(course));
 i=0;do{w->names[i]=String_New(11,w->heap);CopyU16ArrayToString(w->names[i],(const u16 *)(member+18));i++;member+=40;}while(i<3);
 ov96_02219A08(w);return w;
}
