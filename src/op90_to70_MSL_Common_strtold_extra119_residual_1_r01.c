#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma dollar_identifiers on
extern const unsigned short __msl_digit[];
extern const unsigned char __upper_mapC[];
typedef struct Model9 {char bytes[9];} Model9;
typedef struct Model5 {char bytes[5];} Model5;
extern const Model9 model$4439;
extern const Model5 model$4434;
extern float __float_huge;
extern double __double_huge;
typedef struct Decimal { char sign; char pad; short exponent; unsigned char count; unsigned char digits[32]; char tail; } Decimal;
extern double __dec2num(const Decimal *);
extern double nan(const char *);
double __strtold(int, int (*)(void *, int, int), void *, int *, int *);
#define CLASS(c,b) (((c)<0 || (c)>=128) ? 0 : (__msl_digit[c] & (b)))
#define UPPER(c) (((c)<0 || (c)>=128) ? (c) : __upper_mapC[c])
#define NEXT() (readCount++, ch=readFn(context,0,0))
#pragma opt_lifetimes off
#pragma opt_strength_reduction off
// @fn __strtold
double __strtold(int width, int (*readFn)(void *, int, int), void *context, int *consumed, int *overflow)
{
    int hexState=0, readCount=0, state=1;
    int whitespace=0, negative, exponentNegative, exponent, scale, signCount;
    unsigned char *hexData;
    int hexExponent, hexExponentNegative, hexIntegerDigits, hexIndex;
    Decimal decimal={0};
    unsigned char hex[8];
    int ch;
    unsigned int limit;
    int i;
    negative=0;*overflow=negative;
    exponentNegative=0;exponent=0;scale=0;signCount=0;hexExponent=0;hexExponentNegative=0;hexIntegerDigits=0;
    readCount++;ch=readFn(context,0,0);
    while(readCount<=width && ch!=-1 && !(state&0x1800)) {
        switch(state) {
        case 1:
            if(CLASS(ch,0x100)) { ch=readFn(context,0,0); whitespace++; break; }
            switch(UPPER(ch)) {
            case '-':negative=1;
            case '+':NEXT();signCount=1;break;
            case 'I':NEXT();state=0x4000;break;
            case 'N':NEXT();state=0x2000;break;
            default:state=2;break;
            }
            break;
        case 0x4000: {
            state=1;
            Model9 infinityText=model$4439;
            { char *p=infinityText.bytes+1; while(state<8 && *p==UPPER(ch)) { p++;state++;NEXT(); } }
            if(state==3 || state==8) {
                double d=negative ? (double)(0.0f-__float_huge) : (double)__float_huge;
                *consumed=signCount+(whitespace+state);return d;
            }
            state=0x1000;break; }
        case 0x2000: {
            state=1;
            int length=0;
            Model5 nanText=model$4434;
            char nanPayload[32]={0};
            {
            { char *p=nanText.bytes+1; while(state<4 && *p==UPPER(ch)) {p++;state++;NEXT();} }
            if((unsigned)(state-3)<=1) {
                if(state==4) {
                    while(length<32 && (CLASS(ch,8)||CLASS(ch,1)||ch=='.')) { nanPayload[length]=ch;length++;NEXT(); }
                    if(ch!=')') {state=0x1000;break;}
                    length++;
                }
                nanPayload[length]=0;
                { double d=negative ? 0.0-nan(nanPayload) : nan(nanPayload);
                  *consumed=signCount+(length+(whitespace+state));return d;
                }
            }
            }
            state=0x1000;break; }
        case 2:
            if(ch=='.') {state=16;NEXT();break;}
            if(!CLASS(ch,8)) {state=0x1000;break;}
            if(ch=='0') {NEXT();if(UPPER(ch)=='X') {state=0x8000;hexState=1;} else state=4;break;}
            state=8;break;
        case 4:
            if(ch=='0') NEXT(); else state=8; break;
        case 8:
            if(!CLASS(ch,8)) {if(ch=='.') {state=32;NEXT();} else state=64;break;}
            if(decimal.count<20) decimal.digits[decimal.count++]=ch; else scale++;
            NEXT();break;
        case 16:
            if(!CLASS(ch,8)) state=0x1000; else state=32; break;
        case 32:
            if(!CLASS(ch,8)) {state=64;break;}
            if(decimal.count<20) {if(ch!='0'||decimal.count!=0) decimal.digits[decimal.count++]=ch;scale--;}
            NEXT();break;
        case 64:
            if(UPPER(ch)=='E') {state=128;NEXT();} else state=0x800; break;
        case 128:
            if(ch=='+') NEXT(); else if(ch=='-') {NEXT();exponentNegative=1;}
            state=256;break;
        case 256:
            if(!CLASS(ch,8)) {state=0x1000;break;}
            if(ch=='0') {state=512;NEXT();} else state=1024; break;
        case 512:
            if(ch=='0') NEXT(); else state=1024; break;
        case 1024:
            if(!CLASS(ch,8)) {state=0x800;break;}
            exponent=10*exponent+(ch-'0'); if(exponent>32767) *overflow=1;
            NEXT();break;
        case 0x8000:
            switch(hexState) {
            case 1:
                hexData=hex;memset(hexData,0,8);
                hexIntegerDigits=0;hexIndex=0;limit=14;hexState=2;NEXT();break;
            case 2:
                if(ch=='0') NEXT(); else hexState=4; break;
            case 4:
                if(!CLASS(ch,0x400)) {if(ch=='.') {hexState=8;NEXT();} else hexState=16;break;}
                if((unsigned)hexIntegerDigits<limit) {hexIntegerDigits++;{ unsigned char digit, byte = hexData[hexIndex/2]; ch=UPPER(ch); digit=(unsigned char)(ch>='A' ? ch-55 : ch-48); if(hexIndex%2) byte|=digit; else byte |= (unsigned char)(digit<<4); hexData[hexIndex++/2]=byte; }NEXT();} else NEXT();
                break;
            case 8:
                if(!CLASS(ch,0x400)) {hexState=16;break;}
                if((unsigned)hexIntegerDigits<limit) {{ unsigned char digit, byte = hexData[hexIndex/2]; ch=UPPER(ch); digit=(unsigned char)(ch>='A' ? ch-55 : ch-48); if(hexIndex%2) byte|=digit; else byte |= (unsigned char)(digit<<4); hexData[hexIndex++/2]=byte; }NEXT();} else NEXT();
                break;
            case 16:
                if(UPPER(ch)=='P') {hexState=32;NEXT();} else state=0x800; break;
            case 32:
                if(ch=='-') hexExponentNegative=1;
                else if(ch!='+') {readFn(context,ch,1);readCount--;}
                hexState=64;NEXT();break;
            case 64:
                if(!CLASS(ch,8)) {state=0x1000;break;}
                if(ch=='0') {hexState=128;NEXT();} else hexState=256; break;
            case 128:
                if(ch=='0') NEXT(); else hexState=256; break;
            case 256:
                if(!CLASS(ch,8)) {state=0x800;break;}
                hexExponent=10*hexExponent+(ch-'0');if(exponent>32767) *overflow=1;
                NEXT();break;
            }
            break;
        }
    }
    if(state!=0x8000 ? !(state&0xe2c) : (readCount-1<=2 || !(hexState&0x18e))) *consumed=0;
    else *consumed=readCount-1+whitespace;
    readFn(context,ch,1);
    if(!hexState) {
        double value;
        int count;
        unsigned char *p;
        if(exponentNegative) exponent=-exponent;
        count=decimal.count;
        p=decimal.digits+count;
        while(count-- && *--p=='0') scale++;
        decimal.count=count+1;
        if(!decimal.count) decimal.digits[decimal.count++]='0';
        exponent+=scale;
        if(exponent < -32768 || exponent>32767) *overflow=1;
        if(*overflow) {if(exponentNegative) return 0.0;return negative ? 0.0-__double_huge : __double_huge;}
        decimal.exponent=exponent;
        value=__dec2num(&decimal);
        if(value!=0.0 && value<2.2250738585072014e-308) *overflow=1;
        else if(value>1.7976931348623157e308) {*overflow=1;value=__double_huge;}
        if(negative && (state&0xe2c)) value=0.0-value;
        return value;
    } else {
        double result;
        int shift;unsigned int bits;int position;
        unsigned char *p8=(unsigned char *)&result;
        unsigned int leading;unsigned char byte;int source;int target;
        if(hexExponentNegative) hexExponent=-hexExponent;
        hexExponent+=hexIntegerDigits<<2;
        leading=0;
        while(leading<4 && !(hex[0]&(0x80>>leading))) {leading++;hexExponent--;}
        shift=leading+1;
        if(shift) {
            unsigned char carry=0;
            hexData=hex+7;
            while(hexData>=hex) {unsigned char byte=*hexData;*hexData=(byte<<shift)|carry;carry=byte>>(8-shift);hexData--;}
        }
        { bits=0;source=0;position=12;target=1;
        memset(p8,0,8);
          do {
              byte=hex[source];
              if(bits+8>52) byte&=255<<(52-bits);
              p8[target]|=(unsigned char)(byte>>(position&7));target++;bits+=8;
              p8[target]|=(unsigned char)(byte<<(8-(position&7)));position+=8;source++;
          } while(bits<52);
          {unsigned int biased=hexExponent+1022;
           if(biased&~2047) {*overflow=1;return 0.0;}
           p8[0]|=(biased<<21)>>25;p8[1]|=(biased<<21)>>17;
          }
        }
        if(negative) p8[0]|=128;
        for(i=0;i<4;i++) {unsigned char byte=p8[i];p8[i]=p8[7-i];p8[7-i]=byte;}
        return *(double *)p8;
    }
}
#pragma opt_strength_reduction reset
#pragma opt_lifetimes reset
