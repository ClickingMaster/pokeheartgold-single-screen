#pragma force_active on
#include "global.h"
typedef struct {u32 zero;u32 field4,field8,fieldC;u32 *field10;u8 pad[0x4f0];u32 saved4,saved8,saved10,result,pad2[2],flag;} Work;
extern void GF_AssertFail(void);
u32 ov40_022309DC(Work *,u32,u32,u32);
u32 ov40_022309DC(Work *w,u32 group,u32 index,u32 type) {
 u32 result;
 w->saved4=w->field4;w->saved8=w->field8;w->saved10=*w->field10;w->flag=1;result=194;
 switch(type) {
case 0:
 switch(index) {
 case 0: result=244;break;
 case 1: result=245;break;
 case 2: result=246;break;
 case 3: result=247;break;
 case 4: result=248;break;
 case 5: result=249;break;
 case 6: result=250;break;
 case 7: result=251;break;
 case 8: result=252;break;
 case 9: result=253;break;
 case 10: result=254;break;
 case 11: result=255;break;
 case 12: result=256;break;
 case 13: result=257;break;
 case 14: result=258;break;
 default:GF_AssertFail();break;
 }
break;
case 1:
 switch(index) {
 case 0: result=259;break;
 case 1: result=260;break;
 case 2: result=261;break;
 case 3: result=262;break;
 case 4: result=263;break;
 case 5: result=264;break;
 case 6: result=265;break;
 case 7: result=266;break;
 case 8: result=267;break;
 case 9: result=268;break;
 default:GF_AssertFail();break;
 }
break;
case 2:
 switch(group) {
 case 0:
  switch(index) {
  case 0: result=194;break;
  case 1: result=195;break;
  case 2: result=196;break;
  case 3: result=197;break;
  case 4: result=198;break;
  case 5: result=199;break;
  default:GF_AssertFail();break;
  }
 break;
 case 1:
  switch(index) {
  case 0: result=200;break;
  case 1: result=201;break;
  case 2: result=202;break;
  case 3: result=203;break;
  default:GF_AssertFail();break;
  }
 break;
 case 2:
  switch(index) {
  case 0: result=204;break;
  case 1: result=205;break;
  case 2: result=206;break;
  case 3: result=207;break;
  case 4: result=208;break;
  case 5: result=209;break;
  case 6: result=210;break;
  default:GF_AssertFail();break;
  }
 break;
 case 3:
  switch(index) {
  case 0: result=211;break;
  case 1: result=212;break;
  case 2: result=213;break;
  case 3: result=214;break;
  default:GF_AssertFail();break;
  }
 break;
 case 4:
  switch(index) {
  case 0: result=215;break;
  case 1: result=216;break;
  case 2: result=217;break;
  default:GF_AssertFail();break;
  }
 break;
 case 5:
  switch(index) {
  case 0: result=218;break;
  case 1: result=219;break;
  case 2: result=220;break;
  case 3: result=221;break;
  case 4: result=222;break;
  case 5: result=223;break;
  default:GF_AssertFail();break;
  }
 break;
 case 6:
  switch(index) {
  case 0: result=224;break;
  case 1: result=225;break;
  case 2: result=226;break;
  case 3: result=227;break;
  case 4: result=228;break;
  case 5: result=229;break;
  case 6: result=230;break;
  case 7: result=231;break;
  default:GF_AssertFail();break;
  }
 break;
 case 7:
  switch(index) {
  case 0: result=232;break;
  case 1: result=233;break;
  case 2: result=234;break;
  case 3: result=235;break;
  default:GF_AssertFail();break;
  }
 break;
 case 8:
  switch(index) {
  case 0: result=236;break;
  case 1: result=237;break;
  case 2: result=238;break;
  case 3: result=239;break;
  default:GF_AssertFail();break;
  }
 break;
 default:GF_AssertFail();break;
 }
 break;
 }
 w->result=result;return w->result;
}
