#include "wifi_plaza_event.h"

#include <string.h>

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "math_util.h"
#include "overlay_42.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "wifi_plaza_map.h"
#include "wifi_plaza_room.h"

// sound_playback.c (no header declares it)
BOOL PlaySEOnPlayer(u16 seqNo, int playerNo);

// rodata of this overlay (asm/to90_extra2491_anchor.s)
extern const u16 sWiFiPlazaLight1Colors[];
extern const u16 sWiFiPlazaLightColorSets[][5];
extern void (*const ov49_0226A84C[])(UnkStruct_ov49_02268A0C *effect, int arg);
extern void (*const ov49_0226A87C[])(UnkStruct_ov49_02268A0C *effect, u32 arg);
extern const u8 ov49_0226A8B4[];
extern const u32 ov49_0226A894[][2];
extern const u8 ov49_0226A83C[];
extern const u8 ov49_0226A83D[];
extern const u32 ov49_0226A864[];
extern const u32 ov49_0226A868[];
extern const u8 ov49_0226A81C[];
extern const u8 ov49_0226A820[];
extern const u8 ov49_0226A824[];
extern const u8 ov49_0226A828[];
extern const u8 ov49_0226A82C[];
extern const u8 ov49_0226A830[];
extern const u8 ov49_0226A834[];
extern const u8 ov49_0226A8C8[];
extern const u16 ov49_0226A8DC[][6];
extern BOOL (*const ov49_0226A8CC[])(WiFiPlazaEventWobble *wobble, void *models, int index);
extern BOOL (*const ov49_0226A8D4[])(WiFiPlazaEventWobble *wobble, void *models, int index);

void WiFiPlazaLight_Delete(WiFiPlazaLight *light) {
    Heap_Free(light);
}

