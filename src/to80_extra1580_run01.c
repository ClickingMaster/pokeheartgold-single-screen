#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void*a,*b,*c;int pad[12];int priority;}S;extern void*ov96_021E8BAC(void*);extern void Sprite_SetDrawPriority(void*,int);
void ov96_021EABA8(S*s,int v);
void ov96_021EABA8(S*s,int v){Sprite_SetDrawPriority(ov96_021E8BAC(s->a),v);if(s->b)Sprite_SetDrawPriority(s->b,v+2*s->priority);if(s->c)Sprite_SetDrawPriority(s->c,v+s->priority);}
