#pragma force_active on
#pragma dollar_identifiers on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{s8 sign;u8 pad;s16 exp;u8 len,digits[32],pad2;} Decimal;
void __str2dec(Decimal*,const char*,int);void __timesdec(Decimal*,Decimal*,Decimal*);void __two_exp(Decimal*,int);
extern const char fp_str$6E88[];
extern const char fp_str$6EB8[];
extern const char fp_str$6EE0[];
extern const char fp_str$6EF8[];
extern const char fp_str$6F08[];
extern const char fp_str$6F10[];
extern const char fp_str$6F18[];
extern const char fp_str$6F20[];
extern const char fp_str$6F28[];
extern const char fp_str$6F2C[];
extern const char fp_str$6F30[];
extern const char fp_str$6F34[];
extern const char fp_str$6F38[];
extern const char fp_str$6F3C[];
extern const char fp_str$6F40[];
extern const char fp_str$6F44[];
extern const char fp_str$6F48[];
extern const char fp_str$6F4C[];
extern const char fp_str$6F50[];
extern const char fp_str$6F54[];
extern const char fp_str$6F58[];
void __two_exp(Decimal*out,int n){Decimal temp,copy;switch(n){case -64:__str2dec(out,fp_str$6E88,-20);return;case -53:__str2dec(out,fp_str$6EB8,-16);return;case -32:__str2dec(out,fp_str$6EE0,-10);return;case -16:__str2dec(out,fp_str$6EF8,-5);return;case -8:__str2dec(out,fp_str$6F08,-3);return;case -7:__str2dec(out,fp_str$6F10,-3);return;case -6:__str2dec(out,fp_str$6F18,-2);return;case -5:__str2dec(out,fp_str$6F20,-2);return;case -4:__str2dec(out,fp_str$6F28,-2);return;case -3:__str2dec(out,fp_str$6F2C,-1);return;case -2:__str2dec(out,fp_str$6F30,-1);return;case -1:__str2dec(out,fp_str$6F34,-1);return;case 0:__str2dec(out,fp_str$6F38,0);return;case 1:__str2dec(out,fp_str$6F3C,0);return;case 2:__str2dec(out,fp_str$6F40,0);return;case 3:__str2dec(out,fp_str$6F44,0);return;case 4:__str2dec(out,fp_str$6F48,1);return;case 5:__str2dec(out,fp_str$6F4C,1);return;case 6:__str2dec(out,fp_str$6F50,1);return;case 7:__str2dec(out,fp_str$6F54,2);return;case 8:__str2dec(out,fp_str$6F58,2);return;default:break;}__two_exp(&temp,(n+(int)(((u32)n&0x80000000)>>31))>>1);__timesdec(out,&temp,&temp);if(n&1){copy=*out;if(n>0)__str2dec(&temp,fp_str$6F3C,0);else __str2dec(&temp,fp_str$6F34,-1);__timesdec(out,&copy,&temp);}}
