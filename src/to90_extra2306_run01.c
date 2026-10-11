#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0xc];u32 heap;u8 gap;u8 slot;u8 rest[0x10];u16 item;} Head;
typedef struct {u16 move;u8 pad[6];} Move;
typedef struct {Move moves[4];u8 pad[48];} Row;
typedef struct {Head *head;u8 pad[0x30];Row rows[6];u8 rest[0x1e5c];u8 *windows;} Context;
extern const u32 sBattlePartyMoveNameMsgIds[];
extern void FillWindowPixelBuffer(void *,u8),ScheduleWindowCopyToVram(void *),BattlePartyMenu_PrintMonName(Context *,s32,s32,u32,s32,s32),BattlePartyMenu_PrintMoveName(Context *,u32,u32,u32,s32,s32,u32),BattlePartyMenu_PrintMoveListPP(Context *,Move *,u32),BattlePartyMenu_PrintPrompt(Context *,s32);
extern u32 GetItemAttr(u16,s32,u32);
void BattlePartyMenu_PrintRestorePPPage(Context *p);
#pragma force_active on
void BattlePartyMenu_PrintRestorePPPage(Context *p){u32 i;Row *row;FillWindowPixelBuffer(p->windows,0);FillWindowPixelBuffer(p->windows+16,0);FillWindowPixelBuffer(p->windows+32,0);FillWindowPixelBuffer(p->windows+48,0);FillWindowPixelBuffer(p->windows+64,0);BattlePartyMenu_PrintMonName(p,0,0,p->head->slot,0,0);for(i=0;i<4;i++){row=&p->rows[p->head->slot];if(row->moves[i].move){BattlePartyMenu_PrintMoveName(p,row->moves[i].move,i+1,sBattlePartyMoveNameMsgIds[i],4,7,0x70809);BattlePartyMenu_PrintMoveListPP(p,&row->moves[i],i+1);}}if(GetItemAttr(p->head->item,37,p->head->heap)==0)BattlePartyMenu_PrintPrompt(p,94);ScheduleWindowCopyToVram(p->windows+16);ScheduleWindowCopyToVram(p->windows+32);ScheduleWindowCopyToVram(p->windows+48);ScheduleWindowCopyToVram(p->windows+64);}
