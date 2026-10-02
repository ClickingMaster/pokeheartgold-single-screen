#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[16];void *sprite;int p2;u8 state,done;} State;
extern int ov95_021E7450(State *),ov95_021E7514(State *);extern void ToggleBgLayer(int,int),ManagedSprite_SetDrawFlag(void *,int),GfGfx_EngineBTogglePlanes(int,int);
int ov95_021E7258(State *);
int ov95_021E7258(State *s){switch(s->state){case 0:if(ov95_021E7450(s))s->state++;break;case 1:if(ov95_021E7514(s)){GF_ASSERT(s->done);ToggleBgLayer(5,0);ToggleBgLayer(6,0);ManagedSprite_SetDrawFlag(s->sprite,0);GfGfx_EngineBTogglePlanes(16,0);return s->done;}break;default:GF_ASSERT(0);break;}return 0;}
