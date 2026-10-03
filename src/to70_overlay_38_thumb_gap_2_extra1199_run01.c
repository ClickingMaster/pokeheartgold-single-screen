#pragma force_active on
#include "global.h"
typedef struct {s32 state,last,error,length;void *a,*b,*c;void *output;s32 capacity;char *request,*hex;void *d,*e;} NetState;
typedef struct {char bytes[17];} Hex;
typedef struct {char bytes[21];} Prefix;
extern NetState _02225040;extern Hex _02224E4C;extern Prefix ov38_02225074;
extern char ov38_0222508C[],ov38_02225094[],ov38_0222509C[],ov38_022250B4[],ov38_022250CC[],ov38_022250E4[],ov38_022250FC[],ov38_02225114[];
extern char *strncat(char *,const char *,u32),*strcat(char *,const char *);
extern unsigned int strlen(const char *);extern int strncmp(const char *,const char *,u32);extern void *memcpy(void *,const void *,u32);
extern int ov38_0221BA38(void *,void *,void *,void *,void *);
void ov38_0221BBD0(char *,int,int);
void ov38_0221BBD0(char *response,int length,int error) {
 char text[54];Hex alphabet;char *digest;int i,j,result;
 _02225040.last=-1;
 if(_02225040.state==1)return;
 if(error==0) {
  switch(_02225040.state) {
  case 4:
   if(length==32) {
    digest=_02225040.hex+20;alphabet=_02224E4C;
    {const u8 *src=(const u8*)ov38_02225074.bytes;u8 *dst=(u8*)text;u32 count=21;do {*dst++=*src++;}while(--count); }
    strncat(text,response,length);MATH_CalcSHA1(digest,text,strlen(text));strcat(_02225040.request,ov38_0222508C);
    for(i=0,j=0;i<20;i++,j+=2) {
     _02225040.hex[j]=alphabet.bytes[*(u8*)((u32)digest+i)>>4];
     _02225040.hex[j+1]=alphabet.bytes[*(u8*)((u32)digest+i)&15];
    }
    _02225040.hex[40]=0;strcat(_02225040.request,ov38_02225094);
    result=ov38_0221BA38(_02225040.a,_02225040.b,_02225040.c,_02225040.d,_02225040.e);
    switch(result){case 0:break;case 1:case 2:_02225040.state=1;return;}
    _02225040.state=5;return;
   }
   _02225040.state=1;return;
  case 6:
   if(length==22) {
    if(strncmp(response,ov38_0222509C,22)==0){_02225040.state=1;_02225040.error=26;return;}
    if(strncmp(response,ov38_022250B4,22)==0){_02225040.state=1;_02225040.error=27;return;}
    if(strncmp(response,ov38_022250CC,22)==0){_02225040.state=1;_02225040.error=28;return;}
    if(strncmp(response,ov38_022250E4,22)==0){_02225040.state=1;_02225040.error=29;return;}
    if(strncmp(response,ov38_022250FC,22)==0){_02225040.state=1;_02225040.error=30;return;}
    if(strncmp(response,ov38_02225114,22)==0){_02225040.state=1;_02225040.error=31;return;}
   }
   if(length<=_02225040.capacity){memcpy(_02225040.output,response,length);_02225040.state=7;}
   else {memcpy(_02225040.output,response,_02225040.capacity);_02225040.state=1;_02225040.error=32;}
   _02225040.length=length;return;
  }
 }else {
  _02225040.state=1;
  switch(error) {
case 1:_02225040.error=8;return;
case 2:_02225040.error=9;return;
case 3:_02225040.error=10;return;
case 4:_02225040.error=11;return;
case 5:_02225040.error=12;return;
case 6:_02225040.error=13;return;
case 7:_02225040.error=14;return;
case 8:_02225040.error=15;return;
case 9:_02225040.error=16;return;
case 10:_02225040.error=17;return;
case 11:_02225040.error=18;return;
case 12:_02225040.error=19;return;
case 15:_02225040.error=22;return;
 case 16:_02225040.error=23;return;
 case 19:_02225040.error=25;break;
 }
 }
}
