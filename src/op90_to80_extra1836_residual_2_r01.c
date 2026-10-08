#pragma dollar_identifiers on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 justification,sign,precisionSet,alternate,argument,pad5;u16 conversion;int width,precision;} Format;
typedef struct {u8 style;s16 digits;} DecForm;
typedef struct {s8 sign;u8 pad;s16 exponent;u8 length;u8 digits[32];} Decimal;
extern void __num2dec(const DecForm*,double,Decimal*);
extern double fabs(double);
extern u16 *wcscpy(u16*,const u16*);
extern u16 wstr$6DE4[];
extern u16 wstr$6DF0[];
extern u16 wstr$6DFC[];
extern u16 wstr$6E08[];
extern u16 wstr$6E10[];
extern u16 wstr$6E18[];
extern u16 wstr$6E24[];
extern u16 wstr$6E30[];
extern u16 wstr$6E38[];
extern u16 *long2str__wide(long,u16*,Format);
u16 *double2hex__wide(double value,u16 *end,Format f);
#pragma force_active on
// @fn double2hex__wide
u16 *double2hex__wide(double value,u16 *end,Format f){
    Format ef;
    DecForm form;
    Decimal dec;
    u32 exponent;
    char temp;
    u8 *hb;
    int i;
    double v=value;
    int index;
    int j;
    char *bytes;
    u16 *p;
    u8 digit;
    if(f.precision>509)return 0;
    form.style=0;form.digits=32;
    __num2dec(&form,v,&dec);
    if(fabs(value)==0){p=end-6;wcscpy(p,wstr$6DE4);return p;}
    if(dec.digits[0]=='I'){
        if(dec.sign){p=end-5;if(f.conversion=='A')wcscpy(p,wstr$6DF0);else wcscpy(p,wstr$6DFC);}
        else{p=end-4;if(f.conversion=='A')wcscpy(p,wstr$6E08);else wcscpy(p,wstr$6E10);}
        return p;
    }
    if(dec.digits[0]=='N'){
        if(dec.sign){p=end-5;if(f.conversion=='A')wcscpy(p,wstr$6E18);else wcscpy(p,wstr$6E24);}
        else{p=end-4;if(f.conversion=='A')wcscpy(p,wstr$6E30);else wcscpy(p,wstr$6E38);}
        return p;
    }
    ef.justification=1;ef.sign=1;ef.precisionSet=0;ef.alternate=0;ef.argument=0;ef.width=0;ef.precision=1;ef.conversion='d';
    bytes=(char*)&value;
    for(j=0;j<4;j++){temp=bytes[j];bytes[j]=bytes[7-j];bytes[7-j]=temp;}
    exponent=(u32)((u8*)&value)[0]<<25|(u32)((u8*)&value)[1]<<17;
    exponent=exponent>>21&0x7ff;
    p=long2str__wide(exponent-1023,end,ef);
    if(f.conversion=='a')*--p='p';else *--p='P';
    hb=(u8*)&value;
    index=f.precision*4+11;
    for(i=f.precision;i>=1;i--){
        if(index<64){
            digit=hb[index>>3]>>(7-(index&7));
            if((index&~7)!=((index-4)&~7))digit|=(hb[(index>>3)-1]<<8)>>(7-(index&7));
            digit&=15;
            if(digit<10)digit+='0';else if(f.conversion=='a')digit+='a'-10;else digit+='A'-10;
        }else digit='0';
        *--p=digit;
        index-=4;
    }
    if(f.precision||f.alternate)*--p='.';
    *--p='1';
    if(f.conversion=='a')*--p='x';else *--p='X';
    *--p='0';
    if(dec.sign)*--p='-';else if(f.sign==1)*--p='+';else if(f.sign==2)*--p=' ';
    return p;
}
