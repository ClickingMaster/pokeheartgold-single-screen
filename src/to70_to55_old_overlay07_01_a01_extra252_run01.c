#pragma force_active on
#include "global.h"
int abs(int);
#pragma force_active on
struct A{u8 pad[24];u32*pc;};struct T{u32 words[13];};struct Data{u8 pad[192];u8 first[32];u8 second[1];};struct Context{u8 pad[192];struct Data*data;u8 gap[64];struct T templ;};int ov07_0221C468(void*);int ov07_0221C470(void*);int ov07_0223192C(void*,int);int ov07_0221BFC0(void*);s8 ov07_0221F8C8(void*,int);s8 ov07_0221F980(int,int,int);int ov07_0221F9A8(struct A*,u32*,int);void ov07_0221F9E8(struct T*,struct Context*);u8 ov07_0221FA04(struct Context*,int);u8 ov07_0221FA10(struct Context*,int);
s8 ov07_0221F8C8(void*c,int value){int a=ov07_0221C468(c),b=ov07_0221C470(c);ov07_0223192C(c,a);ov07_0223192C(c,b);if(ov07_0223192C(c,a)==3&&ov07_0223192C(c,b)==4){}else if(ov07_0223192C(c,a)==4&&ov07_0223192C(c,b)==3)value*=-1;else if(ov07_0223192C(c,a)==3&&ov07_0223192C(c,b)==3){if(value>0)value*=-1;}else if(ov07_0223192C(c,a)==4&&ov07_0223192C(c,b)==4){if(value<0)value*=-1;}if(ov07_0221BFC0(c)==1)value*=-1;return(s8)value;}
s8 ov07_0221F980(int a,int b,int delta){if(a<b)return(s8)abs(delta);else if(a>b)return(s8)-abs(delta);return 0;}
