#include "drawing_session_link_internal.h"

void *DrawingSessionLink_RegisterCommands(void *arg) {
    return Comm_RegisterCommandTable(gDrawingSessionLinkCommands, 0x6e, arg);
}

void DrawingSessionLink_ReceivePictureChunk(void *a, void *b, void *data, DrawingSessionLinkWork *work) {
    if (Comm_GetLocalNetId() != 0) {
        int offset = U8(data, 0x3ec) * 0x3e8;
        if (offset > 0x3840) {
            MIi_CpuCopyFast((const u32 *)data, (u32 *)(&work->raw[0x43d0] + offset), 0x190);
        } else {
            MIi_CpuCopyFast((const u32 *)data, (u32 *)(&work->raw[0x43d0] + offset), 0x3e8);
        }
        MIi_CpuCopyFast((const u32 *)&work->raw[0x43d0], (u32 *)PTR(work, 0x2d4), 0x3840);
        CopyWindowToVram(&work->raw[0x2c8]);
    } else {
        WorkProgress *progress = (WorkProgress *)work;
        if (progress->progress * 0x3e8 < 0x3840) {
            progress->progress++;
            DrawingSessionLink_SendPictureChunk(work, U32(work, 0x43cc));
        } else {
            Comm_SendParentCommand(0x7c, 0, 0);
        }
    }
}

void sub_0208F8BC(int index, void *unused, const u8 *src, DrawingSessionLinkWork *work) {
    if (index != 0) {
        work = (DrawingSessionLinkWork *)((u8 *)work + index * 10);
        *(TenBytes *)&work->raw[0x93bd] = *(const TenBytes *)src;
    }
}

void sub_0208F8E4(int index, void *unused, const u8 *src, DrawingSessionLinkWork *work) {
    if (work != 0 && index == 0) {
        int row = 0;
        do {
            *(TenBytes *)&work->raw[0x4382] = *(const TenBytes *)src;
            row++;
            src += 10;
            work = (DrawingSessionLinkWork *)((u8 *)work + 10);
        } while (row < 5);
    }
}

void sub_0208F918(void *a, void *b, void *c, DrawingSessionLinkWork *work) {
    DrawingSession_HandleNetEvent(work, 3, 0);
    MIi_CpuClearFast(0, (u32 *)&work->raw[0x43d0], 0x3840);
    if (Comm_GetLocalNetId() == 0) {
        U32(work, 0x318) = Comm_CountConnected();
        U32(work, 0x31c) = Wireless_GetConnectedBitmap();
        U32(work, 0x93f4) = 1;
    }
}

void DrawingSessionLink_HandleJoinRequest(int id, void *unused, const FourBytes *packet, DrawingSessionLinkWork *work) {
    FourBytes local;
    if (id != 0) {
        if (Comm_GetLocalNetId() != 0) {
            return;
        }
        local = *packet;
        local.data[0] = id;
        local.data[1] = U32(work, 0x318);
        switch (packet->data[2]) {
        case 0:
            if (Comm_CountConnected() != U32(work, 0x318) || DrawingSession_GetMemberCount() != U32(work, 0x318) || MATH_CountPopulation(Wireless_GetConnectedBitmap()) != U32(work, 0x318)) {
                local.data[3] = 0;
            } else {
                U32(work, 0x93b4) |= 1 << id;
                local.data[3] = 1;
                CommState_SetMaxEntries(Comm_CountConnected());
            }
            break;
        case 1:
            break;
        }
        Comm_SendParentCommand(0x7e, &local, 4);
    } else {
        int sender;
        switch (packet->data[2]) {
        case 0:
            sender = packet->data[0];
            if (sender != Comm_GetLocalNetId()) {
                return;
            }
            if (packet->data[3] == 0) {
                DrawingSession_HandleNetEvent(work, 9, sender);
            } else {
                U16(work, 0x93b8) = packet->data[1];
                DrawingSession_HandleNetEvent(work, 8, packet->data[0]);
            }
            break;
        case 1:
            DrawingSession_HandleNetEvent(work, 0x15, packet->data[0]);
            break;
        default:
            return;
        }
    }
}

void sub_0208FA50(void) {
}

void sub_0208FA54(void *a, void *b, const u8 *data, DrawingSessionLinkWork *work) {
    DrawingSession_HandleNetEvent(work, 1, data[0]);
    if (Comm_GetLocalNetId() == 0 && U32(work, 0x300) == 1) {
        U32(work, 0x43cc) = 0;
        DrawingSessionLink_SendPictureChunk(work, U32(work, 0x43cc));
    }
}

void sub_0208FA88(void) {
}

void sub_0208FA8C(void) {
}

void sub_0208FA90(void *a, void *b, void *c, DrawingSessionLinkWork *work) {
    if (Comm_GetLocalNetId() != 0) {
        DrawingSession_HandleNetEvent(work, 15, 0);
    }
}

void sub_0208FAA8(int value, void *b, void *c, DrawingSessionLinkWork *work) {
    u8 packet;
    if (Comm_GetLocalNetId() != 0) {
        GF_AssertFail();
    }
    if (Comm_GetLocalNetId() == 0) {
        if (U32(work, 0x93fc) != 0) {
            packet = value;
            Comm_SendParentCommand(0x7b, &packet, 1);
        } else {
            U32(work, 0x93fc) = 1;
        }
    }
    CommState_SetErrorWatch(0, 1);
}

void sub_0208FAEC(void *a, void *b, void *c, DrawingSessionLinkWork *work) {
    U32(work, 0x93f4) = 1;
}

void DrawingSessionLink_SendPictureChunk(DrawingSessionLinkWork *work, int index) {
    MIi_CpuCopyFast((const u32 *)((u8 *)PTR(work, 0x2d4) + index * 0x3e8), (u32 *)&work->raw[0x7c10], 0x3e8);
    {
        int i;
        u32 checksum;
        u32 *p;
        checksum = 0;
        i = checksum;
        p = (u32 *)&work->raw[0x7c10];
        do {
            checksum ^= *p;
            p++;
            i++;
        } while (i < 0xfa);
        U32(work, 0x7ff8) = checksum;
    }
    U8(work, 0x7ffc) = index;
    Comm_SendParentCommandNoCopy(0x76, &work->raw[0x7c10], 0x3f0);
}

void *DrawingSessionLink_GetChunkRecvBuffer(int index, void *base) {
    return (u8 *)base + 0x8000 + index * 0x3f0;
}

int sub_0208FB5C(void) {
    return 4;
}

int sub_0208FB60(void) {
    return 4;
}
