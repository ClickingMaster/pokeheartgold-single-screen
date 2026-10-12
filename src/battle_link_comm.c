#include "battle_link_comm.h"

void BattleLink_Start(BattleSystem *battle) {
    BattleLinkTask *send, *recv;
    if (BattleSystem_GetBattleSpecial(battle) & 0x10) {
        return;
    }
    send = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleLinkTask));
    recv = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleLinkTask));
    Comm_RegisterCommandTable(gBattleLinkCommands, 12, battle);
    send->battle = battle;
    send->state = 0;
    recv->battle = battle;
    recv->state = 0;
    ov12_0223BBFC(battle, &send->state);
    ov12_0223BC08(battle, &recv->state);
    SysTask_CreateOnMainQueue(BattleLink_SendTask, send, 0);
    SysTask_CreateOnMainQueue(BattleLink_RecvTask, recv, 0);
}

void BattleLink_RegisterSetupCommands(BattleLinkWork *work) {
    Comm_RegisterCommandTable(gBattleLinkCommands, 12, work);
}

u32 BattleLink_GetAiFlagsSize(void) {
    return 4;
}

u32 BattleLink_GetProfileSize(void) {
    return PlayerProfile_sizeof();
}

u32 BattleLink_GetPartySize(void) {
    return PartyCore_sizeof();
}

u32 BattleLink_GetChatotSize(void) {
    return 1000;
}

u32 BattleLink_GetTrainerSize(void) {
    return sizeof(Trainer);
}

void *BattleLink_GetProfileRecvBuffer(int sender, BattleLinkWork *work) {
    BattleSetup *setup = work->setup;
    if (setup->battleType & 0x80) {
        return setup->profile[sender * 2];
    }
    return setup->profile[sender];
}

void *BattleLink_GetTrainerRecvBuffer(int sender, BattleLinkWork *work) {
    BattleSetup *setup = work->setup;
    if (setup->battleType & 0x80) {
        return &setup->trainer[sender * 2];
    }
    return &setup->trainer[sender];
}

void *BattleLink_GetPartyRecvBuffer(int sender, BattleLinkWork *work) {
    BattleSetup *setup = work->setup;
    if (setup->battleType & 0x80) {
        return setup->party[sender * 2];
    }
    return setup->party[sender];
}

void *BattleLink_GetChatotRecvBuffer(int sender, BattleLinkWork *work) {
    BattleSetup *setup = work->setup;
    if (setup->battleType & 0x80) {
        return setup->chatot[sender * 2];
    }
    return setup->chatot[sender];
}

void *BattleLink_GetTrainer1RecvBuffer(int sender, BattleLinkWork *work) {
    return &work->setup->trainer[1];
}

void *BattleLink_GetTrainer3RecvBuffer(int sender, BattleLinkWork *work) {
    return &work->setup->trainer[3];
}

void *BattleLink_GetParty1RecvBuffer(int sender, BattleLinkWork *work) {
    return work->setup->party[1];
}

void *BattleLink_GetParty3RecvBuffer(int sender, BattleLinkWork *work) {
    return work->setup->party[3];
}

void *BattleLink_GetPalPadRecvBuffer(int sender, BattleLinkWork *work) {
    return work->friends[sender];
}

const u32 gBattleLinkCommands[] = {
    (u32)BattleLink_RecvEnd,
    (u32)CommCmd_GetSizeVariable,
    0,
    (u32)BattleLink_RecvData,
    (u32)CommCmd_GetSizeVariable,
    0,
    (u32)BattleLink_RecvAiFlags,
    (u32)BattleLink_GetAiFlagsSize,
    0,
    (u32)BattleLink_RecvProfile,
    (u32)BattleLink_GetProfileSize,
    (u32)BattleLink_GetProfileRecvBuffer,
    (u32)BattleLink_RecvTrainer,
    (u32)BattleLink_GetTrainerSize,
    (u32)BattleLink_GetTrainerRecvBuffer,
    (u32)BattleLink_RecvParty,
    (u32)BattleLink_GetPartySize,
    (u32)BattleLink_GetPartyRecvBuffer,
    (u32)BattleLink_RecvChatot,
    (u32)BattleLink_GetChatotSize,
    (u32)BattleLink_GetChatotRecvBuffer,
    (u32)BattleLink_RecvPartnerTrainer,
    (u32)BattleLink_GetTrainerSize,
    (u32)BattleLink_GetTrainer1RecvBuffer,
    (u32)BattleLink_RecvPartnerTrainer,
    (u32)BattleLink_GetTrainerSize,
    (u32)BattleLink_GetTrainer3RecvBuffer,
    (u32)BattleLink_RecvPartnerParty,
    (u32)BattleLink_GetPartySize,
    (u32)BattleLink_GetParty1RecvBuffer,
    (u32)BattleLink_RecvPartnerParty,
    (u32)BattleLink_GetPartySize,
    (u32)BattleLink_GetParty3RecvBuffer,
    (u32)BattleLink_RecvPalPad,
    (u32)BattleLink_GetPalPadSize,
    (u32)BattleLink_GetPalPadRecvBuffer,
};

