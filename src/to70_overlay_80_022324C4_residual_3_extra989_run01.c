#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x370];u8 a[4],b[4],c[4],d[4];u16 values[6];u8 tail[0x690];u16 last;} Work;
typedef struct{u8 p[8];u8 a[4],b[4],c[4],d[4];u8 pad[16];u16 last;} Source;
extern u16 ov80_02232B44(Source *,u8);
void ov80_02232AEC(Work *,Source *);
void ov80_02232AEC(Work *w,Source *s){int i;for(i=0;i<6;i++)w->values[i]=ov80_02232B44(s,(u8)i);for(i=0;i<4;i++){w->a[i]=s->a[i];w->b[i]=s->b[i];w->c[i]=s->c[i];w->d[i]=s->d[i];}w->last=s->last;}
