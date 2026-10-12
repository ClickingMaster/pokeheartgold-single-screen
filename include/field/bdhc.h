#ifndef POKEHEARTGOLD_FIELD_BDHC_H
#define POKEHEARTGOLD_FIELD_BDHC_H

#include "global.h"

#include "filesystem.h"
#include "sys_task.h"

// A map's terrain height data (BDHC): sloped plates over the x/z plane, sorted into strips along z.

// A corner of a plate's bounding rectangle.
typedef struct BdhcPoint {
    fx32 x;
    fx32 z;
} BdhcPoint;

typedef struct BdhcPlate {
    u16 firstPoint; // the two opposite corners of the plate's bounding rectangle
    u16 secondPoint;
    u16 normal;
    u16 constant; // plane: normal . (x, y, z) + constant = 0
} BdhcPlate;

// The plates (access list entries [offset, offset + count)) of the strip that starts at lowerBound along z.
typedef struct BdhcStrip {
    fx32 lowerBound;
    u16 count;
    u16 offset;
} BdhcStrip;

// Element counts at the start of the file (after a 4-byte magic).
typedef struct BdhcHeader {
    u32 pointsCount;
    u32 normalsCount;
    u32 constantsCount;
    u32 platesCount;
    u32 stripsCount;
    u32 accessListCount;
} BdhcHeader;

// The arrays point into one buffer the caller provides (at most 0x9000 bytes).
typedef struct Bdhc {
    BdhcPlate *plates;
    fx32 *constants;
    BdhcStrip *strips;
    u16 *accessList;
    BdhcPoint *points;
    VecFx32 *normals;
    BOOL loaded;
    u32 stripsCount;
} Bdhc;

// A height found under a position.
typedef struct BdhcHeightCandidate {
    fx32 y;
    s32 unused4;
    s32 unused8;
} BdhcHeightCandidate;

// Environment of Task_LoadBdhc.
typedef struct BdhcLoadTask {
    u8 unused0[0x48];
    u32 unused48;
    BdhcHeader header;
    u32 unused64;
    u8 state;
    u8 *buffer;
    Bdhc *bdhc;
    BOOL cancel;
    u32 *pending;     // cleared when the task ends
    NARC *narc;
    u32 unused80;
    SysTask **waitTask; // the task waits until this slot is empty
} BdhcLoadTask;

BOOL Bdhc_IsPointInPlateBounds(BdhcPoint *first, BdhcPoint *second, BdhcPoint *point);
void Bdhc_GetPlateCorners(Bdhc *bdhc, int plate, BdhcPoint *corners);
void Bdhc_GetPlateNormal(Bdhc *bdhc, int plate, VecFx32 *normal);
void Bdhc_GetPlateConstant(Bdhc *bdhc, int plate, fx32 *constant);
void Bdhc_ResetHeightCandidates(BdhcHeightCandidate *candidates);
BOOL Bdhc_FindStrip(BdhcStrip *strips, int count, fx32 z, u16 *strip);
BOOL Bdhc_GetHeight(int mode, fx32 currentY, fx32 x, fx32 z, Bdhc *bdhc, fx32 *y);
void Bdhc_ReadHeader(NARC *narc, BdhcHeader *header);
void Bdhc_SetBuffers(BdhcHeader *header, Bdhc *bdhc, u8 **buffer);
void Bdhc_ReadPoints(NARC *narc, Bdhc *bdhc, BdhcHeader *header);
void Bdhc_ReadNormals(NARC *narc, Bdhc *bdhc, BdhcHeader *header);
void Bdhc_ReadConstants(NARC *narc, Bdhc *bdhc, BdhcHeader *header);
void Bdhc_ReadPlates(NARC *narc, Bdhc *bdhc, BdhcHeader *header);
void Bdhc_ReadStrips(NARC *narc, Bdhc *bdhc, BdhcHeader *header);
void Bdhc_ReadAccessList(NARC *narc, Bdhc *bdhc, BdhcHeader *header);
void Task_LoadBdhc(SysTask *task, void *data);
Bdhc *Bdhc_New(void);
void Bdhc_Load(NARC *narc, u32 unused, Bdhc *bdhc, u8 *buffer);
void Bdhc_Free(void *bdhc);
void Bdhc_Reset(Bdhc *bdhc);
SysTask *Bdhc_StartLoadTask(NARC *narc, u32 unused, Bdhc *bdhc, u32 *pending, u8 **buffer, SysTask **waitTask);
void Bdhc_CancelLoadTask(SysTask *task);
void Bdhc_SetUnloaded(Bdhc *bdhc);

#endif // POKEHEARTGOLD_FIELD_BDHC_H
