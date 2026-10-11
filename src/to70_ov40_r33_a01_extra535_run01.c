#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x748];void *str;} View;
typedef struct {u8 p[8];int state;u8 q[0x49c-12];u8 data[0x860-0x49c];View *v;} Work;
extern void ov40_02238FF4(Work*),ov40_0223992C(View*),ov40_02239514(Work*),ov40_0222F720(void*),String_Delete(void*),ov40_0222F920(void*,Work*),ov40_02239418(Work*),ov40_02239574(Work*),ov40_02238F00(Work*),VsRecorder_SetSubState(Work*,int);
extern int VsRecorder_StepBlend(View*,void*,int,int);
int ov40_0223A280(Work*w);
int ov40_0223A280(Work*w){View*v=w->v;switch(w->state){case 0:ov40_02238FF4(w);ov40_0223992C(v);ov40_02239514(w);ov40_0222F720(w->data);if(v->str)String_Delete(v->str);ov40_0222F920(w->data,w);w->state++;break;case 1:if(VsRecorder_StepBlend(v,(u8*)v+4,1,1))w->state++;break;case 2:if(VsRecorder_StepBlend(v,(u8*)v+4,0,1)){ov40_02239418(w);ov40_02239574(w);ov40_02238F00(w);w->state++;}break;default:VsRecorder_SetSubState(w,6);break;}return 0;}
