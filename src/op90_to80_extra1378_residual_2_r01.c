#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
typedef struct Format {u8 justify,sign,precisionSet,alternate,length,conversion;u8 pad[2];int width,precision;} IntegerFormat;
typedef IntegerFormat ParsedFormat;
typedef IntegerFormat LargeFormat;
typedef struct {ParsedFormat info;char buffer[512];} OutputWorkspace;
typedef char WorkspaceSizeProof[sizeof(OutputWorkspace)==528?1:-1];
typedef char WorkspaceBufferProof[offsetof(OutputWorkspace,buffer)==16?1:-1];
typedef char IntegerSizeProof[sizeof(IntegerFormat)==16?1:-1];
typedef char LargeSizeProof[sizeof(LargeFormat)==16?1:-1];
typedef u32 (*Writer)(void*,const char*,u32);
extern const char __pformatter_empty_string_1[],__pformatter_empty_string_2[];
extern char *strchr(const char*,int);
extern unsigned int strlen(const char*);
extern void *memchr(const void*,int,u32);
extern unsigned int wcstombs(char*,const u16*,unsigned int);
extern char *parse_format(char*,va_list*,ParsedFormat*);
extern char *long2str(s32,char*,IntegerFormat);
extern char *longlong2str(u64,char*,LargeFormat);
extern char *float2str(double,char*,ParsedFormat);
extern char *double2hex(double,char*,ParsedFormat);
int __pformatter(Writer,void*,const char*,va_list);
static inline char *Small(s32 n,char *end,const ParsedFormat *f){return long2str(n,end,*f);}
static inline char *Large(u64 n,char *end,const ParsedFormat *f){return longlong2str(n,end,*f);}
typedef struct{u8 style,pad;s16 digits;}DecFormat;
struct Dec{u8 sign,pad;s16 exponent;u8 length,digits[32];};
typedef struct RoundedDecimal{u8 sign,pad;s16 exponent;u8 count;s8 digits[33];}RoundedDecimal;
typedef union{struct Dec produced;RoundedDecimal rounded;}DecimalBacking;
typedef IntegerFormat Format;
extern void __num2dec(const void*,double,struct Dec*);extern void round_decimal(RoundedDecimal*,int);
extern const u16 __msl_digit[];
extern const char printf_double2hex_neg_inf_upper[],printf_double2hex_neg_inf_lower[],printf_double2hex_inf_upper[],printf_double2hex_inf_lower[],printf_double2hex_neg_nan_upper[],printf_double2hex_neg_nan_lower[],printf_double2hex_nan_upper[],printf_double2hex_nan_lower[];
char*float2str(double,char*,Format);
typedef struct {DecFormat format;DecimalBacking value;} DecimalStage;
typedef char Format16Proof[sizeof(Format)==16?1:-1];
typedef char Decimal38Proof[sizeof(DecimalBacking)==38?1:-1];
typedef char Stage42Proof[sizeof(DecimalStage)==42?1:-1];

