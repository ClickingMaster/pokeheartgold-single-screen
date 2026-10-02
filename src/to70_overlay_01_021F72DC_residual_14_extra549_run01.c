#pragma force_active on
#include "global.h"
typedef struct {u8 dir,pad,state,pad2;void *sprite;} Work;
extern Work *sub_0205F40C(void *);
extern int ov01_021FA2D4(void *);
extern int MapObject_GetFacingDirection(void *);
extern int sub_0205F330(void *);
extern int sub_02023EF4(void *);
extern void sub_02023EE0(void *,int);
extern void sub_02023F40(void *,int);
extern void sub_02023F04(void *,int);
extern void ov01_021FA3E8(void *,void *);
extern void ov01_021F8C88(void *,void *);
void ov01_021F89B8(void *);
void ov01_021F89B8(void *obj){Work *w=sub_0205F40C(obj);void *sprite=w->sprite;int dir;if(ov01_021FA2D4(obj)==1||!sprite)return;dir=MapObject_GetFacingDirection(obj);if(sub_0205F330(obj)==0){if(sub_02023EF4(sprite)!=1){sub_02023EE0(sprite,1);sub_02023F40(sprite,0);}sub_02023F04(sprite,4096);}else{if(sub_02023EF4(sprite)!=0){sub_02023EE0(sprite,0);sub_02023F40(sprite,0);}sub_02023F04(sprite,4096);}w->dir=dir;w->state=sub_0205F330(obj);ov01_021FA3E8(obj,sprite);ov01_021F8C88(obj,sprite);}
