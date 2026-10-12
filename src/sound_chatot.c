#include "sound_chatot.h"

#include "constants/species.h"

#include "math_util.h"
#include "sound.h"
#include "sound_system.h"
#include "sound_playback.h"

BOOL ChatotSoundMain(void) {
    u8 *r4 = GF_SdatGetAttrPtr(16);
    u8 *r0 = GF_SdatGetAttrPtr(30);

    if (*r0 == 1) {
        if (*r4 == 1) {
            if (Sound_IsWaveOutPlaying(14) == 0) {
                Chatot_StopCry();
                return TRUE;
            }
        } else {
            Chatot_StopCry();
            return TRUE;
        }
    }

    return FALSE;
}

BOOL Chatot_CheckCry(SOUND_CHATOT *a0) {
    u8 *r5 = GF_SdatGetAttrPtr(31);
    u8 *r4 = GF_SdatGetAttrPtr(54);

    if (!Chatot_Exists(a0)) {
        return FALSE;
    }

    if (*r4 == 1) {
        return FALSE;
    }

    if (*r5 != 1) {
        return TRUE;
    }

    return FALSE;
}

BOOL Chatot_PlayCry(SOUND_CHATOT *a0, u32 a1, s32 a2, s32 a3) {
    s8 *sp0 = Sound_GetChatotBuffer();
    s8 *sp4 = GF_SdatGetAttrPtr(30);

    if (!Chatot_CheckCry(a0)) {
        return FALSE;
    }

    StopCry(0);
    Chatot_StopCry();
    Sound_AllocWaveOutChannel(14);
    u16 r4 = (LCRandom() % 8192);

    Chatot_Decode(sp0, Chatot_GetData(a0));

    SoundWaveOutParam sp8;

    sp8.unk00 = Sound_GetWaveOutHandle(14);
    sp8.unk04 = NNS_SND_WAVE_FORMAT_PCM8;
    sp8.unk08 = Sound_GetChatotBuffer();
    sp8.unk0c = 0;
    sp8.unk10 = 0;
    sp8.unk14 = 2000;
    sp8.unk18 = 2000;
    sp8.unk20 = (u32)(r4 + 0x8000);
    sp8.unk24 = a3 / 2 + 64;
    sp8.unk1c = a2;

    BOOL ret = Sound_StartWaveOut(&sp8, 14);
    Sound_SetWaveOutVolume(14, a2);
    *sp4 = 1;
    Chatot_SetForceDefaultCry(0);

    return ret;
}

void Chatot_StopCry() {
    u8 *r5 = GF_SdatGetAttrPtr(16);
    u8 *r4 = GF_SdatGetAttrPtr(30);

    if (*r5 == 1) {
        Sound_StopWaveOutAndFreeBuffer(14);
        Sound_FreeWaveOutChannel(14);
    }

    *r4 = 0;
}

BOOL Chatot_StartRecording(void) {
    MICAutoParam st0;

    st0.type = MIC_SAMPLING_TYPE_SIGNED_8BIT;
    st0.buffer = Sound_GetChatotBuffer();
    st0.size = 2000;
    if ((st0.size & 0x1f) != 0) {
        st0.size &= ~0x1f;
    }
    st0.rate = HW_CPU_CLOCK_ARM7 / 2000;
    st0.loop_enable = 0;
    st0.full_callback = 0;
    st0.full_arg = 0;

    return GF_MIC_StartAutoSampling(&st0);
}

void Chatot_StopRecording() {
    GF_MIC_StopAutoSampling();
}

void Chatot_SaveRecording(SOUND_CHATOT *a0) {
    Chatot_Encode(a0, Sound_GetChatotBuffer());
}

void Chatot_SetForceDefaultCry(u8 a0) {
    u8 *r0 = GF_SdatGetAttrPtr(31);
    *r0 = a0;
}

void Chatot_PlayCryWithFallback(SOUND_CHATOT *a0, u32 a1, u32 a2, s32 a3) {
    SOUND_CHATOT **r0 = GF_SdatGetAttrPtr(36);
    BOOL ret;
    if (a0 == 0) {
        ret = Chatot_PlayCry(*r0, a1, a2, a3);
    } else {
        ret = Chatot_PlayCry(a0, a1, a2, a3);
    }

    if (ret == 0) {
        Chatot_SetForceDefaultCry(1);
        PlayCryEx(0, SPECIES_CHATOT, a3, a2, 11, 0);
    }
}

BOOL Chatot_PlayCryWithDelayedFallback(SOUND_CHATOT *a0, u32 a1, u32 a2, s32 a3, u8 a4) {
    SOUND_CHATOT **r0 = GF_SdatGetAttrPtr(36);
    BOOL ret;
    if (a0 == 0) {
        ret = Chatot_PlayCry(*r0, a1, a2, a3);
    } else {
        ret = Chatot_PlayCry(a0, a1, a2, a3);
    }

    if (ret == 0) {
        Chatot_SetForceDefaultCry(1);
        PlayCryDelayedEx(0, SPECIES_CHATOT, a3, a2, 11, a4, 0);
        ret = 1;
    }

    return ret;
}

u32 Chatot_GetVoiceParam(SOUND_CHATOT *chatot) {
    if (!Chatot_Exists(chatot)) {
        return 0;
    }

    s8 r0 = Chatot_GetData(chatot)[15];
    if (r0 < -30) {
        return 1;
    } else if (r0 >= 30 && r0 < 128) {
        return 2;
    }

    return 0;
}

BOOL Chatot_CryPatternUsesRecording(int a0) {
    switch (a0) {
    case 0:
    case 1:
    case 5:
    case 11:
    case 12:
        return TRUE;
    default:
        return FALSE;
    }
}
