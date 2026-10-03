#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[0xe6];u8 kind:2;u8 mode:6;u8 gap[14];u8 flag5,flag6;} Context;
Context *ov13_02239FF0(u32,void*);
void ov13_02234314(const void*,void*);void ov13_02234468(const void*,void*);
typedef struct{u32 pad;volatile u8 flag4,flag5,flag6,flag7;} Info;
extern struct{u32 pad[2];Info *info;} ov13_0224F54C;
int ov13_0223417C(int);void ov13_02234210(int,int,void*);
void ov13_02234028(u32,void*);
#pragma opt_propagation off
void ov13_02234028(u32 type,void*out){Context*c=ov13_02239FF0(type,out);int a,b;switch(type){case 0:case 1:a=0;b=0;if(!ov13_0223417C(0))b=2;break;case 2:{int fctx=c->flag5;int flag=0,mode;Info*i;int f;if(fctx){a=1;mode=4;}else{a=2;mode=3;}i=ov13_0224F54C.info;f=i->flag4;b=flag;if(f)b=1;if(i->flag5)flag=1;ov13_02234210(mode,flag,out);break;}case 3:case 4:case 5:a=0;b=c->flag5?2:0;break;case 6:{int mode,flag=0;Info*i;b=0;if(c->flag6){a=1;mode=4;}else{if(!c->flag5)b=2;a=2;mode=3;}i=ov13_0224F54C.info;if(i->flag6)b=1;if(i->flag7)flag=1;ov13_02234210(mode,flag,out);break;}case 7:case 8:a=0;b=c->flag6?2:0;break;default:a=0;b=2;}ov13_02234210(a,b,out);}
