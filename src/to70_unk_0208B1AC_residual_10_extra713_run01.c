#pragma force_active on
#include "global.h"
#pragma force_active on
void Sprite_SetDrawFlag(void *,int),Sprite_SetAnimCtrlSeq(void *,int),sub_0208C250(void *,void *,u8),sub_0208C2A0(u8 *),sub_0208C320(u8 *);void *sub_0208ACDC(void *,u8);
void sub_0208C2A0(u8 *s){u16 i;for(i=54;i<=66;i++)Sprite_SetDrawFlag(*(void **)(s+1028+i*4),0);if(*(s8 *)(s+1980)==2){for(i=0;i<9;i++){if(i<s[1990]){Sprite_SetDrawFlag(*(void **)(s+1244+i*4),1);sub_0208C250(s,sub_0208ACDC(s,(u8)i),(u8)i);}}Sprite_SetAnimCtrlSeq(*(void **)(s+1292),0);}}
void sub_0208C320(u8 *s){u16 i;for(i=0;i<9;i++){if(s[1989]*9+i<s[1990]){Sprite_SetDrawFlag(*(void **)(s+1244+i*4),1);sub_0208C250(s,sub_0208ACDC(s,(u8)i),(u8)i);}else Sprite_SetDrawFlag(*(void **)(s+1244+i*4),0);}}
