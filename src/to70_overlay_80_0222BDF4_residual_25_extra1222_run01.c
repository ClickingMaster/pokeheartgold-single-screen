#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "math_util.h"
#include "heap.h"
#include "sys_task.h"
#pragma force_active on
typedef struct {s16 start,end,dx,dy,x,y;u32 pad;} Row;
typedef struct {Row rows[96];void *handle;int mode;} Anim;
typedef struct {BgConfig **bg;u8 pad[12];int flag;u8 pad2[16];Anim *anim;} Work;
extern void Heap_Free(void*);extern void SysTask_Destroy(SysTask*);
extern void *ov80_0223B60C(void*);extern void ov80_0223B5E8(void*);
void ov80_0222D644(SysTask*,Work*);
void ov80_0222D644(SysTask *task,Work *work){int i1;u8 *base1;int a,b,c,d;u8 *base2;int i2;MtxFx22 matrix;Anim *anim=work->anim;a=GetBgHOffset(*work->bg,2);b=GetBgHOffset(*work->bg,2);c=GetBgHOffset(*work->bg,3);d=GetBgHOffset(*work->bg,3);
if(work->flag==1){Row *row;base1=ov80_0223B60C(anim->handle);i1=0;row=anim->rows;for(;i1<96;i1++,row++){int j;row->x=0;row->y=0;j=row->start;if(j<row->end){u8 *dest=base1+j*32;do{MTX22_2DAffine(&matrix,0,4096,4096,0);{s16 x=a+row->x,y=b+row->y;if(anim->mode==2){x%=256;y%=256;}else if(anim->mode==1){if(x<0)x+=256;if(y<0)y=-y;x%=256;}G2x_SetBGyAffine_((u32)dest,&matrix,0,0,x,y);}{s16 x=c+row->x,y=d+row->y;if(anim->mode==2){x%=256;y%=256;}else if(anim->mode==1){if(x<0)x+=256;if(y<0)y=-y;x%=256;}G2x_SetBGyAffine_((u32)dest+16,&matrix,0,0,x,y);}j++;dest+=32;}while(j<row->end);}}ov80_0223B5E8(anim->handle);Heap_Free(anim);SysTask_Destroy(task);}else{Row *row;base2=ov80_0223B60C(anim->handle);i2=0;row=anim->rows;for(;i2<96;i2++,row++){int j;row->x+=row->dx;row->y+=row->dy;j=row->start;if(j<row->end){u8 *dest=base2+j*32;do{MTX22_2DAffine(&matrix,0,4096,4096,0);{s16 x=a+row->x,y=b+row->y;if(anim->mode==2){x%=256;y%=256;}else if(anim->mode==1){if(x<0)x+=256;if(y<0)y=-y;x%=256;}G2x_SetBGyAffine_((u32)dest,&matrix,0,0,x,y);}{s16 x=c+row->x,y=d+row->y;if(anim->mode==2){x%=256;y%=256;}else if(anim->mode==1){if(x<0)x+=256;if(y<0)y=-y;x%=256;}G2x_SetBGyAffine_((u32)dest+16,&matrix,0,0,x,y);}j++;dest+=32;}while(j<row->end);}}}}
