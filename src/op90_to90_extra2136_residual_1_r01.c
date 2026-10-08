// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { u8 pad[4]; u32 flag4; u32 flag8; u8 pad2[0x1c]; u32 *out28; } PMWork;
typedef struct { u16 done; u16 pad; u32 x; } PMDone;
typedef struct { u16 *buf; u32 x; } PMBuf;
extern PMWork _021E370C;
extern PMDone _021E3750[];
extern PMBuf _021E3754[];
extern void PMi_CallCallbackAndUnlock(u32);
void PMi_CommonCallback(u32 tag,u32 data,BOOL err);
// @fn PMi_CommonCallback
void PMi_CommonCallback(u32 tag,u32 data,BOOL err){
 u16 command;
 u16 result;
 if(err){
  PMi_CallCallbackAndUnlock(2);
  return;
 }
 command=(u16)((data&0x7f00)>>8);
 result=data&0xff;
 if(command>=0x70&&command<=0x74){
  u32 idx=command-0x70;
  {u16 v=(u8)result;u16 *p=_021E3754[idx].buf;if(p)*p=v;}
  _021E3750[idx].done=1;
  result=0;
 }else if(command==0x60){
  _021E370C.flag4=1;
 }else if(command==0x62){
  _021E370C.flag8=1;
 }else if(command==0x67){
  if(_021E370C.out28)*_021E370C.out28=result;
  result=0;
 }
 PMi_CallCallbackAndUnlock(result);
}
