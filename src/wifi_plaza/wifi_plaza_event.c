#include "wifi_plaza/wifi_plaza_event.h"

#include <string.h>

#include "global.h"

#include "constants/global_fieldmap.h"

#include "msgdata/msg/msg_0777.h"
#include "wifi_plaza/wifi_plaza_map.h"
#include "wifi_plaza/wifi_plaza_room.h"

#include "assert.h"
#include "heap.h"
#include "math_util.h"
#include "overlay_42.h"
#include "sound_chatot.h"
#include "sound_playback.h"

BOOL PlaySEOnPlayer(u16 seqNo, int playerNo);

extern const u16 gWiFiPlazaLight1Colors[];
extern const u16 gWiFiPlazaLightColorSets[][5];
extern void (*const gWiFiPlazaEventEffectPhaseUpdates[])(WiFiPlazaEventEffect *effect, int arg);
extern void (*const gWiFiPlazaEventEffectPhaseStarts[])(WiFiPlazaEventEffect *effect, u32 arg);
extern const u8 gWiFiPlazaEventPhase3PropIds[];
extern const u32 gWiFiPlazaEventPhase3PropGroups[][2];
extern const u8 gWiFiPlazaEventPhase4Steps[];
extern const u8 gWiFiPlazaEventPhase4HideSteps[];
extern const u32 gWiFiPlazaEventPhase5Thresholds[];
extern const u32 gWiFiPlazaEventPhase5AnimSpeeds[];
extern const u8 gWiFiPlazaEventModelIds3[];
extern const u8 gWiFiPlazaEventModelIds4[];
extern const u8 gWiFiPlazaEventModelIds2[];
extern const u8 gWiFiPlazaEventModelIds5[];
extern const u8 gWiFiPlazaEventModelIds1[];
extern const u8 gWiFiPlazaEventModelIds6[];
extern const u8 gWiFiPlazaEventModelIds0[];
extern const u8 gWiFiPlazaMoverMarkerTags[];
extern const u16 gWiFiPlazaMoverSounds[][6];
extern BOOL (*const gWiFiPlazaEventWobbleSteps[])(WiFiPlazaEventWobble *wobble, WiFiPlazaModels *models, int index);
extern BOOL (*const gWiFiPlazaEventWobbleInits[])(WiFiPlazaEventWobble *wobble, WiFiPlazaModels *models, int index);

enum WiFiPlazaSurveyGuideStep {
    WIFI_PLAZA_SURVEY_GUIDE_GREET,
    WIFI_PLAZA_SURVEY_GUIDE_OPEN_YES_NO,
    WIFI_PLAZA_SURVEY_GUIDE_HANDLE_YES_NO,
    WIFI_PLAZA_SURVEY_GUIDE_EXPLAIN,
    WIFI_PLAZA_SURVEY_GUIDE_END,
    WIFI_PLAZA_SURVEY_GUIDE_WAIT_MESSAGE,
};

static void WiFiPlazaLight_Delete(WiFiPlazaLight *light);
static void WiFiPlazaLight_Update(WiFiPlazaLight *light);
static void WiFiPlazaLight_FadeToColorSet(WiFiPlazaLight *light, int set);
static void WiFiPlazaLight_FadeLight1(WiFiPlazaLight *light, int set);
static void WiFiPlazaLight_FadeToThirdSet(WiFiPlazaLight *light, int set);
static void WiFiPlazaLight_PackColor(const s8 *rgb, u16 *color);
static void WiFiPlazaLight_UnpackColor(s8 *rgb, const u16 *color);
static void WiFiPlazaLight_LerpColor(const s8 *start, const s8 *end, s8 *dest, s32 step, s32 total);
static inline void WiFiPlazaEvent_Clear(WiFiPlazaEvent *event);
static int WiFiPlazaEvent_GetColorSet(int value);
static int WiFiPlazaEvent_GetLight1Set(int value);
static int WiFiPlazaEvent_GetThirdSet(int value);
static void WiFiPlazaEvent_TurnOffLight2(WiFiPlazaEvent *event);
static WiFiPlazaEventEffect *WiFiPlazaEventEffect_Create(WiFiPlazaWork *lobby, WiFiPlazaModels *models, enum HeapID heapID);
static void WiFiPlazaEventEffect_Delete(WiFiPlazaEventEffect *effect);
static void WiFiPlazaEventEffect_Update(WiFiPlazaEventEffect *effect);
static void WiFiPlazaEventEffect_RunPhase(WiFiPlazaEventEffect *effect, int index, int arg);
static void WiFiPlazaEventEffect_StartPhase(WiFiPlazaEventEffect *effect, u32 index, u32 arg);
static void WiFiPlazaEvent_GetModelList(u32 kind, WiFiPlazaEventModelList *list);
static void WiFiPlazaEvent_StartLoopSE(WiFiPlazaEventEffect *effect);
static void WiFiPlazaEvent_StopLoopSE(WiFiPlazaEventEffect *effect);
static WiFiPlazaEventMovers *WiFiPlazaEventMovers_Create(WiFiPlazaRoomWork *room, enum HeapID heapID);
static void WiFiPlazaEventMovers_Delete(WiFiPlazaEventMovers *work);
static void WiFiPlazaEventMovers_Update(WiFiPlazaEventMovers *work);
static u8 WiFiPlazaEventMovers_GetState(WiFiPlazaEventMovers *work, u32 index, int field);
static void WiFiPlazaEventMovers_UpdateX(WiFiPlazaEventMovers *work, int index);
static void WiFiPlazaEventMovers_StartActions(WiFiPlazaEventMovers *work, u32 index, BOOL hasOwnSelection, int isOwnIndex, int ownColumn);
static void WiFiPlazaEventMovers_PlaySE(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 seqNo);
static void WiFiPlazaEventMovers_PlayCry(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 species);
static BOOL WiFiPlazaEventWobble_Start(WiFiPlazaEventWobble *wobble, int kind, WiFiPlazaModels *models, int index);
static BOOL WiFiPlazaEventWobble_Update(WiFiPlazaEventWobble *wobble, WiFiPlazaModels *models, int index);
static void WiFiPlazaEvent_UpdateMoverMarkers(WiFiPlazaEventMovers *work, int index);

