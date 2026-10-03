#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 justify,sign,flag,alt,length,pad;u16 conversion;int width,precision;} Format;
u16 *longlong2str__wide(u64 value,u16 *end,Format f);

#pragma force_active on
u16 *longlong2str__wide(u64 value,u16 *end,Format f){
 u64 base;u16 *p;int negative,precision,sign,count; s32 width,conversion,alt;int digit;
 p=end;*--p=0;precision=f.precision;alt=f.alt;conversion=f.conversion;negative=0;count=0;width=f.width;sign=f.sign;
 if(value==0 && precision==0){if(!alt || conversion!='o') return p;}
 switch(conversion){case 'd':case 'i':base=10;if((s64)value<0){if(value!=0x8000000000000000ULL)value=-value;negative=1;}break;case 'o':sign=0;base=8;break;case 'u':sign=0;base=10;break;case 'x':case 'X':sign=0;base=16;break;}
 do{digit=value%base;value/=base;if(digit<10)digit+='0';else if(conversion=='x')digit+='a'-10;else digit+='A'-10;*--p=digit;count++;}while(value);
 if(base==8 && alt && *p!='0'){*--p='0';count++;}
 if(f.justify==2){precision=width;if(negative || sign)precision--;if(base==16 && alt)precision-=2;}
 if(precision+(end-p)>509)return 0;
 while(count<precision){*--p='0';count++;}
 if(base==16 && alt){*--p=conversion;*--p='0';}
 if(negative)*--p='-';else if(sign==1)*--p='+';else if(sign==2)*--p=' ';
 return p;
}
