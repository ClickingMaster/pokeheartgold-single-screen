#pragma force_active on
#include "global.h"
#include "system.h"
#include "sound.h"
extern void PlaySE(int);
extern int PokewalkerApp_GetTouchedButton(int);extern int ov112_021FF090[];
extern void ov112_021EC440(void*,int,int,int);extern void PokewalkerApp_SetCursorPosition(void*,int);
extern int ov112_021EB9C0(void);extern int ov112_021EB9E4(void);extern void ov112_021EB90C(void*,int);extern int ov112_021EB950(void*,int);
typedef struct{u8 pad[0x1ec50];int index;}W;
int ov112_021EB44C(W*);
int ov112_021EB44C(W*w){int choice=PokewalkerApp_GetTouchedButton(2);
 if(choice!=-1){ov112_021EC440(w,choice,ov112_021FF090[choice],14);PlaySE(1501);return 25;}
 if(gSystem.newKeys&64){if(w->index!=0){PlaySE(1500);--w->index;}PokewalkerApp_SetCursorPosition(w,ov112_021FF090[w->index]);}
 else if(gSystem.newKeys&128){if(w->index<2){PlaySE(1500);++w->index;}PokewalkerApp_SetCursorPosition(w,ov112_021FF090[w->index]);}
 else if(gSystem.newKeys&1){PlaySE(1501);ov112_021EC440(w,w->index,ov112_021FF090[w->index],14);return 25;}
 else if(gSystem.newKeys&2){PlaySE(1500);ov112_021EC440(w,2,3,14);return 25;}
 else if(ov112_021EB9C0()){PlaySE(1500);ov112_021EB90C(w,3);return 26;}
 else if(ov112_021EB9E4()){if(ov112_021EB950(w,3)){PlaySE(1500);return 26;}}
 return 13;}
