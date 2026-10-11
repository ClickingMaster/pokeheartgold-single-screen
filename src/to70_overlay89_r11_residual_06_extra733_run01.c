#pragma force_active on
#include "global.h"
typedef struct{int size;volatile u16 tick;volatile u8 state;u8 p;int data[4];} State;
typedef struct{u8 p[0x1c];u8 sprite[0x78];State state;} Work;
extern void ModelInstance_SetScale(void *,int,int,int),ov89_0225ADA4(void *,Work *,int *,int);
int ov89_0225B91C(void *,Work *);
int ov89_0225B91C(void *task,Work *w){State *s=&w->state;switch(s->state){case 0:s->size=4096;s->state++;case 1:s->tick++;if(s->tick>8)return 1;s->size+=768;ModelInstance_SetScale(w->sprite,s->size,s->size,4096);break;}ov89_0225ADA4(w->sprite,w,s->data,1);return 0;}
