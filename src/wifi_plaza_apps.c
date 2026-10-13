#include <nitro/rtc.h>

#include "global.h"

#include "overlay_manager.h"
#include "poke_overlay.h"
#include "wifi_plaza_internal.h"

// The applications of the plaza, one overlay at a time. Each app has an entry in the four tables below (index =
// app): make its arguments, free them, start its overlay, and pick the next app when the overlay has finished.
//   0 the plaza itself (OVY_49)   1 visitor profiles (OVY_48)   2 plaza news (OVY_88)   3, 4 footprint stamp (OVY_89)
//   5 connect (OVY_46)   6, 7, 8 the games (OVY_91, OVY_92, OVY_93, with OVY_90)   9, 10 disconnect (OVY_46)
//   11, 12 the app of ov45_02254B44

FS_EXTERN_OVERLAY(OVY_90);

extern const OverlayManagerTemplate ov45_02254B04; // disconnect (OVY_46)
extern const OverlayManagerTemplate ov45_02254B14; // footprint stamp (OVY_89)
extern const OverlayManagerTemplate ov45_02254B24; // footprint stamp (OVY_89)
extern const OverlayManagerTemplate ov45_02254B34; // OVY_93
extern const OverlayManagerTemplate ov45_02254B44;
extern const OverlayManagerTemplate ov45_02254B54; // OVY_91
extern const OverlayManagerTemplate ov45_02254B64; // connect (OVY_46)
extern const OverlayManagerTemplate ov45_02254B74; // plaza news (OVY_88)
extern const OverlayManagerTemplate ov45_02254B84; // visitor profiles (OVY_48)
extern const OverlayManagerTemplate ov45_02254B94; // the plaza (OVY_49)
extern const OverlayManagerTemplate ov45_02254BA4; // OVY_92

extern void (*const ov45_02254E88[13])(WiFiPlazaApps *apps, void **args, enum HeapID heapID);
extern void (*const _02254E20[13])(void **args);
extern void (*const ov45_02254E54[13])(WiFiPlazaApps *apps, void **args);
extern BOOL (*const ov45_02254EBC[13])(WiFiPlazaApps *apps, void **args);

// Arguments of app 0, the plaza.
typedef struct WiFiPlazaMainArgs {
    SaveData *save;
    int unk_04; // ov45_0222AD2C
    int unk_08; // ov45_0222AD3C
    u32 unk_0c[3];
    u32 result; // what to do next (WiFiPlazaApps_EndMain)
    WiFiPlazaWork *work;
} WiFiPlazaMainArgs;

// Arguments of app 1, the visitor profiles.
typedef struct WiFiPlazaVisitorArgs {
    void *players; // WiFiPlazaWork_GetPlaces
    SaveData *save;
    u32 time; // WiFiPlazaWork.unk_d4
    WiFiPlazaWork *work;
} WiFiPlazaVisitorArgs;

// Arguments of app 2, the plaza news.
typedef struct WiFiPlazaNewsArgs {
    void *unk_00; // WiFiPlazaWork.unk_04
    u32 *time;    // WiFiPlazaWork.unk_d4
    WiFiPlazaWork *work;
} WiFiPlazaNewsArgs;

// Arguments of app 5, connect.
typedef struct WiFiPlazaConnectArgs {
    SaveData *save;
    WiFiPlazaWork *work;
    u32 unk_08; // WiFiPlazaApps.unk_39
    void *unk_0c;
    u32 result; // 0: not connected
} WiFiPlazaConnectArgs;

// Arguments of apps 3 and 4, the footprint stamp.
typedef struct WiFiPlazaFootprintArgs {
    WiFiPlazaWork *work;
    u8 mode;
} WiFiPlazaFootprintArgs;

// Arguments of apps 9 to 12.
typedef struct WiFiPlazaDisconnectArgs {
    SaveData *save;
    WiFiPlazaWork *work;
    u32 mode;
} WiFiPlazaDisconnectArgs;

