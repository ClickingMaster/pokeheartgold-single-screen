#include "global.h"
#include "battle/battle_setup.h"
#include "font.h"
#include "heap.h"
#include "pm_string.h"
#pragma force_active on
void BattleRecord_GetBattlerAndPartyCount(int,int *,int *);
void ov40_02244A84(void *,u32);
void ov40_02244920(void *,BattleSetup *,enum HeapID);
void ov40_02244920(void *record,BattleSetup *input,enum HeapID heap){
 u8 *setup=(u8*)input;
 struct {int last,count;} counts;int i,j;String *str,*checked;int n;u16 *buf;u8 *entry,*dst,*src;
 BattleRecord_GetBattlerAndPartyCount(*((u8*)record+0xaa),&counts.count,&counts.last);
 str=String_New(64,heap);checked=String_New(64,heap);buf=Heap_Alloc(heap,128);
 i=0;if(i<counts.count){entry=setup;dst=setup+0x3c;src=dst;do{
  u32 cursor=(u32)entry+14;
  for(j=7;j>-1;j--,cursor-=2)if(*(const u16*)(cursor+0x3c)==65535)break;
  if(j==-1){ov40_02244A84(checked,heap);CopyStringToU16Array(checked,(u16*)dst,8);}
  else{String_SetEmpty(str);CopyU16ArrayToString(str,(const u16*)src);if(!FontID_String_AllCharsValid((FontID)0,str,checked)){ov40_02244A84(checked,heap);CopyStringToU16Array(checked,(u16*)dst,8);}}
entry+=sizeof(Trainer);dst+=sizeof(Trainer);src+=sizeof(Trainer);i++;}while(i<counts.count);}
 i=0;if(i<counts.count){do{
  n=Party_GetCount(*(Party**)(setup+4));
  for(j=0;j<n;j++){Pokemon*mon=Party_GetMonByIndex(*(Party**)(setup+4),j);if(!GetMonData(mon,172,NULL))break;MIi_CpuClear16(0,buf,128);GetMonData(mon,117,buf);String_SetEmpty(str);CopyU16ArrayToString(str,buf);if(!FontID_String_AllCharsValid((FontID)0,str,checked))SetMonData(mon,179,NULL);}
setup+=4;i++;}while(i<counts.count);}
 String_Delete(str);String_Delete(checked);Heap_Free(buf);
}
