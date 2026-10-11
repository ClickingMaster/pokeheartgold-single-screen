#pragma force_active on
#include "global.h"
typedef struct {u8 dir,pad,state,pad2;void *sprite;} Work;
extern Work *sub_0205F40C(void *);
extern int ov01_021FA2D4(void *);
extern int MapObject_GetFacingDirection(void *);
extern int sub_0205F330(void *);
extern int BillboardObject_GetSequence(void *);
extern void BillboardObject_SetSequence(void *,int);
extern void BillboardObject_SetSequenceFrame(void *,int);
extern void BillboardObject_AdvanceFrame(void *,int);
extern void ov01_021FA3E8(void *,void *);
extern void ov01_021F8C88(void *,void *);
void ov01_021F89B8(void *);
void ov01_021F89B8(void *obj){Work *w=sub_0205F40C(obj);void *sprite=w->sprite;int dir;if(ov01_021FA2D4(obj)==1||!sprite)return;dir=MapObject_GetFacingDirection(obj);if(sub_0205F330(obj)==0){if(BillboardObject_GetSequence(sprite)!=1){BillboardObject_SetSequence(sprite,1);BillboardObject_SetSequenceFrame(sprite,0);}BillboardObject_AdvanceFrame(sprite,4096);}else{if(BillboardObject_GetSequence(sprite)!=0){BillboardObject_SetSequence(sprite,0);BillboardObject_SetSequenceFrame(sprite,0);}BillboardObject_AdvanceFrame(sprite,4096);}w->dir=dir;w->state=sub_0205F330(obj);ov01_021FA3E8(obj,sprite);ov01_021F8C88(obj,sprite);}
