#pragma force_active on
#include "global.h"
#pragma force_active on
int ov07_0221BFD0(void *);
int ov07_0221FB04(void *,int);
void SetBgPriority(u8,u8);
extern const int ov07_0223680C[],ov07_02236810[],ov07_02236814[];
void GfGfxLoader_LoadCharData(int,int,void *,int,int,int,int,int);
void GfGfxLoader_LoadScrnData(int,int,void *,int,int,int,int,int);
void PaletteData_LoadNarc(void *,int,int,int,int,int,int);
void BgClearTilemapBufferAndCommit(void *,u8);
void ov07_02231524(u8 *,int);
#define P(p,o) (*(void **)((p)+(o)))
#define W(p,o) (*(int *)((p)+(o)))
void ov07_02231524(u8 *w,int bg){
 int heap=ov07_0221BFD0(P(w,0x1c));
 SetBgPriority((u8)bg,(u8)ov07_0221FB04(P(w,0x1c),3));
 GfGfxLoader_LoadCharData(W(w,0),ov07_0223680C[W(w,4)*4],P(w,0x2c),bg,0,0,1,heap);
 PaletteData_LoadNarc(P(w,0x30),W(w,0),ov07_02236810[W(w,4)*4],heap,0,32,128);
 BgClearTilemapBufferAndCommit(P(w,0x2c),(u8)bg);
 GfGfxLoader_LoadScrnData(W(w,0),ov07_02236814[W(w,4)*4],P(w,0x2c),bg,0,0,1,heap);
}
