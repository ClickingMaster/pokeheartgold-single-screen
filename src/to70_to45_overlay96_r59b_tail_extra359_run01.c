#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x60];void *a;u8 pad2[0x28];void *b;u8 pad3[0xac];volatile int ticks;u8 pad4[2];u8 flag;u8 pad5[0x49];void *c;u8 pad6[0x10];u8 tail[4];} Work;
typedef struct {u8 pad[0xb0];u16 score;} ScoreView;
extern Work *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern void *PokeathlonCourse_GetDataCopyArea(void *);
extern u32 *ov96_021E8A20(void *);
extern void ov96_021EB63C(void *,int);extern void ov96_021EB144(void *,int);
extern void ov96_021E8318(void *,u16);extern void ov96_021F70AC(Work *,u16);extern void ov96_021F5B60(void *);
extern void ov96_021F6600(void *,Work *);extern void ov96_021F6424(void *);extern void ov96_021F6BB0(Work *);extern void ov96_021F7194(void *,Work *);extern void ov96_021F7598(void *);
static inline u8 stopped(Work *w){return w->ticks<=0;}
int ov96_021F5A88(void *);
int ov96_021F5A88(void *course){Work *w=PokeathlonCourse_GetHeapAllocPtr4(course);u32 *out=ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course));w->flag=0;if(w->ticks)w->ticks--;if(stopped(w)){int score=0,i;ScoreView *s;ov96_021EB63C(w->a,1);ov96_021EB144(w->c,1);i=0;s=(ScoreView *)w;for(;i<3;i++,s=(ScoreView *)((u8 *)s+0x38))score+=s->score;if(score>999)score=999;ov96_021E8318(course,(u16)score);ov96_021F70AC(w,(u16)score);ov96_021F5B60(course);return 1;}ov96_021F6600(course,w);ov96_021F6424(w->tail);ov96_021F6BB0(w);ov96_021F7194(course,w);ov96_021F7598(w->b);ov96_021F5B60(course);*out=w->flag;return 0;}
