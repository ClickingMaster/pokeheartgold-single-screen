#pragma force_active on
#include "global.h"
#pragma force_active on
void PlaySE(u16);
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define SH(p,n) (*(s16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define SB(p,n) (*(s8*)((u8*)(p)+(n)))
int ov72_0223A7F4(void*,int);u8 ov72_0223A8DC(void*);void ov72_02238730(void*,int,void*);void ov72_022386F4(void*,u8);void ov72_0223AED0(void*,int,int,int);void Sprite_SetAnimCtrlSeq(void*,int);extern u8 ov72_0223B478[],ov72_0223B479[];
typedef struct {u8 pad[0xe00];void *effect,*render,*erase,*yes,*no;u8 gap[0x130c-0xe14];u8 editing,type;s8 count;u8 current;s16 cursor;u8 phase,reserved;s8 history[0x54];void *renderData;} Work;
void ov72_0223A8F0(Work*,int);
void ov72_0223A8F0(Work*s,int n){switch(n){case 0:case 1:case 2:break;case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:if(ov72_0223A7F4(&s->editing,n-3)!=-1){s->history[s->cursor]=n-3;if(s->cursor<2)s->cursor++;ov72_02238730(s->render,s->cursor,s->renderData);s->current=n-3;s->current=ov72_0223A8DC(&s->editing);ov72_022386F4(s->effect,s->current);PlaySE(1500);ov72_0223AED0(s,ov72_0223B478[(n-3)*2],ov72_0223B479[(n-3)*2],3);}else PlaySE(1522);break;case 13:s->history[s->cursor]=-1;s->cursor--;if(s->type==100){if(s->cursor<0)s->cursor=0;}else if(s->cursor<1)s->cursor=1;PlaySE(1500);ov72_02238730(s->render,s->cursor,s->renderData);Sprite_SetAnimCtrlSeq(s->erase,5);break;case 14:s->current=n-3;ov72_022386F4(s->effect,s->current);s->count=ov72_0223A7F4(&s->editing,-1);if(s->count<=0)s->count=1;s->phase=7;PlaySE(1500);Sprite_SetAnimCtrlSeq(s->yes,5);break;case 15:s->current=n-3;s->count=-1;ov72_022386F4(s->effect,s->current);s->phase=7;PlaySE(1500);Sprite_SetAnimCtrlSeq(s->no,5);break;}}
