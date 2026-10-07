#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int rows[7][128];} Work;
void ov96_021F6524(Work *,int,int,int);void ov96_021F6424(Work *);
void ov96_021F6424(Work *w){int i;ov96_021F6524(w,0,0,1);ov96_021F6524(w,1,1,2);ov96_021F6524(w,2,2,0);for(i=0;i<128;i++){w->rows[0][i]=w->rows[1][i];if(w->rows[0][i]>w->rows[2][i])w->rows[0][i]=w->rows[2][i];if(w->rows[0][i]>w->rows[3][i])w->rows[0][i]=w->rows[3][i];if(w->rows[0][i]>w->rows[4][i])w->rows[0][i]=w->rows[4][i];if(w->rows[0][i]>w->rows[5][i])w->rows[0][i]=w->rows[5][i];if(w->rows[0][i]>w->rows[6][i])w->rows[0][i]=w->rows[6][i];}}
