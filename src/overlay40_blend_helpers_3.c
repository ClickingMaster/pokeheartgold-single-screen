#include "overlay40_blend_helpers_private.h"
#define BODY(p) do { VsRecorder_LoadMenuRowGfx(p);VsRecorder_MenuPop(p);sub_02088030(p);VsRecorder_CreateMenuRowSprites(p);VsRecorder_SetMenuRowsDrawFlag(p,1);VsRecorder_RefreshChoiceRows(p);VsRecorder_AttachMenuRowText(p);VsRecorder_LayoutMenuRows(p);VsRecorder_RefreshPathRows(p); } while(0)

int ov40_0222DA84(int *p,int direction) { int done=1; if(direction==1) { if(*p<16){*p+=2;done=0;} } else {if(*p>0){*p-=2;done=0;}} return done; }
