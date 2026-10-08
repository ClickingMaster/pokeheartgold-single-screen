// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern const u16 __msl_digit[];
extern const u8 __upper_mapC[];
u32 __strtoul(int base,int max_width,int (*ReadProc)(void *,int,int),void *ReadProcArg,int *chars_scanned,int *negative,int *overflow);
// @fn __strtoul
u32 __strtoul(int base,int max_width,int (*ReadProc)(void *,int,int),void *ReadProcArg,int *chars_scanned,int *negative,int *overflow) {
    int scan_state;
    int count;
    int spaces;
    u32 value;
    u32 value_max;
    int c;
    scan_state=1;
    count=0;
    spaces=0;
    value=0;
    value_max=0;
    *overflow=0;
    *negative=0;
    if(base<0||base==1||base>36||max_width<1) scan_state=64;
    else {
        count++;
        c=ReadProc(ReadProcArg,0,0);
    }
    if(base!=0) value_max=0xFFFFFFFF/base;
    while(count<=max_width&&c!=-1&&!(scan_state&0x60)) {
        switch(scan_state) {
        case 1:
            if((c<0||c>=128?0:__msl_digit[c]&0x100)) {
                c=ReadProc(ReadProcArg,0,0);
                spaces++;
                break;
            }
            if(c=='+') {
                count++;
                c=ReadProc(ReadProcArg,0,0);
            } else if(c=='-') {
                count++;
                c=ReadProc(ReadProcArg,0,0);
                *negative=1;
            }
            scan_state=2;
            break;
        case 2:
            if((base==0||base==16)&&c=='0') {
                scan_state=4;
                count++;
                c=ReadProc(ReadProcArg,0,0);
            } else scan_state=8;
            break;
        case 4:
            if(c=='X'||c=='x') {
                base=16;
                scan_state=8;
                count++;
                c=ReadProc(ReadProcArg,0,0);
            } else {
                if(base==0) base=8;
                scan_state=16;
            }
            break;
        case 8:
        case 16:
            if(base==0) base=10;
            if(value_max==0) value_max=0xFFFFFFFF/base;
            if((c<0||c>=128?0:__msl_digit[c]&8)) {
                c-='0';
                if(c>=base) {
                    if(scan_state==16)scan_state=32;else scan_state=64;
                    c+='0';
                    break;
                }
            } else if(!(c<0||c>=128?0:__msl_digit[c]&1)||(c<0||c>=128?c:__upper_mapC[c])-'A'+10>=base) {
                if(scan_state==16)scan_state=32;else scan_state=64;
                break;
            } else c=(c<0||c>=128?c:__upper_mapC[c])-'A'+10;
            scan_state=16;
            if(value>value_max) *overflow=1;
            value*=base;
            if((u32)c>0xFFFFFFFF-value) *overflow=1;
            value+=c;
            count++;
            c=ReadProc(ReadProcArg,0,0);
            break;
        }
    }
    if(!(scan_state&0x34)) {
        value=0;
        *chars_scanned=0;
    } else *chars_scanned=count-1+spaces;
    ReadProc(ReadProcArg,c,1);
    return value;
}