static void WiFiPlazaLight_Delete(WiFiPlazaLight *light) {
    Heap_Free(light);
}

static void WiFiPlazaLight_Update(WiFiPlazaLight *light) {
    int i;
    int changed;

    changed = 0;
    for (i = 0; i < 2; i++) {
        if (light->lightTimers[i] + 1 <= 60) {
            light->lightTimers[i]++;
            WiFiPlazaLight_LerpColor((s8 *)light->lightsFrom[i], (s8 *)light->lightsTo[i], (s8 *)light->lights[i], light->lightTimers[i], 60);
            changed = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (light->materialTimers[i] + 1 <= 60) {
            light->materialTimers[i]++;
            WiFiPlazaLight_LerpColor((s8 *)light->materialsFrom[i], (s8 *)light->materialsTo[i], (s8 *)light->materials[i], light->materialTimers[i], 60);
            changed = 1;
        }
    }
    if (changed) {
        WiFiPlazaLight_Apply(light);
    }
}

// Fades light 0 and the four material colours to colour set `set`.
static void WiFiPlazaLight_FadeToColorSet(WiFiPlazaLight *light, int set) {
    int i;
    const u16 *colors;

    light->lightsFrom[0][0] = light->lights[0][0];
    light->lightsFrom[0][1] = light->lights[0][1];
    light->lightsFrom[0][2] = light->lights[0][2];
    light->lightsFrom[0][3] = light->lights[0][3];
    light->lightTimers[0] = 0;
    colors = gWiFiPlazaLightColorSets[set];
    WiFiPlazaLight_UnpackColor((s8 *)light->lightsTo[0], colors);
    for (i = 0; i < 4; i++) {
        light->materialsFrom[i][0] = light->materials[i][0];
        light->materialsFrom[i][1] = light->materials[i][1];
        light->materialsFrom[i][2] = light->materials[i][2];
        light->materialsFrom[i][3] = light->materials[i][3];
        light->materialTimers[i] = 0;
        WiFiPlazaLight_UnpackColor((s8 *)light->materialsTo[i], colors + 1 + i);
    }
}

static void WiFiPlazaLight_FadeLight1(WiFiPlazaLight *light, int set) {
    light->lightsFrom[1][0] = light->lights[1][0];
    light->lightsFrom[1][1] = light->lights[1][1];
    light->lightsFrom[1][2] = light->lights[1][2];
    light->lightsFrom[1][3] = light->lights[1][3];
    light->lightTimers[1] = 0;
    WiFiPlazaLight_UnpackColor((s8 *)light->lightsTo[1], &gWiFiPlazaLight1Colors[set]);
}

static void WiFiPlazaLight_FadeToThirdSet(WiFiPlazaLight *light, int set) {
}

static void WiFiPlazaLight_PackColor(const s8 *rgb, u16 *color) {
    *color = rgb[0] | (rgb[1] << 5) | (rgb[2] << 10);
}

static void WiFiPlazaLight_UnpackColor(s8 *rgb, const u16 *color) {
    rgb[0] = color[0] & 0x1F;
    rgb[1] = (color[0] & (0x1F << 5)) >> 5;
    rgb[2] = (color[0] & (0x1F << 10)) >> 10;
}

static void WiFiPlazaLight_LerpColor(const s8 *start, const s8 *end, s8 *dest, s32 step, s32 total) {
    s32 dy = end[1] - start[1];
    s32 dz = end[2] - start[2];
    s32 x = start[0];

    dest[0] = x + (end[0] - x) * step / total;
    dest[1] = start[1] + step * dy / total;
    dest[2] = start[2] + step * dz / total;
}

// colors: light 0 followed by the four material colours
void WiFiPlazaLight_UnpackColorSet(s8 *dest, const u16 *colors) {
    int i;

    WiFiPlazaLight_UnpackColor(dest, colors);
    i = 0;
    colors++;
    dest += 8;
    while (i < 4) {
        WiFiPlazaLight_UnpackColor(dest, colors);
        i++;
        colors++;
        dest += 4;
    }
}

void WiFiPlazaLight_UnpackLight1(WiFiPlazaLight *light, const u16 *color) {
    WiFiPlazaLight_UnpackColor((s8 *)light->lights[1], color);
}

void WiFiPlazaLight_Apply(WiFiPlazaLight *light) {
    u16 color;
    u16 color2;
    int i;
    u8 *rgb;

    i = 0;
    rgb = light->lights[0];
    for (; i < 2; i++, rgb += 4) {
        WiFiPlazaLight_PackColor((s8 *)rgb, &color);
        NNS_G3dGlbLightColor((GXLightId)i, color);
    }
    WiFiPlazaLight_PackColor((s8 *)light->materials[0], &color);
    WiFiPlazaLight_PackColor((s8 *)light->materials[1], &color2);
    NNS_G3dGlbMaterialColorDiffAmb(color, color2, FALSE);
    WiFiPlazaLight_PackColor((s8 *)light->materials[2], &color);
    WiFiPlazaLight_PackColor((s8 *)light->materials[3], &color2);
    NNS_G3dGlbMaterialColorSpecEmi(color, color2, FALSE);
}

static inline void WiFiPlazaEvent_Clear(WiFiPlazaEvent *event) {
    u8 *clear = (u8 *)event;
    u32 count = sizeof(WiFiPlazaEvent);

    while (count != 0) {
        *clear = 0;
        clear = clear + 1;
        count = count - 1;
    }
}

WiFiPlazaEvent *WiFiPlazaEvent_Create(enum HeapID heapID, WiFiPlazaRoomWork *room) {
    WiFiPlazaEvent *event = Heap_Alloc(heapID, sizeof(WiFiPlazaEvent));
    int colorSet;
    int light1Set;
    int thirdSet;
    u32 mode;
    BOOL flagB;
    BOOL flagC;
    u32 flagD;

    WiFiPlazaEvent_Clear(event);
    event->room = room;
    event->lobby = WiFiPlazaRoom_GetLobby(room);
    event->models = WiFiPlazaRoom_GetModels(room);
    event->objManager = WiFiPlazaRoom_GetObjManager(room);
    colorSet = WiFiPlazaEvent_GetColorSet(WiFiPlazaWork_GetColorEvent(event->lobby));
    light1Set = WiFiPlazaEvent_GetLight1Set(WiFiPlazaWork_GetLight1Event(event->lobby));
    thirdSet = WiFiPlazaEvent_GetThirdSet(WiFiPlazaWork_GetThirdEvent(event->lobby));
    event->light = WiFiPlazaLight_Create(heapID, colorSet, light1Set, thirdSet);
    event->movers = WiFiPlazaEventMovers_Create(room, heapID);
    event->effect = WiFiPlazaEventEffect_Create(event->lobby, event->models, heapID);
    WiFiPlazaObj_SetLightSet(event->objManager, colorSet);
    mode = WiFiPlazaWork_GetFireworksState(event->lobby);
    WiFiPlazaWork_IsLobbyOpen(event->lobby);
    flagB = WiFiPlazaWork_IsParadeStarted(event->lobby);
    flagC = WiFiPlazaWork_IsLightingStarted(event->lobby);
    flagD = WiFiPlazaWork_IsClosing(event->lobby);
    WiFiPlazaWork_GetEventScheduleMode5(event->lobby);
    if (mode == 2) {
        WiFiPlazaModels_EnableKind1To4Lights(event->models);
    }
    if ((mode != 1) && (mode == 0) && (flagC == TRUE)) {
        WiFiPlazaModels_SetPairAnim(event->models, WIFI_PLAZA_PAIR_ANIM_LOOP);
    }
    if (flagB == TRUE) {
        WiFiPlazaModels_StartBaseAnim4(event->models);
    }
    if (flagD == 1) {
        WiFiPlazaEvent_TurnOffLight2(event);
    }
    return event;
}

void WiFiPlazaEvent_Delete(WiFiPlazaEvent *event) {
    WiFiPlazaEventEffect_Delete(event->effect);
    WiFiPlazaEventMovers_Delete(event->movers);
    WiFiPlazaLight_Delete(event->light);
    Heap_Free(event);
}

// Reacts to the lobby's change flags.
void WiFiPlazaEvent_Update(WiFiPlazaEvent *event) {
    int set;

    if (WiFiPlazaWork_IsEventFlagSet(event->lobby, 1)) {
        set = WiFiPlazaEvent_GetColorSet(WiFiPlazaWork_GetColorEvent(event->lobby));
        WiFiPlazaLight_FadeToColorSet(event->light, set);
        WiFiPlazaObj_FadeToLightSet(event->objManager, set);
    }
    if (WiFiPlazaWork_IsEventFlagSet(event->lobby, 2)) {
        set = WiFiPlazaEvent_GetLight1Set(WiFiPlazaWork_GetLight1Event(event->lobby));
        WiFiPlazaLight_FadeLight1(event->light, set);
    }
    if (WiFiPlazaWork_IsEventFlagSet(event->lobby, 3)) {
        set = WiFiPlazaEvent_GetThirdSet(WiFiPlazaWork_GetThirdEvent(event->lobby));
        WiFiPlazaLight_FadeToThirdSet(event->light, set);
    }
    if (WiFiPlazaWork_IsLightingStarted(event->lobby) == TRUE && WiFiPlazaWork_GetFireworksState(event->lobby) == 0 && WiFiPlazaModels_GetPairAnimState(event->models) == 0) {
        WiFiPlazaModels_SetPairAnim(event->models, WIFI_PLAZA_PAIR_ANIM_OPEN);
    }
    if (WiFiPlazaWork_IsEventFlagSet(event->lobby, 5) && WiFiPlazaWork_GetFireworksState(event->lobby) == 1) {
        WiFiPlazaModels_EnableKind1To4Lights(event->models);
        WiFiPlazaModels_SetPairAnim(event->models, WIFI_PLAZA_PAIR_ANIM_CLOSE);
    }
    if (WiFiPlazaWork_IsEventFlagSet(event->lobby, 6) && WiFiPlazaWork_IsParadeStarted(event->lobby) == TRUE) {
        WiFiPlazaModels_StartBaseAnim4(event->models);
    }
    if (WiFiPlazaWork_IsClosing(event->lobby) == 1) {
        WiFiPlazaModels_StopBaseAnim4(event->models);
        WiFiPlazaEvent_TurnOffLight2(event);
    }
    WiFiPlazaLight_Update(event->light);
    WiFiPlazaEventMovers_Update(event->movers);
    WiFiPlazaEventEffect_Update(event->effect);
}

u8 WiFiPlazaEvent_GetMoverState(WiFiPlazaEvent *event, u32 index, int field) {
    return WiFiPlazaEventMovers_GetState(event->movers, index, field);
}

// Lobby value 1-6 -> colour set 0-5
static int WiFiPlazaEvent_GetColorSet(int value) {
    switch (value) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return value - 1;
    default:
        GF_AssertFail();
        return 0;
    }
}

