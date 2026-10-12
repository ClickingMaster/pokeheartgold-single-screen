#include "global.h"

#include "communication_club_link_internal.h"
#include "communication_player_state.h"
#include "script_manager.h"
#include "trainer_card_data.h"

// Declared with the types their call sites were matched against (no header, or a header with other types).
void sub_0200FC60(int screen, u16 color);
int LinkSync_GetPlayerState(int index);
void sub_020399FC(int, void *);

void CommClubLink_StartCardExchange(FieldSystem *fieldSystem);

void sub_02058034(void) {
}

void CommClubLink_Init(FieldSystem *fieldSystem) {
    if (gCommClubLink == NULL) {
        sub_02091574(fieldSystem);
        gCommClubLink = Heap_Alloc((enum HeapID)15, sizeof(CommClubLink));
        MI_CpuFill8(gCommClubLink, 0, sizeof(CommClubLink));
        gCommClubLink->timer = 50;
        gCommClubLink->task = SysTask_CreateOnMainQueue(CommClubLink_Task, NULL, 10);
        gCommClubLink->fieldSystem = fieldSystem;
        gCommClubLink->party = NULL;
        sub_02058034();
        CommRandom_Init(gCommClubLink->pad18);
    }
}

void CommClubLink_Free(void) {
    int i;
    if (gCommClubLink) {
        SysTask_Destroy(gCommClubLink->task);
        for (i = 0; i < 4; i++) {
            if (gCommClubLink->cards[i]) {
                Heap_Free(gCommClubLink->cards[i]);
            }
        }
        if (gCommClubLink->party) {
            Heap_Free(gCommClubLink->party);
        }
        Heap_Free(gCommClubLink);
        gCommClubLink = NULL;
    }
}

void sub_020580E4(FieldSystem *fieldSystem, s32 arg1, s32 arg2) {
    if (sub_02037474() == 0) {
        sub_02037D8C(FieldSystem_GetSaveData(fieldSystem), arg1, arg2, fieldSystem->linkBattleRuleset, 0);
        CommClubLink_Init(fieldSystem);
        CommClubLink_SetState(sub_02058304, 0);
    }
}

void sub_02058124(FieldSystem *fieldSystem, u8 arg1, u32 arg2) {
    if (sub_02037474() == 0) {
        sub_02037DD4(FieldSystem_GetSaveData(fieldSystem), arg1, arg2, fieldSystem->linkBattleRuleset, 0);
        CommClubLink_Init(fieldSystem);
        CommClubLink_SetState(sub_0205832C, 0);
    }
}

void sub_02058164(u16 mode) {
    gCommClubLink->mode = mode;
    CommClubLink_SetState(sub_0205834C, 0);
}

void sub_02058180(void) {
    CommClubLink_SetState(sub_020588A0, 0);
}

CommClubLink *gCommClubLink;

void CommClubLink_StartCardExchange(FieldSystem *fieldSystem) {
    int i;
    int netId;

    sub_0200FC60(0, 0);
    sub_0200FC60(1, 0);
    sub_020398D4(1, 1);
    if (sub_02037D78()) {
        if (gCommClubLink == NULL) {
            CommClubLink_Init(fieldSystem);
            gCommClubLink->host = 1;
        } else {
            gCommClubLink->host = 0;
        }
        gCommClubLink->ready = 0;
        netId = Comm_GetLocalNetId();
        CommPlayerInfo_GetProfile(Comm_GetLocalNetId());
        for (i = 0; i < Comm_CountConnected(); i++) {
            if (gCommClubLink->cards[i] == NULL) {
                gCommClubLink->cards[i] = Heap_Alloc((enum HeapID)0, 0x66C);
            }
        }
        TrainerCardData_Build(0, 0, 0, 0xFF, gCommClubLink->fieldSystem, gCommClubLink->cards[netId]);
        LinkSync_StartSync(0x5F);
        CommClubLink_SetState(CommClubLink_SendCard, 0);
    }
}

