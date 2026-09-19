#include "field_ui_863f4_internal.h"

#define PTR(s, off, type) (*(type **)&(s)->raw[(off)])
#define U8(s, off)        (*(u8 *)&(s)->raw[(off)])
#define U16(s, off)       (*(u16 *)&(s)->raw[(off)])
#define S32(s, off)       (*(s32 *)&(s)->raw[(off)])

void sub_020868A0(ModuleState *state) {
    SpriteTemplate template;
    SpriteSystem *system = PTR(state, 0x2f0, SpriteSystem);
    SpriteManager *manager = PTR(state, 0x2f4, SpriteManager);

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.priority = 0;
    template.unk2c = 0;
    template.unk30 = 0;
    template.palette = 0;
    template.resources[0] = 0x3e9;
    template.resources[1] = 0x3e9;
    template.resources[2] = 0x3e9;
    template.resources[3] = 0x3e9;
    template.vram = 1;
    template.backgroundPriority = -1;
    template.transferPriority = -1;
    PTR(state, 0x220, ManagedSprite) = SpriteSystem_NewSprite(system, manager, &template);
    PTR(state, 0x23c, ManagedSprite) = SpriteSystem_NewSprite(system, manager, &template);
    PTR(state, 0x258, ManagedSprite) = SpriteSystem_NewSprite(system, manager, &template);
    sub_02086AE4(state, S32(state, 0x3f0));
    ManagedSprite_SetAnim(PTR(state, 0x220, ManagedSprite), 0);
    ManagedSprite_TickFrame(PTR(state, 0x220, ManagedSprite));
    U16(state, 0x244) = 0;
    U16(state, 0x246) = 0;
    S32(state, 0x230) = 1;
    sub_02086B2C(state, 0);
    ManagedSprite_SetAnim(PTR(state, 0x23c, ManagedSprite), S32(state, 0x230));
    ManagedSprite_TickFrame(PTR(state, 0x23c, ManagedSprite));
    ManagedSprite_SetOamMode(PTR(state, 0x23c, ManagedSprite), 1);
    U16(state, 0x260) = 0;
    U16(state, 0x262) = 0;
    S32(state, 0x24c) = 1;
    sub_02086B2C(state, 0);
    ManagedSprite_SetAnim(PTR(state, 0x258, ManagedSprite), S32(state, 0x24c));
    ManagedSprite_TickFrame(PTR(state, 0x258, ManagedSprite));
    ManagedSprite_SetOamMode(PTR(state, 0x258, ManagedSprite), 1);
    sub_02086AB4(state, 1, 0);
    sub_02086AB4(state, 2, 0);
}

#if 0
void sub_020869BC(ModuleState *state) {
    SpriteTemplate template;
    SpriteSystem *system = PTR(state, 0x2f0, SpriteSystem);
    SpriteManager *manager = PTR(state, 0x2f4, SpriteManager);

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.priority = 10;
    template.vram = 1;
    template.palette = 0;
    template.unk2c = 0;
    template.unk30 = 0;
    template.palette = 0;
    template.resources[0] = 0x3ea;
    template.resources[1] = 0x3ea;
    template.resources[2] = 0x3ea;
    template.resources[3] = 0x3ea;
    template.backgroundPriority = template.vram - 2;
    template.transferPriority = template.backgroundPriority;
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
#endif
