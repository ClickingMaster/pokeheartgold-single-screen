#pragma force_active on
#include "ov01_21f1afc_opus_partial_internal.h"
// @fn Field_PlayerAvatar_ApplyTransitionFlags
void Field_PlayerAvatar_ApplyTransitionFlags(PlayerAvatar *avatar){u32 i = 0; u32 flags = PlayerAvatar_GetTransitionFlags(avatar); for (; i < 15; i++) { if (flags & 1) { sPlayerAvatarBitUpdateFuncs[i](avatar); } flags >>= 1; } PlayerAvatar_SetTransitionFlags(avatar, 0);}
