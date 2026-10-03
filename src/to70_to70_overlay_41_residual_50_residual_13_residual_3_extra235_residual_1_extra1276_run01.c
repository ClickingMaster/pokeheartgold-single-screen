#pragma force_active on
#include "global.h"
typedef struct{void*list,*header;int x,y,z,priority,vram,heap;} Template;
typedef struct{u8 p[16];void*sprites[2];void*window;int value,scaled;u8 p2[8];int*output;u8 p3[0x60];int zero;} Work;
void ov41_0224AFF8(Work*,void*,int,int,int,int,int,int,int,int);
void ov41_0224B0B8(Work*,void*,void*,int);
void*Sprite_Create(Template*);
void ov41_0224B298(Work*);
void ov41_0224AD0C(void*,void*,int,int,int,int,int,int);
void FillWindowPixelBuffer(void*,int);
void ov41_0224AE24(void*,int,int,int,int,int,int,int);
void CopyWindowToVram(void*);
void ov41_0224B118(Work*,void*,void*,int,void*,int*,int);
void ov41_0224B118(Work*w,void*list,void*resource,int value,void*bg,int*output,int last){u8 header[36];Template t;int i,x;
ov41_0224AFF8(w,resource,14,last,229,230,228,227,2,3000);ov41_0224B0B8(w,resource,header,0);t.list=list;t.header=header;t.vram=2;t.heap=14;t.y=58<<12;t.priority=0;t.y+=(2<<20);
for(i=0,x=103;i<2;x+=24,i++){t.x=x;t.x<<=12;w->sprites[i]=Sprite_Create(&t);}
w->value=value;w->scaled=value*30;w->output=output;w->output[0]=value;w->output[2]=value;w->zero=0;ov41_0224B298(w);ov41_0224AD0C(&w->window,bg,10,8,14,4,193,0);FillWindowPixelBuffer(w->window,0);ov41_0224AE24(w->window,27,215,4,0,4,0x10200,255);ov41_0224AE24(w->window,27,215,5,72,4,0x10200,255);CopyWindowToVram(w->window);}
