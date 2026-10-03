#pragma force_active on
#include "global.h"
#pragma force_active on
#include <string.h>
extern int sprintf(char*,const char*,...);
typedef struct {int state;u8 p[12];int a,b,c,d,e;char *buffer,*next;int end,length;} Global;
extern Global _02225040;extern char ov38_0222512C[],ov38_0222508C[],ov38_02225094[];
extern int ov38_0221BB44(int);extern void *ov00_021EC2A8(int,int);
int ov38_0221BEA8(char*,int,int,int,int,int);
#pragma opt_propagation off
int ov38_0221BEA8(char*s,int a,int b,int c,int d,int e) {int len,n;char *p;if(_02225040.state!=2)return 1;_02225040.a=a;_02225040.b=b;_02225040.c=c;_02225040.d=d;_02225040.e=e;c+=8;len=strlen(s);n=ov38_0221BB44(c);_02225040.buffer=ov00_021EC2A8(10,len+69+n);if(_02225040.buffer==0)return 2;sprintf(_02225040.buffer,ov38_0222512C,s,a);p=_02225040.buffer;len=strlen(p);n=strlen(ov38_0222508C);_02225040.next=p+len+n;n=strlen(ov38_02225094);_02225040.end=(int)(_02225040.next+40+n);_02225040.length=ov38_0221BB44(c)+1;_02225040.state=3;return 0;}
