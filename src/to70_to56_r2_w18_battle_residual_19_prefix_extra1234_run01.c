#pragma force_active on
#include "global.h"
#include "palette.h"
#include "party.h"
#include "pokemon.h"
#include "heap.h"
#include "sys_task.h"
typedef struct{Party*party;u32 unknown;void*bs;u32 heap;u8 unused,mode,param12,p13,selectionA,selectionB;u8 pad[12];u16 param22,param24;u16 p26;u32 battler;u8 order[6],key,p33,p34,param35,done;}Menu;
typedef struct{void*bs;Menu*menu;u8 arg,battler,state,kind;u8 selected[4];u32 param10;u16 param14;u8 param16,key,mask;u8 pad[3];u8 order[4][6];}W;
extern PaletteData*BattleSystem_GetPaletteData(void*);extern void*BattleSystem_GetWindow(void*,int);extern void FillWindowPixelBuffer(void*,u8);extern void CopyWindowPixelsToVram_TextMode(void*);extern void*BattleSystem_GetBattleInput(void*);extern u8 BattleInput_GetKeyPressed(void*);extern void BattleInput_SetKeyPressed(void*,int);extern void*BattleSystem_GetMessageIcon(void*);extern void sub_0201649C(void*,int);
extern void ov12_02237B0C(void*);extern u32 BattleSystem_GetBattleType(void*);extern int ov12_0223AB0C(void*,int);extern int BattleSystem_GetBattlerIdPartner(void*,int);extern int BattleSystem_GetPartySize(void*,int);extern Pokemon*BattleSystem_GetPartyMon(void*,int,int);extern Party*BattleSystem_GetParty(void*,int);extern u32 MaskOfFlagNo(int);extern void ov10_0221BE20(Menu*);extern void ov12_02237BB8(void*);extern void BattleController_ReplyPartySelection(void*,int,int);extern void BattleController_EmitCommandComplete(void*,int,int);extern void SysTask_Destroy(SysTask*);
void ov12_0225F4E0(SysTask*,W*);
void ov12_0225F4E0(SysTask*task,W*w){PaletteData*pal=BattleSystem_GetPaletteData(w->bs);int a,b,i,idx;Pokemon*mon;Party*party;
 switch(w->state){case 0:{void*window=BattleSystem_GetWindow(w->bs,0);FillWindowPixelBuffer(window,255);CopyWindowPixelsToVram_TextMode(window);}w->key=BattleInput_GetKeyPressed(BattleSystem_GetBattleInput(w->bs));sub_0201649C(BattleSystem_GetMessageIcon(w->bs),1);PaletteData_BeginPaletteFade(pal,5,0xc00,-8,0,7,0);PaletteData_BeginPaletteFade(pal,10,0xffff,-8,0,16,0);w->state++;break;
 case 1:if(PaletteData_GetSelectedBuffersBitmask(pal))break;ov12_02237B0C(w->bs);w->menu=Heap_Alloc((enum HeapID)5,56);w->menu->party=SaveArray_Party_Alloc((enum HeapID)5);
 if((BattleSystem_GetBattleType(w->bs)&12)==12||BattleSystem_GetBattleType(w->bs)==203){if(ov12_0223AB0C(w->bs,w->battler)==2){a=w->battler;b=BattleSystem_GetBattlerIdPartner(w->bs,a);}else{a=BattleSystem_GetBattlerIdPartner(w->bs,w->battler);b=w->battler;}
 mon=AllocMonZeroed((enum HeapID)5);for(i=0;i<6;i++)Party_AddMon(w->menu->party,mon);Heap_Free(mon);
 i=0;if(BattleSystem_GetPartySize(w->bs,a)>0){W*order=(W*)((u8*)w+6*a);idx=0;do{mon=BattleSystem_GetPartyMon(w->bs,a,order->order[0][0]);CopyPokemonToPokemon(mon,Party_GetMonByIndex(w->menu->party,idx));w->menu->order[idx]=order->order[0][0];order=(W*)((u8*)order+1);idx+=2;i++;}while(i<BattleSystem_GetPartySize(w->bs,a));}
 i=0;if(BattleSystem_GetPartySize(w->bs,b)>0){int destIndex,byteIndex;W*order=(W*)((u8*)w+6*b);destIndex=1;byteIndex=0;do{mon=BattleSystem_GetPartyMon(w->bs,b,order->order[0][0]);CopyPokemonToPokemon(mon,Party_GetMonByIndex(w->menu->party,destIndex));w->menu->order[byteIndex+1]=order->order[0][0];destIndex+=2;order=(W*)((u8*)order+1);byteIndex+=2;i++;}while(i<BattleSystem_GetPartySize(w->bs,b));}
 if(ov12_0223AB0C(w->bs,w->battler)==4)w->menu->mode=1;else w->menu->mode=0;
 }else{if((BattleSystem_GetBattleType(w->bs)&2)&&!(BattleSystem_GetBattleType(w->bs)&8))a=w->battler&1;else a=w->battler;w->menu->mode=ov12_0223AB0C(w->bs,w->battler)==4?1:0;party=BattleSystem_GetParty(w->bs,w->battler);for(i=0;i<Party_GetCount(party);i++){Party_AddMon(w->menu->party,BattleSystem_GetPartyMon(w->bs,a,w->order[a][i]));w->menu->order[i]=w->order[a][i];}}
 w->menu->bs=w->bs;w->menu->heap=5;w->menu->done=0;w->menu->param24=w->param10;w->menu->param12=w->param16;w->menu->param35=w->kind;w->menu->param22=w->param14;w->menu->battler=w->battler;w->menu->key=w->key;
 if(!(MaskOfFlagNo(w->battler)&w->mask))w->menu->selectionA=w->selected[w->battler];else w->menu->selectionA=6;
 if(BattleSystem_GetBattleType(w->bs)&8)w->menu->selectionB=6;else if(!(MaskOfFlagNo(BattleSystem_GetBattlerIdPartner(w->bs,w->battler))&w->mask))w->menu->selectionB=w->selected[BattleSystem_GetBattlerIdPartner(w->bs,w->battler)];else w->menu->selectionB=6;
 ov10_0221BE20(w->menu);w->state++;break;
 case 2:if(!w->menu->done)break;ov12_02237BB8(w->bs);BattleInput_SetKeyPressed(BattleSystem_GetBattleInput(w->bs),w->menu->key);PaletteData_BeginPaletteFade(pal,5,0xc00,-8,7,0,0);PaletteData_BeginPaletteFade(pal,10,0xffff,-8,16,0,0);w->state++;break;
 case 3:if(PaletteData_GetSelectedBuffersBitmask(pal))break;sub_0201649C(BattleSystem_GetMessageIcon(w->bs),0);if(w->menu->mode==6)BattleController_ReplyPartySelection(w->bs,w->battler,255);else BattleController_ReplyPartySelection(w->bs,w->battler,w->menu->order[w->menu->mode]+1);BattleController_EmitCommandComplete(w->bs,w->battler,w->arg);Heap_Free(w->menu->party);Heap_Free(w->menu);Heap_Free(w);SysTask_Destroy(task);break;}}
