#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern void PCBox_CloseMessageWindow(void*,int),PCBox_SetSpriteVisible(void*,int,int),PCBox_SetIconsTranslucent(void*,int,int),PCBox_CloseBoxTray(void*,int);void ov14_021F2734(u8*);
void ov14_021F2734(u8*p){PCBox_CloseMessageWindow(V(p,52),39);PCBox_SetSpriteVisible(V(p,52),9,0);PCBox_SetIconsTranslucent(p,1,1);PCBox_CloseBoxTray(p,63);}
