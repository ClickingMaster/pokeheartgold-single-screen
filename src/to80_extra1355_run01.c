#pragma force_active on
#include "global.h"
extern u16 ov91_02261D64[][4],ov91_02261C0A[],ov91_02261BF8[],ov91_02261BFE[];
extern void sub_02018030(void *,void *,int,int),sub_020181B0(void *,void *),sub_020182A8(void *,int,int,int),sub_020182C4(void *,int,int,int),sub_020182A0(void *,int),sub_020182E0(void *,int,int);
extern int GF_DegreeToSinCosIdxNoWrap(int);
extern void sub_020180BC(void *,void *,void *,int,int,int),sub_020181D4(void *,void *);
void ov91_02260F50(u8 *,int,void *,int,int,int,volatile int);
void ov91_02260F50(u8 *w,int unused,void *resource,int kind,int sel,int heap,volatile int flag) {
 int i,id;u8 *cur,*data;flag=flag;data=w+0x168;cur=w;
 for(i=0;i<3;i++,data+=16,cur+=120) {
  if(i==0)id=36;else if(i==1)id=37;else id=ov91_02261C0A[kind-1];
  sub_02018030(data,resource,id,heap);sub_020181B0(cur,data);sub_020182A8(cur,0,-0x62000,0);sub_020182C4(cur,0x1800,0x1800,0x1800);
  if(i==1)sub_020182A0(cur,0);
  if((u32)i<=1)sub_020182E0(cur,ov91_02261D64[kind-1][sel],1);
  else sub_020182E0(cur,GF_DegreeToSinCosIdxNoWrap(180),1);
 }
 for(i=0,cur=w+0x198;i<3;i++,cur+=20) {
  sub_020180BC(cur,w+0x168+ov91_02261BFE[i]*16,resource,ov91_02261BF8[i],heap,flag);
  sub_020181D4(w+ov91_02261BFE[i]*120,cur);
 }
 *(u32*)(w+0x1d4)=4096;
}
