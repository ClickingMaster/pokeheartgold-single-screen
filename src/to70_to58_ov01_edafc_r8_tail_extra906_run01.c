#pragma force_active on
#include "global.h"
typedef struct{u8 pad[8];void*bg;}Field;
extern void*AllocWindows(int,int);extern void AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16),LoadUserFrameGfx1(void*,int,int,int,int,int),DrawFrameAndWindow1(void*,int,int,int),ov01_021EEC7C(Field*,void*,int);
void*ov01_021EEC00(Field*,int,u8,u8);
void*ov01_021EEC00(Field*f,int msg,u8 x,u8 y){void*w=AllocWindows(4,1);AddWindowParameterized(f->bg,w,3,x,y,10,2,13,41);LoadUserFrameGfx1(f->bg,3,985,11,0,4);DrawFrameAndWindow1(w,1,985,11);ov01_021EEC7C(f,w,msg);return w;}
