#pragma force_active on
#include "global.h"
#pragma force_active on
void Pokepic_SetAttr(void *,int,int);
int ov07_02222468(void *),ov07_02222314(void *,void *);
void ov07_02222418(void *,s16,s16,int),ov07_02222268(void *,s16,s16,s16,s16,int);
int ov07_0222E3E8(u8 *);
#define W(o) (*(int *)(w+(o)))
#define P(o) (*(void **)(w+(o)))
#define H(o) (*(s16 *)(w+(o)))
int ov07_0222E3E8(u8 *w){int result=0;switch(W(76)){
case 0:ov07_02222468(w+16);Pokepic_SetAttr(P(8),9,W(16));if(--W(80)<0){W(76)++;ov07_02222268(w+36,H(12),H(12),H(14),(s16)(H(14)+2),2);}break;
case 1:if(ov07_02222468(w+16))Pokepic_SetAttr(P(8),9,W(16));if(!ov07_02222314(w+36,P(8))){W(76)++;ov07_02222418(w+16,(s16)W(16),0,3);ov07_02222268(w+36,H(12),H(12),(s16)(H(14)+2),H(14),2);}break;
case 2:ov07_02222314(w+36,P(8));if(ov07_02222468(w+16))Pokepic_SetAttr(P(8),9,W(16));else{Pokepic_SetAttr(P(8),0,H(12));Pokepic_SetAttr(P(8),1,H(14));Pokepic_SetAttr(P(8),9,result);W(76)++;}break;
case 3:result=1;break;}return result;}