// Lobby value 7-10 -> light 1 colour 0-3
static int WiFiPlazaEvent_GetLight1Set(int value) {
    switch (value) {
    case 7:
    case 8:
    case 9:
    case 10:
        return value - 7;
    default:
        GF_AssertFail();
        return 0;
    }
}

static int WiFiPlazaEvent_GetThirdSet(int value) {
    switch (value) {
    case 11:
    case 12:
    case 13:
    case 14:
        return value - 11;
    default:
        GF_AssertFail();
        return 0;
    }
}

static void WiFiPlazaEvent_TurnOffLight2(WiFiPlazaEvent *event) {
    NNS_G3dGlbLightColor(GX_LIGHTID_2, 0);
}

static WiFiPlazaEventEffect *WiFiPlazaEventEffect_Create(WiFiPlazaWork *lobby, WiFiPlazaModels *models, enum HeapID heapID) {
    WiFiPlazaEventEffect *effect = Heap_Alloc(heapID, sizeof(WiFiPlazaEventEffect));
    u32 effectId;
    u8 *cursor = (u8 *)effect;
    u32 n = sizeof(WiFiPlazaEventEffect);

    do {
        *cursor++ = 0;
    } while (--n);
    effect->lobby = lobby;
    effect->models = models;
    effectId = WiFiPlazaWork_GetFireworksPhase(lobby);
    effect->activeCount = WiFiPlazaWork_GetFireworksState(lobby);
    if (effect->activeCount && effectId) {
        WiFiPlazaModels_SetEffectActive(models, TRUE);
        WiFiPlazaEvent_StartLoopSE(effect);
        WiFiPlazaEventEffect_StartPhase(effect, effectId, WiFiPlazaWork_GetFireworksCounter(lobby));
    }
    return effect;
}

