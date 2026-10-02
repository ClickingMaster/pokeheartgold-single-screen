#pragma force_active on
#include "global.h"
#include "system.h"
extern int ov57_0223B1A4(void *,u8);
extern int ov57_0223B478(int *);
typedef struct{u8 p[0x3ec];int selected;} Work;
int ov57_0223B288(Work *w);
int ov57_0223B288(Work *w){int selected;if(gSystem.newKeys&2)return ov57_0223B1A4(w,12);if(gSystem.newKeys&1)return ov57_0223B1A4(w,13);if(!(gSystem.newAndRepeatedKeys&240))return 3;selected=w->selected;switch(ov57_0223B478(&selected)){case 0:return 3;default:return ov57_0223B1A4(w,(u8)selected);}}
