#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int state,angle,tick,baseline;u8 descriptor[0x20];} MotionState;
typedef struct {u8 pad[0x18];void *sprite;u8 motion[0x78];MotionState state;u8 padc4[0x17c];union {u32 flags;struct {u32 low:8,parameter:8,high:16;} bits;} control;} View;
u16 ModelInstance_GetRotation(void*,int);
void ModelInstance_SetRotation(void*,u16,int);
void ModelInstance_GetPosition(void*,fx32*,fx32*,fx32*);
void ModelInstance_SetPosition(void*,fx32,fx32,fx32);
int GF_SinDegFX32(int);
void ov89_0225AB64(void*,int);
void ov89_0225A958(void*,void*);
void ov89_0225ADA4(void*,View*,void*,int);
int ov89_0225B46C(void *parent,View *v);
int ov89_0225B46C(void *parent,View *v){
    MotionState *s=&v->state;fx32 x,y,z;
    switch(s->state){
    case 0:{
        u16 angle=ModelInstance_GetRotation(v->motion,2)+0xaaa;
        if(angle>=0x2000){
            ov89_0225AB64(v->sprite,v->control.bits.parameter);
            angle-=0x4000;ov89_0225A958(parent,v->sprite);
            v->control.flags=(v->control.flags&~0xf0)|0x10;s->state++;
        }
        ModelInstance_SetRotation(v->motion,angle,2);break;
    }
    case 1:{
        u16 angle;
        v->control.flags&=~0xf0;angle=ModelInstance_GetRotation(v->motion,2)+0xaaa;
        if(angle<0x4000){s->state++;angle=0;}
        ModelInstance_SetRotation(v->motion,angle,2);break;
    }
    case 2:
        s->tick++;
        if(s->tick<4)break;
        ModelInstance_GetPosition(v->motion,&x,&y,&z);s->baseline=y;s->tick=0;s->state++;
    case 3:{
        fx32 sine;
        ModelInstance_GetPosition(v->motion,&x,&y,&z);
        if(x<-0x50000 || x>0x50000 || y>0x30000 || y<-0x30000)return 1;
        s->angle+=0x8000;if(s->angle>=0x168000)s->angle-=0x168000;
        sine=GF_SinDegFX32(s->angle);
        ModelInstance_SetPosition(v->motion,x-0x1000,s->baseline+(fx32)(((s64)sine*0x8000+0x800)>>12),z);
        break;
    }
    }
    ov89_0225ADA4(v->motion,v,s->descriptor,0);return 0;
}
