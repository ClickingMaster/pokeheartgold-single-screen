#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern void WirelessStatusIcon_Destroy(void *),PokeathlonCourse_ResetField3A4(void *),FreeBgTilemapBuffer(void *,int),RemoveWindow(void *),Heap_Free(void *),ov96_021EB21C(void *),ov96_021EA894(void *),ov96_021E9C0C(void *),OamManager_Free(void),ObjCharTransfer_Destroy(void),ObjPlttTransfer_Destroy(void),ov96_022043B8(void *),ov96_02203A30(void *),FontID_Release(int),Main_SetVBlankIntrCB(void *,void *),Main_SetHBlankIntrCB(void *,void *),PokeathlonCourse_FreePtr4HeapAlloc(void *),GfGfx_SwapDisplay(void),Heap_Destroy(int);
extern u8 gSystem[];
typedef struct {void *bg;u8 windows[0x44];void *obj;u8 pad[0x598-0x4c];void *other;u8 pad2[0x5d4-0x59c];void *a,*b,*c,*d;} State;
int ov96_02201B98(void *);
int ov96_02201B98(void *course){State *s=PokeathlonCourse_GetHeapAllocPtr4(course);int i;u8 *w;WirelessStatusIcon_Destroy(s);PokeathlonCourse_ResetField3A4(course);FreeBgTilemapBuffer(s->bg,0);FreeBgTilemapBuffer(s->bg,1);FreeBgTilemapBuffer(s->bg,3);FreeBgTilemapBuffer(s->bg,4);FreeBgTilemapBuffer(s->bg,5);FreeBgTilemapBuffer(s->bg,6);FreeBgTilemapBuffer(s->bg,7);RemoveWindow(s->windows);for(i=0,w=s->windows+16;i<3;i++,w+=16)RemoveWindow(w);Heap_Free(s->bg);ov96_021EB21C(s->obj);ov96_021EA894(s->b);ov96_021E9C0C(s->a);OamManager_Free();ObjCharTransfer_Destroy();ObjPlttTransfer_Destroy();ov96_022043B8(s->c);ov96_02203A30(s->d);FontID_Release(4);Main_SetVBlankIntrCB(0,0);Main_SetHBlankIntrCB(0,0);Heap_Free(s->other);PokeathlonCourse_FreePtr4HeapAlloc(course);gSystem[0x69]=0;GfGfx_SwapDisplay();*(vu16 *)0x4000050=0;Heap_Destroy(0x92);return 1;}