// Arguments of the games (apps 6 to 8).
typedef struct WiFiPlazaGameArgs {
    u32 unk_00;      // ov45_0222AAC8 of the own player
    u8 slots[4];     // WiFiPlazaWork_GetGameMembers
    u8 unk_08[0x14]; // WiFiPlazaWork_GetSpecialVisitors
    void *unk_1c;
    PlayerProfile *profiles[4];
    WiFiPlazaWork *work;
    SaveData *save;
    u8 unk_38;
    u8 unk_39;
    u8 unk_3a[2];
    u32 unk_3c;
} WiFiPlazaGameArgs;

// The entries of the app tables (and their helpers).
void WiFiPlazaGameArgs_NewProfiles(WiFiPlazaGameArgs *args, enum HeapID heapID);
void WiFiPlazaGameArgs_FreeProfiles(WiFiPlazaGameArgs *args);
void WiFiPlazaGameArgs_Setup(WiFiPlazaGameArgs *args, WiFiPlazaWork *work, enum HeapID heapID);
void WiFiPlazaApps_NewMainArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeMainArgs(void **args);
void WiFiPlazaApps_StartMain(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndMain(WiFiPlazaApps *apps, void **args);
void WiFiPlazaApps_NewVisitorProfilesArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeVisitorProfilesArgs(void **args);
void WiFiPlazaApps_StartVisitorProfiles(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndVisitorProfiles(WiFiPlazaApps *apps);
void WiFiPlazaApps_NewPlazaNewsArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreePlazaNewsArgs(void **args);
void WiFiPlazaApps_StartPlazaNews(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndPlazaNews(WiFiPlazaApps *apps);
void WiFiPlazaApps_NewConnectArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeConnectArgs(void **args);
void WiFiPlazaApps_StartConnect(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndConnect(WiFiPlazaApps *apps, void **args);
void WiFiPlazaApps_NewSwalotPlopArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeSwalotPlopArgs(void **args);
void WiFiPlazaApps_StartSwalotPlop(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndSwalotPlop(WiFiPlazaApps *apps, void **args);
void WiFiPlazaApps_NewMimeJrTopArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeMimeJrTopArgs(void **args);
void WiFiPlazaApps_StartMimeJrTop(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndMimeJrTop(WiFiPlazaApps *apps, void **args);
void WiFiPlazaApps_NewWobbuffetPopArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeWobbuffetPopArgs(void **args);
void WiFiPlazaApps_StartWobbuffetPop(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndWobbuffetPop(WiFiPlazaApps *apps, void **args);
void WiFiPlazaApps_NewFootprintStampArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeFootprintStampArgs(void **args);
BOOL WiFiPlazaApps_EndFootprintStamp(WiFiPlazaApps *apps);
void ov45_0222D3B0(WiFiPlazaApps *apps, void **args);
void ov45_0222D3C4(WiFiPlazaApps *apps, void **args);
void WiFiPlazaApps_NewDisconnectArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeDisconnectArgs(void **args);
void WiFiPlazaApps_StartDisconnect(WiFiPlazaApps *apps, void **args);
void WiFiPlazaApps_StartClosedDisconnect(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndDisconnect(void);
void ov45_0222D44C(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D484(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void WiFiPlazaApps_FreeSurveyArgs(void **args);
void WiFiPlazaApps_StartSurvey(WiFiPlazaApps *apps, void **args);
BOOL WiFiPlazaApps_EndSurvey(WiFiPlazaApps *apps);

int WiFiPlaza_AvatarToIndex(u16 avatar) {
    int i;
    for (i = 0; i < 16; i++) {
        if (avatar == ov45_02254A84[i].unk_00) {
            return i;
        }
    }
    return 16;
}

u16 WiFiPlaza_IndexToAvatar(u32 index) {
    if (index < 16) {
        return ov45_02254A84[index].unk_00;
    }
    return 0xffff;
}

WiFiPlazaApps *WiFiPlazaApps_New(u32 unk_39, SaveData *save, void *unk_44, WiFiPlazaWork *work, enum HeapID heapID) {
    WiFiPlazaApps *apps = Heap_Alloc(heapID, sizeof(WiFiPlazaApps));
    int i;
    memset(apps, 0, sizeof(WiFiPlazaApps));
    apps->work = work;
    apps->save = save;
    apps->unk_44 = unk_44;
    apps->unk_39 = unk_39;
    apps->heapID = heapID;
    for (i = 0; i < 13; i++) {
        WiFiPlazaApps_InitArgs(apps, i, heapID);
    }
    return apps;
}

void WiFiPlazaApps_Delete(WiFiPlazaApps *apps) {
    int i;
    for (i = 0; i < 13; i++) {
        WiFiPlazaApps_FreeArgs(apps, i);
    }
    Heap_Free(apps);
}

void WiFiPlazaApps_StartFirst(WiFiPlazaApps *apps) {
    WiFiPlazaApps_Start(apps, 5);
}

// TRUE when the last app has finished.
BOOL WiFiPlazaApps_Run(WiFiPlazaApps *apps) {
    BOOL done = FALSE;
    if (apps->manager && OverlayManager_Run(apps->manager)) {
        OverlayManager_Delete(apps->manager);
        apps->manager = NULL;
        done = WiFiPlazaApps_End(apps, apps->current);
    }
    return done;
}

void WiFiPlazaApps_VBlank(WiFiPlazaApps *apps) {
}

void WiFiPlazaApps_StartOverlay(WiFiPlazaApps *apps, void **args, const OverlayManagerTemplate *template) {
    if (apps->manager != NULL) {
        GF_AssertFail();
    }
    apps->manager = OverlayManager_New(template, *args, (enum HeapID)apps->heapID);
}

void WiFiPlazaApps_InitArgs(WiFiPlazaApps *apps, u32 app, enum HeapID heapID) {
    if (app >= 13) {
        GF_AssertFail();
    }
    ov45_02254E88[app](apps, &apps->args[app], heapID);
}

void WiFiPlazaApps_FreeArgs(WiFiPlazaApps *apps, u32 app) {
    if (app >= 13) {
        GF_AssertFail();
    }
    _02254E20[app](&apps->args[app]);
}

void WiFiPlazaApps_Start(WiFiPlazaApps *apps, u32 app) {
    if (app >= 13) {
        GF_AssertFail();
    }
    ov45_02254E54[app](apps, &apps->args[app]);
    apps->current = app;
}

BOOL WiFiPlazaApps_End(WiFiPlazaApps *apps, u32 app) {
    if (app >= 13) {
        GF_AssertFail();
    }
    return ov45_02254EBC[app](apps, &apps->args[app]);
}

void WiFiPlazaGameArgs_NewProfiles(WiFiPlazaGameArgs *args, enum HeapID heapID) {
    int i;
    for (i = 0; i < 4; i++) {
        args->profiles[i] = PlayerProfile_New(heapID);
    }
}

void WiFiPlazaGameArgs_FreeProfiles(WiFiPlazaGameArgs *args) {
    int i;
    void *nullp = NULL;
    for (i = 0; i < 4; i++) {
        Heap_Free(args->profiles[i]);
        args->profiles[i] = nullp;
    }
}

void WiFiPlazaGameArgs_Setup(WiFiPlazaGameArgs *args, WiFiPlazaWork *work, enum HeapID heapID) {
    int i;
    args->unk_00 = ov45_0222AAC8(WiFiPlazaWork_GetMyPlayer(work));
    WiFiPlazaWork_GetSpecialVisitors(work, args->unk_08);
    WiFiPlazaWork_GetGameMembers(work, args->slots);
    args->work = work;
    args->unk_1c = NULL;
    for (i = 0; i < 4; i++) {
        WiFiPlazaPlayer *player = WiFiPlazaWork_GetPlayer(work, args->slots[i]);
        if (player) {
            WiFiPlazaPlayer_CopyToProfile(player, args->profiles[i], heapID);
        }
    }
}

// App 0, the plaza.
void WiFiPlazaApps_NewMainArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    WiFiPlazaApps *state = apps;
    void **dest = out;
    WiFiPlazaMainArgs *args;
    *dest = Heap_Alloc(heapID, sizeof(WiFiPlazaMainArgs));
    memset(*dest, 0, sizeof(WiFiPlazaMainArgs));
    args = *dest;
    args->save = state->save;
    args->work = state->work;
}

void WiFiPlazaApps_FreeMainArgs(void **args) {
    if (*args == NULL) {
        GF_AssertFail();
    }
    Heap_Free(*args);
    *args = NULL;
}

void WiFiPlazaApps_StartMain(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaMainArgs *main = *args;
    main->unk_04 = ov45_0222AD2C(apps->work);
    main->unk_08 = ov45_0222AD3C(apps->work);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B94);
}

// The plaza tells which app comes next.
BOOL WiFiPlazaApps_EndMain(WiFiPlazaApps *apps, void **args) {
    switch (((WiFiPlazaMainArgs *)*args)->result) {
    case 0:
        WiFiPlazaApps_Start(apps, 9);
        break;
    case 1:
        WiFiPlazaApps_Start(apps, 1);
        break;
    case 2:
        WiFiPlazaApps_Start(apps, 2);
        break;
    case 6:
        WiFiPlazaApps_Start(apps, 3);
        break;
    case 7:
        WiFiPlazaApps_Start(apps, 4);
        break;
    case 3:
        WiFiPlazaApps_Start(apps, 6);
        break;
    case 4:
        WiFiPlazaApps_Start(apps, 7);
        break;
    case 5:
        WiFiPlazaApps_Start(apps, 8);
        break;
    case 8:
        WiFiPlazaApps_Start(apps, 10);
        break;
    case 9:
        WiFiPlazaApps_Start(apps, 11);
        break;
    case 10:
        WiFiPlazaApps_Start(apps, 12);
        break;
    default:
        GF_AssertFail();
    }
    return FALSE;
}

// App 1, the visitor profiles.
void WiFiPlazaApps_NewVisitorProfilesArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p;
    u32 count;
    p = Heap_Alloc(heapID, sizeof(WiFiPlazaVisitorArgs));
    *out = p;
    count = sizeof(WiFiPlazaVisitorArgs);
    do {
        *p = 0;
        p++;
        count--;
    } while (count);
    p = *out;
    ((WiFiPlazaVisitorArgs *)p)->save = apps->save;
    ((WiFiPlazaVisitorArgs *)p)->work = apps->work;
}

void WiFiPlazaApps_FreeVisitorProfilesArgs(void **args) {
    Heap_Free(*args);
}

void WiFiPlazaApps_StartVisitorProfiles(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaVisitorArgs *visitor = *args;
    visitor->players = WiFiPlazaWork_GetPlaces(apps->work);
    visitor->time = *ov45_0222A22C(apps->work);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B84);
}

BOOL WiFiPlazaApps_EndVisitorProfiles(WiFiPlazaApps *apps) {
    if (WiFiPlazaWork_IsClosed(apps->work)) {
        WiFiPlazaApps_Start(apps, 10);
    } else {
        WiFiPlazaApps_Start(apps, 0);
    }
    return FALSE;
}

// App 2, the plaza news.
void WiFiPlazaApps_NewPlazaNewsArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p = Heap_Alloc(heapID, 12);
    *out = p;
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
    p = *out;
    ((WiFiPlazaNewsArgs *)p)->unk_00 = WiFiPlazaWork_GetNews(apps->work);
    ((WiFiPlazaNewsArgs *)p)->time = ov45_0222A22C(apps->work);
    ((WiFiPlazaNewsArgs *)p)->work = apps->work;
}

void WiFiPlazaApps_FreePlazaNewsArgs(void **args) {
    Heap_Free(*args);
}

void WiFiPlazaApps_StartPlazaNews(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B74);
}

BOOL WiFiPlazaApps_EndPlazaNews(WiFiPlazaApps *apps) {
    if (WiFiPlazaWork_IsClosed(apps->work)) {
        WiFiPlazaApps_Start(apps, 10);
    } else {
        WiFiPlazaApps_Start(apps, 0);
    }
    return FALSE;
}

// App 5, connect.
void WiFiPlazaApps_NewConnectArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p;
    u32 count;
    p = Heap_Alloc(heapID, sizeof(WiFiPlazaConnectArgs));
    *out = p;
    count = sizeof(WiFiPlazaConnectArgs);
    do {
        *p = 0;
        p++;
        count--;
    } while (count);
    p = *out;
    ((WiFiPlazaConnectArgs *)p)->save = apps->save;
    ((WiFiPlazaConnectArgs *)p)->work = apps->work;
    ((WiFiPlazaConnectArgs *)p)->unk_08 = apps->unk_39;
    ((WiFiPlazaConnectArgs *)p)->unk_0c = apps->unk_44;
}

void WiFiPlazaApps_FreeConnectArgs(void **args) {
    Heap_Free(*args);
}

void WiFiPlazaApps_StartConnect(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B64);
}

// TRUE (the plaza ends) unless the connection was made.
BOOL WiFiPlazaApps_EndConnect(WiFiPlazaApps *apps, void **args) {
    if (((WiFiPlazaConnectArgs *)*args)->result == 0) {
        return TRUE;
    }
    WiFiPlazaApps_Start(apps, 0);
    return FALSE;
}

// Apps 6, 7, 8, the games.
void WiFiPlazaApps_NewSwalotPlopArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p;
    *out = Heap_Alloc(heapID, sizeof(WiFiPlazaGameArgs) - 4); // without unk_3c
    memset(*out, 0, sizeof(WiFiPlazaGameArgs) - 4);
    p = *out;
    ((WiFiPlazaGameArgs *)p)->unk_38 = 0;
    ((WiFiPlazaGameArgs *)p)->unk_39 = 1;
    ((WiFiPlazaGameArgs *)p)->save = apps->save;
    WiFiPlazaGameArgs_NewProfiles((WiFiPlazaGameArgs *)p, heapID);
}

void WiFiPlazaApps_FreeSwalotPlopArgs(void **args) {
    WiFiPlazaGameArgs_FreeProfiles(*args);
    Heap_Free(*args);
}

void WiFiPlazaApps_StartSwalotPlop(WiFiPlazaApps *apps, void **args) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    WiFiPlazaGameArgs_Setup(*args, apps->work, (enum HeapID)apps->heapID);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B54);
}

