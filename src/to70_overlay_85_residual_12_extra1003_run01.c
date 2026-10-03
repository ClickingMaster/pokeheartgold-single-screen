#pragma force_active on
#include "global.h"
extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);extern int GF_GetCurrentPlayingBGM(void);extern void sub_02055198(int,u16);typedef struct{int state;u8 gap[24];int bgm;}Work;

int ov85_021E670C(Work*w);

int ov85_021E670C(Work*w){int bgm;BeginNormalPaletteFade(2,0,0,0,8,1,102);bgm=w->bgm;if(bgm!=GF_GetCurrentPlayingBGM())sub_02055198(0,(u16)bgm);w->state=52;return 0;}
