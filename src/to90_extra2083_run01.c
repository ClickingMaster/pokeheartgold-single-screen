#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define S(p,n) (*(s8*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
typedef struct {u8 padding[0x264];u32 count;} Counter;
extern u8 gSystem[],ov86_021E7E9C[];
int GridInputHandler_HandleInput_NoHold(void*),GridInputHandler_GetNextInput(void*),TouchscreenHitbox_FindRectAtTouchNew(const void*);
void PlaySE(int);void ov86_021E71C0(void*),ov86_021E7B68(void*),ov86_021E7CF8(void*),ov86_021E78B8(void*,int),ov86_021E792C(void*,int,int);
static inline void Prev(void*p){S(p,0x25c)--;if(S(p,0x25c)<0)S(p,0x25c)=S(p,0x25d);}
static inline void Next(void*p){S(p,0x25c)++;if(S(p,0x25c)>S(p,0x25d))S(p,0x25c)=0;}
int ov86_021E7984(void*);
int ov86_021E7984(void*p){int input=GridInputHandler_HandleInput_NoHold(P(p,0x254));switch((u32)input){case 8:case -2:PlaySE(0x5dd);ov86_021E71C0(p);B(p,5)=2;return 9;case -3:PlaySE(0x5dc);break;case -4:break;case -1:switch(GridInputHandler_GetNextInput(P(p,0x254))){case 0:case 2:case 4:case 6:if((W(gSystem,0x48)&32)&&S(p,0x25d)!=0){Prev(p);PlaySE(0x5dc);ov86_021E7B68(p);ov86_021E7CF8(p);}break;case 1:case 3:case 5:case 7:if((W(gSystem,0x48)&16)&&S(p,0x25d)!=0){Next(p);PlaySE(0x5dc);ov86_021E7B68(p);ov86_021E7CF8(p);}break;}break;default:if((u32)(input+S(p,0x25c)*8)>=((Counter*)((u8*)p+H(p,0x258)*8))->count){PlaySE(0x5f2);break;}H(p,0x25a)=input;PlaySE(0x5dd);ov86_021E78B8(p,input);B(p,5)=7;return 9;}switch(TouchscreenHitbox_FindRectAtTouchNew(ov86_021E7E9C)){case 0:if(S(p,0x25d)!=0){Prev(p);PlaySE(0x5e0);ov86_021E792C(p,0x22,0x34);B(p,5)=6;return 9;}break;case 1:if(S(p,0x25d)!=0){Next(p);PlaySE(0x5e0);ov86_021E792C(p,0x26,0x34);B(p,5)=6;return 9;}break;}return 5;}