// @fn __pformatter
int __pformatter(Writer write,void *context,const char *format,va_list ap){OutputWorkspace workspace;int field;char padding=' ',space;double value;s64 wide;const char *percent;int length;register int number;register char *text;int count=0;if(!*format)goto done;do{percent=strchr(format,'%');if(!percent){length=strlen(format);count+=length;if(length && !write(context,format,length))return -1;break;}length=percent-format;count+=length;if(length && !write(context,format,length))return -1;format=parse_format((char*)percent,&ap,&workspace.info);switch(workspace.info.conversion){case 'd':case 'i':
if(workspace.info.length==3){ap+=4;number=*(long*)(ap-4);}else if(workspace.info.length==4){ap+=8;wide=*(s64*)(ap-8);}else if(workspace.info.length==6){ap+=8;wide=*(s64*)(ap-8);}else if(workspace.info.length==7){ap+=4;number=*(s32*)(ap-4);}else if(workspace.info.length==8){ap+=4;number=*(u32*)(ap-4);}else{ap+=4;number=*(int*)(ap-4);}if(workspace.info.length==2)number=(s16)number;if(workspace.info.length==1)number=(s8)number;if(workspace.info.length==4 ||workspace.info.length==6){text=Large(wide,workspace.buffer+512,&workspace.info);if(!text)goto invalid;}else {text=Small(number,workspace.buffer+512,&workspace.info);if(!text)goto invalid;}length=workspace.buffer+511-text;break;
case 'o':case 'u':case 'x':case 'X':
if(workspace.info.length==3){ap+=4;number=*(unsigned long*)(ap-4);}else if(workspace.info.length==4){ap+=8;wide=*(u64*)(ap-8);}else if(workspace.info.length==6){ap+=8;wide=*(u64*)(ap-8);}else if(workspace.info.length==7){ap+=4;number=*(s32*)(ap-4);}else if(workspace.info.length==8){ap+=4;number=*(u32*)(ap-4);}else{ap+=4;number=*(unsigned int*)(ap-4);}if(workspace.info.length==2)number=(u16)number;if(workspace.info.length==1)number=(u8)number;if(workspace.info.length==4 ||workspace.info.length==6){text=Large(wide,workspace.buffer+512,&workspace.info);if(!text)goto invalid;}else {text=Small(number,workspace.buffer+512,&workspace.info);if(!text)goto invalid;}length=workspace.buffer+511-text;break;
case 'f':case 'F':case 'e':case 'E':case 'g':case 'G':{if(workspace.info.length==9){ap+=8;value=*(long double*)(ap-8);}else{ap+=8;value=*(double*)(ap-8);}text=float2str(value,workspace.buffer+512,workspace.info);if(!text)goto invalid;length=workspace.buffer+511-text;break;}
case 'a':case 'A':{if(workspace.info.length==9){ap+=8;value=*(long double*)(ap-8);}else{ap+=8;value=*(double*)(ap-8);}text=double2hex(value,workspace.buffer+512,workspace.info);if(!text)goto invalid;length=workspace.buffer+511-text;break;}
case 's':if(workspace.info.length==5){const u16 *value;ap+=4;value=*(const u16**)(ap-4);if(!value)value=(const u16*)__pformatter_empty_string_1;if((s32)wcstombs(workspace.buffer,value,512)<0)goto invalid;text=workspace.buffer;}else{ap+=4;text=*(char**)(ap-4);}if(!text)text=(char*)__pformatter_empty_string_2;if(workspace.info.alternate){length=(u8)*text++;if(workspace.info.precisionSet && length>workspace.info.precision)length=workspace.info.precision;}else if(workspace.info.precisionSet){const char *end;length=workspace.info.precision;end=memchr(text,0,length);if(end)length=end-text;}else length=strlen(text);break;
case 'n':{ap+=4;text=*(char**)(ap-4);switch(workspace.info.length){case 0:*(int*)text=count;break;case 2:*(s16*)text=count;break;case 3:*(long*)text=count;break;case 6:*(s64*)text=count;break;case 7:*(int*)text=count;break;case 8:*(int*)text=count;break;case 4:*(s64*)text=count;break;}continue;}
case 'c':ap+=4;workspace.buffer[0]=*(int*)(ap-4);text=workspace.buffer;length=1;break;
case '%':workspace.buffer[0]='%';text=workspace.buffer;length=1;break;
case 255:goto invalid;default:invalid:{int remainder=strlen(percent);if(remainder && !write(context,percent,remainder))return -1;return count+remainder;}
}{field=length;if(workspace.info.justify!=0){padding=workspace.info.justify==2?'0':' ';if((*text=='+'||*text=='-'||*text==' ')&&padding=='0'){if(!write(context,text,1))return -1;text++;length--;}while(field<workspace.info.width){if(!write(context,&padding,1))return -1;field++;}}if(length && !write(context,text,length))return -1;if(workspace.info.justify==0){while(field<workspace.info.width){space=' ';if(!write(context,&space,1))return -1;field++;}}count+=field;}}while(*format);done:return count;}