static void WiFiPlazaEventEffect_Delete(WiFiPlazaEventEffect *effect) {
    WiFiPlazaEvent_StopLoopSE(effect);
    Heap_Free(effect);
}

static void WiFiPlazaEventEffect_Update(WiFiPlazaEventEffect *effect) {
    int effectId;
    int arg;
    int count;

    effectId = WiFiPlazaWork_GetFireworksPhase(effect->lobby);
    arg = WiFiPlazaWork_GetFireworksCounter(effect->lobby);
    count = WiFiPlazaWork_GetFireworksState(effect->lobby);
    if (effect->activeCount != count) {
        effect->activeCount = count;
        if (count == 1) {
            WiFiPlazaModels_SetEffectActive(effect->models, TRUE);
            WiFiPlazaEventEffect_StartPhase(effect, effectId, arg);
            WiFiPlazaEvent_StartLoopSE(effect);
        }
    }
    if (count != 0) {
        if (effect->effect != effectId) {
            WiFiPlazaEventEffect_StartPhase(effect, effectId, arg);
        }
        WiFiPlazaEventEffect_RunPhase(effect, effectId, arg);
    }
}

static void WiFiPlazaEventEffect_RunPhase(WiFiPlazaEventEffect *effect, int index, int arg) {
    if ((u32)index >= 6) {
        GF_AssertFail();
    }
    if ((u32)index < 6) {
        gWiFiPlazaEventEffectPhaseUpdates[index](effect, arg);
    }
}

void WiFiPlazaEventEffect_UpdateNone(WiFiPlazaEventEffect *effect, u32 arg) {
}

void WiFiPlazaEventEffect_UpdatePhase2(WiFiPlazaEventEffect *effect, u32 arg) {
}

void WiFiPlazaEventEffect_UpdatePhase3(WiFiPlazaEventEffect *effect, u32 tick) {
    if (tick % 14 != 0) {
        return;
    }
    {
        u32 i;
        const u8 *id;
        int result;

        id = gWiFiPlazaEventPhase3PropIds;
        i = 0;
        do {
            int show;
            int active = 0;

            if ((i & 3) == gWiFiPlazaEventPhase3PropGroups[effect->step][0]) {
                show = 1;
                active = 1;
            } else if ((i & 3) == gWiFiPlazaEventPhase3PropGroups[effect->step][1]) {
                show = 0;
                active = 1;
            }
            if (active == 1) {
                if (i < 0x11) {
                    result = WiFiPlazaModels_ShowEffectProp(effect->models, *id, show, 0x1000);
                    if (result != 1) {
                        GF_AssertFail();
                    }
                } else {
                    result = 0;
                }
            } else {
                result = 1;
            }
            id++;
            i++;
        } while (result == 1);
    }
    effect->step = (effect->step + 1) & 3;
}

