#include "constants/sndseq.h"

#include "sound.h"
#include "sound_system.h"

extern int gSoundSystemFlags[4];

static void SoundSys_SetGBSoundsState(u8 enabled);

BOOL NNS_SndPlayerReadVariable(NNSSndHandle *handle, u32 variable, s16 *value);
void NNS_SndPlayerSetTrackAllocatableChannel(NNSSndHandle *handle, u32 tracks, u32 channels);
u8 GF_GetPlayerNoBySeq(u16 seqNo);
void GF_SndHandleSetTrackPitch(int handleNo, u32 tracks, int pitch);
void Sound_SetKeepPlayingCries(u8 value);
void Sound_SetChatotRecordingDisabled(u8 value);
BOOL GF_SndReadSeqVariable(u16 seqNo, u32 variable, s16 *value);
void Sound_ApplyFootstepVariation(u16 seqNo);
int Sound_IsSelectSEMuted(void);
u16 GBSounds_GetGBSeqNoByDSSeqNo(u16 seqNo);
u16 GBSounds_GetDSSeqNoByGBSeqNo(u16 seqNo);
void GBSounds_SetAllocatableChannels(void);
void Sound_SetFieldBGMStopOnce(BOOL enabled);

static const u8 sSoundFootstepVariations[8][4] = {
    { 0,  42,  0, 0 },
    { 64, 107, 0, 0 },
    { 32, 47,  0, 0 },
    { 64, 119, 0, 0 },
    { 0,  62,  0, 0 },
    { 48, 127, 0, 0 },
    { 0,  52,  0, 0 },
    { 48, 117, 0, 0 },
};