BOOL WiFiPlazaApps_EndSwalotPlop(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaGameArgs *game;
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
    game = *args;
    ov45_0222A430(apps->work, game->unk_1c);
    ov45_0222A72C(apps->work, game->unk_00);
    WiFiPlazaApps_Start(apps, 0);
    return FALSE;
}

void WiFiPlazaApps_NewMimeJrTopArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p;
    *out = Heap_Alloc(heapID, sizeof(WiFiPlazaGameArgs));
    memset(*out, 0, sizeof(WiFiPlazaGameArgs));
    p = *out;
    ((WiFiPlazaGameArgs *)p)->unk_3c = 0;
    *(u32 *)&((WiFiPlazaGameArgs *)p)->unk_38 = 1; // this game reads a word at 0x38
    ((WiFiPlazaGameArgs *)p)->save = apps->save;
    WiFiPlazaGameArgs_NewProfiles((WiFiPlazaGameArgs *)p, heapID);
}

void WiFiPlazaApps_FreeMimeJrTopArgs(void **args) {
    WiFiPlazaGameArgs_FreeProfiles(*args);
    Heap_Free(*args);
}

void WiFiPlazaApps_StartMimeJrTop(WiFiPlazaApps *apps, void **args) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    WiFiPlazaGameArgs_Setup(*args, apps->work, (enum HeapID)apps->heapID);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254BA4);
}

