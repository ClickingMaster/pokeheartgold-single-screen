#pragma force_active on
#include "global.h"
#pragma force_active on
struct BitFlag{u8 bit:1,rest:7;};
typedef struct{u8 padding[0x36a];u16 values[7];u16*dest;u16*kind;u8 gap[0x1f];u8 index;struct BitFlag flags;}State;
#define B(p,o) (((u8*)(p))[o])
#define H(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u16*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern s16 ov87_021E82E4[][2];extern u8 ov87_021E8308[],ov87_021E8184[],ov87_021E818C[];
void ov87_021E7A2C(void*),ov87_021E7334(void*),ov87_021E74B8(void*),ov87_021E74D4(void*);void ov87_021E7324(void*,int),ov87_021E734C(void*,int),ov87_021E7FEC(void*,int,int),ov87_021E7FD4(void*,int),ov87_021E8078(void*,int),ov87_021E803C(void*,int),ov87_021E7460(void*,int,u8,int,int);
void BG_LoadCharTilesData(void*,int,void*,u32,int),BgCommitTilemapBufferToVram(void*,int),BgSetPosTextAndCommit(void*,int,int,int),GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),PlaySE(int),StopSE(int,int);int IsSEPlaying(int);
void ClearWindowTilemapAndCopyToVram(void*),FillWindowPixelBuffer(void*,int),ScheduleWindowCopyToVram(void*),sub_0200E5D4(void*,int),sub_02021280(void*,int,int);
void ov87_021E71B4(void*),ov87_021E717C(void*),ov87_021E708C(void*),ov87_021E7550(void*),ov87_021E7734_dummy(void*),ov87_021E70D0(void*),ov87_021E7134(void*),ov87_021E7698(void*),ov87_021E7008(void*),ov87_021E6AE0(void*),ov87_021E6AF4(void*),ov87_021E7998(void*),ov87_021E79A0(void*),ov87_021E6B28(void*),ov87_021E7990(void*);void ov87_021E71EC(void*,u8),ov87_021E75E0(void*,u8),ov87_021E74F4(void*,int),ov87_021E8084(void*,int);u32 ov87_021E7490(void*);int ov87_021E7734(void*),ov87_021E788C(void*),ov87_021E78D8(void*,int),ov87_021E80B4(void*),YesNoPrompt_HandleInputForSave(void*),System_GetTouchHeld(void);int TouchscreenHitbox_FindRectAtTouchHeld(const void*),TouchscreenHitbox_FindRectAtTouchNew(const void*);
int ov87_021E5E00(void*),ov87_021E6080(void*);
int ov87_021E5E00(void*p){int i;switch(B(p,8)){case 0:ov87_021E7A2C(p);BG_LoadCharTilesData(P(p,0x58),0,P(p,0x38c),*(u32*)((u8*)P(p,0x388)+16),0);BgCommitTilemapBufferToVram(P(p,0x58),0);H(p,18)=256;BgSetPosTextAndCommit(P(p,0x58),6,0,H(p,18));BgSetPosTextAndCommit(P(p,0x58),0,0,H(p,18));BgSetPosTextAndCommit(P(p,0x58),1,0,H(p,18));GfGfx_EngineATogglePlanes(1,1);GfGfx_EngineATogglePlanes(2,1);GfGfx_EngineBTogglePlanes(4,1);H(p,16)=-256;B((u8*)p+B(p,14),25)=0;for(i=0;i<0x9600;i++)B((u8*)((u32)p+i),0x3fa)=0;B(p,0x3a1)=0;ov87_021E7334(p);ov87_021E7324(p,B((u8*)p+B(p,14),25));ov87_021E734C(p,B((u8*)p+B(p,14),25));ov87_021E74B8(p);ov87_021E74D4(p);B(p,8)=1;break;
case 1:{int j=0,y=26;u8*q=p;for(;j<4;j++){ov87_021E7FEC(P(q,0x2f8),H(p,16)+60,y);ov87_021E7FD4(P(q,0x2f8),1);y+=42;q+=4;}{int k=0;q=p;for(;k<3;k++){ov87_021E8078(P(q,0x344),1);q+=4;}}{int k=0;const s16*pos=&ov87_021E82E4[0][0];B(p,0x39d)=k;B(p,0x3a0)&=~1;B(p,0x3a2)=177;B(p,0x3a3)=178;B(p,0x3a4)=179;q=p;for(;k<9;k++){ov87_021E7FEC(P(q,0x308),H(p,16)+pos[0],pos[1]);ov87_021E7FD4(P(q,0x308),1);ov87_021E803C(P(q,0x308),B((u8*)((u32)p+k),0x360));pos+=2;q+=4;}}ov87_021E7460(p,1,B((u8*)p+B(p,14),21)+4,32,24);ov87_021E7460(p,6,B((u8*)p+B(p,14),21)+1,32,24);PlaySE(1376);B(p,8)=2;break;}
case 2:BgSetPosTextAndCommit(P(p,0x58),6,0,H(p,18));BgSetPosTextAndCommit(P(p,0x58),0,0,H(p,18));BgSetPosTextAndCommit(P(p,0x58),1,0,H(p,18));H(p,18)-=16;if(H(p,16)>=0)B(p,8)=3;else{int y;u8*q;const s16*pos;i=0;y=26;q=p;for(;i<4;i++){ov87_021E7FEC(P(q,0x2f8),H(p,16)+60,y);y+=42;q+=4;}pos=&ov87_021E82E4[0][0];i=0;q=p;for(;i<9;i++){ov87_021E7FEC(P(q,0x308),H(p,16)+pos[0],pos[1]);pos+=2;q+=4;}}H(p,16)+=16;break;
case 3:H(p,16)=0;H(p,18)=0;return 1;}return 0;}