void WiFiPlazaEventEffect_UpdatePhase4(WiFiPlazaEventEffect *effect, u32 tick) {
    WiFiPlazaEventModelList list;
    u32 kind;
    u32 i;

    if (tick % 6 != 0) {
        return;
    }
    kind = gWiFiPlazaEventPhase4Steps[effect->step * 2];
    WiFiPlazaEvent_GetModelList(gWiFiPlazaEventPhase4HideSteps[effect->step * 2], &list);
    for (i = 0; i < list.count; i++) {
        if (WiFiPlazaModels_ShowEffectProp(effect->models, list.ids[i], FALSE, 0x1000) != 1) {
            GF_AssertFail();
        }
    }
    WiFiPlazaEvent_GetModelList(kind, &list);
    for (i = 0; i < list.count; i++) {
        if (WiFiPlazaModels_ShowEffectProp(effect->models, list.ids[i], TRUE, 0x1000) != 1) {
            GF_AssertFail();
        }
    }
    effect->step = (effect->step + 1) % 7;
}

void WiFiPlazaEventEffect_UpdatePhase5(WiFiPlazaEventEffect *effect, u32 value) {
    u32 step = effect->step;

    if (step < 3 && gWiFiPlazaEventPhase5Thresholds[step * 2] < value) {
        WiFiPlazaModels_TriggerStageShows(effect->models, gWiFiPlazaEventPhase5AnimSpeeds[step * 2]);
        WiFiPlazaModels_TriggerStageBursts(effect->models, gWiFiPlazaEventPhase5AnimSpeeds[effect->step * 2]);
        PlaySE(SEQ_SE_PL_140_2);
        effect->step++;
    }
}

static void WiFiPlazaEventEffect_StartPhase(WiFiPlazaEventEffect *effect, u32 index, u32 arg) {
    GF_ASSERT(index < 6);
    if (index < 6) {
        u8 *step = (u8 *)&effect->step;

        step[0] = 0;
        step[1] = 0;
        step[2] = 0;
        step[3] = 0;
        gWiFiPlazaEventEffectPhaseStarts[index](effect, arg);
        effect->effect = index;
    }
}

void WiFiPlazaEventEffect_StartHideProps(WiFiPlazaEventEffect *effect) {
    WiFiPlazaModels_ShowEffectProps(effect->models, FALSE, 0x1000);
}

void WiFiPlazaEventEffect_StartPhase2(WiFiPlazaEventEffect *effect) {
    WiFiPlazaModels_ShowEffectProps(effect->models, TRUE, 0x1000);
}

void WiFiPlazaEventEffect_StartPhase3(WiFiPlazaEventEffect *effect) {
    WiFiPlazaModels_ShowEffectProps(effect->models, FALSE, 0x1000);
}

void WiFiPlazaEventEffect_StartPhase4(WiFiPlazaEventEffect *effect) {
    WiFiPlazaModels_ShowEffectProps(effect->models, FALSE, 0x1000);
}

void WiFiPlazaEventEffect_StartPhase5(WiFiPlazaEventEffect *effect) {
    WiFiPlazaEvent_StopLoopSE(effect);
    WiFiPlazaModels_ShowEffectProps(effect->models, FALSE, 0x1000);
    WiFiPlazaModels_SetEffectActive(effect->models, FALSE);
}

static void WiFiPlazaEvent_GetModelList(u32 kind, WiFiPlazaEventModelList *list) {
    switch (kind) {
    case 0:
        list->count = 6;
        list->ids = gWiFiPlazaEventModelIds0;
        break;
    case 1:
        list->count = 2;
        list->ids = gWiFiPlazaEventModelIds1;
        break;
    case 2:
        list->count = 2;
        list->ids = gWiFiPlazaEventModelIds2;
        break;
    case 3:
        list->count = 1;
        list->ids = gWiFiPlazaEventModelIds3;
        break;
    case 4:
        list->count = 1;
        list->ids = gWiFiPlazaEventModelIds4;
        break;
    case 5:
        list->count = 2;
        list->ids = gWiFiPlazaEventModelIds5;
        break;
    case 6:
        list->count = 3;
        list->ids = gWiFiPlazaEventModelIds6;
        break;
    default:
        GF_AssertFail();
        break;
    }
}

static void WiFiPlazaEvent_StartLoopSE(WiFiPlazaEventEffect *effect) {
    if (effect->seLooping == FALSE) {
        PlaySE(SEQ_SE_PL_140_3);
        effect->seLooping = TRUE;
    }
}

static void WiFiPlazaEvent_StopLoopSE(WiFiPlazaEventEffect *effect) {
    if (effect->seLooping != FALSE) {
        StopSE(SEQ_SE_PL_140_3, 0);
        effect->seLooping = FALSE;
    }
}

