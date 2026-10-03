#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
int snprintf(char*,u32,const char*,...);
int sprintf(char*,const char*,...);
typedef struct {int state;u8 a[8];int ready;u8 b[0x44];char*buffer,*header,*end;u8 c[0x10];void*callback;} Work;
extern Work ov45_0225762C;
extern char *ov45_02257628;
extern char ov45_02257750[],ov45_022576CC[],ov45_02257758[],ov45_0225775C[],ov45_0225763C[],ov45_02257780[],ov45_022577AC[],ov45_02257764[],ov45_022576A0[];
void ov00_021EC2EC(int,void*,int);
void*ov00_021EC2A8(int,int);
int ov45_02253D50(void*);
int ov45_022542F4(char*,void*,int,void*,void*);
int ov45_0225478C(char*,int,void*,u8*,void*,int);
int ov45_0225478C(char*name,int value,void*arg,u8*data,void*cb,int flag){
 int d,a,b,n,x,y,z,q;char num[16];struct{int value;u8*data;} pair;
 if(ov45_0225762C.ready!=1)return 3;
 if(ov45_0225762C.buffer){ov00_021EC2EC(7,ov45_0225762C.buffer,0);ov45_0225762C.buffer=0;}
 d=ov45_02253D50(data+12);a=strlen(ov45_02257750);b=strlen(ov45_022576CC);
 n=snprintf(num,16,ov45_02257758,value);x=strlen(ov45_0225775C);y=strlen(name);z=strlen(ov45_02257628);q=strlen(ov45_0225763C);
 ov45_0225762C.buffer=ov00_021EC2A8(7,d+(a+(b+(n+(x+(y+(z+q))))+0x29)));
 if(!ov45_0225762C.buffer)return 2;
 sprintf(ov45_0225762C.buffer,ov45_02257764,ov45_02257628,ov45_0225763C,name,value,ov45_02257780,ov45_022577AC);
 {char*p=ov45_0225762C.buffer;ov45_0225762C.end=p+strlen(p);}
 ov45_0225762C.header=ov45_0225762C.end-strlen(ov45_02257750)-0x28;
 pair.value=value;pair.data=data;
 if(ov45_022542F4(ov45_0225762C.end,&pair,8,arg,data)==2){ov00_021EC2EC(7,ov45_0225762C.buffer,0);ov45_0225762C.buffer=0;return 2;}
 if(flag){memcpy(ov45_0225762C.header,ov45_022576A0,0x28);ov45_0225762C.state=6;}
 else{*(ov45_0225762C.header-strlen(ov45_022576CC))=0;ov45_0225762C.state=4;}
 ov45_0225762C.callback=cb;return 0;
}
