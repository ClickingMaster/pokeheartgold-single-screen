#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0xc];u32 heap;u8 gap;u8 slot;u8 rest[0x10];u16 item;} Head;
typedef struct {u16 move;u8 pad[6];} Move;
typedef struct {Move moves[4];u8 pad[48];} Row;
typedef struct {Head *head;u8 pad[0x30];Row rows[6];u8 rest[0x1e5c];u8 *windows;} Context;
extern const u32 ov08_02224FE0[];
extern void FillWindowPixelBuffer(void *,u8),ScheduleWindowCopyToVram(void *),ov08_0221DDCC(Context *,s32,s32,u32,s32,s32),ov08_0221E244(Context *,u32,u32,u32,s32,s32,u32),ov08_0221F284(Context *,Move *,u32),ov08_0221E340(Context *,s32);
extern u32 GetItemAttr(u16,s32,u32);
void ov08_0221FF70(Context *p);
#pragma force_active on
void ov08_0221FF70(Context *p){u32 i;Row *row;FillWindowPixelBuffer(p->windows,0);FillWindowPixelBuffer(p->windows+16,0);FillWindowPixelBuffer(p->windows+32,0);FillWindowPixelBuffer(p->windows+48,0);FillWindowPixelBuffer(p->windows+64,0);ov08_0221DDCC(p,0,0,p->head->slot,0,0);for(i=0;i<4;i++){row=&p->rows[p->head->slot];if(row->moves[i].move){ov08_0221E244(p,row->moves[i].move,i+1,ov08_02224FE0[i],4,7,0x70809);ov08_0221F284(p,&row->moves[i],i+1);}}if(GetItemAttr(p->head->item,37,p->head->heap)==0)ov08_0221E340(p,94);ScheduleWindowCopyToVram(p->windows+16);ScheduleWindowCopyToVram(p->windows+32);ScheduleWindowCopyToVram(p->windows+48);ScheduleWindowCopyToVram(p->windows+64);}
