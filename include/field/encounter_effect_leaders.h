#ifndef POKEHEARTGOLD_FIELD_ENCOUNTER_EFFECT_LEADERS_H
#define POKEHEARTGOLD_FIELD_ENCOUNTER_EFFECT_LEADERS_H

#include "field/encounter_effect.h"

#include "pm_string.h"
#include "save.h"

#define SCALE_SPRITE_COUNT 4

// Sprites that pop in one after another, shrinking from double size.
typedef struct ScaleSprites {
    s16 timer;
    s16 count;
    Sprite *sprites[SCALE_SPRITE_COUNT];
    EncounterEffectInterp scale[SCALE_SPRITE_COUNT];
} ScaleSprites;

typedef struct GymLeaderIntroParams {
    s32 endX;
    u32 trainerId;
    u16 trainerClass;
    u16 unk_0A;
    u8 spriteMembers[4]; // character, palette, cell, animation
    u8 bgMembers[3];
} GymLeaderIntroParams;

extern const GymLeaderIntroParams gEncounterRivalIntroParams;

void ScaleSprites_Init(ScaleSprites *scaleSprites, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, fx32 x, fx32 y, enum HeapID heapID);
void ScaleSprites_Delete(ScaleSprites *scaleSprites);
BOOL ScaleSprites_Update(ScaleSprites *scaleSprites);
String *LeadersEffect_GetTrainerNameString(u32 trainerId, enum HeapID heapID);
String *LeadersEffect_GetRivalNameString(SaveData *saveData, enum HeapID heapID);

#endif // POKEHEARTGOLD_FIELD_ENCOUNTER_EFFECT_LEADERS_H
