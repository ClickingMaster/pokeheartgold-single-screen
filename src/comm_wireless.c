#include <stddef.h>

#include "global.h"

#include "heap.h"
#include "mail_message.h"
#include "player_data.h"
#include "save_link_ruleset.h"

// Wireless link layer of the communication code (ARM9 static 0x02034B0C-0x02035900), above the local wireless
// manager (wireless_manager.c): wireless start-up (WVR), the beacon this game sends as a parent (trainer, link
// kind, member count, profile, ruleset), the list of parents found while scanning (kept 300 frames after their
// last beacon), connecting to one of them, the parent's start sequence (measure channels, pick one, start) and
// the shutdown sequences.

// The game info sent in the beacons (0x5C bytes; at offset 0x50 of a scanned parent's description).
typedef struct CommBeacon {
    u32 trainerId;
    u8 kind;        // 0x04: link kind (CommState_GetLinkMode)
    u8 id;          // 0x05: sub_02039954
    u8 memberCount; // 0x06: connected players
    u8 unk_07;
    MailMessage mail; // 0x08
    u8 profile[0x20]; // 0x10: PlayerProfile
    u8 ruleset[0x20]; // 0x30: LinkBattleRuleset
    u8 unk_50[4];
    u8 entryLocked; // 0x54: Wireless_IsEntryLocked
    u8 unk_55[7];
} CommBeacon;

// A parent found while scanning: its description (0xC0 bytes, as Wireless_StartChild takes it).
typedef struct CommParentInfo {
    u8 unk_00[4];
    u8 macAddress[6]; // 0x04
    u8 unk_0A[0x2C];
    u16 channel; // 0x36
    u8 unk_38[0x18];
    CommBeacon beacon; // 0x50
    u8 unk_AC[0x14];
} CommParentInfo;

typedef struct CommWirelessWork {
    u8 customBeacon[0x54];      // 0x000: sent instead of the beacon in link kind 15
    CommParentInfo received;    // 0x054: last parent reported by the scan
    CommParentInfo parents[16]; // 0x114
    u8 macAddresses[8][6];      // 0xD14
    u16 parentTimers[16];       // 0xD44: frames left until a parent is dropped (0: free slot)
    void *managerBuffer;        // 0xD64: Wireless_Init buffer
    MailMessage mail;           // 0xD68
    u32 unk_D70;
    u8 parentListChanged; // 0xD74
    u8 unk_D75[3];
    PlayerProfile *profile;     // 0xD78
    LinkBattleRuleset *ruleset; // 0xD7C
    u32 ggid;                   // 0xD80
    void *beaconBuffer;         // 0xD84
    CommBeacon *beacon;         // 0xD88: beaconBuffer aligned to 32 bytes
    u16 channel;                // 0xD8C
    u16 firstCount;             // 0xD8E: 0xFFFF until the first CommWireless_Update
    u8 currentChannel;          // 0xD90
    u8 channelTimer;            // 0xD91
    u8 shutdown;                // 0xD92: 0 none, 1 stop and free, 2 end the manager, 3 ended
    u8 portCallbackSet;         // 0xD93
    u8 stopState;               // 0xD94
    u8 linkLost : 1;            // 0xD95
    u8 detectDrops : 1;
    u8 lostWhenAlone : 1;
    u8 renewTgid : 1;
    u8 unk4 : 1;
    u8 entryFlag : 1;
    u8 parentPending : 1; // received holds a parent not stored yet
    u8 unk_D96[2];
} CommWirelessWork;

typedef char CommWirelessWorkSizeCheck[sizeof(CommWirelessWork) == 0xD98 ? 1 : -1];
typedef char CommWirelessTimerCheck[offsetof(CommWirelessWork, parentTimers) == 0xD44 ? 1 : -1];
typedef char CommBeaconSizeCheck[sizeof(CommBeacon) == 0x5C ? 1 : -1];

typedef struct CommWireless {
    u16 tgid;
    u16 unk_02;
    int state; // 0 off, 1 starting up, 2 started (WVR)
    CommWirelessWork *work;
} CommWireless;

