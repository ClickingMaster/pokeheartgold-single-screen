#pragma force_active on
#include "global.h"
typedef struct{u16 a,b;}Entry;extern Entry*sub_02096864(void*);extern void ov42_02228FE0(void*,u16,u8,int);
void ov80_0223947C(u8*w,Entry*new);
void ov80_0223947C(u8*w,Entry*new){Entry*table=sub_02096864(*(void**)(w+8));int i;Entry*p;int j;u16 target=new->a;i=0;p=table;do{if(target==p->a)return;p++;i++;}while(i<24);j=0;p=table;do{if(p->a==65535)break;j++;p++;}while(j<24);if(j==24)GF_AssertFail();table[j]=*new;ov42_02228FE0(*(void**)(w+32),new->a,*(u8*)&new->b,101);}
