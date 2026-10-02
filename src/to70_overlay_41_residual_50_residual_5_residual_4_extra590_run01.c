#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sys_task.h"
#pragma force_active on
typedef struct {int a;NNSG2dCharacterData *character;void *archive;int b,c;} Tile;
extern void ov41_02249E40(void *,int);
extern void ov41_0224A094(SysTask *,void *);
extern SysTask *SysTask_CreateOnVWaitQueue(SysTaskFunc,void *,u32);
void ov41_02249F7C(BgConfig *,int,void *,int,int,int,int,int,int);
SysTask *ov41_02249FFC(NarcId,int,int,int,int,enum HeapID);
void ov41_02249F7C(BgConfig *bg,int layer,void *data,int width,int height,int x,int y,int type,int palette){ov41_02249E40(data,type);CopyToBgTilemapRect(bg,(u8)layer,(u8)x,(u8)y,(u8)width,(u8)height,(u8*)data+12,0,0,(u8)width,(u8)height);BgTilemapRectChangePalette(bg,(u8)layer,(u8)x,(u8)y,(u8)width,(u8)height,(u8)palette);ScheduleBgTilemapBufferTransfer(bg,(u8)layer);}
