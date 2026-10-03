#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int visible;void *a,*b;int active;} Work;extern void ov96_021EB52C(void *,int,int);extern void *ov96_021EB5B8(void *);extern void Sprite_SetAnimActiveFlag(void *,int);void ov96_021FBE54(Work *,int);
void ov96_021FBE54(Work *w,int flag){if(flag){w->visible=1;w->active=1;ov96_021EB52C(w->a,1,1);ov96_021EB52C(w->b,1,0);Sprite_SetAnimActiveFlag(ov96_021EB5B8(w->b),0);}else{w->visible=0;w->active=0;ov96_021EB52C(w->a,0,0);ov96_021EB52C(w->b,0,0);}}
