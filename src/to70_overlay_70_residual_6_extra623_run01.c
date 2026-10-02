#pragma force_active on
#include "global.h"
extern void ov70_022387F8(void*),ov70_022388D0(void),ov70_02238900(void*),ov70_02238B80(void*),ov70_022387AC(void*);
extern void Main_SetVBlankIntrCB(void(*)(void*),void*);
extern void ov70_02241308(void*),SpriteTransfer_DeleteCharTransferTask(void*),SpriteTransfer_DeletePlttTransferTask(void*),Destroy2DGfxResObjMan(void*),SpriteList_Delete(void*),OamManager_Free(void),ObjCharTransfer_Destroy(void),ObjPlttTransfer_Destroy(void);
typedef struct {u8 p[0xbf4];void* sprites;u8 q[0xd20-0xbf8];void*man[4];void*char0;void*pltt0;u8 r[8];void*char1;void*pltt1;} Work;
void ov70_02238E70(void*w);
void ov70_02238E98(Work*w);
void ov70_02238E70(void*w){ov70_022387F8(w);ov70_022388D0();ov70_02238900(w);ov70_02238B80(w);Main_SetVBlankIntrCB(ov70_022387AC,w);}
void ov70_02238E98(Work*w){int i;ov70_02241308(w);SpriteTransfer_DeleteCharTransferTask(w->char0);SpriteTransfer_DeleteCharTransferTask(w->char1);SpriteTransfer_DeletePlttTransferTask(w->pltt0);SpriteTransfer_DeletePlttTransferTask(w->pltt1);for(i=0;i<4;i++)Destroy2DGfxResObjMan(w->man[i]);SpriteList_Delete(w->sprites);w->sprites=0;OamManager_Free();ObjCharTransfer_Destroy();ObjPlttTransfer_Destroy();}
