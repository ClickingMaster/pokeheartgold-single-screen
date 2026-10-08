#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct { u16 offset, mask; } TestReg;
extern const TestReg test_reg[27];
extern const u16 test_pattern[3];
#define ROOT (*(u8 **)0x0380fff4)
#define MACREG(off) (*(vu16 *)(0x04808000 + (off)))

void DiagMacRegister(void);
// @fn DiagMacRegister
void DiagMacRegister(void){u32 err;u32 j,i;err=0;i=0;for(;i<3;i++){for(j=0;j<27;j++){vu16*reg=&MACREG(test_reg[j].offset);u32 val=test_pattern[i]&test_reg[j].mask;*reg=val;if(*reg!=val){if(err++>32)goto end;}}}{u16 p=0x1234;u32 k;for(k=0;k<27;k++){MACREG(test_reg[k].offset)=p&test_reg[k].mask;p+=0x1234;}}{u32 k;u16 p=0x1234;for(k=0;k<27;k++){if(MACREG(test_reg[k].offset)!=(p&test_reg[k].mask)){if(err++>32)goto end;}p+=0x1234;}}{u32 k;for(k=0;k<27;k++){MACREG(test_reg[k].offset)=0;if(MACREG(test_reg[k].offset)!=0){if(err++>32)goto end;}}}end:if(err)*(u16*)(ROOT+0x33e)|=1;}
