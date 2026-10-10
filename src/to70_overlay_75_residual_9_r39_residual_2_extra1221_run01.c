#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "pm_string.h"
#include "msgdata.h"
#include "render_window.h"
#include "text.h"
#include "sound.h"
#include "system.h"
#include "list_menu_cursor.h"
#pragma force_active on
typedef struct {u32 pad;BgConfig *bg;u8 p[0x34-8];MsgData *msg;u8 p2[0x48-0x38];Window old;u8 p3[0xa8-0x58];int state,scroll,selection,total,result,frame,blink;Window first,second;struct ListMenuCursor *cursor;} Context;
extern void ov75_02247C70(Context*),ov75_02248034(Context*),ov75_02247D0C(Context*),ov75_02247D24(Context*);
extern void ClearWindowTilemapAndCopyToVram(Window*);
extern void PlaySE(int);
int ov75_022480B8(Context*);
int ov75_022480B8(Context *c){switch(c->state){case 0:{String *full=NewString_ReadMsgData(c->msg,44);int total=String_CountLines(full);String *line;int i;c->scroll=0;c->selection=0;c->result=0;c->total=total;AddWindowParameterized(c->bg,&c->first,2,1,5,30,12,13,148);AddWindowParameterized(c->bg,&c->second,2,1,19,30,4,13,508);FillWindowPixelBuffer(&c->first,15);line=String_New(String_GetLength(full),(enum HeapID)116);for(i=0;i<6;i++){String_GetLineN(line,full,i);AddTextPrinterParameterized(&c->first,0,line,4,i*16,255,0);}String_Delete(full);String_Delete(line);DrawFrameAndWindow1(&c->first,1,31,11);CopyWindowToVram(&c->first);FillWindowPixelBuffer(&c->second,15);DrawFrameAndWindow1(&c->second,1,31,11);CopyWindowToVram(&c->second);c->cursor=ListMenuCursorNew((enum HeapID)116);FillWindowPixelBuffer(&c->old,15);ClearFrameAndWindow2(&c->old,1);ClearWindowTilemapAndCopyToVram(&c->old);GfGfx_EngineATogglePlanes(8,1);c->frame=0;c->blink=0;c->state++;break;}case 1:{int old=c->scroll;if(gSystem.newAndRepeatedKeys &64){if(old!=0){c->scroll--;PlaySE(1500);}}else if(gSystem.newAndRepeatedKeys &128){if(old+6<c->total){c->scroll++;PlaySE(1500);}if(c->scroll+6==c->total){ov75_02247C70(c);c->state++;PlaySE(1500);}}else if(gSystem.newKeys&2){PlaySE(1500);c->result=2;c->state=255;}if(old!=c->scroll)ov75_02248034(c);break;}case 2:{if((gSystem.newKeys&16)||(gSystem.newKeys&32)){c->selection ^=1;PlaySE(1500);ov75_02247C70(c);}else if(gSystem.newKeys&1){c->result=2-c->selection;PlaySE(1500);c->state++;}else if(gSystem.newKeys&2){c->result=2;PlaySE(1500);c->state++;}else if(gSystem.newKeys&64){ov75_02247D0C(c);PlaySE(1500);c->state--;c->scroll--;ov75_02248034(c);}break;}default:EraseFrameAndWindow1(&c->first,0);ClearWindowTilemapAndCopyToVram(&c->first);RemoveWindow(&c->first);EraseFrameAndWindow1(&c->second,0);ClearWindowTilemapAndCopyToVram(&c->second);RemoveWindow(&c->second);DestroyListMenuCursorObj(c->cursor);BgClearTilemapBufferAndCommit(c->bg,3);GfGfx_EngineATogglePlanes(8,0);c->state=0;return c->result;}ov75_02247D24(c);return 0;}