// Player state handler: talking to the special object (map tag 97), which turns to the player; message bank 1
// with a yes/no menu.
BOOL WiFiPlazaTalk_Survey(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    u32 *nextStep = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *special = WiFiPlazaObj_GetSpecial(objManager);
    WiFiPlazaObj *player = WiFiPlazaObj_GetOwnPlayer(objManager);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_SURVEY_GUIDE_GREET: {
        String *message;
        u32 direction;

        nextStep = WiFiPlazaPlayerState_AllocWork(state, 4);
        PlaySE(SEQ_SE_DP_SELECT);
        direction = Field2D_GetOppositeDirection(WiFiPlazaObj_GetParam(player, 6));
        WiFiPlazaObj_SetHandler(objManager, special, WIFI_PLAZA_OBJ_HANDLER_IDLE);
        WiFiPlazaObj_SetAvatarMode(special, 0);
        WiFiPlazaObj_SetDirection(special, direction);
        if (WiFiPlazaWork_IsClosing(lobby) != 0) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00003);
            *nextStep = WIFI_PLAZA_SURVEY_GUIDE_END;
            WiFiPlazaRoom_PrintMessage(room, message);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_GUIDE_WAIT_MESSAGE);
        } else if (WiFiPlazaWork_IsLightingStarted(lobby) == TRUE) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00088);
            *nextStep = WIFI_PLAZA_SURVEY_GUIDE_END;
            WiFiPlazaRoom_PrintMessage(room, message);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_GUIDE_WAIT_MESSAGE);
        } else if (WiFiPlazaWork_HasAnsweredSurvey(lobby) == 1) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00087);
            *nextStep = WIFI_PLAZA_SURVEY_GUIDE_END;
            WiFiPlazaRoom_PrintMessage(room, message);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_GUIDE_WAIT_MESSAGE);
        } else {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00081);
            *nextStep = WIFI_PLAZA_SURVEY_GUIDE_OPEN_YES_NO;
            WiFiPlazaRoom_PrintMessage(room, message);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_GUIDE_WAIT_MESSAGE);
        }
        break;
    }
    case WIFI_PLAZA_SURVEY_GUIDE_OPEN_YES_NO:
        WiFiPlazaRoom_OpenYesNoMenu(room);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_GUIDE_HANDLE_YES_NO);
        break;
    case WIFI_PLAZA_SURVEY_GUIDE_HANDLE_YES_NO:
        switch (WiFiPlazaRoom_GetYesNoResult(room)) {
        case WIFI_PLAZA_YES_NO_YES:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_GUIDE_EXPLAIN);
            WiFiPlazaRoom_CloseYesNoMenu(room);
            break;
        case WIFI_PLAZA_YES_NO_NO:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_GUIDE_END);
            WiFiPlazaRoom_CloseYesNoMenu(room);
            break;
        case WIFI_PLAZA_YES_NO_OPEN:
            break;
        }
        break;
    case WIFI_PLAZA_SURVEY_GUIDE_EXPLAIN: {
        String *message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00085);

        *nextStep = WIFI_PLAZA_SURVEY_GUIDE_END;
        WiFiPlazaRoom_PrintMessage(room, message);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_GUIDE_WAIT_MESSAGE);
        break;
    }
    case WIFI_PLAZA_SURVEY_GUIDE_END:
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaRoom_CloseYesNoMenu(room);
        WiFiPlazaObj_SetHandler(objManager, player, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaObj_SetAvatarMode(special, 1);
        WiFiPlazaObj_RunCommand(objManager, special, 0, DIR_SOUTH);
        return TRUE;
    case WIFI_PLAZA_SURVEY_GUIDE_WAIT_MESSAGE:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, *nextStep);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

static WiFiPlazaEventMovers *WiFiPlazaEventMovers_Create(WiFiPlazaRoomWork *room, enum HeapID heapID) {
    WiFiPlazaEventMovers *work = Heap_Alloc(heapID, sizeof(WiFiPlazaEventMovers));
    int i;
    const u8 *tag;
    u16 pos[2];

    memset(work, 0, sizeof(WiFiPlazaEventMovers));
    work->room = room;
    work->lobby = WiFiPlazaRoom_GetLobby(room);
    work->models = WiFiPlazaRoom_GetModels(room);
    work->map = WiFiPlazaRoom_GetMap(room);
    work->baseX = WiFiPlazaMap_GetWidth() + 6;
    work->range = -8 - work->baseX;
    work->baseX <<= 16;
    work->range <<= 16;
    tag = gWiFiPlazaMoverMarkerTags;
    i = 0;
    for (; i < 3; i++, tag++) {
        WiFiPlazaMap_FindTileTag(work->map, *tag, &pos[1], &pos[0], 0);
        work->points[i].x = pos[1] << 4;
        work->points[i].z = pos[0] << 4;
    }
    for (i = 0; i < 9; i++) {
        WiFiPlazaModels_SetMoverX(work->models, i, work->baseX);
        if (WiFiPlazaWork_GetGroupState(work->lobby, i) == 1) {
            WiFiPlazaEventMovers_UpdateX(work, i);
            WiFiPlazaEvent_UpdateMoverMarkers(work, i);
        }
    }
    return work;
}

static void WiFiPlazaEventMovers_Delete(WiFiPlazaEventMovers *work) {
    Heap_Free(work);
}

