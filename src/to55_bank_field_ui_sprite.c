#include "to55_bank_field_ui_sprite_private.h"

#define PTR(s, off, type) (*(type **)&(s)->raw[(off)])
#define U8(s, off)        (*(u8 *)&(s)->raw[(off)])
#define U16(s, off)       (*(u16 *)&(s)->raw[(off)])
#define S32(s, off)       (*(s32 *)&(s)->raw[(off)])

void sub_020869BC(ModuleState *state) {
    SpriteTemplate template;
    SpriteSystem *system = PTR(state, 0x2f0, SpriteSystem);
    SpriteManager *manager = PTR(state, 0x2f4, SpriteManager);

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.priority = 10;
    int priority = (template.vram = 1) - 2;
    template.palette = 0;
    template.unk2c = 0;
    template.unk30 = 0;
    template.palette = 0;
    template.resources[0] = 0x3ea;
    template.resources[1] = 0x3ea;
    template.resources[2] = 0x3ea;
    template.resources[3] = 0x3ea;
    template.backgroundPriority = priority;
    template.transferPriority = priority;
    PTR(state, 0x274, ManagedSprite) = SpriteSystem_NewSprite(system, manager, &template);
    template.palette = 1;
    PTR(state, 0x290, ManagedSprite) = SpriteSystem_NewSprite(system, manager, &template);
    ManagedSprite_SetPositionXY(PTR(state, 0x274, ManagedSprite), (U8(state, 0x36e) + U8(state, 0x36f)) / 2, (U8(state, 0x36c) + U8(state, 0x36d)) / 2);
    ManagedSprite_SetAnim(PTR(state, 0x274, ManagedSprite), 0);
    ManagedSprite_TickFrame(PTR(state, 0x274, ManagedSprite));
    ManagedSprite_SetPositionXY(PTR(state, 0x290, ManagedSprite), (U8(state, 0x372) + U8(state, 0x373)) / 2, (U8(state, 0x370) + U8(state, 0x371)) / 2);
    ManagedSprite_SetAnim(PTR(state, 0x290, ManagedSprite), 0);
    ManagedSprite_TickFrame(PTR(state, 0x290, ManagedSprite));
}
