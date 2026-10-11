#include "overlay40_blend_helpers_private.h"
#define BODY(p) do { VsRecorder_LoadMenuRowGfx(p);VsRecorder_MenuPop(p);sub_02088030(p);VsRecorder_CreateMenuRowSprites(p);VsRecorder_SetMenuRowsDrawFlag(p,1);VsRecorder_RefreshChoiceRows(p);VsRecorder_AttachMenuRowText(p);VsRecorder_LayoutMenuRows(p);VsRecorder_RefreshPathRows(p); } while(0)

void ov40_0222D9E8(int *a,int *b,int mode) { ov40_0222D910(a,b,8,18,8,18,mode); }
