#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern int TP_CalcCalibrateParam(TPCalibrateParam *,u16,u16,u8,u8,u16,u16,u8,u8);
BOOL TP_GetUserInfo(TPCalibrateParam *out);
BOOL TP_GetUserInfo(TPCalibrateParam *out) {
 const u8 *p=(const u8*)0x027ffc80;
 u16 x1=*(const u16*)(p+0x58),y1=*(const u16*)(p+0x5a),x2=*(const u16*)(p+0x5e),y2=*(const u16*)(p+0x60);
 u8 sx1=p[0x5c],sy1=p[0x5d],sx2=p[0x62],sy2=p[0x63];
 if((x1==0 && x2==0 && y1==0 && y2==0) || TP_CalcCalibrateParam(out,x1,y1,sx1,sy1,x2,y2,sx2,sy2)) {
 ((u16*)out)[0]=0;((u16*)out)[1]=0;((u16*)out)[2]=0;((u16*)out)[3]=0;return TRUE;
 }
 return TRUE;
}