static const u16 sGBSoundsSeqPairs[136][2] = {
    { SEQ_GS_TITLE,              SEQ_GS_P_TITLE              },
    { SEQ_GS_TITLE01,            SEQ_GS_P_TITLE01            },
    { SEQ_GS_OPENING_TITLE_G,    SEQ_GS_P_OPENING_TITLE_G    },
    { SEQ_GS_OPENING_TITLE_S,    SEQ_GS_P_OPENING_TITLE_S    },
    { SEQ_GS_POKEMON_THEME,      SEQ_GS_P_POKEMON_THEME      },
    { SEQ_GS_SHINKA,             SEQ_GS_P_SHINKA             },
    { SEQ_GS_KOUKAN,             SEQ_GS_P_KOUKAN             },
    { SEQ_GS_BICYCLE,            SEQ_GS_P_BICYCLE            },
    { SEQ_GS_NAMINORI,           SEQ_GS_P_NAMINORI           },
    { SEQ_GS_E_DENDOUIRI,        SEQ_GS_P_E_DENDOUIRI        },
    { SEQ_GS_T_WAKABA,           SEQ_GS_P_T_WAKABA           },
    { SEQ_GS_C_YOSHINO,          SEQ_GS_P_C_YOSHINO          },
    { SEQ_GS_C_KIKYOU,           SEQ_GS_P_C_KIKYOU           },
    { SEQ_GS_T_HIWADA,           SEQ_GS_P_T_HIWADA           },
    { SEQ_GS_C_KOGANE,           SEQ_GS_P_C_KOGANE           },
    { SEQ_GS_C_ENJU,             SEQ_GS_P_C_ENJU             },
    { SEQ_GS_C_ASAGI,            SEQ_GS_P_C_ASAGI            },
    { SEQ_GS_C_TANBA,            SEQ_GS_P_C_TANBA            },
    { SEQ_GS_T_CHOUJI,           SEQ_GS_P_T_CHOUJI           },
    { SEQ_GS_C_FUSUBE,           SEQ_GS_P_C_FUSUBE           },
    { SEQ_GS_R_1_29,             SEQ_GS_P_R_1_29             },
    { SEQ_GS_R_1_30,             SEQ_GS_P_R_1_30             },
    { SEQ_GS_R_2_30,             SEQ_GS_P_R_2_30             },
    { SEQ_GS_R_3_30,             SEQ_GS_P_R_3_30             },
    { SEQ_GS_R_4_34,             SEQ_GS_P_R_4_34             },
    { SEQ_GS_R_5_34,             SEQ_GS_P_R_5_34             },
    { SEQ_GS_R_6_34,             SEQ_GS_P_R_6_34             },
    { SEQ_GS_R_8_34,             SEQ_GS_P_R_8_34             },
    { SEQ_GS_R_6_38,             SEQ_GS_P_R_6_38             },
    { SEQ_GS_R_7_42,             SEQ_GS_P_R_7_42             },
    { SEQ_GS_C_KUCHIBA,          SEQ_GS_P_C_KUCHIBA          },
    { SEQ_GS_C_YAMABUKI,         SEQ_GS_P_C_YAMABUKI         },
    { SEQ_GS_C_HANADA,           SEQ_GS_P_C_HANADA           },
    { SEQ_GS_T_CHION,            SEQ_GS_P_T_CHION            },
    { SEQ_GS_C_TAMAMUSHI,        SEQ_GS_P_C_TAMAMUSHI        },
    { SEQ_GS_C_SEKICHIKU,        SEQ_GS_P_C_SEKICHIKU        },
    { SEQ_GS_C_NIBI,             SEQ_GS_P_C_NIBI             },
    { SEQ_GS_C_TOKIWA,           SEQ_GS_P_C_TOKIWA           },
    { SEQ_GS_T_MASARA,           SEQ_GS_P_T_MASARA           },
    { SEQ_GS_T_GUREN,            SEQ_GS_P_T_GUREN            },
    { SEQ_GS_R_9_01,             SEQ_GS_P_R_9_01             },
    { SEQ_GS_R_16_01,            SEQ_GS_P_R_16_01            },
    { SEQ_GS_R_17_01,            SEQ_GS_P_R_17_01            },
    { SEQ_GS_R_9_03,             SEQ_GS_P_R_9_03             },
    { SEQ_GS_R_10_03,            SEQ_GS_P_R_10_03            },
    { SEQ_GS_R_12_03,            SEQ_GS_P_R_12_03            },
    { SEQ_GS_R_13_03,            SEQ_GS_P_R_13_03            },
    { SEQ_GS_R_14_03,            SEQ_GS_P_R_14_03            },
    { SEQ_GS_R_15_03,            SEQ_GS_P_R_15_03            },
    { SEQ_GS_R_16_03,            SEQ_GS_P_R_16_03            },
    { SEQ_GS_R_17_03,            SEQ_GS_P_R_17_03            },
    { SEQ_GS_R_10_11,            SEQ_GS_P_R_10_11            },
    { SEQ_GS_R_13_11,            SEQ_GS_P_R_13_11            },
    { SEQ_GS_R_12_24,            SEQ_GS_P_R_12_24            },
    { SEQ_GS_R_1_26,             SEQ_GS_P_R_1_26             },
    { SEQ_GS_POKESEN,            SEQ_GS_P_POKESEN            },
    { SEQ_GS_FS,                 SEQ_GS_P_FS                 },
    { SEQ_GS_GYM,                SEQ_GS_P_GYM                },
    { SEQ_GS_UTSUGI_RABO,        SEQ_GS_P_UTSUGI_RABO        },
    { SEQ_GS_OHKIDO,             SEQ_GS_P_OHKIDO             },
    { SEQ_GS_KABURENJOU,         SEQ_GS_P_KABURENJOU         },
    { SEQ_GS_GAME,               SEQ_GS_P_GAME               },
    { SEQ_GS_BATTLETOWER,        SEQ_GS_P_BATTLETOWER        },
    { SEQ_GS_BATTLETOWER2,       SEQ_GS_P_BATTLETOWER2       },
    { SEQ_GS_TO_MADATSUBOMI1,    SEQ_GS_P_TO_MADATSUBOMI1    },
    { SEQ_GS_D_CHIKATSUURO,      SEQ_GS_P_D_CHIKATSUURO      },
    { SEQ_GS_D_UNKNOWN_ISEKI,    SEQ_GS_P_D_UNKNOWN_ISEKI    },
    { SEQ_GS_D_KOUEN,            SEQ_GS_P_D_KOUEN            },
    { SEQ_GS_TO_YAKETA,          SEQ_GS_P_TO_YAKETA          },
    { SEQ_GS_TO_SUZU,            SEQ_GS_P_TO_SUZU            },
    { SEQ_GS_TO_TOUDAI,          SEQ_GS_P_TO_TOUDAI          },
    { SEQ_GS_D_AJITO,            SEQ_GS_P_D_AJITO            },
    { SEQ_GS_D_KOORINONUKE,      SEQ_GS_P_D_KOORINONUKE      },
    { SEQ_GS_RYUUNOANA,          SEQ_GS_P_RYUUNOANA          },
    { SEQ_GS_D_IWAYAMA,          SEQ_GS_P_D_IWAYAMA          },
    { SEQ_GS_D_TOKIWANOMORI3,    SEQ_GS_P_D_TOKIWANOMORI3    },
    { SEQ_GS_D_CHAMPROAD,        SEQ_GS_P_D_CHAMPROAD        },
    { SEQ_GS_CHAMPROAD,          SEQ_GS_P_CHAMPROAD          },
    { SEQ_GS_E_TSURETEKE1,       SEQ_GS_P_E_TSURETEKE1       },
    { SEQ_GS_E_TSURETEKE2,       SEQ_GS_P_E_TSURETEKE2       },
    { SEQ_GS_E_RIVAL1,           SEQ_GS_P_E_RIVAL1           },
    { SEQ_GS_E_RIVAL2,           SEQ_GS_P_E_RIVAL2           },
    { SEQ_GS_TAIKAIMAE,          SEQ_GS_P_TAIKAIMAE          },
    { SEQ_GS_TAIKAI,             SEQ_GS_P_TAIKAI             },
    { SEQ_GS_KAIDENPA,           SEQ_GS_P_KAIDENPA           },
    { SEQ_GS_SENKYO,             SEQ_GS_P_SENKYO             },
    { SEQ_GS_E_LINEAR,           SEQ_GS_P_E_LINEAR           },
    { SEQ_GS_KOUSOKUSEN,         SEQ_GS_P_KOUSOKUSEN         },
    { SEQ_GS_OTSUKIMI_EVENT,     SEQ_GS_P_OTSUKIMI_EVENT     },
    { SEQ_GS_RADIO_JINGLE,       SEQ_GS_P_RADIO_JINGLE       },
    { SEQ_GS_RADIO_KOMORIUTA,    SEQ_GS_P_RADIO_KOMORIUTA    },
    { SEQ_GS_RADIO_MARCH,        SEQ_GS_P_RADIO_MARCH        },
    { SEQ_GS_RADIO_UNKNOWN,      SEQ_GS_P_RADIO_UNKNOWN      },
    { SEQ_GS_HUE,                SEQ_GS_P_HUE                },
    { SEQ_GS_OHKIDO_RABO,        SEQ_GS_P_OHKIDO_RABO        },
    { SEQ_GS_AIKOTOBA,           SEQ_GS_P_AIKOTOBA           },
    { SEQ_GS_E_MINAKI,           SEQ_GS_P_E_MINAKI           },
    { SEQ_GS_IBUKI,              SEQ_GS_P_IBUKI              },
    { SEQ_GS_EYE_J_SHOUJO,       SEQ_GS_P_EYE_J_SHOUJO       },
    { SEQ_GS_EYE_J_SHOUNEN,      SEQ_GS_P_EYE_J_SHOUNEN      },
    { SEQ_GS_EYE_J_AYASHII,      SEQ_GS_P_EYE_J_AYASHII      },
    { SEQ_GS_EYE_BOUZU,          SEQ_GS_P_EYE_BOUZU          },
    { SEQ_GS_EYE_MAIKO,          SEQ_GS_P_EYE_MAIKO          },
    { SEQ_GS_EYE_ROCKET,         SEQ_GS_P_EYE_ROCKET         },
    { SEQ_GS_EYE_K_SHOUJO,       SEQ_GS_P_EYE_K_SHOUJO       },
    { SEQ_GS_EYE_K_SHOUNEN,      SEQ_GS_P_EYE_K_SHOUNEN      },
    { SEQ_GS_EYE_K_AYASHII,      SEQ_GS_P_EYE_K_AYASHII      },
    { SEQ_GS_VS_NORAPOKE,        SEQ_GS_P_VS_NORAPOKE        },
    { SEQ_GS_VS_TRAINER,         SEQ_GS_P_VS_TRAINER         },
    { SEQ_GS_VS_GYMREADER,       SEQ_GS_P_VS_GYMREADER       },
    { SEQ_GS_VS_RIVAL,           SEQ_GS_P_VS_RIVAL           },
    { SEQ_GS_VS_ROCKET,          SEQ_GS_P_VS_ROCKET          },
    { SEQ_GS_VS_SUICUNE,         SEQ_GS_P_VS_SUICUNE         },
    { SEQ_GS_VS_ENTEI,           SEQ_GS_P_VS_ENTEI           },
    { SEQ_GS_VS_RAIKOU,          SEQ_GS_P_VS_RAIKOU          },
    { SEQ_GS_VS_CHAMP,           SEQ_GS_P_VS_CHAMP           },
    { SEQ_GS_VS_NORAPOKE_KANTO,  SEQ_GS_P_VS_NORAPOKE_KANTO  },
    { SEQ_GS_VS_TRAINER_KANTO,   SEQ_GS_P_VS_TRAINER_KANTO   },
    { SEQ_GS_VS_GYMREADER_KANTO, SEQ_GS_P_VS_GYMREADER_KANTO },
    { SEQ_GS_WIN1,               SEQ_GS_P_WIN1               },
    { SEQ_GS_WIN2,               SEQ_GS_P_WIN2               },
    { SEQ_GS_WIN2_NOT_FAN,       SEQ_GS_P_WIN2_NOT_FAN       },
    { SEQ_GS_WIN3,               SEQ_GS_P_WIN3               },
    { SEQ_GS_PT_ENTR,            SEQ_GS_P_PT_ENTR            },
    { SEQ_GS_PT_OPEN,            SEQ_GS_P_PT_OPEN            },
    { SEQ_GS_PT_TITLE,           SEQ_GS_P_PT_TITLE           },
    { SEQ_GS_PT_GAME,            SEQ_GS_P_PT_GAME            },
    { SEQ_GS_PT_GAMEF,           SEQ_GS_P_PT_GAMEF           },
    { SEQ_GS_PT_RESULT,          SEQ_GS_P_PT_RESULT          },
    { SEQ_GS_PT_END,             SEQ_GS_P_PT_END             },
    { SEQ_GS_PT_END_FIELD,       SEQ_GS_P_PT_END_FIELD       },
    { SEQ_GS_WIFITOWER,          SEQ_GS_P_WIFITOWER          },
    { SEQ_GS_SAFARI_ROAD,        SEQ_GS_P_SAFARI_ROAD        },
    { SEQ_GS_SAFARI_HOUSE,       SEQ_GS_P_SAFARI_HOUSE       },
    { SEQ_GS_SAFARI_FIELD,       SEQ_GS_P_SAFARI_FIELD       },
    { SEQ_PL_BICYCLE,            SEQ_PL_P_BICYCLE            },
};

