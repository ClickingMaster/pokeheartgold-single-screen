#ifndef POKEHEARTGOLD_FIELD_FIELD_SHAYMIN_FORM_CHANGE_H
#define POKEHEARTGOLD_FIELD_FIELD_SHAYMIN_FORM_CHANGE_H

#include "field/field_effect_manager.h"

FieldEffect *ShayminFormChange_Spawn(FieldEffectManager *manager, VecFx32 *position);
BOOL ShayminFormChange_IsDone(FieldEffect *effect);

#endif // POKEHEARTGOLD_FIELD_FIELD_SHAYMIN_FORM_CHANGE_H
