#ifndef POKEHEARTGOLD_FIELD_EMOTE_MARK_H
#define POKEHEARTGOLD_FIELD_EMOTE_MARK_H

#include "player_avatar.h"

int EmoteMark_Spawn(LocalMapObject *obj, int kind, int playSound);
BOOL EmoteMark_IsDone(int effect);
void EmoteMark_Hide(int effect);

#endif // POKEHEARTGOLD_FIELD_EMOTE_MARK_H