void BattleLink_QueueSendData(BattleSystem *battle, int command, int battler, u8 *data, u8 size) {
    u8 *header;
    u8 *buffer;
    u16 *write;
    u16 *end;
    int i;

    header = Heap_Alloc(HEAP_ID_BATTLE, 4);
    buffer = BattleSystem_GetSendBufferPtr(battle);
    write = ov12_0223A960(battle);
    end = ov12_0223A96C(battle);
    if ((u32)(*write + 5 + size) > 0x1000) {
        *end = *write;
        *write = 0;
    }
    header[0] = command;
    header[1] = battler;
    *(u16 *)&header[2] = size;
    for (i = 0; (u32)i < 4; i++) {
        buffer[*write] = header[i];
        (*write)++;
    }
    for (i = 0; i < size; i++) {
        buffer[*write] = data[i];
        (*write)++;
    }
    Heap_Free(header);
}

void BattleLink_RecvData(int sender, int size, u8 *data, BattleSystem *battle) {
    int i;
    u8 *buffer = BattleSystem_GetRecvBufferPtr(battle);
    u16 *write = ov12_0223A984(battle);
    u16 *end = ov12_0223A990(battle);
    if (*write + size + 1 > 4096) {
        *end = *write;
        *write = 0;
    }
    for (i = 0; i < size; i++) {
        buffer[*write] = data[i];
        (*write)++;
    }
}

BOOL BattleLink_SendAiFlags(BattleLinkWork *work, u32 value, ...) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    if (LinkSync_IsSynced(51) == 0) {
        return FALSE;
    }
    return Comm_SendCommand(24, &value, 4);
}

void BattleLink_RecvAiFlags(int sender, int unused, u32 *data, BattleLinkWork *work) {
    work->setup->aiFlags[sender] = *data;
    BattleRecord_SetAiFlags(sender, work->setup->aiFlags[sender]);
    work->received++;
}

BOOL BattleLink_PrepareProfile(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    PlayerProfile_Copy(work->setup->profile[0], (PlayerProfile *)&work->buffer);
    return TRUE;
}

BOOL BattleLink_SendProfile(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    if (LinkSync_IsSynced(52) == 0) {
        return FALSE;
    }
    return sub_02036FD8(25, &work->buffer, PlayerProfile_sizeof());
}

void BattleLink_RecvProfile(int sender, int unused, void *data, BattleLinkWork *work) {
    work->received++;
}

BOOL BattleLink_PrepareTrainer(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    work->buffer.trainer = work->setup->trainer[0];
    return TRUE;
}

BOOL BattleLink_SendTrainer(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    if (LinkSync_IsSynced(53) == 0) {
        return FALSE;
    }
    return sub_02036FD8(26, &work->buffer, 52);
}

void BattleLink_RecvTrainer(int sender, int unused, void *data, BattleLinkWork *work) {
    work->received++;
}

BOOL BattleLink_PrepareParty(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    Party_Copy(work->setup->party[0], (Party *)&work->buffer);
    return TRUE;
}

BOOL BattleLink_SendParty(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    if (LinkSync_IsSynced(54) == 0) {
        return FALSE;
    }
    return sub_02036FD8(27, &work->buffer, PartyCore_sizeof());
}

void BattleLink_RecvParty(int sender, int unused, void *data, BattleLinkWork *work) {
    work->received++;
}

BOOL BattleLink_PrepareChatot(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    Chatot_Copy(&work->buffer.chatot, work->setup->chatot[0]);
    return TRUE;
}

BOOL BattleLink_SendChatot(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    if (LinkSync_IsSynced(55) == 0) {
        return FALSE;
    }
    return sub_02036FD8(28, &work->buffer, 1000);
}

BOOL BattleLink_PreparePalPad(BattleLinkWork *work) {
    BattleSetup *setup;
    int index;
    int i;
    if (sub_02037190() != 264) {
        return FALSE;
    }
    setup = work->setup;
    if (setup->battleType & 0x80) {
        index = Comm_GetLocalNetId() * 8;
    } else {
        index = Comm_GetLocalNetId() * 4;
    }
    BattleLink_BuildPalPad(*(PlayerProfile **)((u8 *)setup->profile + index), setup->palPad, (SavePalPad *)&work->buffer);
    for (i = 0; i < 4; i++) {
        work->friends[i] = Heap_Alloc(HEAP_ID_BATTLE, sizeof(SavePalPad));
    }
    return TRUE;
}

