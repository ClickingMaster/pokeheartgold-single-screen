#pragma force_active on
#include "global.h"
extern void ov00_021EC60C(void),ov00_021EC210(void),ov00_021EC8D8(void),ov73_021EA180(void*),ov00_021EC9E0(void*);extern int ov00_021EC5B4(void),ov00_021EC724(void),ov00_021EC0FC(int*);typedef struct{u8 p[0x1c];int state;u8 gap[0xef0];int a,b,c,d;}Work;
int ov73_021E8D2C(Work*w);
int ov73_021E8D2C(Work*w){int b,a;u8 c[60];ov00_021EC60C();if(ov00_021EC5B4()){switch(ov00_021EC724()){case 7:w->a=ov00_021EC0FC(&b);w->b=b;ov00_021EC210();ov00_021EC8D8();ov73_021EA180(w);w->state=55;break;case 0:case 1:case 2:case 3:case 5:case 6:case 8:default:ov00_021EC0FC(&a);ov73_021EA180(w);w->state=53;w->d=-2;break;case 4:ov00_021EC9E0(c);w->state=4;break;}}return 3;}