BOOL sub_02058258(void) {
    if (gCommClubLink->state == CommClubLink_SendCard) {
        return FALSE;
    }
    if (gCommClubLink->state != sub_02058930) {
        return TRUE;
    }
    return FALSE;
}

void sub_02058284(void) {
    if (gCommClubLink) {
        sub_020398D4(0, 0);
        CommClubLink_SetState(sub_02058A60, 5);
    }
}

void sub_020582A8(void) {
    if (gCommClubLink) {
        LinkSync_StartSync(91);
        CommClubLink_SetState(sub_02058A38, 5);
    }
}

void CommClubLink_Task(SysTask *task, void *unused) {
    if (gCommClubLink == NULL) {
        SysTask_Destroy(task);
        return;
    }
    if (gCommClubLink->state && gCommClubLink->paused == 0) {
        gCommClubLink->state();
    }
}

void CommClubLink_SetState(CommClubLinkState state, u16 timer) {
    gCommClubLink->state = state;
    gCommClubLink->timer = timer;
}

void sub_02058304(void) {
    Comm_GetLocalNetId();
    if (((BOOL(*)(void))Comm_IsNetIdConnected)()) {
        ov03_02255C54();
        CommPlayerInfo_SendLocal();
        CommClubLink_SetState(sub_02058328, 0);
    }
}

void sub_02058328(void) {
}

void sub_0205832C(void) {
    if (CommWireless_IsStartedUp()) {
        ov03_02255BEC();
        CommClubLink_SetState(sub_02058348, 0);
    }
}

void sub_02058348(void) {
}

void sub_0205834C(void) {
    sub_02037E1C(gCommClubLink->mode);
    CommClubLink_SetState(sub_0205836C, 0);
}

void sub_0205836C(void) {
    if (sub_02037EC8()) {
        CommPlayerInfo_SendLocal();
        CommClubLink_SetState(sub_02058388, 0);
    }
}

void sub_02058388(void) {
}

void sub_0205838C(void) {
    if (LinkSync_IsSynced(98)) {
        void *data = Heap_Alloc((enum HeapID)15, CommPlayerManager_sizeof());
        CommPlayerManager_Init(data, gCommClubLink->fieldSystem, 0);
        sub_02057F80();
        sub_02037504();
        LinkSync_StartSync(92);
        CommClubLink_SetState(sub_020583EC, 0);
        return;
    }
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    gCommClubLink->timer = 30;
    LinkSync_StartSync(98);
}

void sub_020583EC(void) {
    if (Comm_GetLocalNetId() == 0) {
        CommPlayerInfo_HostRedistribute();
    }
    if (LinkSync_IsSynced(92)) {
        BeginNormalPaletteFade((enum FadeMode)0, (enum FadeType)1, (enum FadeType)1, 0, 6, 1, (enum HeapID)4);
        ResetVisibleHardwareWindows((PMLCDTarget)0);
        ResetVisibleHardwareWindows((PMLCDTarget)1);
        sub_02056E60();
        CommPlayerManager_SendLocalPosition(0);
        CommClubLink_SetState(sub_02058478, 1);
    }
}

void sub_02058444(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    if (LinkSync_IsSynced(30)) {
        sub_020374E4();
        CommClubLink_SetState(sub_02058608, 0);
    }
}

void sub_02058478(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    if (Comm_GetLocalNetId() == 0) {
        if (sub_02057E50() == 0) {
            return;
        }
        LinkSync_StartSync(30);
    } else {
        LinkSync_StartSync(30);
    }
    CommClubLink_SetState(sub_02058444, 20);
}

void sub_020584BC(void) {
    if (LinkSync_IsSynced(98)) {
        void *data = Heap_Alloc((enum HeapID)15, CommPlayerManager_sizeof());
        CommPlayerManager_Init(data, gCommClubLink->fieldSystem, 0);
        sub_02057F80();
        LinkSync_StartSync(92);
        CommClubLink_SetState(sub_02058518, 0);
        return;
    }
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    gCommClubLink->timer = 30;
    LinkSync_StartSync(98);
}

