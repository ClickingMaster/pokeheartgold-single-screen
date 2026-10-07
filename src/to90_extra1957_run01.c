#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int x,y,z;} Vec;
typedef union{u32 word;struct{u32 low:14,b14:1,b15:1,number:13,b29:1,high:2;}b;} Flags;
typedef union{u32 word;struct{u32 low:26,b26:1,b27:1,b28:1,b29:1,b30:1,b31:1;}b;} MotionFlags;
typedef struct{u8 pad[0x14];u32 state;u8 pad18[4];s16 x,y;u8 pad20[12];Vec pos;u8 pad38[0x28];MotionFlags flags;} Motion;
typedef struct{s16 x,y,tx,ty;Flags flags;Motion motion;u8 pad[0xa8-12-sizeof(Motion)];} Record;
extern int ov96_022158D4(int,int),ov96_0221862C(Motion*),ov96_021E6104(void),ov96_022158EC(Vec*,Vec*,int);
extern void ov96_0221910C(Motion*),ov96_022186B8(Motion*,Record*),ov96_02218B1C(Motion*,void*),ov96_02217E7C(void*),ov96_02217FD0(Record*,void*);
static inline float Round(s16 x){if(x>0)return 0.5f+(float)(x<<12);return (float)(x<<12)-0.5f;}
static inline float Radius(void){if(ov96_021E6104()>0)return 0.5f+(float)(ov96_021E6104()<<12);return (float)(ov96_021E6104()<<12)-0.5f;}
void ov96_022180CC(Record*,void*);
static inline int AbsDelta(int x){return x<0?-x:x;}
void ov96_022180CC(Record*base,void*env){int i;for(i=0;i<4;i++){Record*r=&base[i];Vec target={0,0,0},current={0,0,0};Flags*f=&r->flags;Motion*m=&r->motion;target.x=(int)Round(r->x);target.y=(int)Round(r->y);current.x=(int)Round(r->tx);current.y=(int)Round(r->ty);
if(f->b.b14){if(ov96_022158D4(r->x,r->y)){if(ov96_0221862C(m))ov96_0221910C(m);}else{int radius=(int)Radius();if(!m->flags.b.b26&&m->state!=3&&ov96_022158EC(&m->pos,&target,radius))f->b.b29=1;}}
else if(f->b.b15){if(f->b.b29==1){s16 x=m->pos.x>>12,y=m->pos.y>>12;if(m->state==3)f->b.b29=0;else if(AbsDelta(r->x-x)>1||AbsDelta(r->y-y)>1)ov96_022186B8(m,r);}}
else if(f->b.number){if(f->b.b29==1&&(m->state==1||m->state==4)&&!m->flags.b.b28&&!m->flags.b.b26){if(ov96_022158EC(&current,&target,0x4000)){if(f->b.number<=7)m->flags.word=(m->flags.word&~0x40000000u)|0x20000000u;}else if(!ov96_022158EC(&current,&target,0xc000)&&f->b.number<=7){m->flags.word=(m->flags.word|0x40000000u)&~0x20000000u;m->x=*(u16*)&r->x;m->y=*(u16*)&r->y;}}f->b.b29=0;}
ov96_02218B1C(&r->motion,env);}ov96_02217E7C(env);ov96_02217FD0(base,env);}
