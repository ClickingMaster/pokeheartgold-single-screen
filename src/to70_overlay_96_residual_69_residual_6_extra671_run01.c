#pragma force_active on
#include "global.h"
typedef struct{void *a,*b,*c;} Work;
typedef struct{u32 a,b,c;} Triple;
typedef struct{void *sprite;Triple data;} Result;
typedef struct{void *a,*window,*image,*palette;u32 e,f,g,h,i,j,k;void *l;} Template;
extern void FillWindowPixelBufferText_AssumeTileSize32(void *,int),AddTextPrinterParameterizedWithColorAndSpacing(void *,int,void *,int,int,int,int,int,int,int),sub_02021AC8(void *,int,int,Triple *),sub_02013880(void *,int);
extern void *ov96_021EB5E8(void *),*ObjPlttTransfer_GetPlttProxyByID(int),*sub_020135D8(Template *);
void ov96_021F9134(Work *,void *,void *,int,int,int,int,Result *);
void ov96_021F9134(Work *w,void *window,void *str,int x,int arg4,int arg5,int arg6,Result *out){Template t;Triple data;void *s;FillWindowPixelBufferText_AssumeTileSize32(window,0);AddTextPrinterParameterizedWithColorAndSpacing(window,0,str,x,0,255,0x10200,0,0,0);sub_02021AC8(*(void**)((u8*)window+0x14),1,2,&data);t.a=w->c;t.window=window;t.image=ov96_021EB5E8(w->b);t.palette=ObjPlttTransfer_GetPlttProxyByID(104);t.e=arg6;t.f=data.b;t.g=arg4;t.h=arg5;t.i=0;t.j=0;t.k=2;t.l=w->a;s=sub_020135D8(&t);sub_02013880(s,4);out->sprite=s;out->data=data;}
