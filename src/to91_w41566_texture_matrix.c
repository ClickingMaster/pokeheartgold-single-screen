// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include "to91_tex_callback_41566_private.h"
#pragma force_active on
void texmtxCalc_flagRS___xsi(void *,void *);
// @fn texmtxCalc_flagR___xsi
void texmtxCalc_flagR___xsi(void *output,void *input){
 MtxFx44 *m=output;
 u32 zero=0;
 m->_00=To91TexSignedWord41566(input,24);m->_11=To91TexSignedWord41566(input,28);m->_01=zero;
 s32 offsetS=(s32)(((s64)To91TexSignedWord41566(input,36)*To91TexSignedWord41566(input,24))>>12);
 s32 offsetT=(s32)(((s64)(s32)(0u-To91TexWord41566(input,40))*To91TexSignedWord41566(input,28))>>12);
 m->_30=(s32)((To91TexHalf41566(input,44)*(0u-(u32)offsetS))<<4);
 m->_31=(s32)(((0u-To91TexHalf41566(input,46))*(To91TexWord41566(input,28)+(u32)offsetT-4096u))<<4);
 m->_10=zero;
}
// @fn texmtxCalc_flagRS___xsi
void texmtxCalc_flagRS___xsi(void *output,void *input){
 MtxFx44 *m=output;u32 zero=0;
 m->_00=4096;m->_11=4096;m->_01=zero;
 u32 x=0u-To91TexWord41566(input,36);
 u32 width=To91TexHalf41566(input,44);
 u32 y=0u-To91TexWord41566(input,40);
 m->_30=(s32)((width*x)<<4);
 m->_31=(s32)(((0u-To91TexHalf41566(input,46))*y)<<4);
 m->_10=zero;
}
