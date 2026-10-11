#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct{u32 a;void *bg;u32 b;void *c,*d,*e;u8 p[0x74];void *f,*g;} Work;
extern Work *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern void ov96_021ED09C(void *),ov96_021ECBF4(void *),ov96_021EE808(void *),FreeBgTilemapBuffer(void *,int),PokeathlonCourseMonObjects_Free(void *),PokeathlonCourseSprites_FreeResources(void *),WirelessStatusIcon_Destroy(void),ov96_021EC51C(Work *),PokeathlonCourse_FreePtr4HeapAlloc(void *),GfGfx_SwapDisplay(void),UnloadOverlayByID(u32);
extern u8 SDK_OVERLAY_OVY_98_ID[];
int ov96_021ED48C(void *);
int ov96_021ED48C(void *course){Work *w=PokeathlonCourse_GetHeapAllocPtr4(course);Main_SetVBlankIntrCB(0,0);Main_SetHBlankIntrCB(0,0);ov96_021ED09C(w->g);ov96_021ECBF4(w->f);ov96_021EE808(w->c);FreeBgTilemapBuffer(w->bg,0);FreeBgTilemapBuffer(w->bg,1);FreeBgTilemapBuffer(w->bg,4);FreeBgTilemapBuffer(w->bg,5);Heap_Free(w->bg);PokeathlonCourseMonObjects_Free(w->e);PokeathlonCourseSprites_FreeResources(w->d);WirelessStatusIcon_Destroy();ov96_021EC51C(w);PokeathlonCourse_FreePtr4HeapAlloc(course);gSystem.screensFlipped=0;GfGfx_SwapDisplay();UnloadOverlayByID((u32)SDK_OVERLAY_OVY_98_ID);Heap_Destroy((enum HeapID)135);return 1;}
