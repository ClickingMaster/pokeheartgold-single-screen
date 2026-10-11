#pragma force_active on
#include "global.h"
#include "palette.h"
#include "heap.h"
#include "sys_task.h"
typedef struct{void*bs,*effects,*a,*b;u32 unused;u8 state,timer,fade;}W;
extern void*BattleSystem_GetBgConfig(void*);extern PaletteData*BattleSystem_GetPaletteData(void*);extern int BattleSystem_GetTerrainId(void*);extern int BattleSystem_GetBackgroundId(void*);extern u32 BattleSystem_GetBattleType(void*);extern void*BattleSystem_GetMessageIcon(void*);
extern u32 ov12_0226D18C[];extern u8 ov12_0226D350[];
extern void*BattleAnimEmitterManager_New(int,int);extern void*BattleAnimEmitterManager_AddEmitter(void*,void*);extern void BattleAnimEmitter_Start(void*);extern int BattleAnimEmitter_IsActive(void*);extern void BattleAnimEmitterManager_Free(void*);extern void BattleAnimEmitterManager_StopAll(void*);
extern void PlaySE(int);extern void*ov12_0223BAE0(void*);extern void*ov12_0223BAEC(void*);extern void PaletteBlink_SetState(void*,int);
#define RESTORE() dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)0);MIi_CpuCopy16(ov12_0223BAE0(w->bs),dest,224);if(BattleSystem_GetBattleType(w->bs)==74){dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)2);MIi_CpuCopy16(ov12_0223BAEC(w->bs),dest,160);}else if(BattleSystem_GetBattleType(w->bs)&2){dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)2);MIi_CpuCopy16(ov12_0223BAEC(w->bs),dest,224);}else if(BattleSystem_GetBattleType(w->bs)&1){dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)2);MIi_CpuCopy16(ov12_0223BAEC(w->bs),dest,160);}else{dest=PaletteData_GetUnfadedBuf(pal,(PaletteBufferId)2);MIi_CpuCopy16(ov12_0223BAEC(w->bs),dest,128);}
extern void SysTask_Destroy(SysTask*);
void ov12_02260668(SysTask*,W*);
void ov12_02260668(SysTask*task,W*w){PaletteData*pal;int terrain;u32 color;int params[2];u16*dest;u16 i;BattleSystem_GetBgConfig(w->bs);pal=BattleSystem_GetPaletteData(w->bs);terrain=BattleSystem_GetTerrainId(w->bs);color=ov12_0226D18C[BattleSystem_GetBackgroundId(w->bs)];w->timer++;
 switch(w->state){case 0:w->effects=BattleAnimEmitterManager_New(5,0);terrain=ov12_0226D350[terrain];params[0]=terrain;params[1]=1;w->a=BattleAnimEmitterManager_AddEmitter(w->effects,params);params[0]=terrain+1;params[1]=1;w->b=BattleAnimEmitterManager_AddEmitter(w->effects,params);BattleAnimEmitter_Start(w->a);PlaySE(2127);w->state=1;
 case 1:if(w->timer==10){PaletteData_BeginPaletteFade(pal,1,0xf3ff,0,0,16,(u16)color);PaletteData_BeginPaletteFade(pal,4,0x3fff,0,0,16,(u16)color);}if(w->timer>=10&&w->fade<16){w->fade++;if(w->fade>16)w->fade=16;{u16*fill=PaletteData_GetFadedBuf(pal,(PaletteBufferId)1);for(i=0;i<256;i++){u16 level=(31*w->fade)>>4;fill[i]=level|(level<<5)|(level<<10);}}}
 if(w->timer==20)BattleAnimEmitter_Start(w->b);if(w->timer==23)PlaySE(2128);if(w->timer==28){RESTORE();PaletteData_BeginPaletteFade(pal,1,0xf3ff,0,16,0,(u16)color);PaletteData_BeginPaletteFade(pal,4,0x3fff,0,16,0,(u16)color);PaletteData_BeginPaletteFade(pal,10,0xffff,0,16,0,(u16)color);}if(w->timer>=50){PaletteData_BeginPaletteFade(pal,1,0xc00,0,16,0,0);w->state++;}break;
 case 2:if(!BattleAnimEmitter_IsActive(w->b)){BattleAnimEmitterManager_Free(w->effects);w->state++;}break;
 case 3:PaletteBlink_SetState(BattleSystem_GetMessageIcon(w->bs),0);Heap_Free(w);SysTask_Destroy(task);break;
 case 4:BattleAnimEmitterManager_StopAll(w->effects);BattleAnimEmitterManager_Free(w->effects);w->state++;
 case 5:RESTORE();PaletteData_BeginPaletteFade(pal,1,0xffff,0,0,0,0x7fff);PaletteData_BeginPaletteFade(pal,4,0x3fff,0,0,0,0xffff);PaletteData_BeginPaletteFade(pal,10,0xffff,0,0,0,0x7fff);w->state=3;break;}}
