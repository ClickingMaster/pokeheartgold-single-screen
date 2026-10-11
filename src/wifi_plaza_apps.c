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
    void *players; // ov45_0222A214
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
    u8 slots[4];     // ov45_0222A498
    u8 unk_08[0x14]; // ov45_0222AB38
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
void ov45_0222CE78(WiFiPlazaGameArgs *args, enum HeapID heapID);
void ov45_0222CE94(WiFiPlazaGameArgs *args);
void ov45_0222CEB0(WiFiPlazaGameArgs *args, WiFiPlazaWork *work, enum HeapID heapID);
void ov45_0222CF00(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222CF24(void **args);
void ov45_0222CF40(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222CF68(WiFiPlazaApps *apps, void **args);
void ov45_0222CFF4(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D01C(void **args);
void ov45_0222D028(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222D054(WiFiPlazaApps *apps);
void ov45_0222D078(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D0BC(void **args);
void ov45_0222D0C8(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222D0D8(WiFiPlazaApps *apps);
void ov45_0222D0FC(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D130(void **args);
void ov45_0222D13C(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222D14C(WiFiPlazaApps *apps, void **args);
void ov45_0222D164(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D19C(void **args);
void ov45_0222D1B0(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222D1DC(WiFiPlazaApps *apps, void **args);
void ov45_0222D20C(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D23C(void **args);
void ov45_0222D250(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222D27C(WiFiPlazaApps *apps, void **args);
void ov45_0222D2AC(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D2E4(void **args);
void ov45_0222D2F8(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222D324(WiFiPlazaApps *apps, void **args);
void ov45_0222D354(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D380(void **args);
BOOL ov45_0222D38C(WiFiPlazaApps *apps);
void ov45_0222D3B0(WiFiPlazaApps *apps, void **args);
void ov45_0222D3C4(WiFiPlazaApps *apps, void **args);
void ov45_0222D3D8(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D410(void **args);
void ov45_0222D41C(WiFiPlazaApps *apps, void **args);
void ov45_0222D428(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222D448(void);
void ov45_0222D44C(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D484(WiFiPlazaApps *apps, void **out, enum HeapID heapID);
void ov45_0222D4C0(void **args);
void ov45_0222D4CC(WiFiPlazaApps *apps, void **args);
BOOL ov45_0222D4DC(WiFiPlazaApps *apps);

int ov45_0222CCE4(u16 avatar) {
    int i;
    for (i = 0; i < 16; i++) {
        if (avatar == ov45_02254A84[i].unk_00) {
            return i;
        }
    }
    return 16;
}

u16 ov45_0222CD04(u32 index) {
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

void ov45_0222CE78(WiFiPlazaGameArgs *args, enum HeapID heapID) {
    int i;
    for (i = 0; i < 4; i++) {
        args->profiles[i] = PlayerProfile_New(heapID);
    }
}

void ov45_0222CE94(WiFiPlazaGameArgs *args) {
    int i;
    void *nullp = NULL;
    for (i = 0; i < 4; i++) {
        Heap_Free(args->profiles[i]);
        args->profiles[i] = nullp;
    }
}

void ov45_0222CEB0(WiFiPlazaGameArgs *args, WiFiPlazaWork *work, enum HeapID heapID) {
    int i;
    args->unk_00 = ov45_0222AAC8(WiFiPlazaWork_GetMyPlayer(work));
    ov45_0222AB38(work, args->unk_08);
    ov45_0222A498(work, args->slots);
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
void ov45_0222CF00(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    WiFiPlazaApps *state = apps;
    void **dest = out;
    WiFiPlazaMainArgs *args;
    *dest = Heap_Alloc(heapID, sizeof(WiFiPlazaMainArgs));
    memset(*dest, 0, sizeof(WiFiPlazaMainArgs));
    args = *dest;
    args->save = state->save;
    args->work = state->work;
}

void ov45_0222CF24(void **args) {
    if (*args == NULL) {
        GF_AssertFail();
    }
    Heap_Free(*args);
    *args = NULL;
}

void ov45_0222CF40(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaMainArgs *main = *args;
    main->unk_04 = ov45_0222AD2C(apps->work);
    main->unk_08 = ov45_0222AD3C(apps->work);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B94);
}

// The plaza tells which app comes next.
BOOL ov45_0222CF68(WiFiPlazaApps *apps, void **args) {
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
void ov45_0222CFF4(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
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

void ov45_0222D01C(void **args) {
    Heap_Free(*args);
}

void ov45_0222D028(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaVisitorArgs *visitor = *args;
    visitor->players = ov45_0222A214(apps->work);
    visitor->time = *ov45_0222A22C(apps->work);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B84);
}

BOOL ov45_0222D054(WiFiPlazaApps *apps) {
    if (ov45_0222A33C(apps->work)) {
        WiFiPlazaApps_Start(apps, 10);
    } else {
        WiFiPlazaApps_Start(apps, 0);
    }
    return FALSE;
}

// App 2, the plaza news.
void ov45_0222D078(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
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
    ((WiFiPlazaNewsArgs *)p)->unk_00 = ov45_0222A210(apps->work);
    ((WiFiPlazaNewsArgs *)p)->time = ov45_0222A22C(apps->work);
    ((WiFiPlazaNewsArgs *)p)->work = apps->work;
}

void ov45_0222D0BC(void **args) {
    Heap_Free(*args);
}

void ov45_0222D0C8(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B74);
}

BOOL ov45_0222D0D8(WiFiPlazaApps *apps) {
    if (ov45_0222A33C(apps->work)) {
        WiFiPlazaApps_Start(apps, 10);
    } else {
        WiFiPlazaApps_Start(apps, 0);
    }
    return FALSE;
}

// App 5, connect.
void ov45_0222D0FC(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
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

void ov45_0222D130(void **args) {
    Heap_Free(*args);
}

void ov45_0222D13C(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B64);
}

// TRUE (the plaza ends) unless the connection was made.
BOOL ov45_0222D14C(WiFiPlazaApps *apps, void **args) {
    if (((WiFiPlazaConnectArgs *)*args)->result == 0) {
        return TRUE;
    }
    WiFiPlazaApps_Start(apps, 0);
    return FALSE;
}

// Apps 6, 7, 8, the games.
void ov45_0222D164(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p;
    *out = Heap_Alloc(heapID, sizeof(WiFiPlazaGameArgs) - 4); // without unk_3c
    memset(*out, 0, sizeof(WiFiPlazaGameArgs) - 4);
    p = *out;
    ((WiFiPlazaGameArgs *)p)->unk_38 = 0;
    ((WiFiPlazaGameArgs *)p)->unk_39 = 1;
    ((WiFiPlazaGameArgs *)p)->save = apps->save;
    ov45_0222CE78((WiFiPlazaGameArgs *)p, heapID);
}

void ov45_0222D19C(void **args) {
    ov45_0222CE94(*args);
    Heap_Free(*args);
}

void ov45_0222D1B0(WiFiPlazaApps *apps, void **args) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    ov45_0222CEB0(*args, apps->work, (enum HeapID)apps->heapID);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B54);
}

BOOL ov45_0222D1DC(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaGameArgs *game;
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
    game = *args;
    ov45_0222A430(apps->work, game->unk_1c);
    ov45_0222A72C(apps->work, game->unk_00);
    WiFiPlazaApps_Start(apps, 0);
    return FALSE;
}

void ov45_0222D20C(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p;
    *out = Heap_Alloc(heapID, sizeof(WiFiPlazaGameArgs));
    memset(*out, 0, sizeof(WiFiPlazaGameArgs));
    p = *out;
    ((WiFiPlazaGameArgs *)p)->unk_3c = 0;
    *(u32 *)&((WiFiPlazaGameArgs *)p)->unk_38 = 1; // this game reads a word at 0x38
    ((WiFiPlazaGameArgs *)p)->save = apps->save;
    ov45_0222CE78((WiFiPlazaGameArgs *)p, heapID);
}

void ov45_0222D23C(void **args) {
    ov45_0222CE94(*args);
    Heap_Free(*args);
}

void ov45_0222D250(WiFiPlazaApps *apps, void **args) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    ov45_0222CEB0(*args, apps->work, (enum HeapID)apps->heapID);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254BA4);
}

BOOL ov45_0222D27C(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaGameArgs *game;
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
    game = *args;
    ov45_0222A430(apps->work, game->unk_1c);
    ov45_0222A72C(apps->work, game->unk_00);
    WiFiPlazaApps_Start(apps, 0);
    return FALSE;
}

void ov45_0222D2AC(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
    u8 *p;
    *out = Heap_Alloc(heapID, sizeof(WiFiPlazaGameArgs) - 4); // without unk_3c
    memset(*out, 0, sizeof(WiFiPlazaGameArgs) - 4);
    p = *out;
    ((WiFiPlazaGameArgs *)p)->unk_38 = 0;
    ((WiFiPlazaGameArgs *)p)->unk_39 = 1;
    ((WiFiPlazaGameArgs *)p)->save = apps->save;
    ov45_0222CE78((WiFiPlazaGameArgs *)p, heapID);
}

void ov45_0222D2E4(void **args) {
    ov45_0222CE94(*args);
    Heap_Free(*args);
}

void ov45_0222D2F8(WiFiPlazaApps *apps, void **args) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    ov45_0222CEB0(*args, apps->work, (enum HeapID)apps->heapID);
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B34);
}

BOOL ov45_0222D324(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaGameArgs *game;
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
    game = *args;
    ov45_0222A430(apps->work, game->unk_1c);
    ov45_0222A72C(apps->work, game->unk_00);
    WiFiPlazaApps_Start(apps, 0);
    return FALSE;
}

// Apps 3 and 4, the footprint stamp.
void ov45_0222D354(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
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

void ov45_0222D380(void **args) {
    Heap_Free(*args);
}

BOOL ov45_0222D38C(WiFiPlazaApps *apps) {
    if (ov45_0222A33C(apps->work)) {
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
void ov45_0222D3D8(WiFiPlazaApps *apps, void **out, enum HeapID heapID) {
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

void ov45_0222D410(void **args) {
    Heap_Free(*args);
}

void ov45_0222D41C(WiFiPlazaApps *apps, void **args) {
    ov45_0222D434(apps, args, 0);
}

void ov45_0222D428(WiFiPlazaApps *apps, void **args) {
    ov45_0222D434(apps, args, 1);
}

void ov45_0222D434(WiFiPlazaApps *apps, void **args, u32 mode) {
    ((WiFiPlazaDisconnectArgs *)*args)->mode = mode;
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B04);
}

BOOL ov45_0222D448(void) {
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

void ov45_0222D4C0(void **args) {
    Heap_Free(*args);
}

void ov45_0222D4CC(WiFiPlazaApps *apps, void **args) {
    WiFiPlazaApps_StartOverlay(apps, args, &ov45_02254B44);
}

BOOL ov45_0222D4DC(WiFiPlazaApps *apps) {
    if (ov45_0222A33C(apps->work)) {
        WiFiPlazaApps_Start(apps, 10);
    } else {
        WiFiPlazaApps_Start(apps, 0);
    }
    return FALSE;
}

// Hour, minute and second of a time in seconds, in the low three bytes of out.
void ov45_0222D500(u32 *out, s64 *seconds) {
    RTCDate date;
    RTCTime time;
    RTC_ConvertSecondToDateTime(&date, &time, *seconds);
    ((u8 *)out)[0] = time.hour;
    ((u8 *)out)[1] = time.minute;
    ((u8 *)out)[2] = time.second;
}
