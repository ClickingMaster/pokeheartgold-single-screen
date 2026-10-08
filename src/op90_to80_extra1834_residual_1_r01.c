#pragma dollar_identifiers on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
extern u32 mbstowcs(u16*,const char*,u32);
typedef struct{u8 style,pad;s16 digits;}DecFormat;
typedef struct Dec {s8 sign;u8 pad;s16 exponent;u8 length,digits[32];} Decimal;
typedef struct {DecFormat input;Decimal output;} DecimalStage;
typedef struct {char bytes[512];} OutputBuffer;
typedef char CheckDecFormat[(sizeof(DecFormat)==4)?1:-1];
typedef char CheckDecimal[(sizeof(Decimal)==38)?1:-1];
typedef char CheckStage[(sizeof(DecimalStage)==42)?1:-1];
typedef struct Format {u8 align,sign,precision_set,is_pascal,length,pad;u16 conversion;int width,precision;} Format;
extern void __num2dec(const void*,double,Decimal*),round_decimal__wide(Decimal*,int);
extern const u16 __wctype_mapC[];
extern u16*wcscpy(u16*,const u16*);
extern const u16 wstr$6DF0[],wstr$6DFC[],wstr$6E08[],wstr$6E10[],wstr$6E18[],wstr$6E24[],wstr$6E30[],wstr$6E38[];
u16*float2str__wide(double,u16*,Format);
// @fn float2str__wide
// {'precision': 'late', 'conversion': 'local', 'alternate': 'late', 'sign': 'late', 'x': 'both'}
u16*float2str__wide(double x,u16*wideend,Format fmt){
    DecimalStage stage;
    OutputBuffer backing;
    int exp;
    int n;
    int i;
    int integer;
    int sign;
    int expsign;
    int alternate;
    u16 *wide;
    int frac;
    char *p;
    int conversion=fmt.conversion;
    long precision=fmt.precision;
    char *out;
    alternate=fmt.is_pascal;
    sign=fmt.sign;
    if(precision>509)return NULL;
    stage.input.style=0;stage.input.digits=32;
    __num2dec(&stage.input,x,&stage.output);
    p=(char*)stage.output.digits+stage.output.length;
    while(stage.output.length>1&&*--p=='0'){stage.output.length--;stage.output.exponent++;}
    switch(stage.output.digits[0]){
    case '0':stage.output.exponent=0;break;
    case 'I':
        if(x<0.0){wide=wideend-5;if(((unsigned)conversion>=128?0:__wctype_mapC[conversion]&0x200))wcscpy(wide,wstr$6DF0);else wcscpy(wide,wstr$6DFC);}
        else{wide=wideend-4;if(((unsigned)conversion>=128?0:__wctype_mapC[conversion]&0x200))wcscpy(wide,wstr$6E08);else wcscpy(wide,wstr$6E10);}
        return wide;
    case 'N':
        if(stage.output.sign){wide=wideend-5;if(((unsigned)conversion>=128?0:__wctype_mapC[conversion]&0x200))wcscpy(wide,wstr$6E18);else wcscpy(wide,wstr$6E24);}
        else{wide=wideend-4;if(((unsigned)conversion>=128?0:__wctype_mapC[conversion]&0x200))wcscpy(wide,wstr$6E30);else wcscpy(wide,wstr$6E38);}
        return wide;
    }
    stage.output.exponent+=stage.output.length-1;
    out=backing.bytes+512;
    *--out=0;
    switch(conversion){
    case 'g':case 'G':
        if(stage.output.length>precision)round_decimal__wide(&stage.output,precision);
        if(stage.output.exponent<-4||stage.output.exponent>=precision){
            if(alternate)precision--;else precision=stage.output.length-1;
            if(conversion=='g')conversion='e';else conversion='E';
            goto exponential;
        }
        if(alternate)precision-=stage.output.exponent+1;
        else{precision=stage.output.length-(stage.output.exponent+1);if(precision<0)precision=0;}
        goto fixed;
    case 'e':case 'E':
    exponential:
        if(stage.output.length>precision+1)round_decimal__wide(&stage.output,precision+1);
        exp=stage.output.exponent;expsign='+';
        if(exp<0){exp=-exp;expsign='-';}
        for(n=0;exp!=0||n<2;n++){*--out=exp%10+'0';exp/=10;}
        *--out=expsign;*--out=conversion;
        if((backing.bytes-out)+precision>509)return NULL;
        if(stage.output.length<precision+1){n=precision+2-stage.output.length;while(--n)*--out='0';}
        p=(char*)stage.output.digits+stage.output.length;n=stage.output.length;
        while(--n)*--out=*--p;
        if(precision!=0||alternate)*--out='.';
        *--out=stage.output.digits[0];
        if(stage.output.sign)*--out='-';else if(sign==1)*--out='+';else if(sign==2)*--out=' ';
        break;
    case 'f':case 'F':
    fixed:
        frac=stage.output.length-stage.output.exponent-1;if(frac<0)frac=0;
        if(frac>precision){
            round_decimal__wide(&stage.output,stage.output.length-(frac-precision));
            frac=stage.output.length-stage.output.exponent-1;if(frac<0)frac=0;
        }
        integer=stage.output.exponent+1;if(integer<0)integer=0;
        if(integer+frac>509)return NULL;
        p=(char*)stage.output.digits+stage.output.length;
        for(n=0;n<precision-frac;n++)*--out='0';
        for(i=0;i<frac&&i<stage.output.length;i++)*--out=*--p;
        for(;i<frac;i++)*--out='0';
        if(precision||alternate)*--out='.';
        if(integer){
            for(i=0;i<integer-stage.output.length;i++)*--out='0';
            for(;i<integer;i++)*--out=*--p;
        }else *--out='0';
        if(stage.output.sign)*--out='-';else if(sign==1)*--out='+';else if(sign==2)*--out=' ';
        break;
    }
    wide=wideend-strlen(out)-1;
    mbstowcs(wide,out,strlen(out));
    return wide;
}
