#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
#pragma dollar_identifiers on
typedef u16 Wide;
typedef struct Format{u8 align,sign,precision_set,is_pascal,length,pad;u16 conversion;int width,precision;}Format;
typedef int(*Writer)(void*,const Wide*,int);
Wide*wcschr(const Wide*,int);u32 wcslen(const Wide*);Wide*wmemchr(const Wide*,int,u32);int mbstowcs(Wide*,const char*,u32);int mbtowc(Wide*,const char*,u32);
Wide*parse_format__wide(Wide*,void**,Format*);Wide*long2str__wide(int,Wide*,Format);Wide*longlong2str__wide(long long,Wide*,Format);Wide*float2str__wide(double,Wide*,Format);Wide*double2hex__wide(double,Wide*,Format);
extern Wide buff_ptr$6E40[];
extern char strptr$6E44[];
int __wpformatter(Writer,void*,Wide*,void*);
#define NEXT(t) (*(t*)((char*)(args=(char*)args+sizeof(t))-sizeof(t)))
int __wpformatter(Writer write,void*cookie,Wide*format,void*args){Wide buffer[512];Format spec;Wide padding=32,space;char narrow;long long large;int total=0;Wide*start,*text;int length,width;int number;
 while(*format){start=wcschr(format,37);if(!start){length=wcslen(format);total+=length;if(length&&!write(cookie,format,length))return -1;break;}length=start-format;total+=length;if(length&&!write(cookie,format,length))return -1;format=parse_format__wide(start,&args,&spec);
 switch(spec.conversion){
 case 'd':case 'i':
 if(spec.length==3)number=NEXT(long);else if(spec.length==4)large=NEXT(long long);else if(spec.length==6)large=NEXT(long long);else if(spec.length==7)number=NEXT(long);else if(spec.length==8)number=NEXT(unsigned int);else number=NEXT(int);
 if(spec.length==2)number=(s16)number;if(spec.length==4||spec.length==6){text=longlong2str__wide(large,buffer+512,spec);if(!text)goto invalid;}else {text=long2str__wide(number,buffer+512,spec);if(!text)goto invalid;}length=buffer+511-text;break;
 case 'X':case 'o':case 'u':case 'x':
 if(spec.length==3)number=NEXT(unsigned long);else if(spec.length==4)large=NEXT(unsigned long long);else if(spec.length==6)large=NEXT(unsigned long long);else if(spec.length==7)number=NEXT(unsigned long);else if(spec.length==8)number=NEXT(u32);else number=NEXT(unsigned int);
 if(spec.length==2)number=(u16)number;if(spec.length==4||spec.length==6){text=longlong2str__wide(large,buffer+512,spec);if(!text)goto invalid;}else {text=long2str__wide(number,buffer+512,spec);if(!text)goto invalid;}length=buffer+511-text;break;
 case 'E':case 'F':case 'G':case 'e':case 'f':case 'g':{double value;if(spec.length==9)value=NEXT(long double);else value=NEXT(double);text=float2str__wide(value,buffer+512,spec);if(!text)goto invalid;length=buffer+511-text;break;}
 case 'A':case 'a':{double value;if(spec.length==9)value=NEXT(long double);else value=NEXT(double);text=double2hex__wide(value,buffer+512,spec);if(!text)goto invalid;length=buffer+511-text;break;}
 case 's':if(spec.length==5){text=NEXT(Wide*);if(!text)text=buff_ptr$6E40;if(spec.is_pascal){length=*text++&255;if(spec.precision_set&&length>spec.precision)length=spec.precision;}else if(spec.precision_set){Wide*end;length=spec.precision;end=wmemchr(text,0,length);if(end)length=end-text;}else length=wcslen(text);}else{char*str=NEXT(char*);int count;if(!str)str=strptr$6E44;if(spec.is_pascal){count=*text&255;if(spec.precision_set&&count>spec.precision)count=spec.precision;}else if(spec.precision_set){char*end;count=spec.precision;end=memchr(str,0,count);if(end)count=end-str;}else count=strlen(str);length=mbstowcs(buffer,str,count);if(length<0)goto invalid;text=buffer;}break;
 case 'n':{text=NEXT(Wide*);switch(spec.length){case 0:*(int*)text=total;break;case 2:*(short*)text=total;break;case 3:*(long*)text=total;break;case 6:*(long long*)text=total;break;case 7:*(int*)text=total;break;case 8:*(int*)text=total;break;case 4:*(long long*)text=total;break;default:break;}continue;}
 case 'c':text=buffer;if(spec.length==5){*text=NEXT(int);length=1;}else{narrow=NEXT(int);length=mbtowc(text,&narrow,1);}break;
 case '%':buffer[0]=37;text=buffer;length=1;break;
 case 65535:default:invalid:length=wcslen(start);if(length&&!write(cookie,start,length))return -1;return total+length;
 }
 width=length;if(spec.align){u32 pad=spec.align==2?48:32;padding=pad;if((*text==43||*text==45||*text==32)&&pad==48){if(!write(cookie,text,1))return -1;text++;length--;}while(width<spec.width){if(!write(cookie,&padding,1))return -1;width++;}}
 if(length&&!write(cookie,text,length))return -1;if(!spec.align)while(width<spec.width){space=32;if(!write(cookie,&space,1))return -1;width++;}total+=width;
 }return total;
}
