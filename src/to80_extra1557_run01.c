#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (((u8*)(p))[o])
#define H(p,o) (*(s16*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern u8 gSystem[],gOverlayTemplate_PokemonSummary[];
void PlaySE(int);void StopSE(int,int);void BeginNormalPaletteFade(int,int,int,int,int,int,int);int IsPaletteFadeFinished(void);int Options_GetFrame(void*);void*Save_PlayerData_GetOptionsAddr(void*);int Options_GetTextFrameDelay(void*);int TextPrinterCheckActive(int);void CopyWindowToVram(void*);void ScheduleBgTilemapBufferTransfer(void*,int);void*OverlayManager_New(const void*,void*,int);
void ov82_0223F300(void*,int);int ov82_0223F53C(void*),ov82_0223F488(void*),ov82_0223F6E4(void*),ov82_0223F6CC(void*);int ov80_02237920(int),ov80_0223792C(int);void*ov82_0223F6C4(int);u32 sub_02030BD0(void*,void*);void ov82_0223F5E0(void*,int,int);void ov82_0223F948(int);void ov82_0223F84C(void*),ov82_0223F834(void*),ov82_0223F8E4(void*),ov82_0223F90C(void*),ov82_0223F224(void*),ov82_0223E8C4(void*);void ov82_0223FD78(void*,int);int ov82_0223EF7C(void*,int,int);int ov82_0223FE18(void*);int ov82_0223EF1C(void*,void*,int,int,int,int,int,int,int,int);void ov82_0223FCFC(void*,int),ov82_0223F580(void*,void*);int ov82_0223F558(void*),ov82_0223F570(void*);void ov82_0223FCBC(void*,int,int);
int ov82_0223E2EC(void*);
int ov82_0223E2EC(void*p){switch(B(p,8)){case 0:B(p,11)=0;B(p,8)=1;break;case 1:ov82_0223F300(p,*(int*)(gSystem+0x48));if(!ov82_0223F53C(p)&&!ov82_0223F488(p))break;if(ov80_02237920(B(p,13))==254){PlaySE(1501);BeginNormalPaletteFade(0,0,0,0,6,1,105);H(p,16)=1;B(p,8)=7;break;}if(ov82_0223F6E4(p)==1){if(ov80_02237920(B(p,13))!=9){PlaySE(1523);return 0;}}else{if(sub_02030BD0(ov82_0223F6C4(B(p,13)),P(p,0x218))>=10){PlaySE(1523);return 0;}if(ov80_02237920(B(p,13))==9){PlaySE(1523);return 0;}}PlaySE(1501);if(!ov80_0223792C(B(p,9))){ov82_0223F5E0(P(p,0x48),B(p,13),1);ScheduleBgTilemapBufferTransfer(P(p,0x48),3);}H(p,20)=0;B(p,8)=2;break;
case 2:H(p,20)++;ov82_0223F948(-H(p,20));if(H(p,20)==8){if(!ov80_0223792C(B(p,9))){ov82_0223F84C(p);B(p,8)=3;}else{ov82_0223FD78((u8*)p+0x4c,Options_GetFrame(P(p,0x9c)));B(p,10)=ov82_0223EF7C(p,0,1);B(p,26)=B(p,13);B(p,23)=1;return 1;}}break;
case 3:switch(ov82_0223FE18(P(p,0x8c))){case 0:break;case 1:ov82_0223F834(p);if(ov82_0223F6CC(p)==1&&ov82_0223F6E4(p)==0){B(p,31)=B(p,13);B(p,8)=5;break;}StopSE(1500,0);PlaySE(1571);if(B(p,31)!=117)B(p,13)=B(p,31);return 1;case 2:ov82_0223F834(p);B(p,8)=4;break;}break;
case 4:H(p,20)--;ov82_0223F948(-H(p,20));if(H(p,20)<=0){ov82_0223F8E4(p);if(B(p,30)==1)B(p,8)=0;else if(B(p,30)==0)B(p,8)=1;else GF_AssertFail();}break;
case 5:ov82_0223FD78((u8*)p+0x4c,Options_GetFrame(P(p,0x9c)));B(p,10)=ov82_0223EF1C(p,(u8*)p+0x4c,32,1,1,Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(P(p,0xa0))),1,2,15,1);CopyWindowToVram((u8*)p+0x4c);B(p,8)=6;break;
case 6:if(!TextPrinterCheckActive(B(p,10))){int a,b;B(p,30)=1;ov82_0223F90C(p);ov82_0223FCFC(P(p,0x204),1);ov82_0223F5E0(P(p,0x48),19,0);ov82_0223F580(p,P(p,0x48));B(p,13)=19;a=ov82_0223F558(p);b=ov82_0223F570(p);ov82_0223FCBC(P(p,0x204),a,b);B(p,8)=4;}break;
case 7:if(IsPaletteFadeFinished()==1){ov82_0223F224(p);ov82_0223E8C4(p);P(p,4)=OverlayManager_New(gOverlayTemplate_PokemonSummary,P(p,0xa4),105);B(p,11)=1;return 1;}break;}return 0;}
