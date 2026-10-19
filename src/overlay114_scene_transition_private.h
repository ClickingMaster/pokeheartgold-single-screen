#include "global.h"

#include "field/encounter_effect.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade_legacy.h"
#include "sprite.h"

typedef struct {
    EncounterEffectInterp fade;
    EncounterEffectQuadInterp scale;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprite;
} FrontierOrLinkSingleEffectWork;

typedef struct {
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprites[4];
    EncounterEffectQuadInterp horiz;
    EncounterEffectQuadInterp vert;
} DoubleEffectWork;

void EncounterEffect_FrontierOrLinkSingle(SysTask *task, EncounterEffectTask *ctx);
void EncounterEffect_Double(SysTask *task, EncounterEffectTask *ctx);
