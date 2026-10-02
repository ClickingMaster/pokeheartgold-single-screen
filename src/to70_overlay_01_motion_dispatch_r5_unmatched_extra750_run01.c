#pragma force_active on
#include "global.h"
typedef struct {u8 p[2];u8 dir,mode;void *model;} Data;
extern Data *sub_0205F40C(void*);extern int ov01_021FA2D4(void*);extern int MapObject_GetFacingDirection(void*);extern int sub_0205F330(void*);extern void (*ov01_02208B0C[10])(void*,void*,Data*,int);extern void ov01_021FA3E8(void*,void*);extern void ov01_021F8C88(void*,void*);
void ov01_021F7C14(void*);
void ov01_021F7C14(void*w) {volatile int direction[1];Data *data[1];int mode;void *m;data[0]=sub_0205F40C(w);m=data[0]->model;if(ov01_021FA2D4(w)!=1 && m!=0){direction[0]=MapObject_GetFacingDirection(w);mode=sub_0205F330(w);if(mode>=10)GF_AssertFail();ov01_02208B0C[mode](w,m,data[0],direction[0]);data[0]->dir=direction[0];data[0]->mode=sub_0205F330(w);ov01_021FA3E8(w,m);ov01_021F8C88(w,m);}}
