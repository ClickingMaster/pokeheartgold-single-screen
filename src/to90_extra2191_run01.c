#pragma force_active on
#include "global.h"
#pragma force_active on
extern void PokedexApp_SetSpriteAnim(void *,int,int),PokedexApp_SetSpriteVisible(void *,int,int);
void PokedexApp_DrawWeightDigits(void *,int,u32);
#pragma opt_strength_reduction off
void PokedexApp_DrawWeightDigits(void *ctx,int start,u32 number){
 u32 shown,i,divisor,digit;
 if(number==9999)number=99990;else number=(number*220462+50000)/100000;
 shown=0;divisor=10000;for(i=shown;i<5;i++){
  digit=number/divisor;
  if(digit!=0||shown==1){shown=1;PokedexApp_SetSpriteAnim(ctx,start+i,digit+43);PokedexApp_SetSpriteVisible(ctx,start+i,1);}
  else PokedexApp_SetSpriteVisible(ctx,start+i,0);
  number%=divisor;divisor/=10;if(i==2)shown=1;
 }
}