BOOL WiFiPlazaApps_EndMimeJrTop(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaGameArgs *game;
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
    game = *args;
    ov45_0222A430(apps->work, game->unk_1c);
    ov45_0222A72C(apps->work, game->unk_00);
    WiFiPlazaApps_Start(apps, 0);
    return FALSE;
}

void WiFiPlazaApps_NewWobbuffetPopArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p;
    *out = Heap_Alloc(heapID, sizeof(WiFiPlazaGameArgs) - 4); // without unk_3c
    memset(*out, 0, sizeof(WiFiPlazaGameArgs) - 4);
    p = *out;
    ((WiFiPlazaGameArgs *)p)->unk_38 = 0;
    ((WiFiPlazaGameArgs *)p)->unk_39 = 1;
    ((WiFiPlazaGameArgs *)p)->save = apps->save;
    WiFiPlazaGameArgs_NewProfiles((WiFiPlazaGameArgs *)p, heapID);
}

void WiFiPlazaApps_FreeWobbuffetPopArgs(void **args) {
    WiFiPlazaGameArgs_FreeProfiles(*args);
    Heap_Free(*args);
}

void WiFiPlazaApps_StartWobbuffetPop(WiFiPlazaApps *apps, void **args) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    WiFiPlazaGameArgs_Setup(*args, apps->work, (enum HeapID)apps->heapID);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B34);
}

