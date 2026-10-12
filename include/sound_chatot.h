#ifndef POKEHEARTGOLD_SOUND_CHATOT_H
#define POKEHEARTGOLD_SOUND_CHATOT_H

#include "global.h"

#include "sav_chatot.h"

BOOL ChatotSoundMain(void);
BOOL Chatot_CheckCry(SOUND_CHATOT *a0);
BOOL Chatot_PlayCry(SOUND_CHATOT *a0, u32 a1, s32 a2, s32 a3);
void Chatot_StopCry();
BOOL Chatot_StartRecording(void);
void Chatot_StopRecording();
void Chatot_SaveRecording(SOUND_CHATOT *a0);
void Chatot_SetForceDefaultCry(u8 a0);
void Chatot_PlayCryWithFallback(SOUND_CHATOT *a0, u32 a1, u32 a2, s32 a3);
BOOL Chatot_PlayCryWithDelayedFallback(SOUND_CHATOT *a0, u32 a1, u32 a2, s32 a3, u8 a4);
u32 Chatot_GetVoiceParam(SOUND_CHATOT *chatot);
BOOL Chatot_CryPatternUsesRecording(int a0);

BOOL PlayCry(u16 species, u8 form);
void PlayCryDelayedEx(int, u16, int, int, int, u8, u8);

#endif // POKEHEARTGOLD_SOUND_CHATOT_H