void WiFiPlazaLight_Update(WiFiPlazaLight *light) {
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
void WiFiPlazaLight_FadeToColorSet(WiFiPlazaLight *light, int set) {
    int i;
    const u16 *colors;

    light->lightsFrom[0][0] = light->lights[0][0];
    light->lightsFrom[0][1] = light->lights[0][1];
    light->lightsFrom[0][2] = light->lights[0][2];
    light->lightsFrom[0][3] = light->lights[0][3];
    light->lightTimers[0] = 0;
    colors = sWiFiPlazaLightColorSets[set];
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

void WiFiPlazaLight_FadeLight1(WiFiPlazaLight *light, int set) {
    light->lightsFrom[1][0] = light->lights[1][0];
    light->lightsFrom[1][1] = light->lights[1][1];
    light->lightsFrom[1][2] = light->lights[1][2];
    light->lightsFrom[1][3] = light->lights[1][3];
    light->lightTimers[1] = 0;
    WiFiPlazaLight_UnpackColor((s8 *)light->lightsTo[1], &sWiFiPlazaLight1Colors[set]);
}

void ov49_02268620(WiFiPlazaLight *light, int set) {
}

void WiFiPlazaLight_PackColor(const s8 *rgb, u16 *color) {
    *color = rgb[0] | (rgb[1] << 5) | (rgb[2] << 10);
}

void WiFiPlazaLight_UnpackColor(s8 *rgb, const u16 *color) {
    rgb[0] = color[0] & 0x1F;
    rgb[1] = (color[0] & (0x1F << 5)) >> 5;
    rgb[2] = (color[0] & (0x1F << 10)) >> 10;
}

void WiFiPlazaLight_LerpColor(const s8 *start, const s8 *end, s8 *dest, s32 step, s32 total) {
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
    NNS_G3dGlbMaterialColorDiffAmb(color, color2, 0);
    WiFiPlazaLight_PackColor((s8 *)light->materials[2], &color);
    WiFiPlazaLight_PackColor((s8 *)light->materials[3], &color2);
    NNS_G3dGlbMaterialColorSpecEmi(color, color2, 0);
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
    colorSet = WiFiPlazaEvent_GetColorSet(ov45_0222A3BC(event->lobby));
    light1Set = WiFiPlazaEvent_GetLight1Set(ov45_0222A3D4(event->lobby));
    thirdSet = ov49_022689D4(ov45_0222A3EC(event->lobby));
    event->light = WiFiPlazaLight_Create(heapID, colorSet, light1Set, thirdSet);
    event->unk14 = ov49_02268FAC(room, heapID);
    event->unk18 = ov49_02268A0C(event->lobby, event->models, heapID);
    WiFiPlazaObj_SetLightSet(event->objManager, colorSet);
    mode = ov45_0222A35C(event->lobby);
    ov45_0222A324(event->lobby);
    flagB = ov45_0222A374(event->lobby);
    flagC = ov45_0222A3A0(event->lobby);
    flagD = ov45_0222A330(event->lobby);
    ov45_0222A394(event->lobby);
    if (mode == 2) {
        WiFiPlazaModels_EnableKind1To4Lights(event->models);
    }
    if ((mode != 1) && (mode == 0) && (flagC == TRUE)) {
        WiFiPlazaModels_SetPairAnim(event->models, 3);
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
    ov49_02268A6C(event->unk18);
    ov49_02269090(event->unk14);
    WiFiPlazaLight_Delete(event->light);
    Heap_Free(event);
}

// Reacts to the lobby's change flags.
void WiFiPlazaEvent_Update(WiFiPlazaEvent *event) {
    int set;

    if (ov45_0222A288(event->lobby, 1)) {
        set = WiFiPlazaEvent_GetColorSet(ov45_0222A3BC(event->lobby));
        WiFiPlazaLight_FadeToColorSet(event->light, set);
        WiFiPlazaObj_FadeToLightSet(event->objManager, set);
    }
    if (ov45_0222A288(event->lobby, 2)) {
        set = WiFiPlazaEvent_GetLight1Set(ov45_0222A3D4(event->lobby));
        WiFiPlazaLight_FadeLight1(event->light, set);
    }
    if (ov45_0222A288(event->lobby, 3)) {
        set = ov49_022689D4(ov45_0222A3EC(event->lobby));
        ov49_02268620(event->light, set);
    }
    if (ov45_0222A3A0(event->lobby) == TRUE && ov45_0222A35C(event->lobby) == 0 && WiFiPlazaModels_GetPairAnimState(event->models) == 0) {
        WiFiPlazaModels_SetPairAnim(event->models, 1);
    }
    if (ov45_0222A288(event->lobby, 5) && ov45_0222A35C(event->lobby) == 1) {
        WiFiPlazaModels_EnableKind1To4Lights(event->models);
        WiFiPlazaModels_SetPairAnim(event->models, 2);
    }
    if (ov45_0222A288(event->lobby, 6) && ov45_0222A374(event->lobby) == TRUE) {
        WiFiPlazaModels_StartBaseAnim4(event->models);
    }
    if (ov45_0222A330(event->lobby) == 1) {
        WiFiPlazaModels_StopBaseAnim4(event->models);
        WiFiPlazaEvent_TurnOffLight2(event);
    }
    WiFiPlazaLight_Update(event->light);
    ov49_02269098(event->unk14);
    ov49_02268A7C(event->unk18);
}

u8 ov49_02268968(WiFiPlazaEvent *event, u32 index, int field) {
    return ov49_02269154(event->unk14, index, field);
}

// Lobby value 1-6 -> colour set 0-5
int WiFiPlazaEvent_GetColorSet(int value) {
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
int WiFiPlazaEvent_GetLight1Set(int value) {
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

int ov49_022689D4(int value) {
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

void WiFiPlazaEvent_TurnOffLight2(WiFiPlazaEvent *event) {
    NNS_G3dGlbLightColor(GX_LIGHTID_2, 0);
}

#pragma force_active on

UnkStruct_ov49_02268A0C *ov49_02268A0C(WiFiPlazaWork *lobby, void *models, enum HeapID heapID) {
    UnkStruct_ov49_02268A0C *effect = Heap_Alloc(heapID, sizeof(UnkStruct_ov49_02268A0C));
    u32 effectId;
    u8 *cursor = (u8 *)effect;
    u32 n = sizeof(UnkStruct_ov49_02268A0C);

    do {
        *cursor++ = 0;
    } while (--n);
    effect->lobby = lobby;
    effect->models = models;
    effectId = ov45_0222B1DC(lobby);
    effect->activeCount = ov45_0222A35C(lobby);
    if (effect->activeCount && effectId) {
        WiFiPlazaModels_SetEffectActive(models, 1);
        WiFiPlazaEvent_StartLoopSE(effect);
        ov49_02268C74(effect, effectId, ov45_0222B1EC(lobby));
    }
    return effect;
}

#pragma force_active reset

void ov49_02268A6C(UnkStruct_ov49_02268A0C *effect) {
    WiFiPlazaEvent_StopLoopSE(effect);
    Heap_Free(effect);
}

void ov49_02268A7C(UnkStruct_ov49_02268A0C *effect) {
    int effectId;
    int arg;
    int count;

    effectId = ov45_0222B1DC(effect->lobby);
    arg = ov45_0222B1EC(effect->lobby);
    count = ov45_0222A35C(effect->lobby);
    if (effect->activeCount != count) {
        effect->activeCount = count;
        if (count == 1) {
            WiFiPlazaModels_SetEffectActive(effect->models, 1);
            ov49_02268C74(effect, effectId, arg);
            WiFiPlazaEvent_StartLoopSE(effect);
        }
    }
    if (count != 0) {
        if (effect->effect != effectId) {
            ov49_02268C74(effect, effectId, arg);
        }
        ov49_02268ADC(effect, effectId, arg);
    }
}

void ov49_02268ADC(UnkStruct_ov49_02268A0C *effect, int index, int arg) {
    if ((u32)index >= 6) {
        GF_AssertFail();
    }
    if ((u32)index < 6) {
        ov49_0226A84C[index](effect, arg);
    }
}

void ov49_02268B04(UnkStruct_ov49_02268A0C *effect, u32 arg) {
}

void ov49_02268B08(UnkStruct_ov49_02268A0C *effect, u32 arg) {
}

void ov49_02268B0C(UnkStruct_ov49_02268A0C *effect, u32 tick) {
    if (tick % 14 != 0) {
        return;
    }
    {
        u32 i;
        const u8 *id;
        int result;

        id = ov49_0226A8B4;
        i = 0;
        do {
            int show;
            int active = 0;

            if ((i & 3) == ov49_0226A894[effect->step][0]) {
                show = 1;
                active = 1;
            } else if ((i & 3) == ov49_0226A894[effect->step][1]) {
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

void ov49_02268B90(UnkStruct_ov49_02268A0C *effect, u32 tick) {
    WiFiPlazaEventModelList list;
    u32 kind;
    u32 i;

    if (tick % 6 != 0) {
        return;
    }
    kind = ov49_0226A83C[effect->step * 2];
    WiFiPlazaEvent_GetModelList(ov49_0226A83D[effect->step * 2], &list);
    for (i = 0; i < list.count; i++) {
        if (WiFiPlazaModels_ShowEffectProp(effect->models, list.ids[i], 0, 0x1000) != 1) {
            GF_AssertFail();
        }
    }
    WiFiPlazaEvent_GetModelList(kind, &list);
    for (i = 0; i < list.count; i++) {
        if (WiFiPlazaModels_ShowEffectProp(effect->models, list.ids[i], 1, 0x1000) != 1) {
            GF_AssertFail();
        }
    }
    effect->step = (effect->step + 1) % 7;
}

void ov49_02268C2C(UnkStruct_ov49_02268A0C *effect, u32 value) {
    u32 step = effect->step;

    if (step < 3 && ov49_0226A864[step * 2] < value) {
        WiFiPlazaModels_TriggerStageShows(effect->models, ov49_0226A868[step * 2]);
        WiFiPlazaModels_TriggerStageBursts(effect->models, ov49_0226A868[effect->step * 2]);
        PlaySE(0x5B4);
        effect->step++;
    }
}

void ov49_02268C74(UnkStruct_ov49_02268A0C *effect, u32 index, u32 arg) {
    GF_ASSERT(index < 6);
    if (index < 6) {
        u8 *step = (u8 *)&effect->step;

        step[0] = 0;
        step[1] = 0;
        step[2] = 0;
        step[3] = 0;
        ov49_0226A87C[index](effect, arg);
        effect->effect = index;
    }
}

void ov49_02268CAC(UnkStruct_ov49_02268A0C *effect) {
    WiFiPlazaModels_ShowEffectProps(effect->models, 0, 0x1000);
}

void ov49_02268CBC(UnkStruct_ov49_02268A0C *effect) {
    WiFiPlazaModels_ShowEffectProps(effect->models, 1, 0x1000);
}

void ov49_02268CCC(UnkStruct_ov49_02268A0C *effect) {
    WiFiPlazaModels_ShowEffectProps(effect->models, 0, 0x1000);
}

void ov49_02268CDC(UnkStruct_ov49_02268A0C *effect) {
    WiFiPlazaModels_ShowEffectProps(effect->models, 0, 0x1000);
}

void ov49_02268CEC(UnkStruct_ov49_02268A0C *effect) {
    WiFiPlazaEvent_StopLoopSE(effect);
    WiFiPlazaModels_ShowEffectProps(effect->models, 0, 0x1000);
    WiFiPlazaModels_SetEffectActive(effect->models, 0);
}

void WiFiPlazaEvent_GetModelList(u32 kind, WiFiPlazaEventModelList *list) {
    switch (kind) {
    case 0:
        list->count = 6;
        list->ids = ov49_0226A834;
        break;
    case 1:
        list->count = 2;
        list->ids = ov49_0226A82C;
        break;
    case 2:
        list->count = 2;
        list->ids = ov49_0226A824;
        break;
    case 3:
        list->count = 1;
        list->ids = ov49_0226A81C;
        break;
    case 4:
        list->count = 1;
        list->ids = ov49_0226A820;
        break;
    case 5:
        list->count = 2;
        list->ids = ov49_0226A828;
        break;
    case 6:
        list->count = 3;
        list->ids = ov49_0226A830;
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void WiFiPlazaEvent_StartLoopSE(UnkStruct_ov49_02268A0C *effect) {
    if (effect->seLooping == FALSE) {
        PlaySE(0x5B5);
        effect->seLooping = TRUE;
    }
}

void WiFiPlazaEvent_StopLoopSE(UnkStruct_ov49_02268A0C *effect) {
    if (effect->seLooping != FALSE) {
        StopSE(0x5B5, 0);
        effect->seLooping = FALSE;
    }
}

// Player state handler: talking to the special object (map tag 97), which turns to the player; message bank 1
// with a yes/no menu.
BOOL ov49_02268DCC(void *state, WiFiPlazaRoomWork *room) {
    u32 *nextStep = ov49_0225EF84(state);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    void *objManager = WiFiPlazaRoom_GetObjManager(room);
    void *special = WiFiPlazaObj_GetSpecial(objManager);
    void *player = WiFiPlazaObj_GetOwnPlayer(objManager);

    switch (ov49_0225EF88(state)) {
    case 0: {
        String *message;
        u32 direction;

        nextStep = ov49_0225EF40(state, 4);
        PlaySE(0x5DC);
        direction = ov42_022282A4(WiFiPlazaObj_GetParam(player, 6));
        WiFiPlazaObj_SetHandler(objManager, special, 0);
        ov49_02259130(special, 0);
        WiFiPlazaObj_SetDirection(special, direction);
        if (ov45_0222A330(lobby) != 0) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 3);
            *nextStep = 4;
            WiFiPlazaRoom_PrintMessage(room, message);
            ov49_0225EF8C(state, 5);
        } else if (ov45_0222A3A0(lobby) == TRUE) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x58);
            *nextStep = 4;
            WiFiPlazaRoom_PrintMessage(room, message);
            ov49_0225EF8C(state, 5);
        } else if (ov45_0222A2E0(lobby) == 1) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x57);
            *nextStep = 4;
            WiFiPlazaRoom_PrintMessage(room, message);
            ov49_0225EF8C(state, 5);
        } else {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x51);
            *nextStep = 1;
            WiFiPlazaRoom_PrintMessage(room, message);
            ov49_0225EF8C(state, 5);
        }
        break;
    }
    case 1:
        WiFiPlazaRoom_OpenYesNoMenu(room);
        ov49_0225EF8C(state, 2);
        break;
    case 2:
        switch (WiFiPlazaRoom_GetYesNoResult(room)) {
        case 0:
            ov49_0225EF8C(state, 3);
            WiFiPlazaRoom_CloseYesNoMenu(room);
            break;
        case 1:
            ov49_0225EF8C(state, 4);
            WiFiPlazaRoom_CloseYesNoMenu(room);
            break;
        case 2:
            break;
        }
        break;
    case 3: {
        String *message = WiFiPlazaRoom_GetMsgString(room, 1, 0x55);

        *nextStep = 4;
        WiFiPlazaRoom_PrintMessage(room, message);
        ov49_0225EF8C(state, 5);
        break;
    }
    case 4:
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaRoom_CloseYesNoMenu(room);
        WiFiPlazaObj_SetHandler(objManager, player, 1);
        ov49_0225EF68(state);
        ov49_02259130(special, 1);
        ov49_02258EAC(objManager, special, 0, 1);
        return TRUE;
    case 5:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            ov49_0225EF8C(state, *nextStep);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

UnkStruct_ov49_02268FAC *ov49_02268FAC(WiFiPlazaRoomWork *room, enum HeapID heapID) {
    UnkStruct_ov49_02268FAC *work = Heap_Alloc(heapID, sizeof(UnkStruct_ov49_02268FAC));
    int i;
    const u8 *tag;
    u16 pos[2];

    memset(work, 0, sizeof(UnkStruct_ov49_02268FAC));
    work->room = room;
    work->lobby = WiFiPlazaRoom_GetLobby(room);
    work->models = WiFiPlazaRoom_GetModels(room);
    work->map = WiFiPlazaRoom_GetMap(room);
    work->baseX = WiFiPlazaMap_GetWidth() + 6;
    work->range = -8 - work->baseX;
    work->baseX <<= 16;
    work->range <<= 16;
    tag = ov49_0226A8C8;
    i = 0;
    for (; i < 3; i++, tag++) {
        WiFiPlazaMap_FindTileTag(work->map, *tag, &pos[1], &pos[0], 0);
        work->points[i].x = pos[1] << 4;
        work->points[i].z = pos[0] << 4;
    }
    for (i = 0; i < 9; i++) {
        WiFiPlazaModels_SetMoverX(work->models, i, work->baseX);
        if (ov45_0222AD80(work->lobby, i) == 1) {
            ov49_02269178(work, i);
            ov49_022695C4(work, i);
        }
    }
    return work;
}

void ov49_02269090(UnkStruct_ov49_02268FAC *work) {
    Heap_Free(work);
}

#pragma force_active on

void ov49_02269098(UnkStruct_ov49_02268FAC *work) {
    int selected = ov45_0222ADA8(work->lobby, ov45_0222A53C(work->lobby));
    u32 column;
    int present;
    int isOwnIndex;
    int i;

    if (selected == -1) {
        present = FALSE;
    } else {
        ov45_0222AE08(selected, (u32 *)&selected, &column);
        present = TRUE;
    }
    for (i = 0; i < 9; i++) {
        if (ov45_0222AD80(work->lobby, i) == 1) {
            WiFiPlazaModels_SetMoverVisible(work->models, i, 1);
            ov49_02269178(work, i);
            ov49_022695C4(work, i);
            if (present) {
                if (selected == i) {
                    isOwnIndex = TRUE;
                } else {
                    isOwnIndex = FALSE;
                }
            }
            ov49_02269240(work, i, present, isOwnIndex, column);
        } else {
            WiFiPlazaModels_SetMoverVisible(work->models, i, 0);
        }
    }
    for (i = 0; i < 9; i++) {
        WiFiPlazaEventWobble *wobble = &work->wobbles[i];

        work->states[i][0] = WiFiPlazaModels_IsMoverAnim1Playing(work->models, i);
        work->states[i][1] = ov49_02269430(wobble, work->models, i);
        work->states[i][2] = WiFiPlazaModels_IsMoverAnim3Playing(work->models, i);
    }
}

#pragma force_active reset

u8 ov49_02269154(UnkStruct_ov49_02268FAC *work, u32 index, int field) {
    u8 *entry;

    if (index >= 9) {
        GF_AssertFail();
    }
    if (field >= 3) {
        GF_AssertFail();
    }
    // address computed as in the original: (work + index * 3 + field)[offset of states]
    entry = (u8 *)work + index * sizeof(work->states[0]);
    entry += field;
    return entry[offsetof(UnkStruct_ov49_02268FAC, states)];
}

#pragma force_active on

void ov49_02269178(UnkStruct_ov49_02268FAC *work, int index) {
    int value = ov45_0222AD90(work->lobby, index);
    int max = ov45_0222ADA0(work->lobby);
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

void ov49_02269240(UnkStruct_ov49_02268FAC *work, u32 index, BOOL hasOwnSelection, int isOwnIndex, int ownColumn) {
    int type = ov45_0222AD3C(work->lobby);
    int i;

    ov45_0222AD2C(work->lobby);
    for (i = 0; i < 3; i++) {
        int active = ov45_0222ADE8(work->lobby, index * 3 + i);
        int isOwnColumn = FALSE;

        if (isOwnIndex == TRUE && i == ownColumn) {
            isOwnColumn = TRUE;
        }
        if (active == 1) {
            switch (i) {
            case 0:
                if (WiFiPlazaModels_StartMoverAnim1(work->models, index) == 1) {
                    if (index % 3 == 2) {
                        ov49_022693D4(hasOwnSelection, isOwnIndex, isOwnColumn, ov49_0226A8DC[type][1]);
                        if (ov49_0226A8DC[type][4] != 0xFFFE) {
                            ov49_022693A4(hasOwnSelection, isOwnIndex, isOwnColumn, ov49_0226A8DC[type][4]);
                        }
                    } else {
                        ov49_022693D4(hasOwnSelection, isOwnIndex, isOwnColumn, ov49_0226A8DC[type][0]);
                        if (ov49_0226A8DC[type][2] != 0xFFFE) {
                            ov49_022693A4(hasOwnSelection, isOwnIndex, isOwnColumn, ov49_0226A8DC[type][2]);
                        }
                    }
                    ov45_0222ADF8(work->lobby, index * 3 + i);
                }
                break;
            case 1:
                if (ov49_022693F8(&work->wobbles[index], index % 3 == 2, work->models, index) == 1) {
                    ov45_0222ADF8(work->lobby, index * 3 + i);
                    ov49_022693A4(hasOwnSelection, isOwnIndex, isOwnColumn, 1478);
                }
                break;
            case 2:
                if (WiFiPlazaModels_StartMoverAnim3(work->models, index) == 1) {
                    ov45_0222ADF8(work->lobby, index * 3 + i);
                    if (index % 3 == 2) {
                        ov49_022693A4(hasOwnSelection, isOwnIndex, isOwnColumn, ov49_0226A8DC[type][5]);
                    } else {
                        ov49_022693A4(hasOwnSelection, isOwnIndex, isOwnColumn, ov49_0226A8DC[type][3]);
                    }
                }
                break;
            }
        }
    }
}

#pragma force_active reset

// While the own player has a selection, only the own index is heard (on the player channel for the own column).
void ov49_022693A4(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 seqNo) {
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

void ov49_022693D4(BOOL hasOwnSelection, BOOL isOwnIndex, BOOL isOwnColumn, u32 species) {
    if (hasOwnSelection != FALSE) {
        if (isOwnIndex == FALSE) {
            return;
        }
        PlayCry(species, 0);
        return;
    }
    PlayCry(species, 0);
}

BOOL ov49_022693F8(WiFiPlazaEventWobble *wobble, int kind, void *models, int index) {
    if ((u32)kind >= 2) {
        GF_AssertFail();
    }
    if (wobble->active == TRUE) {
        return FALSE;
    }
    wobble->kind = kind;
    wobble->active = TRUE;
    ov49_0226A8D4[kind](wobble, models, index);
    return TRUE;
}

BOOL ov49_02269430(WiFiPlazaEventWobble *wobble, void *models, int index) {
    if (wobble->active != FALSE) {
        if (ov49_0226A8CC[wobble->kind](wobble, models, index) == TRUE) {
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

void ov49_02269474(WiFiPlazaEventWobble *wobble) {
    wobble->frame = 0;
    wobble->duration = 8;
    wobble->maxAngle = GF_DegreeToSinCosIdx(5);
    wobble->maxHeight = 2;
    wobble->repeats = 4;
}

void ov49_02269494(WiFiPlazaEventWobble *wobble) {
    wobble->frame = 0;
    wobble->duration = 16;
    wobble->maxAngle = GF_DegreeToSinCosIdx(10);
    wobble->maxHeight = 4;
    wobble->repeats = 2;
}

BOOL ov49_022694B4(WiFiPlazaEventWobble *wobble, void *models, int index) {
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
