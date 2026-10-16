#ifndef POKEHEARTGOLD_DRAWING_SESSION_LINK_INTERNAL_H
#define POKEHEARTGOLD_DRAWING_SESSION_LINK_INTERNAL_H

#include "global.h"

#include "drawing_session.h"
typedef struct FourBytes {
    u8 data[4];
} FourBytes;

extern const u8 gDrawingSessionLinkCommands[];
void *Comm_RegisterCommandTable(const void *, int, void *);
int Comm_GetLocalNetId(void);
void Comm_SendParentCommand(int, const void *, int);
int Comm_CountConnected(void);
int Wireless_GetConnectedBitmap(void);
int MATH_CountPopulation(u32);
void CommState_SetMaxEntries(int);
void GF_AssertFail(void);
void CommState_SetErrorWatch(int, int);
void Comm_SendParentCommandNoCopy(int, const void *, int);

void *DrawingSessionLink_RegisterCommands(void *);
void DrawingSessionLink_ReceivePictureChunk(void *, void *, const DrawingSessionPictureChunk *, DrawingSessionWork *);
void DrawingSessionLink_ReceiveMemberStroke(int, void *, const DrawingSessionStroke *, DrawingSessionWork *);
void DrawingSessionLink_ReceiveStrokes(int, void *, const DrawingSessionStroke *, DrawingSessionWork *);
void DrawingSessionLink_ReceivePictureDone(void *, void *, void *, DrawingSessionWork *);
void DrawingSessionLink_HandleJoinRequest(int, void *, const FourBytes *, DrawingSessionWork *);
void DrawingSessionLink_ReceiveDummy103(void);
void DrawingSessionLink_ReceiveMemberJoined(void *, void *, const u8 *, DrawingSessionWork *);
void DrawingSessionLink_ReceiveDummy99(void);
void DrawingSessionLink_ReceiveDummy100(void);
void DrawingSessionLink_ReceiveEvent15(void *, void *, void *, DrawingSessionWork *);
void DrawingSessionLink_ReceiveJoinNotice(int, void *, void *, DrawingSessionWork *);
void DrawingSessionLink_ReceiveSetJoinState1(void *, void *, void *, DrawingSessionWork *);
void DrawingSessionLink_SendPictureChunk(DrawingSessionWork *, int);
DrawingSessionPictureChunk *DrawingSessionLink_GetChunkRecvBuffer(int, DrawingSessionWork *);
int DrawingSessionLink_GetSize4(void);
int DrawingSessionLink_GetJoinRequestSize(void);

#endif
