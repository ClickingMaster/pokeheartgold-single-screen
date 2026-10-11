#pragma force_active on
#include "global.h"
#define P(d,n) ((char*)(d)+(n))
#define W(d,n) (*(void**)P(d,n))
#define I(d,n) (*(int*)P(d,n))
#define B(d,n) (*(u8*)P(d,n))
extern u8 gSystem[],sButtonNavTable[],ov15_02200641[],ov15_02200642[],ov15_02200643[];
int BagApp_PressButton(void*,int,u8*,int,int,int);int BagApp_GetAdjacentPocket(void*,int,int);void BagApp_SetCursorPosition(void*,int);void PlaySE(int);
void BagApp_SetPocketPositionFromButton(void*,int);void BagApp_PrintCursorDescription(void*);int BagApp_GetTouchedButton(void*);int BagApp_ShouldMoveCursorToButton(void*,int);int BagApp_FindPocketIndex(void*,int);void BagApp_SetMenuInputState(void*,int);int BagApp_StartButtonAnim(void*,int,int,int);
int BagApp_State_HandleInput(void*);
int BagApp_State_HandleInput(void*d){int changed=0,result,target;u8 flags[5];
 if(I(gSystem,0x4c)&0x40){I(d,0x644)=sButtonNavTable[I(d,0x644)*4];changed++;}
 else if(I(gSystem,0x4c)&0x80){I(d,0x644)=ov15_02200641[I(d,0x644)*4];changed++;}
 else if(I(gSystem,0x4c)&0x20){target=ov15_02200642[I(d,0x644)*4];
 if(target==14)return BagApp_PressButton(d,14,&flags[4],1,2,0);
 if(I(d,0x644)!=16){if(I(d,0x644)>=0&&I(d,0x644)<8){result=BagApp_GetAdjacentPocket(d,I(d,0x644),-1);if(I(d,0x644)!=result){I(d,0x644)=result;changed++;}}
 else{I(d,0x644)=target;changed++;}}
 }
 else if(I(gSystem,0x4c)&0x10){target=ov15_02200643[I(d,0x644)*4];
 if(target==15)return BagApp_PressButton(d,15,&flags[3],1,2,0);
 if(I(d,0x644)!=16){if(I(d,0x644)>=0&&I(d,0x644)<8){result=BagApp_GetAdjacentPocket(d,I(d,0x644),1);if(I(d,0x644)!=result){I(d,0x644)=result;changed++;}}
 else{I(d,0x644)=target;changed++;}}
 }
 else if(I(gSystem,0x48)&0x200){
 if(I(d,0x644)>=0&&I(d,0x644)<8){I(d,0x644)=BagApp_GetAdjacentPocket(d,B(W(d,0x234),0x64),-1);BagApp_SetCursorPosition(d,I(d,0x644));return BagApp_PressButton(d,I(d,0x644),&flags[2],1,2,changed);}
 else return BagApp_PressButton(d,BagApp_GetAdjacentPocket(d,B(W(d,0x234),0x64),-1),&flags[2],1,2,0);
 }
 else if(I(gSystem,0x48)&0x100){
 if(I(d,0x644)>=0&&I(d,0x644)<8){I(d,0x644)=BagApp_GetAdjacentPocket(d,B(W(d,0x234),0x64),1);BagApp_SetCursorPosition(d,I(d,0x644));return BagApp_PressButton(d,I(d,0x644),&flags[1],1,2,changed);}
 else return BagApp_PressButton(d,BagApp_GetAdjacentPocket(d,B(W(d,0x234),0x64),1),&flags[1],1,2,0);
 }
 if(I(d,0x644)==17)I(d,0x644)=B(W(d,0x234),0x64);
 if(changed){PlaySE(0x5dc);BagApp_SetCursorPosition(d,I(d,0x644));BagApp_SetPocketPositionFromButton(d,I(d,0x644));BagApp_PrintCursorDescription(d);}
 flags[0]=0;result=BagApp_GetTouchedButton(d);
 if(result!=-1){
 if(BagApp_ShouldMoveCursorToButton(d,result)){
 if((u32)result<8){if(BagApp_FindPocketIndex(d,result)!=-1){I(d,0x644)=result;BagApp_SetCursorPosition(d,I(d,0x644));}}
 else{I(d,0x644)=result;BagApp_SetCursorPosition(d,I(d,0x644));if(I(d,0x644)>=8&&I(d,0x644)<=13)BagApp_SetPocketPositionFromButton(d,I(d,0x644));}
 }
 result=BagApp_PressButton(d,result,&flags[0],1,2,1);if(result!=1)return result;
 }else if(I(gSystem,0x48)&1){result=BagApp_PressButton(d,I(d,0x644),&flags[0],1,2,0);if(I(d,0x644)>=8&&I(d,0x644)<=13)BagApp_SetPocketPositionFromButton(d,I(d,0x644));if(result!=1)return result;}
 else if(I(gSystem,0x48)&2){result=BagApp_PressButton(d,16,&flags[0],1,2,0);BagApp_SetMenuInputState(d,0);if(I(d,0x644)>=8&&I(d,0x644)<=13)BagApp_SetPocketPositionFromButton(d,I(d,0x644));if(result!=1)return result;}
 if(flags[0]==1)return BagApp_StartButtonAnim(d,20,41,27);return 1;
}
