#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int visible;void *a,*b;int active;} Work;extern void PokeathlonCourseSpritePoolSprite_Show(void *,int,int);extern void *PokeathlonCourseSpritePoolSprite_GetSprite(void *);extern void Sprite_SetAnimActiveFlag(void *,int);void ov96_021FBE54(Work *,int);
void ov96_021FBE54(Work *w,int flag){if(flag){w->visible=1;w->active=1;PokeathlonCourseSpritePoolSprite_Show(w->a,1,1);PokeathlonCourseSpritePoolSprite_Show(w->b,1,0);Sprite_SetAnimActiveFlag(PokeathlonCourseSpritePoolSprite_GetSprite(w->b),0);}else{w->visible=0;w->active=0;PokeathlonCourseSpritePoolSprite_Show(w->a,0,0);PokeathlonCourseSpritePoolSprite_Show(w->b,0,0);}}
