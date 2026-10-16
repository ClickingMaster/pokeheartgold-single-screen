#include "drawing_session_link_internal.h"

void *DrawingSessionLink_RegisterCommands(void *arg) {
    return Comm_RegisterCommandTable(gDrawingSessionLinkCommands, 0x6e, arg);
}

void DrawingSessionLink_ReceivePictureChunk(void *a, void *b, const DrawingSessionPictureChunk *chunk, DrawingSessionWork *work) {
    if (Comm_GetLocalNetId() != 0) {
        int offset = chunk->index * 0x3e8;
        if (offset > 0x3840) {
            MIi_CpuCopyFast((const u32 *)chunk->data, (u32 *)(work->picture + offset), 0x190);
        } else {
            MIi_CpuCopyFast((const u32 *)chunk->data, (u32 *)(work->picture + offset), 0x3e8);
        }
        MIi_CpuCopyFast((const u32 *)work->picture, (u32 *)work->canvasWindow.pixelBuffer, 0x3840);
        CopyWindowToVram(&work->canvasWindow);
    } else {
        if (work->pictureChunkIndex * 0x3e8 < 0x3840) {
            work->pictureChunkIndex++;
            DrawingSessionLink_SendPictureChunk(work, work->pictureChunkIndex);
        } else {
            Comm_SendParentCommand(0x7c, 0, 0);
        }
    }
}

void DrawingSessionLink_ReceiveMemberStroke(int index, void *unused, const DrawingSessionStroke *stroke, DrawingSessionWork *work) {
    if (index != 0) {
        ((DrawingSessionStroke *)work->strokeSendBuffer)[index] = *stroke;
    }
}

void DrawingSessionLink_ReceiveStrokes(int index, void *unused, const DrawingSessionStroke *strokes, DrawingSessionWork *work) {
    if (work != NULL && index == 0) {
        int row = 0;
        do {
            work->strokes[row] = strokes[row];
            row++;
        } while (row < DRAWING_SESSION_MAX_MEMBERS);
    }
}

void DrawingSessionLink_ReceivePictureDone(void *a, void *b, void *c, DrawingSessionWork *work) {
    DrawingSession_HandleNetEvent(work, 3, 0);
    MIi_CpuClearFast(0, (u32 *)work->picture, 0x3840);
    if (Comm_GetLocalNetId() == 0) {
        work->connectedCount = Comm_CountConnected();
        work->connectedBitmap = Wireless_GetConnectedBitmap();
        work->joinState = 1;
    }
}

void DrawingSessionLink_HandleJoinRequest(int id, void *unused, const FourBytes *packet, DrawingSessionWork *work) {
    FourBytes local;
    if (id != 0) {
        if (Comm_GetLocalNetId() != 0) {
            return;
        }
        local = *packet;
        local.data[0] = id;
        local.data[1] = work->connectedCount;
        switch (packet->data[2]) {
        case 0:
            if (Comm_CountConnected() != work->connectedCount || DrawingSession_GetMemberCount() != work->connectedCount || MATH_CountPopulation(Wireless_GetConnectedBitmap()) != work->connectedCount) {
                local.data[3] = 0;
            } else {
                work->leavingMask |= 1 << id;
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
                work->assignedCount = packet->data[1];
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

void DrawingSessionLink_ReceiveDummy103(void) {
}

void DrawingSessionLink_ReceiveMemberJoined(void *a, void *b, const u8 *data, DrawingSessionWork *work) {
    DrawingSession_HandleNetEvent(work, 1, data[0]);
    if (Comm_GetLocalNetId() == 0 && work->mainState == 1) {
        work->pictureChunkIndex = 0;
        DrawingSessionLink_SendPictureChunk(work, work->pictureChunkIndex);
    }
}

void DrawingSessionLink_ReceiveDummy99(void) {
}

void DrawingSessionLink_ReceiveDummy100(void) {
}

void DrawingSessionLink_ReceiveEvent15(void *a, void *b, void *c, DrawingSessionWork *work) {
    if (Comm_GetLocalNetId() != 0) {
        DrawingSession_HandleNetEvent(work, 15, 0);
    }
}

void DrawingSessionLink_ReceiveJoinNotice(int value, void *b, void *c, DrawingSessionWork *work) {
    u8 packet;
    if (Comm_GetLocalNetId() != 0) {
        GF_AssertFail();
    }
    if (Comm_GetLocalNetId() == 0) {
        if (work->unk_93FC != 0) {
            packet = value;
            Comm_SendParentCommand(0x7b, &packet, 1);
        } else {
            work->unk_93FC = 1;
        }
    }
    CommState_SetErrorWatch(0, 1);
}

void DrawingSessionLink_ReceiveSetJoinState1(void *a, void *b, void *c, DrawingSessionWork *work) {
    work->joinState = 1;
}

void DrawingSessionLink_SendPictureChunk(DrawingSessionWork *work, int index) {
    MIi_CpuCopyFast((const u32 *)((u8 *)work->canvasWindow.pixelBuffer + index * 0x3e8), (u32 *)work->sendChunk.data, 0x3e8);
    {
        int i;
        u32 checksum;
        u32 *p;
        checksum = 0;
        i = checksum;
        p = (u32 *)work->sendChunk.data;
        do {
            checksum ^= *p;
            p++;
            i++;
        } while (i < 0xfa);
        work->sendChunk.checksum = checksum;
    }
    work->sendChunk.index = index;
    Comm_SendParentCommandNoCopy(0x76, &work->sendChunk, sizeof(DrawingSessionPictureChunk));
}

DrawingSessionPictureChunk *DrawingSessionLink_GetChunkRecvBuffer(int index, DrawingSessionWork *work) {
    return &work->recvChunks[index];
}

int DrawingSessionLink_GetSize4(void) {
    return 4;
}

int DrawingSessionLink_GetJoinRequestSize(void) {
    return 4;
}
