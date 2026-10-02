#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work {void*owner,*other;int state;u8 padc[0x28];void*text;u8 pad38[12];void*busy;u8 pad48[0x40];void*prompt;u8 pad8c[8];int phase;} Work;
extern int ov75_02249534(void*),YesNoPrompt_HandleInput(void*);extern void ov75_02247878(Work*),ov75_022494CC(Work*,void*,int,int,int),YesNoPrompt_Destroy(void*),ov75_02247854(Work*,int,int);extern void*ov75_02247890(void*,int,int);
int ov75_02248B8C(Work*);
int ov75_02248B8C(Work*w){if(ov75_02249534(w->busy)==1)return 0;switch(w->phase){case 0:ov75_02247878(w);ov75_022494CC(w,w->text,13,1,0xf0f);w->phase++;break;case 1:w->prompt=ov75_02247890(w->other,564,0);w->phase++;break;case 2:{int input=YesNoPrompt_HandleInput(w->prompt);if(input==1){YesNoPrompt_Destroy(w->prompt);w->phase++;}else if(input==2){YesNoPrompt_Destroy(w->prompt);w->state=21;}break;}case 3:ov75_022494CC(w,w->text,17,1,0xf0f);ov75_02247854(w,34,20);break;}return 0;}
