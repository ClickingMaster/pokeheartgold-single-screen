#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
u32 __msl_generic_count_bits64(u64);
int __signbitd(double);
int __fpclassifyd(double);
double scalbn(double,int);
extern double frexp(double,int *);
extern double ldexp(double,int);
u32 __msl_generic_count_bits64(u64 value) {value-=((value>>1)&0x5555555555555555ULL);value=(value&0x3333333333333333ULL)+((value>>2)&0x3333333333333333ULL);value=(value+(value>>4))&0x0f0f0f0f0f0f0f0fULL;value+=value>>8;value+=value>>16;value+=value>>32;return (u32)value&255;}
int __signbitd(double value) {return ((u32 *)&value)[1]&0x80000000;}
int __fpclassifyd(double value) {u32 high=((u32 *)&value)[1];u32 exp=high&0x7ff00000;if(exp==0)goto zero;if(exp==0x7ff00000)return (high&0xfffff)||((u32 *)&value)[0]?1:2;goto normal;zero:return (high&0xfffff)||((u32 *)&value)[0]?5:3;normal:return 4;}
double scalbn(double value,int shift) {int exponent;value=frexp(value,&exponent);exponent+=shift;return ldexp(value,exponent);}
