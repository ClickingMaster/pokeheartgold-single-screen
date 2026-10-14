#ifndef POKEHEARTGOLD_DRAWING_SESSION_LINK_INTERNAL_H
#define POKEHEARTGOLD_DRAWING_SESSION_LINK_INTERNAL_H

#include "global.h"
typedef struct DrawingSessionLinkWork {
    u8 raw[0x9400];
} DrawingSessionLinkWork;
typedef struct TenBytes {
    u8 data[10];
} TenBytes;
typedef struct WorkProgress {
    u8 pad[0x43cc];
    int progress;
} WorkProgress;
typedef struct FourBytes {
    u8 data[4];
} FourBytes;

#define U8(p, o)  (((u8 *)(p))[o])
#define U16(p, o) (*(u16 *)&((u8 *)(p))[o])
#define U32(p, o) (*(u32 *)&((u8 *)(p))[o])
#define S32(p, o) (*(int *)&((u8 *)(p))[o])
#define PTR(p, o) (*(void **)&((u8 *)(p))[o])

extern const u8 gDrawingSessionLinkCommands[];
void *Comm_RegisterCommandTable(const void *, int, void *);
int Comm_GetLocalNetId(void);
void Comm_SendParentCommand(int, const void *, int);
void CopyWindowToVram(void *);
void DrawingSession_HandleNetEvent(void *, int, int);
int Comm_CountConnected(void);
int Wireless_GetConnectedBitmap(void);
int DrawingSession_GetMemberCount(void);
int MATH_CountPopulation(u32);
void CommState_SetMaxEntries(int);
void GF_AssertFail(void);
void CommState_SetErrorWatch(int, int);
void Comm_SendParentCommandNoCopy(int, const void *, int);

void *DrawingSessionLink_RegisterCommands(void *);
void DrawingSessionLink_ReceivePictureChunk(void *, void *, void *, DrawingSessionLinkWork *);
void DrawingSessionLink_ReceiveMemberStroke(int, void *, const u8 *, DrawingSessionLinkWork *);
void DrawingSessionLink_ReceiveStrokes(int, void *, const u8 *, DrawingSessionLinkWork *);
void DrawingSessionLink_ReceivePictureDone(void *, void *, void *, DrawingSessionLinkWork *);
void DrawingSessionLink_HandleJoinRequest(int, void *, const FourBytes *, DrawingSessionLinkWork *);
void DrawingSessionLink_ReceiveDummy103(void);
void DrawingSessionLink_ReceiveMemberJoined(void *, void *, const u8 *, DrawingSessionLinkWork *);
void DrawingSessionLink_ReceiveDummy99(void);
void DrawingSessionLink_ReceiveDummy100(void);
void DrawingSessionLink_ReceiveEvent15(void *, void *, void *, DrawingSessionLinkWork *);
void DrawingSessionLink_ReceiveJoinNotice(int, void *, void *, DrawingSessionLinkWork *);
void DrawingSessionLink_ReceiveSetJoinState1(void *, void *, void *, DrawingSessionLinkWork *);
void DrawingSessionLink_SendPictureChunk(DrawingSessionLinkWork *, int);
void *DrawingSessionLink_GetChunkRecvBuffer(int, void *);
int DrawingSessionLink_GetSize4(void);
int DrawingSessionLink_GetJoinRequestSize(void);

#endif
