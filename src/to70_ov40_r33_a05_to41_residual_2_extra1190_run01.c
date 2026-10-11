#pragma force_active on
#include "global.h"
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
#define U8(p,o) (*(u8*)((u8*)(p)+(o)))
#define U16(p,o) (*(u16*)((u8*)(p)+(o)))
#define S16(p,o) (*(s16*)((u8*)(p)+(o)))
typedef struct {u32 a,b,c,d;} Result;
extern int ov40_0223D5CC(void*),ov39_022274B4(void*),GlobalTerminalNet_GetError(void*,Result**),GlobalTerminalNet_RequestRankings(void*,void*,void*),ov40_0222FBB4(void*),ov40_0222DA84(void*,int),ov40_0222DA00(void*,void*,int,int),ov39_022276A4(void*),ov39_022276E0(void*),ov39_02227648(void*),ov39_022275E8(void*),sub_02087E1C(void*);
extern void *ov40_0223D540(void*),*Save_PlayerData_GetIGTAddr(void*);
extern int GetIGTHours(void*),GetIGTMinutes(void*),GetIGTSeconds(void*),ov40_02239538(void*,int);
extern void ov40_0223077C(void*,void*,int,int),OrbitCursor_SetVisible(void*,int),OrbitCursor_SetRadius(void*,int,int),ov40_0222DED0(void*,int),PlaySE(int),StopSE(int,int),ov40_0222DFB0(void*),ov40_02230CDC(void*,int,u32,u32),ov40_02230964(void*,int),ov40_022398F8(void*),TouchHitboxController_Destroy(void*),ov40_0222DAA8(void*),ov40_0222D88C(void*),BgClearTilemapBufferAndCommit(void*,u8),ov40_0222FB90(void*,int),ov40_0222DD08(void*),PaletteData_BlendPalettes(void*,int,int,u8,u16),ov40_0222BF64(void*,int,int,u32),ov40_0222BF80(void*,int),Heap_Free(void*),ov40_02238FF4(void*),ov40_0223B480(void*),sub_0203A948(int,int);
int ov40_0223B75C(u8*);
int ov40_0223B75C(u8 *w){u8 *s=PTR(w,0x860);Result *result;
if(!ov40_0223D5CC(w))return 0;
switch(U32(w,8)){
case 0:ov40_0223077C(w,PTR(w,0x6f0),128,96);OrbitCursor_SetVisible(PTR(w,0x6f0),1);OrbitCursor_SetRadius(PTR(w,0x6f0),24,24);if((u32)(U32(w,0x86c)-300)<=1)ov40_0222DED0(w,279);else ov40_0222DED0(w,292);PlaySE(1405);U32(w,8)++;break;
case 1:if(U32(w,0x86c)==300){U32(s,12)=0;if(ov39_022276A4(ov40_0223D540(w))==1)U32(w,8)++;}else if(U32(w,0x86c)==301){U32(s,12)=1;if(ov39_022276E0(ov40_0223D540(w))==1)U32(w,8)++;}else if(U32(w,0x86c)==200){U32(s,12)=2;if(sub_02087E1C(w)==1){if(ov39_02227648(ov40_0223D540(w))==1)U32(w,8)++;}else if(ov39_022275E8(ov40_0223D540(w))==1)U32(w,8)++;}else GF_AssertFail();break;
case 2:ov40_0222DFB0(w);if(GlobalTerminalNet_GetError(ov40_0223D540(w),&result)==1){StopSE(1405,0);ov40_02230CDC(w,7,result->d,result->b);U32(w,8)++;}else{StopSE(1405,0);PlaySE(1399);if(U32(w,0x4138)==0){ov40_02230CDC(w,5,0,0);U32(w,0x510)=U32(s,12)==2?118:300;OrbitCursor_SetVisible(PTR(w,0x6f0),0);U32(w,8)++;}else U32(w,8)=255;}break;
case 3:OrbitCursor_SetVisible(PTR(w,0x6f0),0);OrbitCursor_SetRadius(PTR(w,0x6f0),0,0);U32(w,8)++;break;
case 4:ov40_0222DA84(s+8,1);if(ov40_0222DA00(s,s+4,1,0))U32(w,8)++;PaletteData_BlendPalettes(PTR(w,0x28),3,12,U32(s,8),U32(w,0x58));break;
case 5:ov40_02230964(w,1);ov40_0223B480(w);ov40_0222D88C(w);ov40_02230964(w,0);sub_0203A948(1,109);ov40_0222FB90(w,1);U32(w,8)++;break;
case 6:if(ov40_0222FBB4(w)){if(ov40_0222DA84(s+8,0)){ov40_0222DD08(w);ov40_0222DAA8(s+8);PaletteData_BlendPalettes(PTR(w,0x28),2,12,16,U32(w,0x58));ov40_0222BF64(w,1,1,U32(w,0x10));ov40_0222BF80(w,5);BgClearTilemapBufferAndCommit(PTR(w,0x24),2);BgClearTilemapBufferAndCommit(PTR(w,0x24),6);BgClearTilemapBufferAndCommit(PTR(w,0x24),3);BgClearTilemapBufferAndCommit(PTR(w,0x24),7);Heap_Free(s);}else{PaletteData_BlendPalettes(PTR(w,0x28),1,2,U32(s,8),U32(w,0x58));PaletteData_BlendPalettes(PTR(w,0x28),3,12,U32(s,8),U32(w,0x58));}}break;
default:OrbitCursor_SetVisible(PTR(w,0x6f0),0);OrbitCursor_SetRadius(PTR(w,0x6f0),0,0);ov40_0222BF80(w,3);break;}return 0;}
