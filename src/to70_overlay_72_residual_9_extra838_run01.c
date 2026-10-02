#pragma force_active on
#include "global.h"
extern int ov72_0223AF48(void*);extern void sub_0203946C(void),ov72_022387D0(void*,int,int),ov72_0223A350(void*,void*,int,int,int),ov72_02238680(void*,int,int),ov72_0223A420(void*);
int ov72_02239108(u8*w);
int ov72_02239108(u8*w){int state=ov72_0223AF48(w+0x136c);if(state){if(state==2){sub_0203946C();ov72_022387D0(w,0,0);*(int*)(w+28)=46;}else{ov72_0223A350(w,*(void**)(w+0xbd8),1,1,0xf0f);ov72_02238680(w,47,2);ov72_0223A420(w);}}return 3;}
