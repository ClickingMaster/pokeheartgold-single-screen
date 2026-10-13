#include "link_synchronization_internal.h"

void LinkSync_Init(enum HeapID heap) {
    int i;
    if (gLinkSync == NULL) {
        gLinkSync = Heap_Alloc(heap, sizeof(LinkSync));
        MI_CpuFill8(gLinkSync, 0, sizeof(LinkSync));
    }
    for (i = 0; i < 8; i++) {
        gLinkSync->states[i] = 255;
    }
    gLinkSync->completed = 255;
    gLinkSync->requested = 255;
    gLinkSync->pending = 0;
    gLinkSync->resend = 0;
}

void LinkSync_Free(void) {
    Heap_Free(gLinkSync);
    gLinkSync = NULL;
}

BOOL LinkSync_Exists(void) {
    return gLinkSync != NULL;
}

void LinkSync_RecvRequest(int sender, int unused, u8 *data) {
    u8 state = data[0];
    u8 packet[2];
    int i;
    if (Comm_GetLocalNetId() != 0) {
        return;
    }
    packet[0] = sender;
    packet[1] = state;
    Comm_SendFixedParentCommand(18, packet);
    gLinkSync->states[sender] = state;
    for (i = 0; i < 8; i++) {
        if (Comm_IsNetIdConnected((u16)i) && state != gLinkSync->states[i]) {
            return;
        }
    }
    if (Comm_SendFixedParentCommand(17, &state) == 0) {
        gLinkSync->resend = 1;
    }
}

void LinkSync_RecvState(int sender, int unused, u8 *data) {
    gLinkSync->states[data[0]] = data[1];
}

void LinkSync_RecvDone(int sender, int unused, u8 *data) {
    gLinkSync->completed = data[0];
}

void LinkSync_StartSync(u8 state) {
    gLinkSync->requested = state;
    gLinkSync->pending = 1;
}

void LinkSync_Update(void) {
    if (gLinkSync) {
        if (gLinkSync->pending && Comm_SendFixedCommand(16, &gLinkSync->requested)) {
            gLinkSync->pending = 0;
        }
        if (gLinkSync->resend && Comm_SendFixedParentCommand(17, gLinkSync->states)) {
            gLinkSync->resend = 0;
        }
    }
}

BOOL LinkSync_IsSynced(u8 state) {
    if (gLinkSync == NULL) {
        return TRUE;
    }
    if (gLinkSync->completed == state) {
        return TRUE;
    }
    return FALSE;
}

u8 LinkSync_GetPlayerState(int index) {
    return gLinkSync->states[index];
}

void LinkSync_RecvValue(int sender, int unused, u8 *data) {
    gLinkSync->values[sender][0] = data[0];
    gLinkSync->values[sender][1] = data[1];
}

u32 LinkSync_GetValueSize(void) {
    return 2;
}

void LinkSync_SendValue(u32 key, u8 value) {
    u8 packet[2];
    packet[0] = key;
    packet[1] = value;
    Comm_SendFixedCommand(19, packet);
}

s32 LinkSync_GetValue(s32 sender, s32 key) {
    if (gLinkSync == NULL) {
        return -1;
    }
    if (gLinkSync->values[sender][0] == key) {
        return gLinkSync->values[sender][1];
    }
    return -1;
}

void LinkSync_ClearValues(void) {
    int i;
    for (i = 0; i < 8; i++) {
        MI_CpuFill8(gLinkSync->values[i], 0, 2);
    }
}

void LinkSync_ClearPackets(void) {
    int i;
    for (i = 0; i < 8; i++) {
        gLinkSync->received[i] = 0;
    }
}

u32 LinkSync_SendPacket(u32 index, s16 *data) {
    if (gLinkSync) {
        MI_CpuCopy8(data, &gLinkSync->packets[index], 0x46);
        Comm_SendFixedCommand(20, &gLinkSync->packets[index]);
        return TRUE;
    }
    return FALSE;
}

u16 *LinkSync_GetPacket(s32 index) {
    if (gLinkSync->received[index]) {
        return (u16 *)&gLinkSync->packets[index];
    }
    return NULL;
}

void LinkSync_RecvPacket(int sender, int unused, void *data) {
    gLinkSync->received[sender] = 1;
    MI_CpuCopy8(data, &gLinkSync->packets[sender], 0x46);
}
