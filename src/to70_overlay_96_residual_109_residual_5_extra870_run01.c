#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x738];int score;u8 gap[0x814-0x73c];u8 values[4],flags[4];} Work;

void ov96_02214418(Work*w);

void ov96_02214418(Work*w){u8 max=0,i;if(w->score<=300){for(i=0;i<4;i++)w->flags[i]=0;return;}for(i=0;i<4;i++)if(max<w->values[i])max=w->values[i];for(i=0;i<4;i++){if(max!=0 && max==w->values[i])w->flags[i]=1;else w->flags[i]=0;}}