void sub_02058518(void) {
    if (Comm_GetLocalNetId() == 0) {
        CommPlayerInfo_HostRedistribute();
    }
    if (LinkSync_IsSynced(92)) {
        u8 flag;
        sub_02056E60();
        CommPlayerManager_SendLocalPosition(0);
        flag = 1;
        sub_020376E0(94, &flag);
        BeginNormalPaletteFade((enum FadeMode)0, (enum FadeType)1, (enum FadeType)1, 0, 6, 1, (enum HeapID)4);
        ResetVisibleHardwareWindows((PMLCDTarget)0);
        ResetVisibleHardwareWindows((PMLCDTarget)1);
        CommClubLink_SetState(sub_020586EC, 0);
    }
}

void sub_0205857C(void) {
    int i;

    for (i = 0; i < Comm_CountConnected(); i++) {
        if (i != Comm_GetLocalNetId() && LinkSync_GetPlayerState(i) == 0x5e && gCommClubLink->fieldSystem->taskman == NULL && gCommClubLink->fieldSystem->unkD2_0 == 0) {
            int j;
            for (j = 0; j < 4; j++) {
                if (gCommClubLink->cards[j] != NULL) {
                    Heap_Free(gCommClubLink->cards[j]);
                    gCommClubLink->cards[j] = NULL;
                }
            }
            StartMapSceneScript(gCommClubLink->fieldSystem, 0x238e, 0);
        }
    }
    sub_020399FC(4, gCommClubLink->fieldSystem->bgConfig);
}

void sub_02058608(void) {
    if (sub_02057C94() == 0) {
        u8 flag;
        gCommClubLink->ready = 0;
        flag = 1;
        sub_020376E0(94, &flag);
        CommClubLink_SetState(sub_020586EC, 0);
    }
    sub_0205857C();
}

void sub_02058640(int start, Party *party) {
    if (party) {
        gCommClubLink->party = SaveArray_Party_Alloc((enum HeapID)11);
        Party_Copy(party, gCommClubLink->party);
    }
    if (start) {
        CommClubLink_SetState(sub_02058870, 3);
        return;
    }
    {
        u8 flag = 3;
        sub_020376E0(94, &flag);
        CommClubLink_SetState(sub_02058608, 0);
    }
}

void sub_02058690(void) {
    CommClubLink_SetState(sub_02058608, 0);
}

void sub_020586A0(void) {
    if (sub_02037958() == 0 && CommPlayerManager_GetMoveTimer(Comm_GetLocalNetId()) == 0) {
        if (gCommClubLink->timer) {
            gCommClubLink->timer--;
            return;
        }
        sub_02057F58();
        Colosseum_StartEntry(gCommClubLink->fieldSystem, sub_02058640);
        CommClubLink_SetState(sub_02058690, 0);
    }
}

void sub_020586EC(void) {
    if (gCommClubLink->ready) {
        u8 flag;
        CommClubLink_SetState(sub_020586A0, 5);
        flag = 0;
        sub_020376E0(94, &flag);
    }
    sub_0205857C();
}

void sub_02058720(int sender, int unused, u8 *data) {
    if (*data == Comm_GetLocalNetId()) {
        gCommClubLink->ready = 1;
    }
}

BOOL sub_02058740(void) {
    if (gCommClubLink && (gCommClubLink->state == sub_020586EC || gCommClubLink->state == sub_02058608)) {
        return TRUE;
    }
    return FALSE;
}

void sub_0205876C(void) {
    int battleType;
    u8 data[8];
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    battleType = 5;
    switch (sub_0203993C()) {
    case 4:
    case 5:
        battleType = 15;
        break;
    case 2:
    case 38:
        battleType = 7;
        break;
    }
    sub_02039980(data);
    if (gCommClubLink->party == NULL) {
        sub_02051598(gCommClubLink->fieldSystem, data, battleType);
    } else {
        sub_020515FC(gCommClubLink->fieldSystem, gCommClubLink->party, battleType);
        Heap_Free(gCommClubLink->party);
        gCommClubLink->party = NULL;
    }
    CommClubLink_Free();
}