BOOL WiFiPlazaApps_EndWobbuffetPop(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaGameArgs *game;
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
    game = *args;
    ov45_0222A430(apps->work, game->unk_1c);
    ov45_0222A72C(apps->work, game->unk_00);
    WiFiPlazaApps_Start(apps, 0);
    return FALSE;
}

// Apps 3 and 4, the footprint stamp.
void WiFiPlazaApps_NewFootprintStampArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p = Heap_Alloc(heapID, 8);
    *out = p;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
    p[7] = 0;
    ((WiFiPlazaFootprintArgs *)*out)->work = apps->work;
}

void WiFiPlazaApps_FreeFootprintStampArgs(void **args) {
    Heap_Free(*args);
}

BOOL WiFiPlazaApps_EndFootprintStamp(WiFiPlazaApps *apps) {
    if (WiFiPlazaWork_IsClosed(apps->work)) {
        WiFiPlazaApps_Start(apps, 10);
    } else {
        WiFiPlazaApps_Start(apps, 0);
    }
    return FALSE;
}

void ov45_0222D3B0(WiFiPlazaApps *apps, void **args) {
    ((WiFiPlazaFootprintArgs *)*args)->mode = 0;
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B24);
}

void ov45_0222D3C4(WiFiPlazaApps *apps, void **args) {
    ((WiFiPlazaFootprintArgs *)*args)->mode = 1;
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B14);
}

