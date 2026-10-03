#pragma force_active on
#include "global.h"
typedef struct{void *list,*header;VecFx32 pos,scale;u16 rotation;u32 priority,screen,heap;} Template;
typedef struct{u32 a[2],heap;u8 p[0x28];void *list;u8 q[0x170];u32 headers[18];u8 z[0x18];void *sprite;u32 skip;int dy;} Global;
typedef struct{void *list;u8 p[0x128];void *managers[6];void *charA,*plttA;u8 q[0x10];void *charB,*plttB;} Resources;
extern Global ov74_0223D454;extern u32 ov74_0223D654[];extern Resources ov74_0223D488;
extern void *Sprite_CreateAffine(Template *);
extern void Sprite_SetAnimActiveFlag(void *,int),Sprite_SetPriority(void *,int),Sprite_SetAnimCtrlSeq(void *,int),Sprite_SetDrawFlag(void *,int),Sprite_Delete(void *),SpriteTransfer_DeleteCharTransferTask(void *),SpriteTransfer_DeletePlttTransferTask(void *),Destroy2DGfxResObjMan(void *),SpriteList_Delete(void *),OamManager_Free(void),ObjCharTransfer_Destroy(void),ObjPlttTransfer_Destroy(void),Main_SetVBlankIntrCB(void *,void *);
void *ov74_02235930(int,void *,int,int,int);void ov74_022359BC(void);
void *ov74_02235930(int index,void *sprite,int x,int y,int seq){Template t;Global *g=&ov74_0223D454;if(!sprite){t.list=g->list;x<<=12;t.header=(u8 *)g->headers+index*0x24;t.pos.z=0;t.scale.x=4096;t.scale.y=4096;t.scale.z=4096;t.rotation=0;t.pos.x=x;t.pos.y=y<<12;t.priority=10;t.screen=index==0?1:2;t.heap=g->heap;if(t.screen==2)t.pos.y+=g->dy;sprite=Sprite_CreateAffine(&t);}Sprite_SetAnimActiveFlag(sprite,1);Sprite_SetPriority(sprite,0);Sprite_SetAnimCtrlSeq(sprite,seq);Sprite_SetDrawFlag(sprite,1);return sprite;}
void ov74_022359BC(void){Global *g=&ov74_0223D454;Resources *r=&ov74_0223D488;int i;if(ov74_0223D654[2]){Sprite_Delete(g->sprite);g->sprite=0;}if(r->charA)SpriteTransfer_DeleteCharTransferTask(r->charA);if(r->charB)SpriteTransfer_DeleteCharTransferTask(r->charB);if(r->plttA)SpriteTransfer_DeletePlttTransferTask(r->plttA);if(r->plttB)SpriteTransfer_DeletePlttTransferTask(r->plttB);for(i=0;i<6;i++){Destroy2DGfxResObjMan(r->managers[i]);r->managers[i]=0;}SpriteList_Delete(r->list);r->list=0;OamManager_Free();ObjCharTransfer_Destroy();ObjPlttTransfer_Destroy();Main_SetVBlankIntrCB(0,0);}
