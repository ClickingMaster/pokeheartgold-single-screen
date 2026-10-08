#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 justify,sign,flag,alt,length,conversion,pad[2];int width,precision;} Format;
typedef struct {u8 style,pad;s16 digits;} DecForm;
typedef struct {s8 sign,pad;s16 exp;u8 count;u8 digits[32];} Decimal;
typedef struct DecimalStage {DecForm form;Decimal decimal;Format integerFormat;} DecimalStage;
extern void __num2dec(const void*,double,Decimal*);
extern double fabs(double);
typedef struct {char bytes[4];} S4;typedef struct {char bytes[5];} S5;typedef struct {char bytes[6];} S6;
#define COPY(n,p,s) memcpy((p),(s),(n))
extern char printf_double2hex_0p0[];
extern char printf_double2hex_neg_inf_upper[];
extern char printf_double2hex_neg_inf_lower[];
extern char printf_double2hex_inf_upper[];
extern char printf_double2hex_inf_lower[];
extern char printf_double2hex_neg_nan_upper[];
extern char printf_double2hex_neg_nan_lower[];
extern char printf_double2hex_nan_upper[];
extern char printf_double2hex_nan_lower[];
extern char *long2str(int,char*,Format);
char *double2hex(double value,char *end,Format f);
#pragma push
#pragma opt_propagation off
// @fn double2hex
char *double2hex(double value,char *end,Format f){
 DecimalStage stage;int sign;int alt;int conversion;int precision;int i;char temp;char *p;double v=value;
 precision=f.precision;if(precision>509)return 0;conversion=f.conversion;alt=f.alt;sign=f.sign;stage.form.style=0;stage.form.digits=32;__num2dec(&stage.form,v,&stage.decimal);
 if(fabs(value)==0){p=end-6;COPY(6,p,printf_double2hex_0p0);return p;}
 if(stage.decimal.digits[0]=='I'){if(stage.decimal.sign){p=end-5;if(conversion=='A')COPY(5,p,printf_double2hex_neg_inf_upper);else COPY(5,p,printf_double2hex_neg_inf_lower);}else{p=end-4;if(conversion=='A')COPY(4,p,printf_double2hex_inf_upper);else COPY(4,p,printf_double2hex_inf_lower);}return p;}
 if(stage.decimal.digits[0]=='N'){if(stage.decimal.sign){p=end-5;if(conversion=='A')COPY(5,p,printf_double2hex_neg_nan_upper);else COPY(5,p,printf_double2hex_neg_nan_lower);}else{p=end-4;if(conversion=='A')COPY(4,p,printf_double2hex_nan_upper);else COPY(4,p,printf_double2hex_nan_lower);}return p;}
 {char *bytes;
 stage.integerFormat.justify=1;stage.integerFormat.sign=1;stage.integerFormat.flag=0;stage.integerFormat.alt=0;stage.integerFormat.length=0;stage.integerFormat.width=0;stage.integerFormat.precision=1;stage.integerFormat.conversion='d';
 bytes=(char*)&value;i=0;do{temp=bytes[i];bytes[i]=bytes[7-i];bytes[7-i]=temp;}while(++i<4);
 {int exponent=(((u32)(u8)((char*)&value)[0]<<25)|((u32)(u8)((char*)&value)[1]<<17))>>21&2047;p=long2str(exponent-1023,end,stage.integerFormat);}

 if(conversion=='a')*--p='p';else *--p='P';
 {char *b=(char*)&value;int k,index=(u32)precision*4+11;u8 digit;for(k=precision;k>=1;k--){if(index<64){digit=(u8)b[index>>3]>>(7-(index&7));if((index&~7)!=((index-4)&~7))digit|=((u8)b[(index>>3)-1]<<8)>>(7-(index&7));digit&=15;if(digit<10)digit+='0';else if(conversion=='a')digit+='a'-10;else digit+='A'-10;}else digit='0';*--p=digit;index-=4;}}
 if(precision||alt)*--p='.';p[-1]='1';*(p-=2)=conversion=='a'?'x':'X';*--p='0';
 if(stage.decimal.sign)*--p='-';else if(sign==1)*--p='+';else if(sign==2)*--p=' ';
 return p;
 }
}
#pragma pop
