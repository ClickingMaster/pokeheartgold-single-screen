#include "constants/species.h"

#include "application/pokedex/pokedex_internal.h"
#include "msgdata/msg.naix"

#include "msgdata.h"
#include "pokedex_language.h"

// Callers see these through pokedex_text_util.h with a 32-bit species (as their calls were compiled).
String *PokedexApp_GetSpeciesNameString(u16 species, int language, enum HeapID heapId);
String *PokedexApp_GetCategoryString(u16 species, int language, enum HeapID heapId);
String *PokedexApp_GetFlavorTextString(u16 species, int language, int a2, enum HeapID heapId);

#if defined(HEARTGOLD)
#define ZUKAN_FLAVOR_GMM (NARC_msg_msg_0803_bin)
#elif defined(SOULSILVER)
#define ZUKAN_FLAVOR_GMM (NARC_msg_msg_0804_bin)
#else
#error "Unable to determine game"
#endif // HGSS

static int deadstrip_0(int a0);
static String *PokedexApp_ReadMsgString(int msgBank, int msgNum, enum HeapID heapId);
static void PokedexApp_GetLanguageMsgInfo(u16 species, int language, int *pMsgNo, int *pLanguageFlag, int *pLanguageFlagNativeMask);

static const int sForeignCategoryMsgBanks[] = {
    NARC_msg_msg_0828_bin,
    NARC_msg_msg_0823_bin,
    NARC_msg_msg_0824_bin,
    NARC_msg_msg_0825_bin,
    NARC_msg_msg_0826_bin,
    NARC_msg_msg_0827_bin,
};

static const int sForeignSpeciesNameMsgBanks[] = {
    NARC_msg_msg_0822_bin,
    NARC_msg_msg_0817_bin,
    NARC_msg_msg_0818_bin,
    NARC_msg_msg_0819_bin,
    NARC_msg_msg_0820_bin,
    NARC_msg_msg_0821_bin,
};

static const int sForeignFlavorTextMsgBanks[] = {
    NARC_msg_msg_0810_bin,
    NARC_msg_msg_0805_bin,
    NARC_msg_msg_0806_bin,
    NARC_msg_msg_0807_bin,
    NARC_msg_msg_0808_bin,
    NARC_msg_msg_0809_bin,
};

String *PokedexApp_GetSpeciesNameString(u16 species, int language, enum HeapID heapId) {
    int languageMsgNum;
    int unused;
    int msgNum;
    int msgBank;

    PokedexApp_GetLanguageMsgInfo(species, language, &languageMsgNum, &unused, &msgNum);
    if (msgNum == DEX_LANGUAGE_FLAG_MAX) {
        return GetSpeciesName(species, heapId);
    } else {
        int msgBanks[6];
        ARRAY_ASSIGN(msgBanks, sForeignSpeciesNameMsgBanks);

        msgBank = msgBanks[msgNum];
        msgNum = languageMsgNum;
    }
    return PokedexApp_ReadMsgString(msgBank, msgNum, heapId);
}

String *PokedexApp_GetCategoryString(u16 species, int language, enum HeapID heapId) {
    int languageMsgNum;
    int unused;
    int msgNum;
    int msgBank;

    PokedexApp_GetLanguageMsgInfo(species, language, &languageMsgNum, &unused, &msgNum);
    if (msgNum == DEX_LANGUAGE_FLAG_MAX) {
        msgNum = species;
        msgBank = NARC_msg_msg_0816_bin;
    } else {
        int msgBanks[6];
        ARRAY_ASSIGN(msgBanks, sForeignCategoryMsgBanks);

        msgBank = msgBanks[msgNum];
        msgNum = languageMsgNum;
    }
    return PokedexApp_ReadMsgString(msgBank, msgNum, heapId);
}

String *PokedexApp_GetFlavorTextString(u16 species, int language, int a2, enum HeapID heapId) {
    int languageMsgNum;
    int unused;
    int msgNum;
    int msgBank;

    PokedexApp_GetLanguageMsgInfo(species, language, &languageMsgNum, &unused, &msgNum);
    if (msgNum == DEX_LANGUAGE_FLAG_MAX) {
        GF_ASSERT(a2 < 1);
        msgNum = species + a2;
        msgBank = ZUKAN_FLAVOR_GMM;
    } else {
        int msgBanks[6];
        ARRAY_ASSIGN(msgBanks, sForeignFlavorTextMsgBanks);

        GF_ASSERT(a2 < 1);
        msgBank = msgBanks[msgNum];
        msgNum = languageMsgNum + a2;
    }
    return PokedexApp_ReadMsgString(msgBank, msgNum, heapId);
}

static String *PokedexApp_ReadMsgString(int msgBank, int msgNum, enum HeapID heapId) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msgBank, heapId);
    if (msgData != NULL) {
        String *ret = String_New(256, heapId);
        if (ret != NULL) {
            ReadMsgDataIntoString(msgData, msgNum, ret);
        }
        DestroyMsgData(msgData);
        return ret;
    }

    return NULL;
}

static inline int PokedexApp_LanguageFlagToBankIndex(int languageFlag) {
    GF_ASSERT(languageFlag < DEX_LANGUAGE_FLAG_MAX);
    if (languageFlag == DEX_LANGUAGE_FLAG_NATIVE) {
        languageFlag = DEX_LANGUAGE_FLAG_MAX;
    }
    return languageFlag;
}

static inline BOOL PokedexApp_IsLanguageMsgValid(int species, int languageFlag) {
    if (species > MAX_SPECIES && languageFlag != DEX_LANGUAGE_FLAG_MAX) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void PokedexApp_GetLanguageMsgInfo(u16 species, int language, int *pMsgNo, int *pLanguageFlag, int *pLanguageFlagNativeMask) {
    *pLanguageFlag = LanguageToDexFlag(language);
    GF_ASSERT(*pLanguageFlag < DEX_LANGUAGE_FLAG_MAX);
    *pMsgNo = species;
    *pLanguageFlagNativeMask = PokedexApp_LanguageFlagToBankIndex(*pLanguageFlag);
    GF_ASSERT(PokedexApp_IsLanguageMsgValid(*pMsgNo, *pLanguageFlagNativeMask));
}
