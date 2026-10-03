#pragma force_active on
#include "global.h"
typedef struct Object{char padding[0x1e4];int x,y;} Object;
typedef struct Motion{int value;char padding[20];} Motion;
typedef struct Data{int state,mode,timer;int*status;Object*object;char padding[20];int delta;char padding2[24];Motion a,b;int flag;void*owner;} Data;
void SysTask_Destroy(void*);
void Heap_Free(void*);
int IsPaletteFadeFinished(void);
void PlaySE(int);
void ov92_02260860(Motion*,int,int,int);
int ov92_02260870(Motion*);
void ov92_022630E8(void*);
void ov92_02260428(void*,int,int,int,int,float,int);
void ov92_0225EF4C(void*,Data*);
void ov92_0225EF4C(void*task,Data*d){
 int delta=(s16)(d->delta>>12),a,b;
 if(!IsPaletteFadeFinished()||*((u8*)d->owner+0x34)==1){SysTask_Destroy(task);Heap_Free(d);return;}
 switch(d->state){
 case 0:
 if(d->mode==0){
 ov92_02260860(&d->a,d->object->x,d->object->x+0xc000,4);
 if(delta>0)ov92_02260860(&d->b,d->object->y,d->object->y+0x10000,4);
 else ov92_02260860(&d->b,d->object->y,d->object->y-0x10000,4);
 }else{
 ov92_02260860(&d->a,d->object->x,d->object->x+0x30000,8);
 if(delta>0)ov92_02260860(&d->b,d->object->y,d->object->y-0x10000,8);
 else ov92_02260860(&d->b,d->object->y,d->object->y+0x10000,4);
 }
 PlaySE(0x60a);d->timer=0;d->state++;break;
 case 1:
 a=ov92_02260870(&d->a);b=ov92_02260870(&d->b);
 d->object->x=d->a.value;d->object->y=d->b.value;
 if(!a||!b)break;
 if(d->mode==0){
 *d->status=1;ov92_022630E8((char*)d->object+0x190);ov92_022630E8((char*)d->object+0x1a0);
 ov92_02260428(d->object,0,0,5,5,0.8f,0);ov92_02260428(d->object,0,0,-5,-5,0.8f,0);
 ov92_02260860(&d->a,d->object->x,d->object->x-0x30000,8);
 if(delta>0)ov92_02260860(&d->b,d->object->y,d->object->y+0x10000,8);
 else ov92_02260860(&d->b,d->object->y,d->object->y-0x10000,8);
 }else{
 ov92_02260860(&d->a,d->object->x,d->object->x-0xc000,4);
 if(delta>0)ov92_02260860(&d->b,d->object->y,d->object->y-0x10000,4);
 else ov92_02260860(&d->b,d->object->y,d->object->y+0x10000,4);
 }
 d->state++;break;
 case 2:
 a=ov92_02260870(&d->a);b=ov92_02260870(&d->b);
 d->object->x=d->a.value;d->object->y=d->b.value;
 if(!a||!b)break;
 if(d->mode==0)PlaySE(0x630);d->state++;break;
 case 3:
 if(d->mode==0){if(d->timer==8)PlaySE(0x58d);d->timer++;if(d->timer<40)break;*d->status=2;d->state++;}
 else d->state++;
 break;
 default:
 if(d->mode==0){d->timer++;if(d->timer<45)break;*d->status=0;PlaySE(0x60a);d->state=0;d->mode++;}
 else{d->timer++;if(d->timer<10)break;if(d->flag)d->object->x=-0x8000;else d->object->x=-0x14000;d->object->y=0;SysTask_Destroy(task);Heap_Free(d);}
 break;
 }
}
