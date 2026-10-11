#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W { u8 pad[0x150]; void *a[2]; void *b[2]; u16 delay[2]; u32 state; int selected; } W;
extern VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(void *);
extern void PokeathlonCourseSpritePoolSprite_SetPosition(void *, const VecFx32 *);
extern void ov96_02200454(W *, u8, int);
extern void ov96_02200BD8(W *, u8);
int ov96_022006BC(W *, int);
int ov96_022006BC(W *w, int offset) {
 VecFx32 v0,v2,v3;
 u32 *state=&w->state;
 switch(*state) {
 default: break;
case 0: {
 v0 = *PokeathlonCourseSpritePoolSprite_GetPosition(w->a[w->selected]);
 v0.y += 0x20000;
 PokeathlonCourseSpritePoolSprite_SetPosition(w->a[w->selected], &v0);
 PokeathlonCourseSpritePoolSprite_SetPosition(w->b[w->selected], &v0);
 if(v0.y >= 0x2e0000) {
 v0.y = 0x1d8000;
 PokeathlonCourseSpritePoolSprite_SetPosition(w->a[w->selected], &v0);
 PokeathlonCourseSpritePoolSprite_SetPosition(w->b[w->selected], &v0);
(*state)++;
} break;
}
case 1:
 ov96_02200454(w, (u8)w->selected, offset);
 if(w->delay[w->selected]==0)w->delay[w->selected]=2;else w->delay[w->selected]--;
 w->selected = (w->selected+1)%2;
 (*state)++; break;
case 2: {
 v2 = *PokeathlonCourseSpritePoolSprite_GetPosition(w->a[w->selected]);
 v2.y += 0x20000;
 PokeathlonCourseSpritePoolSprite_SetPosition(w->a[w->selected], &v2);
 PokeathlonCourseSpritePoolSprite_SetPosition(w->b[w->selected], &v2);
 if(v2.y >= 0x288000) {
 v2.y = 0x288000;
 PokeathlonCourseSpritePoolSprite_SetPosition(w->a[w->selected], &v2);
 PokeathlonCourseSpritePoolSprite_SetPosition(w->b[w->selected], &v2);
w->selected = (w->selected+1)%2;
(*state)++;
} break;
}
case 3: {
 v3 = *PokeathlonCourseSpritePoolSprite_GetPosition(w->a[w->selected]);
 v3.y += 0x20000;
 PokeathlonCourseSpritePoolSprite_SetPosition(w->a[w->selected], &v3);
 PokeathlonCourseSpritePoolSprite_SetPosition(w->b[w->selected], &v3);
 if(v3.y >= 0x230000) {
 v3.y = 0x230000;
 PokeathlonCourseSpritePoolSprite_SetPosition(w->a[w->selected], &v3);
 PokeathlonCourseSpritePoolSprite_SetPosition(w->b[w->selected], &v3);
w->selected = (w->selected+1)%2;
ov96_02200BD8(w, (u8)((offset+2)%3)); *state=0; return 1;
} break;
}
} return 0;
}
