#pragma force_active on
#include "global.h"
#pragma force_active on
void ScheduleBgTilemapBufferTransfer(void *,int),FillWindowPixelBuffer(void *,int),BgClearTilemapBufferAndCommit(void *,int),ScheduleWindowCopyToVram(void *);
void *Save_PlayerData_GetProfile(void *);void BufferPlayersName(void *,int,void *);void MapID_GetLandmarkName(int,int,void *);
void BufferString(void *,int,void *,int,int,int),CopyU16ArrayToString(void *,const u16 *),BufferIntegerAsString(void *,int,int,int,int,int);
u32 ov109_021E7850(void *);void StringExpandPlaceholders(void *,void *,void *),AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,int),ov109_021E7114(void *,u8,int,int,int);
void ov109_021E7248(u8 *,u8 *);
#define P(w,o) (*(void **)((w)+(o)))
#define W(w,o) (*(u32 *)((w)+(o)))
void ov109_021E7248(u8 *w,u8 *r){ScheduleBgTilemapBufferTransfer(P(w,20),6);ScheduleBgTilemapBufferTransfer(P(w,20),7);FillWindowPixelBuffer(w+112,0);BgClearTilemapBufferAndCommit(P(w,20),4);BgClearTilemapBufferAndCommit(P(w,20),7);if(!r){ScheduleWindowCopyToVram(w+112);return;}
 BufferPlayersName(P(w,40),0,Save_PlayerData_GetProfile(*(void **)((u8 *)P(w,16)+12)));
 MapID_GetLandmarkName(*(u16 *)(r+50),W(w,0),P(w,44));BufferString(P(w,40),1,P(w,44),2,0,2);CopyU16ArrayToString(P(w,44),(u16 *)(r+24));BufferString(P(w,40),2,P(w,44),2,0,2);
 BufferIntegerAsString(P(w,40),3,2000+(u8)(W(r,56)>>24),4,2,1);BufferIntegerAsString(P(w,40),4,(u8)(W(r,56)>>16),2,2,1);BufferIntegerAsString(P(w,40),5,(u8)(W(r,56)>>8),2,2,1);
 if(ov109_021E7850(r)>1)StringExpandPlaceholders(P(w,40),P(w,44),P(w,76));else StringExpandPlaceholders(P(w,40),P(w,44),P(w,72));
 AddTextPrinterParameterizedWithColor(w+112,0,P(w,44),0,0,255,0x30200,0);ScheduleWindowCopyToVram(w+112);ov109_021E7114(w,(u8)((((u32)r[4]<<24)>>25)+1),7,13,8);
}