static void WiFiPlazaEventMovers_Update(WiFiPlazaEventMovers *work) {
    int selected = WiFiPlazaWork_FindGroupPlace(work->lobby, WiFiPlazaWork_GetOwnSlot(work->lobby));
    u32 column;
    int present;
    int isOwnIndex;
    int i;

    if (selected == -1) {
        present = FALSE;
    } else {
        WiFiPlaza_SplitGroupPlace(selected, (u32 *)&selected, &column);
        present = TRUE;
    }
    for (i = 0; i < 9; i++) {
        if (WiFiPlazaWork_GetGroupState(work->lobby, i) == 1) {
            WiFiPlazaModels_SetMoverVisible(work->models, i, TRUE);
            WiFiPlazaEventMovers_UpdateX(work, i);
            WiFiPlazaEvent_UpdateMoverMarkers(work, i);
            if (present) {
                if (selected == i) {
                    isOwnIndex = TRUE;
                } else {
                    isOwnIndex = FALSE;
                }
            }
            WiFiPlazaEventMovers_StartActions(work, i, present, isOwnIndex, column);
        } else {
            WiFiPlazaModels_SetMoverVisible(work->models, i, FALSE);
        }
    }
    for (i = 0; i < 9; i++) {
        WiFiPlazaEventWobble *wobble = &work->wobbles[i];

        work->states[i][0] = WiFiPlazaModels_IsMoverAnim1Playing(work->models, i);
        work->states[i][1] = WiFiPlazaEventWobble_Update(wobble, work->models, i);
        work->states[i][2] = WiFiPlazaModels_IsMoverAnim3Playing(work->models, i);
    }
}

static u8 WiFiPlazaEventMovers_GetState(WiFiPlazaEventMovers *work, u32 index, int field) {
    u8 *entry;

    if (index >= 9) {
        GF_AssertFail();
    }
    if (field >= 3) {
        GF_AssertFail();
    }
    entry = (u8 *)work + index * sizeof(work->states[0]);
    entry += field;
    return entry[offsetof(WiFiPlazaEventMovers, states)];
}

static void WiFiPlazaEventMovers_UpdateX(WiFiPlazaEventMovers *work, int index) {
    int value = WiFiPlazaWork_GetGroupCounter(work->lobby, index);
    int max = WiFiPlazaWork_GetGroupDuration(work->lobby);
    fx32 ratio = FX_Div(work->range, 8192);
    float maxF;
    float valueF;
    fx32 scaled;
    fx32 x;

    // fx32 conversions of int values, rounded away from zero
    if (max > 0) {
        maxF = 0.5f + (float)(max << 12);
    } else {
        maxF = (float)(max << 12) - 0.5f;
    }
    if (value > 0) {
        valueF = 0.5f + (float)(value << 12);
    } else {
        valueF = (float)(value << 12) - 0.5f;
    }
    scaled = (fx32)(((s64)(int)valueF * ratio + 2048) >> 12);
    x = FX_Div(scaled, (int)maxF);
    x = (fx32)((((s64)x << 13) + 2048) >> 12);
    x += work->baseX;
    WiFiPlazaModels_SetMoverX(work->models, index, x);
}

static void WiFiPlazaEventMovers_StartActions(WiFiPlazaEventMovers *work, u32 index, BOOL hasOwnSelection, int isOwnIndex, int ownColumn) {
    int type = WiFiPlazaWork_GetScheduleValue2(work->lobby);
    int i;

    WiFiPlazaWork_GetScheduleValue3(work->lobby);
    for (i = 0; i < 3; i++) {
        int active = WiFiPlazaWork_GetGroupActionRequest(work->lobby, index * 3 + i);
        int isOwnColumn = FALSE;

        if (isOwnIndex == TRUE && i == ownColumn) {
            isOwnColumn = TRUE;
        }
        if (active == 1) {
            switch (i) {
            case 0:
                if (WiFiPlazaModels_StartMoverAnim1(work->models, index) == 1) {
                    if (index % 3 == 2) {
                        WiFiPlazaEventMovers_PlayCry(hasOwnSelection, isOwnIndex, isOwnColumn, gWiFiPlazaMoverSounds[type][1]);
                        if (gWiFiPlazaMoverSounds[type][4] != 0xFFFE) {
                            WiFiPlazaEventMovers_PlaySE(hasOwnSelection, isOwnIndex, isOwnColumn, gWiFiPlazaMoverSounds[type][4]);
                        }
                    } else {
                        WiFiPlazaEventMovers_PlayCry(hasOwnSelection, isOwnIndex, isOwnColumn, gWiFiPlazaMoverSounds[type][0]);
                        if (gWiFiPlazaMoverSounds[type][2] != 0xFFFE) {
                            WiFiPlazaEventMovers_PlaySE(hasOwnSelection, isOwnIndex, isOwnColumn, gWiFiPlazaMoverSounds[type][2]);
                        }
                    }
                    WiFiPlazaWork_ClearGroupActionRequest(work->lobby, index * 3 + i);
                }
                break;
            case 1:
                if (WiFiPlazaEventWobble_Start(&work->wobbles[index], index % 3 == 2, work->models, index) == 1) {
                    WiFiPlazaWork_ClearGroupActionRequest(work->lobby, index * 3 + i);
                    WiFiPlazaEventMovers_PlaySE(hasOwnSelection, isOwnIndex, isOwnColumn, SEQ_SE_PL_FLOAT13);
                }
                break;
            case 2:
                if (WiFiPlazaModels_StartMoverAnim3(work->models, index) == 1) {
                    WiFiPlazaWork_ClearGroupActionRequest(work->lobby, index * 3 + i);
                    if (index % 3 == 2) {
                        WiFiPlazaEventMovers_PlaySE(hasOwnSelection, isOwnIndex, isOwnColumn, gWiFiPlazaMoverSounds[type][5]);
                    } else {
                        WiFiPlazaEventMovers_PlaySE(hasOwnSelection, isOwnIndex, isOwnColumn, gWiFiPlazaMoverSounds[type][3]);
                    }
                }
                break;
            }
        }
    }
}

