#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void ov00_022115B0(void *,void *,int,u8 *,int,u8 *,int,u8 *);
void ov00_02211608(void *,void *,u8 *,int);
typedef struct {u8 entries[256];int count;} List;
typedef struct {char data[0x800];int length;} Buffer;
typedef struct {u8 p[0x88];void (*base)(u8,Buffer *,void *);void (*first)(u8,int,Buffer *,void *);void (*second)(u8,int,Buffer *,void *);void (*list)(int,List *,void *);int (*count)(int,void *);u8 q[0x10c-0x9c];void *context;} Work;
extern char *ov00_0221A0B8[];extern char ov00_02219EFC[],ov00_02219F04[],ov00_02219F08[];extern void ov00_02210DFC(Buffer *,char *);extern int sprintf(char *,const char *,...);
void ov00_022116C0(Work *,Buffer *,int);
void ov00_02211608(void *a,void *b,u8 *data,int length){u8 x,y,z;u8 *p=0,*q=0,*r=0;if(length<3)return;x=*data++;length--;if(x!=0&&x!=255){p=data;length-=x;data+=x;}if(length<2)return;y=*data++;length--;if(y!=0&&y!=255){q=data;length-=y;data+=y;}if(length<1)return;z=*data;length--;if(z!=0&&z!=255){length-=z;r=data+1;}if(length<0)return;ov00_022115B0(a,b,x,p,y,q,z,r);}
