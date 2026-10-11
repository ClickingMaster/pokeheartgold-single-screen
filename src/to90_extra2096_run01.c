#pragma force_active on
#include "global.h"
extern void FillWindowPixelBuffer(void *,int),ScheduleWindowCopyToVram(void *),BattlePartyMenu_PrintMonName(void *,int,int,int,int,int),BattlePartyMenu_PrintMoveName(void *,int,int,u32,int,int,int),BattlePartyMenu_PrintMoveListPP(void *,void *,int),BattlePartyMenu_PrintButtonLabel(void *,int,int);
extern const u32 sBattlePartyMoveNameMsgIds[];
void BattlePartyMenu_PrintCheckMovesPage(void *);
#pragma force_active on
void BattlePartyMenu_PrintCheckMovesPage(void *argument){
 u8 *work=argument;u16 base=work[0x2075]*6,i;int w1,w2,w3,w4;u8 *records;
 FillWindowPixelBuffer((*(u8 **)(work+0x2070))+base*16,0);w1=(base+1)*16;FillWindowPixelBuffer((*(u8 **)(work+0x2070))+w1,0);w2=(base+2)*16;FillWindowPixelBuffer((*(u8 **)(work+0x2070))+w2,0);w3=(base+3)*16;FillWindowPixelBuffer((*(u8 **)(work+0x2070))+w3,0);w4=(base+4)*16;FillWindowPixelBuffer((*(u8 **)(work+0x2070))+w4,0);FillWindowPixelBuffer((*(u8 **)(work+0x2070))+0x50,0);
 BattlePartyMenu_PrintMonName(work,base,0,(*(u8 **)work)[0x11],0,0);
 records=work;records+=0x34;for(i=0;i<4;i++){u8 *row=records+(*(u8 **)work)[0x11]*80,*slot=row+i*8;if(*(u16 *)slot){BattlePartyMenu_PrintMoveName(work,*(u16 *)slot,base+1+i,sBattlePartyMoveNameMsgIds[i],4,7,0x70809);BattlePartyMenu_PrintMoveListPP(work,slot,base+1+i);}}
 BattlePartyMenu_PrintButtonLabel(work,5,18);ScheduleWindowCopyToVram((*(u8 **)(work+0x2070))+w1);ScheduleWindowCopyToVram((*(u8 **)(work+0x2070))+w2);ScheduleWindowCopyToVram((*(u8 **)(work+0x2070))+w3);ScheduleWindowCopyToVram((*(u8 **)(work+0x2070))+w4);work[0x2075]^=1;
}
