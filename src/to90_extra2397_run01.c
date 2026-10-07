#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
FS_EXTERN_OVERLAY(OVY_98);
typedef struct {void *course,*bg,*narc;enum HeapID heap;u8 pad[16];u8 a,pad2,b,pad3;int mode,result;u8 pad4[0x48-0x2c];} State;
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *),*PokeathlonCourse_AllocPtr4FromHeap(void *,int),*NARC_New(int,int),*BgConfig_Alloc(int);
extern void HandleLoadOverlay(u32,int),GfGfx_DisableEngineAPlanes(void),GfGfx_DisableEngineBPlanes(void),ov96_021EF260(void),GfGfx_SwapDisplay(void),sub_0200FC20(int);extern int ov96_021EF2A0(void *),PokeathlonCourse_GetCurrentParticipantIndex(void *),PokeathlonCourse_GetMode(void *),ov96_021EF2AC(void *);extern void ov96_021EF23C(void*);

void ov96_021EF2C0(void *);
void ov96_021EF2C0(void *course){State *s;enum HeapID heap;PokeathlonCourse_GetHeapAllocPtr4(course);Heap_Create((enum HeapID)92,(enum HeapID)136,0x40000);heap=(enum HeapID)136;HandleLoadOverlay(FS_OVERLAY_ID(OVY_98),2);Main_SetVBlankIntrCB(0,0);Main_SetHBlankIntrCB(0,0);GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();*(vu32 *)0x4000000&=0xffffe0ff;*(vu32 *)0x4001000&=0xffffe0ff;ov96_021EF260();s=PokeathlonCourse_AllocPtr4FromHeap(course,72);MI_CpuFill8(s,0,72);s->narc=NARC_New(221,heap);s->bg=BgConfig_Alloc(heap);s->a=ov96_021EF2A0(course);s->b=PokeathlonCourse_GetCurrentParticipantIndex(course);s->mode=PokeathlonCourse_GetMode(course)==1?1:0;s->course=course;s->heap=(enum HeapID)136;Main_SetVBlankIntrCB(ov96_021EF23C,s);gSystem.screensFlipped=1;GfGfx_SwapDisplay();sub_0200FC20(32767);if(s->mode)s->result=0;else s->result=ov96_021EF2AC(course);}