extern CommWireless gCommWireless;

typedef struct CommBssid {
    u8 data[6];
} CommBssid;

extern const CommBssid gCommWirelessAnyBssid; // FF:FF:FF:FF:FF:FF

void OS_Terminate(void);
void Sys_SetSleepDisableFlag(u32 flag);
void Sys_ClearSleepDisableFlag(u32 flag);
int WVR_StartUpAsync(int mode, void (*callback)(void *, int), void *arg);
int WVR_TerminateAsync(void (*callback)(void *), void *arg);
u16 WM_GetNextTgid(void);
u16 WM_GetDispersionBeaconPeriod(void);
int CommPlayerInfo_DropDisconnected(void);

int CommState_GetLinkMode(void);
int sub_02039954(void);
void CommState_ShowErrorAndHalt(u32);
BOOL sub_02037474(void);
BOOL sub_0203401C(int value);
int sub_02033FC4(u16);
void sub_02036904(void);
void sub_020367A8(void);

u32 Wireless_GetWorkSize(void);
void Wireless_Init(void *buffer, int mode);
void Wireless_SetGgid(void *ggid);
void Wireless_SetParentGameInfo(void *gameInfo, u32 length);
void Wireless_ClearBeaconCount(void);
void Wireless_SetReceiveCallback(void (*callback)(void), int port);
int Wireless_StartMeasureChannels(void);
BOOL Wireless_StartScan(void (*callback)(void *), const void *bssid, int channel);
int Wireless_IsScanning(void);
void Wireless_StopScan(void);
int Wireless_IsBusy(void);
int Wireless_HasError(void);
int Wireless_IsIdle();
int Wireless_GetConnectedBitmap();
int Wireless_GetState();
void Wireless_Stop(void);
void Wireless_ScanAndConnect(int mode, void *bssid, int channel);
void Wireless_StartChild(int mode, void *parentInfo);
int Wireless_IsEntryLocked(void);
void Wireless_UpdateGameInfo(void *gameInfo, int length, void *ggid, u16 tgid);
int Wireless_GetAid(void);
u32 Wireless_GetError(void);
int Wireless_End(void);
u16 Wireless_SelectChannel(void);
BOOL Wireless_StartParent(int, u16, u16, int, u32, u32);
void Wireless_HasSentBeacons(void);

void CommWireless_Init(PlayerProfile *profile, int mode);
BOOL CommWireless_IsInitialized(void);
BOOL CommWireless_BytesEqual(const u8 *a, const u8 *b, int size);
void CommWireless_OnParentFound(void *parentInfo);
void CommWireless_StoreFoundParent(void);
void CommWireless_OnStartedUp(void *unused, int failed);
void CommWireless_OnTerminated(void *unused);
void CommWireless_StartUp(void);
BOOL CommWireless_IsStartedUp(void);
BOOL CommWireless_IsOn(void);
void CommWireless_Terminate(void);
void CommWireless_InitManager(int mode);
void CommWireless_ClearParents(void);
void CommWireless_SetRenewTgid(BOOL renew);
void CommWireless_ResetFlags(void);
BOOL CommWireless_StartParent(int unused, BOOL renewTgid, BOOL entryFlag);
BOOL CommWireless_StartScan(int unused, BOOL clearParents);
BOOL CommWireless_Stop(void);
BOOL CommWireless_RequestStopAndFree(void);
void CommWireless_SetManagerEnded(BOOL end);
void CommWireless_Free(void);
int CommWireless_CountParents(void);
int CommWireless_GetParentSlot(int ordinal);
u8 CommWireless_IsParentListChanged(void);
void CommWireless_ClearParentListChanged(void);
BOOL CommWireless_GetParentMemberCount(int slot);
void CommWireless_CopyParentProfile(int ordinal, PlayerProfile *profile);
BOOL CommWireless_ConnectToParent(int slot);
void CommWireless_UpdateParents(void);
void CommWireless_BuildBeacon(void);
void CommWireless_UpdateBeaconMemberCount(void);
void CommWireless_Update(int count, CommWirelessWork *work);
BOOL CommWireless_Main(int count);
BOOL CommWireless_IsAidConnected(u16 aid);
int CommWireless_CountConnected(void);
int CommWireless_IsManagerEnded(void);
BOOL CommWireless_Exists(void);
int CommWireless_IsIdle(void);
int CommWireless_GetConnectedChildren(void);
BOOL CommWireless_IsLinkLost(void);
void CommWireless_SetLostWhenAlone(BOOL value);
void CommWireless_SetDetectDrops(BOOL value);
u32 CommWireless_GetBeaconPeriod(u32 kind);
void *CommWireless_GetParentInfo(int slot);
void *CommWireless_GetProfile(void);
void *CommWireless_GetParentProfile(int slot);
void CommWireless_SetMacAddress(void *macAddress, int index);
BOOL sub_020357FC(void);
void sub_0203581C(void);
void CommWireless_SetMail(void *mail);
void CommWireless_SetRuleset(void *ruleset);
void *CommWireless_GetRuleset(void);
void CommWireless_RefreshBeacon(void);
void CommWireless_HasSentBeacons(void);
void CommWireless_SetCustomBeacon(const void *src);
void *CommWireless_GetParentMail(int slot);