// While the own player has a selection, only the own index is heard (on the player channel for the own column).
static void WiFiPlazaEventMovers_PlaySE(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 seqNo) {
    if (hasOwnSelection != FALSE) {
        if (isOwnIndex == FALSE) {
            return;
        }
        if (isOwnColumn) {
            PlaySEOnPlayer(seqNo, 5);
            return;
        } else {
            PlaySE(seqNo);
            return;
        }
    }
    PlaySE(seqNo);
}

static void WiFiPlazaEventMovers_PlayCry(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 species) {
    if (hasOwnSelection != FALSE) {
        if (isOwnIndex == FALSE) {
            return;
        }
        PlayCry(species, 0);
        return;
    }
    PlayCry(species, 0);
}

static BOOL WiFiPlazaEventWobble_Start(WiFiPlazaEventWobble *wobble, int kind, WiFiPlazaModels *models, int index) {
    if ((u32)kind >= 2) {
        GF_AssertFail();
    }
    if (wobble->active == TRUE) {
        return FALSE;
    }
    wobble->kind = kind;
    wobble->active = TRUE;
    gWiFiPlazaEventWobbleInits[kind](wobble, models, index);
    return TRUE;
}

static BOOL WiFiPlazaEventWobble_Update(WiFiPlazaEventWobble *wobble, WiFiPlazaModels *models, int index) {
    if (wobble->active != FALSE) {
        if (gWiFiPlazaEventWobbleSteps[wobble->kind](wobble, models, index) == TRUE) {
            u8 *p = (u8 *)wobble;

            p[0] = 0;
            p[1] = 0;
            p[2] = 0;
            p[3] = 0;
            p[4] = 0;
            p[5] = 0;
            p[6] = 0;
            p[7] = 0;
            p[8] = 0;
            p[9] = 0;
            p[10] = 0;
            p[11] = 0;
            p[12] = 0;
            p[13] = 0;
        }
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaEventWobble_InitSmall(WiFiPlazaEventWobble *wobble) {
    wobble->frame = 0;
    wobble->duration = 8;
    wobble->maxAngle = GF_DegreeToSinCosIdx(5);
    wobble->maxHeight = 2;
    wobble->repeats = 4;
}

void WiFiPlazaEventWobble_InitLarge(WiFiPlazaEventWobble *wobble) {
    wobble->frame = 0;
    wobble->duration = 16;
    wobble->maxAngle = GF_DegreeToSinCosIdx(10);
    wobble->maxHeight = 4;
    wobble->repeats = 2;
}

BOOL WiFiPlazaEventWobble_Step(WiFiPlazaEventWobble *wobble, WiFiPlazaModels *models, int index) {
    VecFx32 offset = { 0, 0, 0 };
    BOOL done = FALSE;
    int next = wobble->frame + 1;
    u16 angle;
    int amplitude;
    float rounded;
    s64 product;
    s16 sine;

    if (next < wobble->duration) {
        wobble->frame = next;
    } else {
        wobble->frame = 0;
        if (wobble->repeats - 1 > 0) {
            wobble->repeats--;
        } else {
            done = TRUE;
        }
    }
    angle = (u16)((wobble->frame * 0xFFFF) / wobble->duration);
    if (wobble->maxAngle != 0) {
        rounded = (float)(wobble->maxAngle << 12) + 0.5f;
    } else {
        rounded = (float)(wobble->maxAngle << 12) - 0.5f;
    }
    amplitude = (int)rounded;
    sine = FX_SinCosTable_[(angle >> 4) * 2];
    product = (s64)sine * amplitude;
    amplitude = (int)((product + 0x800) >> 12);
    angle = (u32)amplitude >> 12;
    if (wobble->maxHeight > 0) {
        rounded = (float)(wobble->maxHeight << 12) + 0.5f;
    } else {
        rounded = (float)(wobble->maxHeight << 12) - 0.5f;
    }
    amplitude = (int)rounded;
    product = (s64)sine * amplitude;
    offset.y = (fx32)((product + 0x800) >> 12);
    WiFiPlazaModels_SetMoverAngle(models, index, angle);
    WiFiPlazaModels_SetMoverOffset(models, index, &offset);
    return done;
}

// For each of the three sides of mover `index` (lobby slot index * 3 + side), reports to the lobby every marker
// from the count the lobby already holds on that the front of the side has passed (movers travel towards -x:
// front x below the marker x plus one cell).
static void WiFiPlazaEvent_UpdateMoverMarkers(WiFiPlazaEventMovers *work, int index) {
    struct {
        int index, base;
    } input;

    input.index = index;
    int i = 0, j;
    int twice = (s32)((u32)index * 2);
    input.base = (s32)((u32)input.index + (u32)twice);
    VecFx32 front;
    for (i = 0; i < 3; i++) {
        j = WiFiPlazaWork_GetGroupMarkerCount(work->lobby, (s32)((u32)i + (u32)input.base));
        WiFiPlazaModels_GetMoverFrontPos(work->models, input.index, i, &front);
        for (; j < 3; j++) {
            if ((work->points[j].x + 16) * FX32_ONE > front.x) {
                WiFiPlazaWork_SetGroupMarkerPassed(work->lobby, (s32)((u32)i + (u32)input.base), j);
            }
        }
    }
}
