#pragma force_active on
#include "global.h"
extern void *Sprite_GetImageProxy(void *);
extern void ObjCharTransfer_DeleteTaskCopyByProxyPtr(void *),Sprite_Delete(void *),SpriteTransfer_DeleteCharTransferTask(void *),SpriteTransfer_DeletePlttTransferTask(void *),DestroySingle2DGfxResObj(void *,void *);
typedef struct{u8 pad[0x68];void *sprite;void *res[4];} Work;
typedef struct{u8 pad[0x130];void *mgr[4];} Managers;
typedef struct{u32 a,b,len;u16 *data;} Colors;

void ov49_0225C328(Work *w,Managers *m);
void ov49_0225C368(Colors *c);

void ov49_0225C328(Work *w,Managers *m){int i;ObjCharTransfer_DeleteTaskCopyByProxyPtr(Sprite_GetImageProxy(w->sprite));Sprite_Delete(w->sprite);w->sprite=0;SpriteTransfer_DeleteCharTransferTask(w->res[0]);SpriteTransfer_DeletePlttTransferTask(w->res[1]);for(i=0;i<4;i++)DestroySingle2DGfxResObj(m->mgr[i],w->res[i]);}