void CommWireless_Init(PlayerProfile *profile, int mode) {
    if (gCommWireless.work == NULL) {
        void *allocation;
        CommWirelessWork *work;
        u8 *raw;
        u32 remainder;
        u32 adjustment;

        allocation = Heap_Alloc(HEAP_ID_15, sizeof(CommWirelessWork));
        gCommWireless.work = allocation;
        MI_CpuFill8(allocation, 0, sizeof(CommWirelessWork));

        allocation = Heap_Alloc(HEAP_ID_15, Wireless_GetWorkSize());
        gCommWireless.work->managerBuffer = allocation;
        MI_CpuFill8(gCommWireless.work->managerBuffer, 0, Wireless_GetWorkSize());

        allocation = Heap_Alloc(HEAP_ID_15, LinkBattleRuleset_sizeof());
        gCommWireless.work->ruleset = allocation;
        MI_CpuFill8(gCommWireless.work->ruleset, 0, LinkBattleRuleset_sizeof());

        allocation = Heap_Alloc(HEAP_ID_15, 0x90);
        gCommWireless.work->beaconBuffer = allocation;
        work = gCommWireless.work;
        raw = work->beaconBuffer;
        remainder = (u32)raw & 0x1F;
        adjustment = 0x20 - remainder;
        work->beacon = (CommBeacon *)(adjustment + (u32)raw);
        gCommWireless.work->ggid = 0x333;
        gCommWireless.work->profile = profile;
        MailMsg_Init(&gCommWireless.work->mail);
        CommWireless_InitManager(mode);
        gCommWireless.tgid = WM_GetNextTgid();
    }
}

BOOL CommWireless_IsInitialized(void) {
    return gCommWireless.work != NULL;
}

BOOL CommWireless_BytesEqual(const u8 *a, const u8 *b, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (*a != *b) {
            return 0;
        }
        a++;
        b++;
    }
    return 1;
}

// Scan callback: keeps a parent of our link kind (and id), stored by CommWireless_StoreFoundParent.
void CommWireless_OnParentFound(void *parentInfo) {
    u8 *data = parentInfo;
    u8 *beacon;
    int kind;
    int id;

    kind = CommState_GetLinkMode();
    id = sub_02039954();
    beacon = data + 0x50;

    if (kind != 14) {
        if (!sub_0203401C(beacon[4]) || !sub_0203401C(kind)) {
            if (beacon[0x54] != 0 && beacon[4] == 10) {
                return;
            }
            if (beacon[4] != kind) {
                return;
            }
        }
    }
    if (kind != 14 && beacon[5] != id) {
        return;
    }
    MI_CpuCopy8(data, &gCommWireless.work->received, 0xC0);
    gCommWireless.work->parentPending = 1;
}

