#ifndef POKEHEARTGOLD_OVERLAY_01_021F1AFC_H
#define POKEHEARTGOLD_OVERLAY_01_021F1AFC_H

#include "player_avatar.h"

// Player avatar state transitions (PLAYER_TRANSITION_* bits) and the field move tasks: Strength boulders, Surf,
// Rock Climb, Waterfall and Whirlpool (src/field/player_avatar_actions.c). CallFieldTask_* are in overlay_01.h.
void Field_PlayerAvatar_OrrTransitionFlags(PlayerAvatar *playerAvatar, int flags);
void Field_PlayerAvatar_ApplyTransitionFlags(PlayerAvatar *playerAvatar);
BOOL Field_PlayerAvatar_TryFieldMoveStep(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int playerEvents);
void Field_PlayerAvatar_TryEnterPoketchPose(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TryLeavePoketchPose(PlayerAvatar *playerAvatar);

#endif // POKEHEARTGOLD_OVERLAY_01_021F1AFC_H