void Sound_SetKeepPlayingCries(u8 value) {
    *(u8 *)GF_SdatGetAttrPtr(53) = value;
}
void Sound_SetChatotRecordingDisabled(u8 value) {
    *(u8 *)GF_SdatGetAttrPtr(54) = value;
}
BOOL GF_SndReadSeqVariable(u16 seqNo, u32 variable, s16 *value) {
    GF_ASSERT(value != NULL);
    GF_ASSERT(variable <= 15);
    return NNS_SndPlayerReadVariable(GF_GetSoundHandle(GF_GetSndHandleByPlayerNo(GF_GetPlayerNoBySeq(seqNo))), variable, value);
}
void Sound_ApplyFootstepVariation(u16 seqNo) {
    u8 *state = GF_SdatGetAttrPtr(55);
    GF_SetVolumeBySeqNo(seqNo, sSoundFootstepVariations[state[0]][1]);
    GF_SndHandleSetTrackPitch(SND_HANDLE_SE_2, 65535, sSoundFootstepVariations[state[0]][0]);
    if (state[1] >= 8) {
        state[1] = 0;
    }
}
void Sound_SetSelectSEMuted(BOOL enabled) {
    *(u8 *)GF_SdatGetAttrPtr(56) = enabled;
}
int Sound_IsSelectSEMuted(void) {
    return *(u8 *)GF_SdatGetAttrPtr(56);
}
static void SoundSys_SetGBSoundsState(u8 enabled) {
    *(u8 *)GF_SdatGetAttrPtr(57) = enabled;
}
BOOL SoundSys_GetGBSoundsState(void) {
    return *(u8 *)GF_SdatGetAttrPtr(57);
}
void SoundSys_ToggleGBSounds(void) {
    if (SoundSys_GetGBSoundsState() == 0) {
        SoundSys_SetGBSoundsState(1);
    } else {
        SoundSys_SetGBSoundsState(0);
    }
    if (GF_GetQueuedBGM() == 0) {
        u16 seqNo = GF_GetCurrentPlayingBGM();
        u8 volume = GF_SndWorkGetGbSoundsVolume();
        if (seqNo != GBSounds_GetGBSeqNoByDSSeqNo(seqNo)) {
            PlayBGM(seqNo);
        }
        GF_SndHandleMoveVolume(0, volume, 0);
    }
}
u16 GBSounds_GetGBSeqNoByDSSeqNo(u16 seqNo) {
    u16 i;
    for (i = 0; i < 136; i++) {
        if (seqNo == sGBSoundsSeqPairs[i][0]) {
            return sGBSoundsSeqPairs[i][1];
        }
    }
    return seqNo;
}
u16 GBSounds_GetDSSeqNoByGBSeqNo(u16 seqNo) {
    u16 i;
    for (i = 0; i < 136; i++) {
        if (seqNo == sGBSoundsSeqPairs[i][1]) {
            return sGBSoundsSeqPairs[i][0];
        }
    }
    return seqNo;
}
void GBSounds_SetAllocatableChannels(void) {
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(0), 15, 0xA7FE);
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(7), 15, 0xA7FE);
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(2), 15, 0xA7FE);
}
void Sound_SetFieldBGMStopOnce(BOOL enabled) {
    gSoundSystemFlags[3] = enabled;
}
void Sound_ResetFieldBGMStopOnce(void) {
    gSoundSystemFlags[3] = 0;
    gSoundSystemFlags[1] = 0;
}