void CommWireless_StoreFoundParent(void) {
    CommWirelessWork *work;
    u8 *received;
    u8 *slot;
    int i;

    work = gCommWireless.work;
    received = (u8 *)&work->received;
    if (work->parentPending == 0) {
        return;
    }
    work->parentPending = 0;
    for (i = 0; i < 16; i++) {
        if (gCommWireless.work->parentTimers[i] != 0) {
            if (CommWireless_BytesEqual(gCommWireless.work->parents[i].macAddress, received + 4, 6)) {
                gCommWireless.work->parentTimers[i] = 300;
                MI_CpuCopy8(received, &gCommWireless.work->parents[i], 0xC0);
                return;
            }
        }
    }
    slot = (u8 *)gCommWireless.work;
    for (i = 0; i < 16; i++, slot += 2) {
        if (*(u16 *)(slot + offsetof(CommWirelessWork, parentTimers)) == 0) {
            break;
        }
    }
    if (i < 16) {
        gCommWireless.work->parentTimers[i] = 300;
        MI_CpuCopy8(received, &gCommWireless.work->parents[i], 0xC0);
        gCommWireless.work->parentListChanged = 1;
    }
}

void CommWireless_OnStartedUp(void *unused, int failed) {
    if (failed != 0) {
        OS_Terminate();
    }
    gCommWireless.state = 2;
}

void CommWireless_OnTerminated(void *unused) {
    gCommWireless.state = 0;
    Sys_ClearSleepDisableFlag(4);
}

void CommWireless_StartUp(void) {
    Sys_SetSleepDisableFlag(4);
    gCommWireless.state = 1;
    if (WVR_StartUpAsync(8, CommWireless_OnStartedUp, NULL) != 1) {
        OS_Terminate();
    }
}

BOOL CommWireless_IsStartedUp(void) {
    return gCommWireless.state == 2;
}

BOOL CommWireless_IsOn(void) {
    return gCommWireless.state != 0;
}

void CommWireless_Terminate(void) {
    WVR_TerminateAsync(CommWireless_OnTerminated, NULL);
}

void CommWireless_InitManager(int mode) {
    u8 *buffer;
    u32 misalignment;

    gCommWireless.work->unk_D70 = 0;
    buffer = gCommWireless.work->managerBuffer;
    misalignment = (u32)buffer & 0x1F;
    Wireless_Init(buffer + (0x20 - misalignment), mode);
    Wireless_SetGgid((void *)gCommWireless.work->ggid);
}

void CommWireless_ClearParents(void) {
    int i;
    for (i = 0; i < 16; i++) {
        gCommWireless.work->parentTimers[i] = 0;
    }
    MI_CpuFill8(gCommWireless.work->parents, 0, sizeof(gCommWireless.work->parents));
}

void CommWireless_SetRenewTgid(BOOL renew) {
    gCommWireless.work->renewTgid = renew;
}

void CommWireless_ResetFlags(void) {
    gCommWireless.work->parentListChanged = 0;
    gCommWireless.work->linkLost = 0;
    gCommWireless.work->lostWhenAlone = 0;
    gCommWireless.work->shutdown = 0;
    gCommWireless.work->unk4 = 0;
    gCommWireless.work->stopState = 0;
    gCommWireless.work->portCallbackSet = 0;
}

