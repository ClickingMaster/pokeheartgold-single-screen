#pragma force_active on
#include "global.h"
#pragma force_active on
u32 MTRandom(void);
void ov91_0225E728(void *,VecFx32 *),ov91_0225EA54(void *,VecFx32 *);
void ov91_0225E7E8(u8 *,void *,int,fx32,int);
#define W(p,o) (*(fx32 *)((p)+(o)))
#define MUL(a,b) ((fx32)(((s64)(a)*(b)+2048)>>12))
int ov91_0225DFB0(void *,void *),ov91_0225E990(void *),ov91_0225E9C0(void *);
void ov91_0225E7C4(void *,void *,int),ov91_0225E3F4(void *,void *,int,void *);
void ov91_0225E8BC(u8 *,u8 *);
void ov91_0225E7E8(u8 *w,void *ctx,int reverse,fx32 factor,int jitter){
 VecFx32 v;
 if(reverse){
 W(w,8)=-W(w,8);W(w,16)=-W(w,16);
 if(jitter){
 u32 r=MTRandom();
 int y=W(w,16);
 int ay=y<0?-y:y;
 int x=W(w,8);
 int ax=x<0?-x:x;
 if(ax<=ay){int a=MUL(y,4096);if((r&1)==1)W(w,8)=-a;else W(w,8)=a;}
 else{int a=MUL(x,4096);if((r&1)==1)W(w,16)=-a;else W(w,16)=a;}
 }
 }
 W(w,20)=MUL(W(w,20),factor);*(u16 *)(w+2)=0;w[0]=5;
 ov91_0225E728(w,&v);ov91_0225EA54(w,&v);(*(u16 *)(w+2))++;
}

void ov91_0225E8BC(u8 *w,u8 *ctx){
 int done=0;
 int old;
 VecFx32 v;
 if(w[0]==5){
 old=W(w,48);ov91_0225E728(w,&v);ov91_0225EA54(w,&v);(*(u16 *)(w+2))++;
 if(ov91_0225DFB0(ctx+0x19cc,w+44)==1)ov91_0225E7C4(w,ctx,1);
 else if(W(w,48)<=-0x32000){
 if(ov91_0225E990(w)==0){if(old<=-0x32000)done=1;else ov91_0225E7C4(w,ctx,0);}
 else if(W(w,48)<=-0x64000)done=1;
 if(W(w,20)<=8192)done=1;
 }else if(ov91_0225E9C0(w)==1){
 VEC_Subtract((VecFx32 *)(w+44),&v,(VecFx32 *)(w+44));ov91_0225E7E8(w,ctx,1,1024,0);
 }
 if(W(w,20)<=4096)done=1;
 if(done==1)ov91_0225E3F4(ctx,w,0,*(void **)(ctx+28));
 }
}
