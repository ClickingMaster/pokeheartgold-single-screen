#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov18_021F118C(void *,int,int),ov18_021F11C0(void *,int,int);
void ov18_021F39C4(void *,int,u32);
#pragma opt_strength_reduction off
void ov18_021F39C4(void *ctx,int start,u32 number){
 u32 shown,i,divisor,digit;
 if(number==9999)number=99990;else number=(number*220462+50000)/100000;
 shown=0;divisor=10000;for(i=shown;i<5;i++){
  digit=number/divisor;
  if(digit!=0||shown==1){shown=1;ov18_021F118C(ctx,start+i,digit+43);ov18_021F11C0(ctx,start+i,1);}
  else ov18_021F11C0(ctx,start+i,0);
  number%=divisor;divisor/=10;if(i==2)shown=1;
 }
}
