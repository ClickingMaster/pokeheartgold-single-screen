#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define G (*(W40749Work **)0x0380fff4)
typedef struct W40749Work {u8 prefix[0x33e];u16 errors;u8 gap340[0x5f8-0x340];u16 chip;u8 tail[6];} W40749Work;
typedef char W40749Size[(sizeof(W40749Work)==0x600)?1:-1];
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define P(p,n) (*(u8**)((u8*)(p)+(n)))
#define V(n) (*(volatile u16*)(n))
extern const u16 BBPDiagSkipAdrsES1[],BBPDiagSkipAdrsRelease[];
int BBP_Write(u32,u32);u32 BBP_Read(u32);void DiagBaseBand(void);
#pragma force_active off
static inline int Different40749a(u32 x,u32 y){if((u16)x==y)return 0;else return 1;}
static inline int Different40749b(u32 x,u32 y){if((u16)x==y)return 0;else return 1;}
static inline int Different40749c(u32 x,u32 y){if((u16)x==y)return 0;else return 1;}
#pragma force_active on
// @fn DiagBaseBand
void DiagBaseBand(void){
 u32 walki,walkk,errors=0;u16 pattern;const u16*skip;
 if(G->chip==5)skip=BBPDiagSkipAdrsES1;else skip=BBPDiagSkipAdrsRelease;
 {walki=0;for(;walki<105;walki++){if(BBP_Write(walki,255)==-1){errors=1;goto done;}}}
 {walki=0;walkk=0;for(;walki<105;walki++){if(walki==skip[walkk]){walkk++;continue;}if((u16)BBP_Read(walki)!=255){if(errors++>32)goto done;}}}
 {walki=0;for(;walki<105;walki++)BBP_Write(walki,0);}
 {walki=0;walkk=0;for(;walki<105;walki++){if(walki==skip[walkk]){walkk++;continue;}if((u16)BBP_Read(walki)!=0){if(errors++>32)goto done;}}}
 {pattern=0x55;walki=0;for(;walki<105;walki++,pattern=(u16)~pattern){BBP_Write(walki,pattern);}}
 {u32 i;u16 pat;u32 k;i=0;k=0;pat=0x55;for(;i<105;pat=(u8)~pat,i++){if(i==skip[k]){k++;continue;}if(Different40749a(BBP_Read(i),pat)){if(errors++>32)goto done;}}}
 {pattern=255;walki=0;for(;walki<105;walki++,pattern--){BBP_Write(walki,pattern);}}
 {u32 i;u16 pat;u32 k;i=0;k=0;pat=255;for(;i<105;i++,pat--){if(i==skip[k]){k++;continue;}if(Different40749b(BBP_Read(i),pat)){if(errors++>32)goto done;}}}
 {for(walki=0,walkk=0;walki<105;walki++){if(walki==skip[walkk]){walkk++;continue;}{u32 j,bit;for(bit=1,j=0;j<9;bit=(bit<<1)&255,j++){BBP_Write(walki,bit);if(Different40749c(BBP_Read(walki),bit)){if(errors++>32)goto done;}}}}}
 done:if(errors){W40749Work *p=G;p->errors|=8;}
}