// Measures the channels; CommWireless_Update starts the parent on the quietest one.
BOOL CommWireless_StartParent(int unused, BOOL renewTgid, BOOL entryFlag) {
    (void)unused;
    CommWireless_ResetFlags();
    CommWireless_SetRenewTgid(renewTgid);
    Wireless_ClearBeaconCount();
    if (gCommWireless.work->portCallbackSet == 0) {
        Wireless_SetReceiveCallback(sub_02036904, 14);
        gCommWireless.work->portCallbackSet = 1;
    }
    gCommWireless.work->entryFlag = entryFlag;
    if (Wireless_GetState() == 1 && Wireless_StartMeasureChannels() != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL CommWireless_StartScan(int unused, BOOL clearParents) {
    CommBssid bssid;
    (void)unused;
    CommWireless_ResetFlags();
    if (clearParents) {
        CommWireless_ClearParents();
    }
    if (gCommWireless.work->portCallbackSet == 0) {
        Wireless_SetReceiveCallback(sub_020367A8, 14);
        gCommWireless.work->portCallbackSet = 1;
    }
    if (Wireless_GetState() == 1) {
        bssid = gCommWirelessAnyBssid;
        if (Wireless_StartScan(CommWireless_OnParentFound, &bssid, 0) != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

// Ends the scan / the link; TRUE once the manager is idle.
BOOL CommWireless_Stop(void) {
    if (gCommWireless.work == NULL) {
        return TRUE;
    }
    switch (gCommWireless.work->stopState) {
    case 0:
        if (Wireless_IsScanning() != 0) {
            Wireless_StopScan();
            gCommWireless.work->stopState = 1;
        } else if (Wireless_IsBusy() == 0) {
            Wireless_Stop();
            gCommWireless.work->stopState = 2;
        }
        break;
    case 1:
        if (Wireless_IsBusy() == 0) {
            Wireless_Stop();
            gCommWireless.work->stopState = 2;
        }
        break;
    case 2:
        if (Wireless_IsIdle() != 0) {
            return TRUE;
        }
        if (Wireless_HasError() != 0) {
            gCommWireless.work->stopState = 1;
        }
        break;
    }
    return FALSE;
}

BOOL CommWireless_RequestStopAndFree(void) {
    if (gCommWireless.work != NULL && gCommWireless.work->shutdown == 0) {
        gCommWireless.work->shutdown = 1;
        Wireless_Stop();
        return 1;
    }
    return 0;
}

void CommWireless_SetManagerEnded(BOOL end) {
    if (gCommWireless.work != NULL) {
        if (end) {
            gCommWireless.work->shutdown = 2;
        } else {
            gCommWireless.work->shutdown = 0;
            CommWireless_InitManager(1);
        }
    }
}

void CommWireless_Free(void) {
    Heap_Free(gCommWireless.work->ruleset);
    Heap_Free(gCommWireless.work->managerBuffer);
    Heap_Free(gCommWireless.work->beaconBuffer);
    Heap_Free(gCommWireless.work);
    gCommWireless.work = NULL;
}

int CommWireless_CountParents(void) {
    int i;
    u8 *ptr;
    int count;

    if (!sub_02037474()) {
        return 0;
    }
    count = 0;
    ptr = (u8 *)gCommWireless.work;
    for (i = 0; i < 16; i++, ptr += 2) {
        if (*(u16 *)(ptr + offsetof(CommWirelessWork, parentTimers)) != 0) {
            count++;
        }
    }
    return count;
}

int CommWireless_GetParentSlot(int ordinal) {
    int i;
    int count = 0;
    u8 *ptr;

    ptr = (u8 *)gCommWireless.work;
    for (i = 0; i < 16; i++, ptr += 2) {
        if (*(u16 *)(ptr + offsetof(CommWirelessWork, parentTimers)) != 0) {
            if (count == ordinal) {
                return i;
            }
            count++;
        }
    }
    GF_AssertFail();
    return 0;
}

u8 CommWireless_IsParentListChanged(void) {
    return gCommWireless.work->parentListChanged;
}

void CommWireless_ClearParentListChanged(void) {
    gCommWireless.work->parentListChanged = 0;
}

// The players connected to the parent in slot (at least 1), FALSE for a free slot.
BOOL CommWireless_GetParentMemberCount(int slot) {
    CommWirelessWork *work;
    u16 value;

    work = gCommWireless.work;
    value = work->parentTimers[slot];
    if (value != 0) {
        u8 count = work->parents[slot].beacon.memberCount;
        if (count == 0) {
            return TRUE;
        }
        return count;
    }
    return FALSE;
}

void CommWireless_CopyParentProfile(int ordinal, PlayerProfile *profile) {
    int i;
    int current = 0;
    u8 *ptr = (u8 *)gCommWireless.work;

    for (i = 0; i < 16; i++, ptr += 2) {
        if (*(u16 *)(ptr + offsetof(CommWirelessWork, parentTimers)) != 0) {
            if (ordinal == current) {
                PlayerProfile_Copy(CommWireless_GetParentProfile(i), profile);
                return;
            }
            current++;
        }
    }
}

BOOL CommWireless_ConnectToParent(int slot) {
    int state = Wireless_GetState();

    if (state == 2) {
        Wireless_StopScan();
        return FALSE;
    }
    if (Wireless_GetState() == 1) {
        int kind = CommState_GetLinkMode();
        u32 offset = slot * sizeof(CommParentInfo);
        gCommWireless.work->currentChannel = *(u16 *)((u8 *)gCommWireless.work + offset + offsetof(CommWirelessWork, parents[0].channel));
        if (sub_0203401C(kind)) {
            Wireless_ScanAndConnect(1, (u8 *)gCommWireless.work + offsetof(CommWirelessWork, parents[0].macAddress) + offset, 0);
        } else {
            Wireless_StartChild(1, (u8 *)gCommWireless.work + offsetof(CommWirelessWork, parents) + offset);
        }
        return TRUE;
    }
    return FALSE;
}

// Stores a parent the scan reported and counts down the others' timers.
void CommWireless_UpdateParents(void) {
    int i;
    int offset;

    CommWireless_StoreFoundParent();
    for (i = 0, offset = 0; i < 16; i++, offset += 2) {
        if (*(u16 *)((u8 *)gCommWireless.work + offset + offsetof(CommWirelessWork, parentTimers)) != 0) {
            if (*(u16 *)((u8 *)gCommWireless.work + offset + offsetof(CommWirelessWork, parentTimers)) != 0) {
                --*(u16 *)((u8 *)gCommWireless.work + offset + offsetof(CommWirelessWork, parentTimers));
                if (*(u16 *)((u8 *)gCommWireless.work + offset + offsetof(CommWirelessWork, parentTimers)) == 0) {
                    gCommWireless.work->parentListChanged = 1;
                }
            }
        }
    }
}

void CommWireless_BuildBeacon(void) {
    int kind = CommState_GetLinkMode();
    PlayerProfile *profile = CommWireless_GetProfile();

    if (kind != 15) {
        u8 *beacon = (u8 *)gCommWireless.work->beacon;
        if ((int)LinkBattleRuleset_sizeof() > 0x20) {
            GF_AssertFail();
        }
        if (PlayerProfile_sizeof() != 0x20) {
            GF_AssertFail();
        }
        MI_CpuCopy8(profile, beacon + 0x10, PlayerProfile_sizeof());
        MI_CpuCopy8(gCommWireless.work->ruleset, beacon + 0x30, LinkBattleRuleset_sizeof());
        *(u32 *)beacon = PlayerProfile_GetTrainerID(profile);
        beacon[4] = CommState_GetLinkMode();
        beacon[5] = sub_02039954();
        MI_CpuCopy8(&gCommWireless.work->mail, beacon + 8, 8);
        beacon += 0x54;
        *beacon = Wireless_IsEntryLocked();
    } else {
        u8 *beacon = (u8 *)gCommWireless.work->beacon;
        *(u32 *)beacon = PlayerProfile_GetTrainerID(profile);
        beacon[4] = CommState_GetLinkMode();
        beacon[5] = sub_02039954();
        MI_CpuCopy8(gCommWireless.work->customBeacon, beacon + 8, 0x54);
    }
    DC_FlushRange(gCommWireless.work->beacon, sizeof(CommBeacon));
    Wireless_SetParentGameInfo(gCommWireless.work->beacon, sizeof(CommBeacon));
}

void CommWireless_UpdateBeaconMemberCount(void) {
    CommBeacon *beacon = gCommWireless.work->beacon;

    if (beacon->memberCount != CommWireless_CountConnected()) {
        beacon->memberCount = CommWireless_CountConnected();
        DC_FlushRange(gCommWireless.work->beacon, sizeof(CommBeacon));
        Wireless_SetParentGameInfo(gCommWireless.work->beacon, sizeof(CommBeacon));
        Wireless_UpdateGameInfo(gCommWireless.work->beacon, sizeof(CommBeacon), (void *)gCommWireless.work->ggid, gCommWireless.tgid);
    }
}

// Per frame: notices a lost link (alone as parent, fewer players than at the start, players dropped, errors),
// follows the shutdown request, and starts the parent once the channels are measured.
void CommWireless_Update(int count, CommWirelessWork *unused) {
    int state = Wireless_GetState(count);
    int dropped = CommPlayerInfo_DropDisconnected();
    CommWireless_UpdateBeaconMemberCount();
    if (Wireless_GetAid() == 0 && CommWireless_GetConnectedChildren() == 0 && gCommWireless.work->lostWhenAlone) {
        gCommWireless.work->linkLost = 1;
    }
    if (gCommWireless.work->firstCount == 0xFFFF) {
        gCommWireless.work->firstCount = count;
    }
    if (gCommWireless.work->detectDrops) {
        if (gCommWireless.work->firstCount > (u32)count) {
            gCommWireless.work->linkLost = 1;
        }
        if (dropped != 0) {
            gCommWireless.work->linkLost = 1;
        }
    }
    if (Wireless_GetError() == 0x19) {
        CommState_ShowErrorAndHalt(0);
    }
    switch (state) {
    case 0:
        if (gCommWireless.work->shutdown == 1) {
            CommWireless_Free();
            return;
        }
        if (gCommWireless.work->shutdown == 2) {
            gCommWireless.work->shutdown = 3;
            return;
        }
        break;
    case 1:
        if (gCommWireless.work->shutdown == 1) {
            if (Wireless_End() != 0) {
                break;
            }
        }
        if (gCommWireless.work->shutdown == 2) {
            if (Wireless_End() == 0) {
                return;
            }
            return;
        }
        break;
    case 8:
    case 9:
        if (gCommWireless.work != NULL) {
            gCommWireless.work->linkLost = 1;
            return;
        }
        break;
    case 7: {
        u16 channel = Wireless_SelectChannel();
        u16 saved;
        int maxEntry;
        u32 beaconPeriod;
        if (gCommWireless.work->channelTimer == 0) {
            gCommWireless.work->channel = channel;
            gCommWireless.work->channelTimer = 5;
        } else {
            gCommWireless.work->channelTimer--;
        }
        saved = gCommWireless.work->channel;
        if (gCommWireless.work->renewTgid) {
            gCommWireless.tgid = WM_GetNextTgid();
        }
        CommWireless_BuildBeacon();
        maxEntry = sub_02033FC4((u16)CommState_GetLinkMode());
        beaconPeriod = CommWireless_GetBeaconPeriod((u16)CommState_GetLinkMode());
        Wireless_StartParent(0, gCommWireless.tgid, saved, maxEntry, beaconPeriod, gCommWireless.work->entryFlag);
        gCommWireless.work->currentChannel = saved;
        break;
    }
    }
    return;
}

BOOL CommWireless_Main(int count) {
    if (gCommWireless.work != NULL) {
        return ((BOOL(*)(int, CommWirelessWork *))CommWireless_Update)(count, gCommWireless.work);
    }
}

BOOL CommWireless_IsAidConnected(u16 aid) {
    if (gCommWireless.work == NULL) {
        return FALSE;
    }
    if (Wireless_GetState() != 4) {
        return FALSE;
    }
    if (Wireless_GetConnectedBitmap() & (1 << aid)) {
        return TRUE;
    }
    return FALSE;
}

int CommWireless_CountConnected(void) {
    int count = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (CommWireless_IsAidConnected((u16)i)) {
            count++;
        }
    }
    return count;
}

int CommWireless_IsManagerEnded(void) {
    if (gCommWireless.work != NULL && gCommWireless.work->shutdown == 3) {
        return 1;
    }
    return 0;
}

BOOL CommWireless_Exists(void) {
    return gCommWireless.work != NULL;
}

int CommWireless_IsIdle(void) {
    if (gCommWireless.work != NULL) {
        return Wireless_IsIdle(gCommWireless.work);
    }
    return 1;
}

int CommWireless_GetConnectedChildren(void) {
    if (gCommWireless.work != NULL) {
        return Wireless_GetConnectedBitmap(gCommWireless.work) & 0xFFFE;
    }
    return 0;
}

BOOL CommWireless_IsLinkLost(void) {
    if (gCommWireless.work != NULL) {
        if (gCommWireless.work->linkLost != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

void CommWireless_SetLostWhenAlone(BOOL value) {
    if (gCommWireless.work != NULL) {
        gCommWireless.work->lostWhenAlone = value;
    }
}

void CommWireless_SetDetectDrops(BOOL value) {
    if (gCommWireless.work != NULL) {
        gCommWireless.work->detectDrops = value;
        gCommWireless.work->firstCount = 0xFFFF;
    }
}

// A quarter of the beacon period for link kinds 9, 10 and 13.
u32 CommWireless_GetBeaconPeriod(u32 kind) {
    u16 period = WM_GetDispersionBeaconPeriod();

    GF_ASSERT(kind < 41);
    if (kind == 10) {
        return ((u32)period << 14) >> 16;
    }
    if (kind == 9 || kind == 13) {
        return ((u32)period << 14) >> 16;
    }
    return period;
}

void *CommWireless_GetParentInfo(int slot) {
    if (gCommWireless.work != NULL) {
        if (gCommWireless.work->parentTimers[slot] != 0) {
            return &gCommWireless.work->parents[slot];
        }
    }
    return NULL;
}

void *CommWireless_GetProfile(void) {
    return gCommWireless.work->profile;
}

void *CommWireless_GetParentProfile(int slot) {
    u8 *result;
    if (gCommWireless.work->parentTimers[slot] == 0) {
        return NULL;
    }
    result = (u8 *)&gCommWireless.work->parents[slot].beacon;
    return result + offsetof(CommBeacon, profile);
}

void CommWireless_SetMacAddress(void *macAddress, int index) {
    if (gCommWireless.work != NULL) {
        if (index >= 8) {
            GF_AssertFail();
        }
        MI_CpuCopy8(macAddress, gCommWireless.work->macAddresses[index], 6);
    }
}

BOOL sub_020357FC(void) {
    if (gCommWireless.work != NULL) {
        return gCommWireless.work->unk4;
    }
    return 0;
}

void sub_0203581C(void) {
    if (gCommWireless.work != NULL) {
        gCommWireless.work->unk4 = 1;
    }
}

void CommWireless_SetMail(void *mail) {
    MI_CpuCopy8(mail, &gCommWireless.work->mail, 8);
}

void CommWireless_SetRuleset(void *ruleset) {
    MI_CpuCopy8(ruleset, gCommWireless.work->ruleset, LinkBattleRuleset_sizeof());
}

void *CommWireless_GetRuleset(void) {
    return gCommWireless.work->ruleset;
}

void CommWireless_RefreshBeacon(void) {
    CommWireless_BuildBeacon();
    Wireless_UpdateGameInfo(gCommWireless.work->beacon, sizeof(CommBeacon), (void *)gCommWireless.work->ggid, gCommWireless.tgid);
}

void CommWireless_HasSentBeacons(void) {
    Wireless_HasSentBeacons();
}

void CommWireless_SetCustomBeacon(const void *src) {
    MI_CpuCopy8(src, gCommWireless.work->customBeacon, sizeof(gCommWireless.work->customBeacon));
    CommWireless_RefreshBeacon();
}

void *CommWireless_GetParentMail(int slot) {
    if (gCommWireless.work != NULL) {
        if (gCommWireless.work->parentTimers[slot] != 0) {
            u8 *result = (u8 *)&gCommWireless.work->parents[slot].beacon;
            return result + offsetof(CommBeacon, mail);
        }
    }
    return NULL;
}
