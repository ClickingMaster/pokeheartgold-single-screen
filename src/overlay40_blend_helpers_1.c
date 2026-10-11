#include "overlay40_blend_helpers_private.h"
#define BODY(p) do { VsRecorder_LoadMenuRowGfx(p);VsRecorder_MenuPop(p);sub_02088030(p);VsRecorder_CreateMenuRowSprites(p);VsRecorder_SetMenuRowsDrawFlag(p,1);VsRecorder_RefreshChoiceRows(p);VsRecorder_AttachMenuRowText(p);VsRecorder_LayoutMenuRows(p);VsRecorder_RefreshPathRows(p); } while(0)

void ov40_0222D88C(void *p) { BODY(p); }

void ov40_0222D8C8(void *p) { BODY(p); (*(u32 *)((u8 *)p+0x6D8))++; }
