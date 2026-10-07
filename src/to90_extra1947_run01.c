#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
typedef struct{u8 data[4];} Bytes4;typedef struct{u8 data[2];} Bytes2;typedef struct{u8 pad[0x4a8];u16 port;} Work;
extern int ov00_02214C60(Work*,void*,const u8*,int,int);
extern void*ov00_022132C4(Work*,u32,u16);extern int ov00_0221339C(void*),ov00_02214A44(u8),ov00_02214AF4(Work*,const u8*,int),ov00_02214A70(const u8*,int);extern void ov00_02213C6C(Work*,void*);extern u8 ov00_0221A63C[];

void ov00_02214BF0(Work*w,const u8*p,int length,Bytes4*out,Bytes2*port);
int ov00_02214F2C(Work*w,const u8*p,int length);

typedef struct{char*name;int type;} Key;
extern int ArrayLength(void*);extern Key*ov00_021FE6EC(void*,int);extern void ov00_02213370(void*,u8),ov00_02213378(void*,u32,u16),ov00_02213384(void*,u32),ov00_02212D10(void*,const char*,int),ov00_02212CD0(void*,const char*,const char*);extern u8 ov00_02213394(void*);extern void ov00_0221338C(void*,u8);

#pragma force_active on
int ov00_02214C60(Work*w,void*record,const u8*p,int length,int decode){struct{int length;u16 port,number;u32 value;} frame;int left=length;u32 flags;flags=*p;p+=5;frame.length=length;ov00_02213370(record,(u8)flags);left-=5;if(flags&0x10){p+=2;left-=2;}if(flags&2){memcpy(&frame.value,p,4);p+=4;left-=4;}else frame.value=0;if(flags&0x20){memcpy(&frame.port,p,2);p+=2;left-=2;}else frame.port=w->port;ov00_02213378(record,frame.value,frame.port);if(flags&8){memcpy(&frame.value,p,4);p+=4;left-=4;ov00_02213384(record,frame.value);}if(flags&0x40){int count=ArrayLength(*(void**)((u8*)w+8));int i;for(i=0;i<count;i++){Key*k=ov00_021FE6EC(*(void**)((u8*)w+8),i);switch(k->type){case 1:ov00_02212D10(record,k->name,*p++);left--;break;case 2:memcpy(&frame.number,p,2);ov00_02212D10(record,k->name,(u16)(((int)frame.number>>8)&255 | ((int)frame.number<<8)&0xff00));p+=2;left-=2;break;case 0:{int code;if(decode){code=*p++;left--;}else code=255;if(code==255){int n;ov00_02212CD0(record,k->name,(const char*)p);n=strlen((const char*)p)+1;p+=n;left-=n;}else ov00_02212CD0(record,k->name,((const char**)((u8*)w+0x84))[code]);break;}default:break;}}ov00_0221338C(record,(u8)(ov00_02213394(record)|1));}if(flags&0x80){while(*(const s8*)p!=0 && left>0){const char*name=(const char*)p;int n=strlen(name)+1;p+=n;left-=n;ov00_02212CD0(record,name,(const char*)p);n=strlen((const char*)p)+1;p+=n;left-=n;}left--;ov00_0221338C(record,(u8)(ov00_02213394(record)|2));}return frame.length-left;}
