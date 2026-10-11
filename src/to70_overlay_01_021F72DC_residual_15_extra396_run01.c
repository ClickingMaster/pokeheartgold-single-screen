#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W {s8 facing,pad,mode,pad3;void *sprite;} W;
extern W* sub_0205F40C(void*); extern int ov01_021FA2D4(void*),MapObject_GetFacingDirection(void*),sub_0205F330(void*);extern VecFx32* MapObject_GetFacingVector(void*);extern void BillboardObject_SetSequence(void*,int),BillboardObject_SetSequenceFrame(void*,int),BillboardObject_AdvanceFrame(void*,int),ov01_021FA3E8(void*,void*),ov01_021F8C88(void*,void*),ov01_021F8BE0(void*,void*,int);extern void(*ov01_022089CC[])(void*,void*,W*,int);
void ov01_021F8AB0(void*),ov01_021F8B30(void*);
void ov01_021F8AB0(void*obj){int mode;W*w=sub_0205F40C(obj);void*s=w->sprite;if(ov01_021FA2D4(obj)!=1&&s){int facing=MapObject_GetFacingDirection(obj);mode=sub_0205F330(obj);if(mode!=w->mode){if(mode==0)BillboardObject_SetSequence(s,0);else BillboardObject_SetSequence(s,1);BillboardObject_SetSequenceFrame(s,0);}BillboardObject_AdvanceFrame(s,4096);MapObject_GetFacingVector(obj)->z=2048;w->facing=facing;w->mode=mode;ov01_021FA3E8(obj,s);ov01_021F8C88(obj,s);}}
void ov01_021F8B30(void*obj){int facing;W*w=sub_0205F40C(obj);void*s=w->sprite;if(s&&ov01_021FA2D4(obj)!=1){facing=MapObject_GetFacingDirection(obj);int mode=sub_0205F330(obj);if(mode>=1)GF_AssertFail();ov01_022089CC[mode](obj,s,w,facing);ov01_021F8BE0(obj,s,facing);w->facing=facing;w->mode=sub_0205F330(obj);ov01_021FA3E8(obj,s);ov01_021F8C88(obj,s);}}
