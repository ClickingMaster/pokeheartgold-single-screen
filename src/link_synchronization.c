#include "link_synchronization_internal.h"

static LinkSync *sLinkSync;

void LinkSync_Init(enum HeapID heap) {
    int i;
    if (sLinkSync == NULL) {
        sLinkSync = Heap_Alloc(heap, sizeof(LinkSync));
        MI_CpuFill8(sLinkSync, 0, sizeof(LinkSync));
    }
    for (i = 0; i < 8; i++) {
        sLinkSync->states[i] = 255;
    }
    sLinkSync->completed = 255;
    sLinkSync->requested = 255;
    sLinkSync->pending = 0;
    sLinkSync->resend = 0;
}

void LinkSync_Free(void) {
    Heap_Free(sLinkSync);
    sLinkSync = NULL;
}

BOOL LinkSync_Exists(void) {
    return sLinkSync != NULL;
}

void LinkSync_RecvRequest(int sender, int size, void *data, void *context) {
    u8 *request = data;
    u8 state = request[0];
    u8 packet[2];
    int i;
    if (Comm_GetLocalNetId() != 0) {
        return;
    }
    packet[0] = sender;
    packet[1] = state;
    Comm_SendFixedParentCommand(18, packet);
    sLinkSync->states[sender] = state;
    for (i = 0; i < 8; i++) {
        if (Comm_IsNetIdConnected((u16)i) && state != sLinkSync->states[i]) {
            return;
        }
    }
    if (Comm_SendFixedParentCommand(17, &state) == 0) {
        sLinkSync->resend = 1;
    }
}

void LinkSync_RecvState(int sender, int size, void *data, void *context) {
    u8 *state = data;
    sLinkSync->states[state[0]] = state[1];
}

void LinkSync_RecvDone(int sender, int size, void *data, void *context) {
    u8 *state = data;
    sLinkSync->completed = state[0];
}

void LinkSync_StartSync(u8 state) {
    sLinkSync->requested = state;
    sLinkSync->pending = 1;
}

void LinkSync_Update(void) {
    if (sLinkSync) {
        if (sLinkSync->pending && Comm_SendFixedCommand(16, &sLinkSync->requested)) {
            sLinkSync->pending = 0;
        }
        if (sLinkSync->resend && Comm_SendFixedParentCommand(17, sLinkSync->states)) {
            sLinkSync->resend = 0;
        }
    }
}

BOOL LinkSync_IsSynced(u8 state) {
    if (sLinkSync == NULL) {
        return TRUE;
    }
    if (sLinkSync->completed == state) {
        return TRUE;
    }
    return FALSE;
}

u8 LinkSync_GetPlayerState(int index) {
    return sLinkSync->states[index];
}

void LinkSync_RecvValue(int sender, int size, void *data, void *context) {
    u8 *value = data;
    sLinkSync->values[sender][0] = value[0];
    sLinkSync->values[sender][1] = value[1];
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
    if (sLinkSync == NULL) {
        return -1;
    }
    if (sLinkSync->values[sender][0] == key) {
        return sLinkSync->values[sender][1];
    }
    return -1;
}

void LinkSync_ClearValues(void) {
    int i;
    for (i = 0; i < 8; i++) {
        MI_CpuFill8(sLinkSync->values[i], 0, 2);
    }
}

void LinkSync_ClearPackets(void) {
    int i;
    for (i = 0; i < 8; i++) {
        sLinkSync->received[i] = 0;
    }
}

u32 LinkSync_SendPacket(u32 index, s16 *data) {
    if (sLinkSync) {
        MI_CpuCopy8(data, &sLinkSync->packets[index], 0x46);
        Comm_SendFixedCommand(20, &sLinkSync->packets[index]);
        return TRUE;
    }
    return FALSE;
}

u16 *LinkSync_GetPacket(s32 index) {
    if (sLinkSync->received[index]) {
        return (u16 *)&sLinkSync->packets[index];
    }
    return NULL;
}

void LinkSync_RecvPacket(int sender, int size, void *data, void *context) {
    sLinkSync->received[sender] = 1;
    MI_CpuCopy8(data, &sLinkSync->packets[sender], 0x46);
}
