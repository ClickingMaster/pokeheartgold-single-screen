#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct {u8 p[8];SpriteSystem *sys;SpriteManager *man;} Sub;typedef struct {u8 p[0x14];Sub *sub;u8 q[0xd8];ManagedSprite *sprite;} W;extern void ov92_0225DDD8(ManagedSpriteTemplate*,int,int,int,int,int);
void ov92_0225E93C(W*);
void ov92_0225E93C(W*w) {ManagedSpriteTemplate t;SpriteSystem*sys=w->sub->sys;SpriteManager*man=w->sub->man;ov92_0225DDD8(&t,128,128,1,7,9007);t.bgPriority=0;t.drawPriority=0;w->sprite=SpriteSystem_NewSprite(sys,man,&t);ManagedSprite_SetAnim(w->sprite,1);ManagedSprite_SetDrawFlag(w->sprite,1);ManagedSprite_TickFrame(w->sprite);ManagedSprite_SetPaletteOverrideOffset(w->sprite,7);}
