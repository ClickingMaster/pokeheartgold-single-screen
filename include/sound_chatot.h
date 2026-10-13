#ifndef POKEHEARTGOLD_SOUND_CHATOT_H
#define POKEHEARTGOLD_SOUND_CHATOT_H

#include "global.h"

#include "sav_chatot.h"

BOOL ChatotSoundMain(void);
BOOL Chatot_CheckCry(SOUND_CHATOT *chatot);
BOOL Chatot_PlayCry(SOUND_CHATOT *chatot, u32 a1, s32 volume, s32 pan);
void Chatot_StopCry();
BOOL Chatot_StartRecording(void);
void Chatot_StopRecording();
void Chatot_SaveRecording(SOUND_CHATOT *chatot);
void Chatot_SetForceDefaultCry(u8 forceDefault);
void Chatot_PlayCryWithFallback(SOUND_CHATOT *chatot, u32 a1, u32 volume, s32 pan);
BOOL Chatot_PlayCryWithDelayedFallback(SOUND_CHATOT *chatot, u32 a1, u32 volume, s32 pan, u8 delay);
u32 Chatot_GetVoiceParam(SOUND_CHATOT *chatot);
BOOL Chatot_CryPatternUsesRecording(int pattern);

BOOL PlayCry(u16 species, u8 form);
void PlayCryDelayedEx(int, u16, int, int, int, u8, u8);

#endif // POKEHEARTGOLD_SOUND_CHATOT_H
