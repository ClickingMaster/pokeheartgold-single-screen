#pragma force_active on
#include "global.h"
#include "palette.h"
#include "heap.h"
#include "sys_task.h"
typedef struct{void*bs,*effects,*a,*b;u32 unused;u8 state,timer,fade;}W;
extern void*BattleSystem_GetBgConfig(void*);extern PaletteData*BattleSystem_GetPaletteData(void*);extern int BattleSystem_GetTerrainId(void*);extern int BattleSystem_GetBackgroundId(void*);extern u32 BattleSystem_GetBattleType(void*);extern void*BattleSystem_GetMessageIcon(void*);
extern u32 ov12_0226D18C[];extern u8 ov12_0226D350[];
extern void*ov07_0223458C(int,int);extern void*ov07_022345C8(void*,void*);extern void ov07_02234694(void*);extern int ov07_022346BC(void*);extern void ov07_02234604(void*);extern void ov07_02234628(void*);
extern void PlaySE(int);extern void*ov12_0223BAE0(void*);extern void*ov12_0223BAEC(void*);extern void sub_0201649C(void*,int);
#define RESTORE() dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)0);MIi_CpuCopy16(ov12_0223BAE0(w->bs),dest,224);if(BattleSystem_GetBattleType(w->bs)==74){dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)2);MIi_CpuCopy16(ov12_0223BAEC(w->bs),dest,160);}else if(BattleSystem_GetBattleType(w->bs)&2){dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)2);MIi_CpuCopy16(ov12_0223BAEC(w->bs),dest,224);}else if(BattleSystem_GetBattleType(w->bs)&1){dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)2);MIi_CpuCopy16(ov12_0223BAEC(w->bs),dest,160);}else{dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)2);MIi_CpuCopy16(ov12_0223BAEC(w->bs),dest,128);}
extern void SysTask_Destroy(SysTask*);
void ov12_02260668(SysTask*,W*);
void ov12_02260668(SysTask*task,W*w){PaletteData*pal;int terrain;u32 color;int params[2];u16*dest;u16 i;BattleSystem_GetBgConfig(w->bs);pal=BattleSystem_GetPaletteData(w->bs);terrain=BattleSystem_GetTerrainId(w->bs);color=ov12_0226D18C[BattleSystem_GetBackgroundId(w->bs)];w->timer++;
 switch(w->state){case 0:w->effects=ov07_0223458C(5,0);terrain=ov12_0226D350[terrain];params[0]=terrain;params[1]=1;w->a=ov07_022345C8(w->effects,params);params[0]=terrain+1;params[1]=1;w->b=ov07_022345C8(w->effects,params);ov07_02234694(w->a);PlaySE(2127);w->state=1;
 case 1:if(w->timer==10){PaletteData_BeginPaletteFade(pal,1,0xf3ff,0,0,16,(u16)color);PaletteData_BeginPaletteFade(pal,4,0x3fff,0,0,16,(u16)color);}if(w->timer>=10&&w->fade<16){w->fade++;if(w->fade>16)w->fade=16;{u16*fill=PaletteData_GetFadedBuf(pal,(PaletteBufferId)1);for(i=0;i<256;i++){u16 level=(31*w->fade)>>4;fill[i]=level|(level<<5)|(level<<10);}}}
 if(w->timer==20)ov07_02234694(w->b);if(w->timer==23)PlaySE(2128);if(w->timer==28){RESTORE();PaletteData_BeginPaletteFade(pal,1,0xf3ff,0,16,0,(u16)color);PaletteData_BeginPaletteFade(pal,4,0x3fff,0,16,0,(u16)color);PaletteData_BeginPaletteFade(pal,10,0xffff,0,16,0,(u16)color);}if(w->timer>=50){PaletteData_BeginPaletteFade(pal,1,0xc00,0,16,0,0);w->state++;}break;
 case 2:if(!ov07_022346BC(w->b)){ov07_02234604(w->effects);w->state++;}break;
 case 3:sub_0201649C(BattleSystem_GetMessageIcon(w->bs),0);Heap_Free(w);SysTask_Destroy(task);break;
 case 4:ov07_02234628(w->effects);ov07_02234604(w->effects);w->state++;
 case 5:RESTORE();PaletteData_BeginPaletteFade(pal,1,0xffff,0,0,0,0x7fff);PaletteData_BeginPaletteFade(pal,4,0x3fff,0,0,0,0xffff);PaletteData_BeginPaletteFade(pal,10,0xffff,0,0,0,0x7fff);w->state=3;break;}}
