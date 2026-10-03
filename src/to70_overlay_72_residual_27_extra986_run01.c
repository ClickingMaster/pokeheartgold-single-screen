#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x1312];u8 state;} W;
extern int IsPaletteFadeFinished(void);extern void ov72_0223AD94(W*);extern void ov72_02238B18(W*,int);extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);
int ov72_0223AC2C(W*);
int ov72_0223AC2C(W*w) {if(IsPaletteFadeFinished()==1){ov72_0223AD94(w);ov72_02238B18(w,0);BeginNormalPaletteFade(4,1,1,0,4,1,67);w->state=9;}return 0;}
