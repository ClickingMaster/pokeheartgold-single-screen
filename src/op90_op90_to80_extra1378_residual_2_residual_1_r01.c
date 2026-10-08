#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
#include <stddef.h>
typedef struct Format {u8 justify,sign,precisionSet,alternate,length,conversion;u8 pad[2];int width,precision;} Format;
typedef struct{u8 style,pad;s16 digits;}DecFormat;
struct Dec{u8 sign,pad;s16 exponent;u8 length,digits[32];};
typedef struct RoundedDecimal{u8 sign,pad;s16 exponent;u8 count;s8 digits[33];}RoundedDecimal;
typedef union{struct Dec produced;RoundedDecimal rounded;}DecimalBacking;
typedef struct {DecFormat format;DecimalBacking value;} DecimalStage;
typedef char Format16Proof[sizeof(Format)==16?1:-1];
typedef char Decimal38Proof[sizeof(DecimalBacking)==38?1:-1];
typedef char Stage42Proof[sizeof(DecimalStage)==42?1:-1];
extern void __num2dec(const void*,double,struct Dec*);
extern void round_decimal(RoundedDecimal*,int);
extern const u16 __msl_digit[];
extern const char printf_double2hex_neg_inf_upper[],printf_double2hex_neg_inf_lower[],printf_double2hex_inf_upper[],printf_double2hex_inf_lower[],printf_double2hex_neg_nan_upper[],printf_double2hex_neg_nan_lower[],printf_double2hex_nan_upper[],printf_double2hex_nan_lower[];
char*float2str(double,char*,Format);
// @fn float2str
char*float2str(double x,char*end,Format fmt){
    DecimalStage stage;
    int exp;
    int n;
    int i;
    int integer;
    int sign;
    int expsign;
    int alternate;
    int frac;
    char *p;
    int conversion=fmt.conversion;
    long precision=fmt.precision;
    char *out;
    alternate=fmt.alternate;
    sign=fmt.sign;
    if(precision>509)return NULL;
    stage.format.style=0;stage.format.digits=32;
    __num2dec(&stage.format,x,&stage.value.produced);
    p=(char*)stage.value.produced.digits+stage.value.produced.length;
    while(stage.value.produced.length>1&&*--p=='0'){stage.value.produced.length--;stage.value.produced.exponent++;}
    switch(stage.value.produced.digits[0]){
    case '0':stage.value.produced.exponent=0;break;
    case 'I':
        if(x<0.0){out=end-5;if((conversion<0||conversion>=128?0:__msl_digit[conversion]&0x200))memcpy(out,printf_double2hex_neg_inf_upper,5);else memcpy(out,printf_double2hex_neg_inf_lower,5);}
        else{out=end-4;if((conversion<0||conversion>=128?0:__msl_digit[conversion]&0x200))memcpy(out,printf_double2hex_inf_upper,4);else memcpy(out,printf_double2hex_inf_lower,4);}
        return out;
    case 'N':
        if(*(const s8*)&stage.value.produced){out=end-5;if((conversion<0||conversion>=128?0:__msl_digit[conversion]&0x200))memcpy(out,printf_double2hex_neg_nan_upper,5);else memcpy(out,printf_double2hex_neg_nan_lower,5);}
        else{out=end-4;if((conversion<0||conversion>=128?0:__msl_digit[conversion]&0x200))memcpy(out,printf_double2hex_nan_upper,4);else memcpy(out,printf_double2hex_nan_lower,4);}
        return out;
    }
    stage.value.produced.exponent+=stage.value.produced.length-1;
    out=end;
    *--out=0;
    switch(conversion){
    case 'g':case 'G':
        if(stage.value.produced.length>precision)round_decimal(&stage.value.rounded,precision);
        if(stage.value.produced.exponent<-4||stage.value.produced.exponent>=precision){
            if(alternate)precision--;else precision=stage.value.produced.length-1;
            if(conversion=='g')conversion='e';else conversion='E';
            goto exponential;
        }
        if(alternate)precision-=stage.value.produced.exponent+1;
        else{precision=stage.value.produced.length-(stage.value.produced.exponent+1);if(precision<0)precision=0;}
        goto fixed;
    case 'e':case 'E':
    exponential:
        if(stage.value.produced.length>precision+1)round_decimal(&stage.value.rounded,precision+1);
        exp=stage.value.produced.exponent;expsign='+';
        if(exp<0){exp=-exp;expsign='-';}
        for(n=0;exp!=0||n<2;n++){*--out=exp%10+'0';exp/=10;}
        *--out=expsign;*--out=conversion;
        if((end-out)+precision>509)return NULL;
        if(stage.value.produced.length<precision+1){n=precision+2-stage.value.produced.length;while(--n)*--out='0';}
        p=(char*)stage.value.produced.digits+stage.value.produced.length;n=stage.value.produced.length;
        while(--n)*--out=*--p;
        if(precision!=0||alternate)*--out='.';
        *--out=stage.value.produced.digits[0];
        if(*(const s8*)&stage.value.produced)*--out='-';else if(sign==1)*--out='+';else if(sign==2)*--out=' ';
        break;
    case 'f':case 'F':
    fixed:
        frac=stage.value.produced.length-stage.value.produced.exponent-1;if(frac<0)frac=0;
        if(frac>precision){
            round_decimal(&stage.value.rounded,stage.value.produced.length-(frac-precision));
            frac=stage.value.produced.length-stage.value.produced.exponent-1;if(frac<0)frac=0;
        }
        integer=stage.value.produced.exponent+1;if(integer<0)integer=0;
        if(integer+frac>509)return NULL;
        p=(char*)stage.value.produced.digits+stage.value.produced.length;
        for(n=0;n<precision-frac;n++)*--out='0';
        for(i=0;i<frac&&i<stage.value.produced.length;i++)*--out=*--p;
        for(;i<frac;i++)*--out='0';
        if(precision||alternate)*--out='.';
        if(integer){
            for(i=0;i<integer-stage.value.produced.length;i++)*--out='0';
            for(;i<integer;i++)*--out=*--p;
        }else *--out='0';
        if(*(const s8*)&stage.value.produced)*--out='-';else if(sign==1)*--out='+';else if(sign==2)*--out=' ';
        break;
    }
    return out;
}