BOOL BattleLink_SendPalPad(BattleLinkWork *work) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    if (LinkSync_IsSynced(56) == 0) {
        return FALSE;
    }
    return sub_02036FD8(33, &work->buffer, 1000);
}

void BattleLink_RecvChatot(int sender, int unused, void *data, BattleLinkWork *work) {
    work->received++;
}

BOOL BattleLink_PreparePartnerTrainer(BattleLinkWork *work, int index) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    work->buffer.trainer = work->setup->trainer[index];
    return TRUE;
}

BOOL BattleLink_SendPartnerTrainer(BattleLinkWork *work, int index, int command) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    if (!LinkSync_IsSynced((u8)command)) {
        return FALSE;
    }
    if (index == 1) {
        return sub_02036FD8(29, &work->buffer, 52);
    }
    return sub_02036FD8(30, &work->buffer, 52);
}

void BattleLink_RecvPartnerTrainer(int sender, int unused, void *data, BattleLinkWork *work) {
    work->received++;
}

BOOL BattleLink_PreparePartnerParty(BattleLinkWork *work, int index) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    Party_Copy(work->setup->party[index], (Party *)&work->buffer);
    return TRUE;
}

BOOL BattleLink_SendPartnerParty(BattleLinkWork *work, int index, int command) {
    if (sub_02037190() != 264) {
        return FALSE;
    }
    if (!LinkSync_IsSynced((u8)command)) {
        return FALSE;
    }
    if (index == 1) {
        return sub_02036FD8(31, &work->buffer, PartyCore_sizeof());
    }
    return sub_02036FD8(32, &work->buffer, PartyCore_sizeof());
}

void BattleLink_RecvPartnerParty(int sender, int unused, void *data, BattleLinkWork *work) {
    work->received++;
}

void BattleLink_SendTask(SysTask *task, void *ptr) {
    BattleLinkTask *work = ptr;
    u8 *buffer = BattleSystem_GetSendBufferPtr(work->battle);
    u16 *read = ov12_0223A954(work->battle);
    u16 *write = ov12_0223A960(work->battle);
    u16 *end = ov12_0223A96C(work->battle);
    switch (work->state) {
    case 0: {
        int size;
        if (sub_02037190() != 264) {
            break;
        }
        if (*read == *write) {
            break;
        }
        if (*read == *end) {
            *read = 0;
            *end = 0;
        }
        size = (buffer[*read + 2] | (buffer[*read + 3] << 8)) + 4;
        if (Comm_SendCommand(23, &buffer[*read], size) == TRUE) {
            *read += size;
        }
        break;
    }
    case 255:
    default:
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void BattleLink_RecvTask(SysTask *task, void *ptr) {
    BattleLinkTask *work = ptr;
    u8 *buffer = BattleSystem_GetRecvBufferPtr(work->battle);
    u16 *read = ov12_0223A978(work->battle);
    u16 *write = ov12_0223A984(work->battle);
    u16 *end = ov12_0223A990(work->battle);
    switch (work->state) {
    case 0:
        if (*read == *write) {
            break;
        }
        if (*read == *end) {
            *read = 0;
            *end = 0;
        }
        if (BattleController_RecvLinkData(work->battle, &buffer[*read]) == TRUE) {
            *read += (buffer[*read + 2] | (buffer[*read + 3] << 8)) + 4;
        }
        break;
    case 255:
    default:
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void BattleLink_RecvEnd(int sender, int unused, void *data, BattleSystem *battle) {
    ov12_0223BC14(battle, 255);
    ov12_0223BC20(battle, 255);
    ov12_0223BC2C(battle, 1);
}

void BattleLink_BuildPalPad(PlayerProfile *profile, SavePalPad *friends, SavePalPad *out) {
    int i;
    CopyU16StringArray(out->name, PlayerProfile_GetNamePtr(profile));
    out->otid = PlayerProfile_GetTrainerID(profile);
    out->filler_14[0] = PlayerProfile_GetLanguage(profile);
    out->filler_14[1] = PlayerProfile_GetVersion(profile);
    out->filler_14[2] = PlayerProfile_GetTrainerGender(profile);
    for (i = 0; i < 16; i++) {
        out->unk_18[i] = friends[i].otid;
        out->unk_58[i] = friends[i].filler_14[1];
        out->unk_68[i] = friends[i].filler_14[0];
        out->filler_78[i] = friends[i].filler_14[2];
    }
}

void BattleLink_RecvPalPad(int sender, int unused, SavePalPad *data, BattleLinkWork *work) {
    if (sender != Comm_GetLocalNetId()) {
        SavePalPad_Merge(work->setup->palPad, data, 1, HEAP_ID_BATTLE);
    }
    work->received++;
}

u32 BattleLink_GetPalPadSize(void) {
    return 0x88;
}
