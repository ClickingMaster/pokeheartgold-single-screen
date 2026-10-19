#include "drawing_session_link_internal.h"

static void DrawingSessionLink_ReceivePictureChunk(int netId, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveMemberStroke(int index, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveStrokes(int index, int size, void *data, void *context);
static void DrawingSessionLink_ReceivePictureDone(int netId, int size, void *data, void *context);
static void DrawingSessionLink_HandleJoinRequest(int id, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveDummy103(int netId, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveMemberJoined(int netId, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveDummy99(int netId, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveDummy100(int netId, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveEvent15(int netId, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveJoinNotice(int value, int size, void *data, void *context);
static void DrawingSessionLink_ReceiveSetJoinState1(int netId, int size, void *data, void *context);
static void DrawingSessionLink_SendPictureChunk(DrawingSessionWork *work, int index);
static void *DrawingSessionLink_GetChunkRecvBuffer(int index, void *context, int size);
static u32 DrawingSessionLink_GetSize4(void);
static u32 DrawingSessionLink_GetJoinRequestSize(void);

static const CommCommand sDrawingSessionLinkCommands[] = {
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     DrawingSessionLink_GetSize4,           NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { DrawingSessionLink_ReceivePictureChunk,  CommCmd_GetSize1008,                   DrawingSessionLink_GetChunkRecvBuffer },
    { DrawingSessionLink_ReceiveMemberStroke,  CommCmd_GetSize10,                     NULL                                  },
    { DrawingSessionLink_ReceiveStrokes,       CommCmd_GetSize50,                     NULL                                  },
    { DrawingSessionLink_ReceiveDummy99,       CommCmd_GetSize1,                      NULL                                  },
    { DrawingSessionLink_ReceiveDummy100,      CommCmd_GetSize1,                      NULL                                  },
    { DrawingSessionLink_ReceiveMemberJoined,  CommCmd_GetSize1,                      NULL                                  },
    { DrawingSessionLink_ReceivePictureDone,   CommCmd_GetSize0,                      NULL                                  },
    { DrawingSessionLink_ReceiveDummy103,      CommCmd_GetSize0,                      NULL                                  },
    { DrawingSessionLink_HandleJoinRequest,    DrawingSessionLink_GetJoinRequestSize, NULL                                  },
    { DrawingSessionLink_ReceiveEvent15,       CommCmd_GetSize0,                      NULL                                  },
    { DrawingSessionLink_ReceiveJoinNotice,    CommCmd_GetSize0,                      NULL                                  },
    { DrawingSessionLink_ReceiveSetJoinState1, CommCmd_GetSize0,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
    { CommCmd_RecvIgnored,                     CommCmd_GetSize1,                      NULL                                  },
};

void DrawingSessionLink_RegisterCommands(void *arg) {
    Comm_RegisterCommandTable(sDrawingSessionLinkCommands, NELEMS(sDrawingSessionLinkCommands), arg);
}

static void DrawingSessionLink_ReceivePictureChunk(int netId, int size, void *data, void *context) {
    const DrawingSessionPictureChunk *chunk = data;
    DrawingSessionWork *work = context;

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

static void DrawingSessionLink_ReceiveMemberStroke(int index, int size, void *data, void *context) {
    const DrawingSessionStroke *stroke = data;
    DrawingSessionWork *work = context;

    if (index != 0) {
        ((DrawingSessionStroke *)work->strokeSendBuffer)[index] = *stroke;
    }
}

static void DrawingSessionLink_ReceiveStrokes(int index, int size, void *data, void *context) {
    const DrawingSessionStroke *strokes = data;
    DrawingSessionWork *work = context;

    if (work != NULL && index == 0) {
        int row = 0;
        do {
            work->strokes[row] = strokes[row];
            row++;
        } while (row < DRAWING_SESSION_MAX_MEMBERS);
    }
}

static void DrawingSessionLink_ReceivePictureDone(int netId, int size, void *data, void *context) {
    DrawingSessionWork *work = context;

    DrawingSession_HandleNetEvent(work, 3, 0);
    MIi_CpuClearFast(0, (u32 *)work->picture, 0x3840);
    if (Comm_GetLocalNetId() == 0) {
        work->connectedCount = Comm_CountConnected();
        work->connectedBitmap = Wireless_GetConnectedBitmap();
        work->joinState = 1;
    }
}

static void DrawingSessionLink_HandleJoinRequest(int id, int size, void *data, void *context) {
    const FourBytes *packet = data;
    DrawingSessionWork *work = context;
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

static void DrawingSessionLink_ReceiveDummy103(int netId, int size, void *data, void *context) {
}

static void DrawingSessionLink_ReceiveMemberJoined(int netId, int size, void *data, void *context) {
    const u8 *member = data;
    DrawingSessionWork *work = context;

    DrawingSession_HandleNetEvent(work, 1, member[0]);
    if (Comm_GetLocalNetId() == 0 && work->mainState == 1) {
        work->pictureChunkIndex = 0;
        DrawingSessionLink_SendPictureChunk(work, work->pictureChunkIndex);
    }
}

static void DrawingSessionLink_ReceiveDummy99(int netId, int size, void *data, void *context) {
}

static void DrawingSessionLink_ReceiveDummy100(int netId, int size, void *data, void *context) {
}

static void DrawingSessionLink_ReceiveEvent15(int netId, int size, void *data, void *context) {
    DrawingSessionWork *work = context;

    if (Comm_GetLocalNetId() != 0) {
        DrawingSession_HandleNetEvent(work, 15, 0);
    }
}

static void DrawingSessionLink_ReceiveJoinNotice(int value, int size, void *data, void *context) {
    DrawingSessionWork *work = context;
    u8 packet;

    if (Comm_GetLocalNetId() != 0) {
        GF_AssertFail();
    }
    if (Comm_GetLocalNetId() == 0) {
        if (work->firstJoinSeen != 0) {
            packet = value;
            Comm_SendParentCommand(0x7b, &packet, 1);
        } else {
            work->firstJoinSeen = 1;
        }
    }
    CommState_SetErrorWatch(0, 1);
}

static void DrawingSessionLink_ReceiveSetJoinState1(int netId, int size, void *data, void *context) {
    DrawingSessionWork *work = context;

    work->joinState = 1;
}

static void DrawingSessionLink_SendPictureChunk(DrawingSessionWork *work, int index) {
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

static void *DrawingSessionLink_GetChunkRecvBuffer(int index, void *context, int size) {
    DrawingSessionWork *work = context;

    return &work->recvChunks[index];
}

static u32 DrawingSessionLink_GetSize4(void) {
    return 4;
}

static u32 DrawingSessionLink_GetJoinRequestSize(void) {
    return 4;
}
