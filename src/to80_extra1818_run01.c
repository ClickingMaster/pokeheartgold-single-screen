#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 a,b;s32 x,y;}Point;extern int ov96_022143FC(int,int);
int ov96_022143DC(Point*p);
int ov96_022143DC(Point*p){return ov96_022143FC(p->x/4096,p->y/4096);}
