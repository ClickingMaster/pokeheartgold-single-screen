#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{s16 x,y;}Touch;
typedef struct{u8 pad[4];void*object;u8 p2[4];int enabled;u8 p3[12];int x,y;u8 rest[0x48-0x24];}Entry;
typedef struct{u8 p[8];struct{Entry e[3];u8 p[12];}rows[4];}Work;
typedef struct{s16 x,y;u8 row,col;}Out;
extern void ov96_021EB06C(void*,int,int,int*,int*);
extern int ov96_021EAF8C(void*);

int ov96_0220FBEC(Touch*t,Work*w,Out*out);

int ov96_0220FBEC(Touch*t,Work*w,Out*out){s16 y;int i;if(t->y<288)return 0;y=t->y-288;for(i=0;i<12;i++){VecFx32 v={0,0,0};int x1,y1;int col=i%3,row=i/3;Entry *p=(Entry*)((u32)w+row*228+8+col*72);if(p->enabled==1){ov96_021EB06C(p->object,p->x>>12,p->y>>12,&x1,&y1);v.x=x1-t->x;v.y=y1-y;v.x<<=12;v.y<<=12;if(VEC_Mag(&v)<=(ov96_021EAF8C(p->object)+14)<<12){out->row=row;out->col=col;out->x=x1;out->y=y1;return 1;}}}return 0;}
