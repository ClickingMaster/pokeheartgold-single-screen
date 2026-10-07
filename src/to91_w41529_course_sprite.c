#include "global.h"
#include "sprite_system.h"
#pragma force_active on
extern const ManagedSpriteTemplate ov96_0221D910;
void *ov96_022199A8(void *,void *,u16,u16,u16,u16);
void *ov96_022199A8(void *system,void *manager,u16 x,u16 y,u16 animation,u16 priority) {
 ManagedSpriteTemplate template=ov96_0221D910; ManagedSprite *sprite;
 GF_ASSERT(system); GF_ASSERT(manager);
 template.x=x; template.y=y; template.animation=animation; template.drawPriority=priority;
 sprite=SpriteSystem_NewSpriteWithYOffset(system,manager,&template,30<<16);
 ManagedSprite_SetAnimateFlag(sprite,TRUE);
 return sprite;
}