// Apps 9 and 10, disconnect.
void WiFiPlazaApps_NewDisconnectArgs(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p = Heap_Alloc(heapID, 12);
    *out = p;
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
    p = *out;
    ((WiFiPlazaDisconnectArgs *)p)->save = apps->save;
    ((WiFiPlazaDisconnectArgs *)p)->work = apps->work;
}

void WiFiPlazaApps_FreeDisconnectArgs(void **args) {
    Heap_Free(*args);
}

void WiFiPlazaApps_StartDisconnect(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaApps_StartDisconnectOverlay(apps, args, 0);
}

void WiFiPlazaApps_StartClosedDisconnect(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaApps_StartDisconnectOverlay(apps, args, 1);
}

void WiFiPlazaApps_StartDisconnectOverlay(WiFiPlazaApps *apps, void **args, u32 mode) {
    ((WiFiPlazaDisconnectArgs *)*args)->mode = mode;
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B04);
}

BOOL WiFiPlazaApps_EndDisconnect(void) {
    return TRUE;
}

// Apps 11 and 12.
void ov45_0222D44C(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p = Heap_Alloc(heapID, 12);
    *out = p;
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
    p = *out;
    ((WiFiPlazaDisconnectArgs *)p)->save = apps->save;
    ((WiFiPlazaDisconnectArgs *)p)->work = apps->work;
    ((WiFiPlazaDisconnectArgs *)p)->mode = 0;
}

void ov45_0222D484(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p = Heap_Alloc(heapID, 12);
    *out = p;
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
    p = *out;
    ((WiFiPlazaDisconnectArgs *)p)->save = apps->save;
    ((WiFiPlazaDisconnectArgs *)p)->work = apps->work;
    ((WiFiPlazaDisconnectArgs *)p)->mode = 1;
}

void WiFiPlazaApps_FreeSurveyArgs(void **args) {
    Heap_Free(*args);
}

void WiFiPlazaApps_StartSurvey(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B44);
}

BOOL WiFiPlazaApps_EndSurvey(WiFiPlazaApps *apps) {
    if (WiFiPlazaWork_IsClosed(apps->work)) {
        WiFiPlazaApps_Start(apps, 10);
    } else {
        WiFiPlazaApps_Start(apps, 0);
    }
    return FALSE;
}

// Hour, minute and second of a time in seconds, in the low three bytes of out.
void WiFiPlaza_SecondsToTime(u32 *out, s64 *seconds) {
    RTCDate date;
    RTCTime time;
    RTC_ConvertSecondToDateTime(&date, &time, *seconds);
    ((u8 *)out)[0] = time.hour;
    ((u8 *)out)[1] = time.minute;
    ((u8 *)out)[2] = time.second;
}