void sub_020587E8(void) {
    if (sub_02036010()) {
        if (gCommClubLink->timer) {
            gCommClubLink->timer--;
        }
        if (gCommClubLink->timer == 90) {
            LinkSync_StartSync(4);
        }
        if (LinkSync_IsSynced(4)) {
            CommClubLink_SetState(sub_0205876C, 0);
        }
    }
}

void sub_0205882C(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    sub_02035FD8();
    CommClubLink_SetState(sub_020587E8, 120);
}

void sub_02058854(void) {
    if (LinkSync_IsSynced(3)) {
        CommClubLink_SetState(sub_0205882C, 2);
    }
}

void sub_02058870(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    CommPlayerManager_Delete(0);
    LinkSync_StartSync(3);
    CommClubLink_SetState(sub_02058854, 0);
}

void sub_020588A0(void) {
    sub_02037E38();
    CommClubLink_SetState(sub_020588B4, 2);
}

void sub_020588B4(void) {
    if (CommWireless_IsIdle()) {
        CommClubLink_SetState(sub_0205836C, 10);
    }
}

void CommClubLink_SetCardReceived(int index) {
    gCommClubLink->received[index] = 1;
}

void *CommClubLink_GetCard(int index) {
    GF_ASSERT(index < 4);
    return gCommClubLink->cards[index];
}

void CommClubLink_SendCard(void) {
    int id = Comm_GetLocalNetId();
    if (LinkSync_IsSynced(95)) {
        sub_02036FD8(88, gCommClubLink->cards[id], 0x66c);
        CommClubLink_SetState(sub_02058930, 0);
    }
}

void sub_02058930(void) {
    int i;
    for (i = 0; i < Comm_CountConnected(); i++) {
        if (gCommClubLink->received[i] == 0) {
            return;
        }
    }
    LinkSync_StartSync(97);
    CommClubLink_SetState(sub_020589D8, 0);
}

void sub_0205896C(void) {
    if (sub_02036010() == 0) {
        if (gCommClubLink->timer) {
            gCommClubLink->timer--;
        }
        if (gCommClubLink->timer == 90) {
            LinkSync_StartSync(5);
        }
        if (LinkSync_IsSynced(5)) {
            CommClubLink_SetState(sub_020589F4, 0);
        }
    }
}

void sub_020589B0(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    sub_02035FE4();
    CommClubLink_SetState(sub_0205896C, 120);
}

void sub_020589D8(void) {
    if (LinkSync_IsSynced(97)) {
        CommClubLink_SetState(sub_020589B0, 2);
    }
}

void sub_020589F4(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    LinkSync_StartSync(98);
    if (gCommClubLink->host) {
        CommClubLink_SetState(sub_0205838C, 30);
        return;
    }
    CommClubLink_SetState(sub_020584BC, 30);
}

void sub_02058A38(void) {
    if (LinkSync_IsSynced(91)) {
        sub_020398D4(0, 0);
        CommPlayerManager_Delete(1);
        CommClubLink_SetState(sub_02058A78, 5);
    }
}

void sub_02058A60(void) {
    CommPlayerManager_Delete(1);
    CommClubLink_SetState(sub_02058A78, 5);
}

void sub_02058A78(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    sub_02037E9C();
    CommClubLink_SetState(CommClubLink_Free, 0);
}

BOOL sub_02058AA0(void) {
    if (gCommClubLink == NULL) {
        return FALSE;
    }
    if (gCommClubLink->state == sub_02058870 || gCommClubLink->state == sub_02058854 || gCommClubLink->state == sub_0205882C || gCommClubLink->state == sub_020587E8 || gCommClubLink->state == sub_0205876C) {
        return TRUE;
    }
    return FALSE;
}
