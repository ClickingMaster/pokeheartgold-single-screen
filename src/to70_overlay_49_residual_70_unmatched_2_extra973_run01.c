#pragma force_active on
#include "global.h"
typedef struct{u8 p[2];u8 window;} RowEntry;
typedef struct{u8 p[0x106dc];u8 windows[256][20];} MainWork;
typedef struct{u8 p[0x87c];u8 *rows[18];u8 gap[0];int scroll[18][2];} View;
extern int sub_020181A4(void *);
int ov49_02265B94(MainWork *,View *,int,int,int);
int ov49_02265B94(MainWork *w,View *v,int page,int slot,int step){int max=sub_020181A4(w->windows[((RowEntry*)(v->rows[page]+slot))->window]);int next=v->scroll[page][slot]+step;if(next<max){v->scroll[page][slot]=next;return 0;}v->scroll[page][slot]=max-2048;return 1;}
