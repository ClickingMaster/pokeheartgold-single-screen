#ifndef POKEHEARTGOLD_FIELD_EMOTE_BUBBLE_H
#define POKEHEARTGOLD_FIELD_EMOTE_BUBBLE_H

#include "field/field_effect_manager.h"

// The emote bubbles over map objects.

typedef struct EmoteBubble EmoteBubble;

EmoteBubble *EmoteBubble_New(FieldEffectManager *manager);
void EmoteBubble_Free(EmoteBubble *bubble);

#endif // POKEHEARTGOLD_FIELD_EMOTE_BUBBLE_H
