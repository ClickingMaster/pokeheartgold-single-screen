#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *Heap_Alloc(int,u32);
typedef struct W120Inner {void *window,*text;} W120Inner;
typedef struct W120Parent {u8 a[8];void *bg;} W120Parent;
typedef struct W120 {u32 state,done,unused;W120Inner *inner;W120Parent *parent;u32 *flag;} W120;
extern void *AllocWindows(int,int),*ov120_0225F268(int);
extern void AddWindowParameterized(void *,void *,int,int,int,int,int,int,int),BG_LoadPlttData(int,const void *,int,int),FillWindowPixelBuffer(void *,int),ScheduleWindowCopyToVram(void *),ov01_021EFCF8(int,int,int,u32 *,int),ov120_0225F2B8(void *,int,int,void *,int,int),sub_0200FC20(int),ov120_0225F294(void *),ClearWindowTilemapAndCopyToVram(void *),RemoveWindow(void *),WindowArray_Delete(void *,int),ov01_021EFCDC(W120 *,void *);
extern int ov120_0225F318(void *);
void ov120_0225FA74(void *,W120 *,int);
void ov120_0225FA74(void *task,W120 *ctx,int direction){W120Inner *inner=ctx->inner;u16 color;int result,step;switch(ctx->state){case 0:{u8 *p=Heap_Alloc(4,8);ctx->inner=(W120Inner *)p;p[0]=0;p[1]=0;p[2]=0;p[3]=0;p[4]=0;p[5]=0;p[6]=0;p[7]=0;inner=ctx->inner;inner->window=AllocWindows(4,1);AddWindowParameterized(ctx->parent->bg,inner->window,3,0,0,32,32,0,0);color=0;BG_LoadPlttData(3,&color,2,30);FillWindowPixelBuffer(inner->window,0);ScheduleWindowCopyToVram(inner->window);inner->text=ov120_0225F268(4);ctx->state=1;break;}case 1:step=direction?16:-16;ov01_021EFCF8(1,step,step,&ctx->done,2);ctx->state=2;break;case 2:if(ctx->done)ctx->state=3;break;case 3:ov120_0225F2B8(inner->text,0,4,inner->window,15,direction);ctx->state=4;break;case 4:result=ov120_0225F318(inner->text);ScheduleWindowCopyToVram(inner->window);if(result)ctx->state=5;break;case 5:sub_0200FC20(0);if(ctx->flag)*ctx->flag=1;ov120_0225F294(inner->text);ClearWindowTilemapAndCopyToVram(inner->window);RemoveWindow(inner->window);WindowArray_Delete(inner->window,1);ov01_021EFCDC(ctx,task);break;}}
