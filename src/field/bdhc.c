#include "field/bdhc.h"

#include "global.h"

#include "constants/heap.h"

#include "assert.h"
#include "geometry_math.h"
#include "heap.h"
#include "sys_task_api.h"

#define BDHC_MAX_HEIGHT_CANDIDATES 10
#define BDHC_BUFFER_SIZE           0x9000

enum BdhcHeightMode {
    BDHC_HEIGHT_NEAREST, // the height closest to the current one
    BDHC_HEIGHT_HIGHEST,
    BDHC_HEIGHT_LOWEST,
};

enum BdhcLoadState {
    BDHC_LOAD_HEADER,
    BDHC_LOAD_DATA,
    BDHC_LOAD_END,
};

BOOL Bdhc_IsPointInPlateBounds(BdhcPoint *first, BdhcPoint *second, BdhcPoint *point) {
    fx32 *minX;
    fx32 *maxX;
    fx32 *minZ;
    fx32 *maxZ;

    if (first->x <= second->x) {
        minX = &first->x;
        maxX = &second->x;
    } else {
        minX = &second->x;
        maxX = &first->x;
    }
    if (first->z <= second->z) {
        minZ = &first->z;
        maxZ = &second->z;
    } else {
        minZ = &second->z;
        maxZ = &first->z;
    }
    if (*minX <= point->x && point->x <= *maxX && *minZ <= point->z && point->z <= *maxZ) {
        return TRUE;
    }
    return FALSE;
}

void Bdhc_GetPlateCorners(Bdhc *bdhc, int plate, BdhcPoint *corners) {
    corners[0] = bdhc->points[bdhc->plates[plate].firstPoint];
    corners[1] = bdhc->points[bdhc->plates[plate].secondPoint];
}

void Bdhc_GetPlateNormal(Bdhc *bdhc, int plate, VecFx32 *normal) {
    *normal = bdhc->normals[bdhc->plates[plate].normal];
}

void Bdhc_GetPlateConstant(Bdhc *bdhc, int plate, fx32 *constant) {
    *constant = bdhc->constants[bdhc->plates[plate].constant];
}

void Bdhc_ResetHeightCandidates(BdhcHeightCandidate *candidates) {
    int i;

    for (i = 0; i < BDHC_MAX_HEIGHT_CANDIDATES; i++) {
        candidates[i].y = 0;
        candidates[i].unused4 = -1;
        candidates[i].unused8 = -1;
    }
}

// Binary search for the strip that contains z.
BOOL Bdhc_FindStrip(BdhcStrip *strips, int count, fx32 z, u16 *strip) {
    int lo;
    int mid;

    if (count == 0) {
        return FALSE;
    }
    if (count == 1) {
        *strip = 0;
        return TRUE;
    }
    count--;
    lo = 0;
    mid = count / 2;
    while (TRUE) {
        if (strips[mid].lowerBound > z) {
            count--;
            if (count > lo) {
                count = mid;
                mid = (lo + count) / 2;
            } else {
                *strip = mid;
                return TRUE;
            }
        } else {
            lo++;
            if (lo < count) {
                lo = mid;
                mid = (lo + count) / 2;
            } else {
                *strip = mid + 1;
                return TRUE;
            }
        }
    }
}

// The terrain height at (x, z). Where plates overlap, mode (enum BdhcHeightMode) picks one of the heights.
BOOL Bdhc_GetHeight(int mode, fx32 currentY, fx32 x, fx32 z, Bdhc *bdhc, fx32 *y) {
    BdhcStrip *strips;
    BdhcPoint point;
    BdhcPoint corners[2];
    VecFx32 normal;
    BdhcHeightCandidate candidates[BDHC_MAX_HEIGHT_CANDIDATES];
    u16 strip;
    fx32 constant;
    int n;
    int best;
    fx32 maxY;
    u16 j;
    u16 i;
    u16 count;
    u16 offset;
    int plate;
    fx32 cur;
    u16 k;
    fx32 bestVal;

    if (bdhc->loaded == FALSE) {
        return FALSE;
    }
    point.x = x;
    point.z = z;
    n = 0;
    Bdhc_ResetHeightCandidates(candidates);
    strips = bdhc->strips;
    if (!Bdhc_FindStrip(strips, (u16)bdhc->stripsCount, point.z, &strip)) {
        return FALSE;
    }
    count = strips[strip].count;
    offset = strips[strip].offset;
    i = 0;
    while (i < count) {
        plate = bdhc->accessList[offset + i];
        Bdhc_GetPlateCorners(bdhc, plate, corners);
        if (Bdhc_IsPointInPlateBounds(&corners[0], &corners[1], &point) == TRUE) {
            Bdhc_GetPlateNormal(bdhc, plate, &normal);
            Bdhc_GetPlateConstant(bdhc, plate, &constant);
            candidates[n].y = FX_Div(-(constant + (FX_Mul(normal.x, point.x) + FX_Mul(normal.z, point.z))), normal.y);
            n++;
            if (n >= BDHC_MAX_HEIGHT_CANDIDATES) {
                break;
            }
        }
        i++;
    }

    if (n > 1) {
        best = 0;
        switch (mode) {
        case BDHC_HEIGHT_HIGHEST:
            maxY = (fx32)0xFF000000;
            for (j = 0; j < n; j++) {
                cur = candidates[j].y;
                if (maxY < cur) {
                    maxY = cur;
                    best = j;
                }
            }
            break;
        case BDHC_HEIGHT_LOWEST:
            bestVal = (fx32)0x01000000;
            for (k = 0; k < n; k++) {
                cur = candidates[k].y;
                if (bestVal > cur) {
                    bestVal = cur;
                    best = k;
                }
            }
            break;
        case BDHC_HEIGHT_NEAREST:
        default:
            bestVal = Math_Max(currentY, candidates[0].y) - Math_Min(currentY, candidates[0].y);
            for (j = 1; j < n; j++) {
                cur = Math_Max(currentY, candidates[j].y) - Math_Min(currentY, candidates[j].y);
                if (bestVal > cur) {
                    bestVal = cur;
                    best = j;
                }
            }
            break;
        }
        *y = candidates[best].y;
        return TRUE;
    } else if (n == 1) {
        *y = candidates[0].y;
        return TRUE;
    } else if (n != 0) {
        *y = candidates[0].y;
        return TRUE;
    } else {
        return FALSE;
    }
}

void Bdhc_ReadHeader(NARC *narc, BdhcHeader *header) {
    u32 magic[2];

    MI_CpuClear32(header, sizeof(BdhcHeader));
    NARC_ReadFile(narc, 4, magic);
    NARC_ReadFile(narc, 2, &header->pointsCount);
    NARC_ReadFile(narc, 2, &header->normalsCount);
    NARC_ReadFile(narc, 2, &header->constantsCount);
    NARC_ReadFile(narc, 2, &header->platesCount);
    NARC_ReadFile(narc, 2, &header->stripsCount);
    NARC_ReadFile(narc, 2, &header->accessListCount);
}

void Bdhc_SetBuffers(BdhcHeader *header, Bdhc *bdhc, u8 **buffer) {
    s32 offset = 0;

    bdhc->points = (BdhcPoint *)*buffer;
    offset += header->pointsCount * sizeof(BdhcPoint);
    bdhc->normals = (VecFx32 *)(*buffer + offset);
    offset += header->normalsCount * sizeof(VecFx32);
    bdhc->constants = (fx32 *)(*buffer + offset);
    offset += header->constantsCount * sizeof(fx32);
    bdhc->plates = (BdhcPlate *)(*buffer + offset);
    offset += header->platesCount * sizeof(BdhcPlate);
    bdhc->strips = (BdhcStrip *)(*buffer + offset);
    offset += header->stripsCount * sizeof(BdhcStrip);
    bdhc->accessList = (u16 *)(*buffer + offset);
    offset += header->accessListCount * sizeof(u16);
    GF_ASSERT(offset <= BDHC_BUFFER_SIZE);
}

void Bdhc_ReadPoints(NARC *narc, Bdhc *bdhc, BdhcHeader *header) {
    NARC_ReadFile(narc, header->pointsCount * sizeof(BdhcPoint), bdhc->points);
}

void Bdhc_ReadNormals(NARC *narc, Bdhc *bdhc, BdhcHeader *header) {
    NARC_ReadFile(narc, header->normalsCount * sizeof(VecFx32), bdhc->normals);
}

void Bdhc_ReadConstants(NARC *narc, Bdhc *bdhc, BdhcHeader *header) {
    NARC_ReadFile(narc, header->constantsCount * sizeof(fx32), bdhc->constants);
}

void Bdhc_ReadPlates(NARC *narc, Bdhc *bdhc, BdhcHeader *header) {
    NARC_ReadFile(narc, header->platesCount * sizeof(BdhcPlate), bdhc->plates);
}

void Bdhc_ReadStrips(NARC *narc, Bdhc *bdhc, BdhcHeader *header) {
    NARC_ReadFile(narc, header->stripsCount * sizeof(BdhcStrip), bdhc->strips);
}

void Bdhc_ReadAccessList(NARC *narc, Bdhc *bdhc, BdhcHeader *header) {
    NARC_ReadFile(narc, header->accessListCount * sizeof(u16), bdhc->accessList);
}

void Task_LoadBdhc(SysTask *task, void *data) {
    BdhcLoadTask *loadTask = data;
    BOOL advance;

    if (loadTask->cancel == TRUE) {
        loadTask->state = BDHC_LOAD_END;
    }

    switch (loadTask->state) {
    case BDHC_LOAD_HEADER:
        if (*loadTask->waitTask != NULL) {
            advance = FALSE;
            break;
        }
        Bdhc_ReadHeader(loadTask->narc, &loadTask->header);
        loadTask->bdhc->stripsCount = loadTask->header.stripsCount;
        Bdhc_SetBuffers(&loadTask->header, loadTask->bdhc, &loadTask->buffer);
        advance = TRUE;
        break;
    case BDHC_LOAD_DATA:
        Bdhc_ReadPoints(loadTask->narc, loadTask->bdhc, &loadTask->header);
        Bdhc_ReadNormals(loadTask->narc, loadTask->bdhc, &loadTask->header);
        Bdhc_ReadConstants(loadTask->narc, loadTask->bdhc, &loadTask->header);
        Bdhc_ReadPlates(loadTask->narc, loadTask->bdhc, &loadTask->header);
        Bdhc_ReadStrips(loadTask->narc, loadTask->bdhc, &loadTask->header);
        Bdhc_ReadAccessList(loadTask->narc, loadTask->bdhc, &loadTask->header);
        advance = TRUE;
        break;
    case BDHC_LOAD_END:
        *loadTask->pending = 0;
        Heap_Free(loadTask);
        SysTask_Destroy(task);
        return;
    }

    if (advance == TRUE) {
        loadTask->state++;
        if (loadTask->state == BDHC_LOAD_END) {
            loadTask->bdhc->loaded = TRUE;
        }
    }
}

Bdhc *Bdhc_New(void) {
    Bdhc *bdhc = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Bdhc));

    bdhc->points = NULL;
    bdhc->normals = NULL;
    bdhc->plates = NULL;
    bdhc->strips = NULL;
    bdhc->accessList = NULL;
    bdhc->loaded = FALSE;
    bdhc->stripsCount = 0;
    return bdhc;
}

void Bdhc_Load(NARC *narc, u32 unused, Bdhc *bdhc, u8 *buffer) {
    BdhcHeader *header = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(BdhcHeader));

    Bdhc_ReadHeader(narc, header);
    bdhc->stripsCount = header->stripsCount;
    Bdhc_SetBuffers(header, bdhc, &buffer);
    Bdhc_ReadPoints(narc, bdhc, header);
    Bdhc_ReadNormals(narc, bdhc, header);
    Bdhc_ReadConstants(narc, bdhc, header);
    Bdhc_ReadPlates(narc, bdhc, header);
    Bdhc_ReadStrips(narc, bdhc, header);
    Bdhc_ReadAccessList(narc, bdhc, header);
    Heap_Free(header);
    bdhc->loaded = TRUE;
}

void Bdhc_Free(void *bdhc) {
    if (bdhc != NULL) {
        Heap_Free(bdhc);
    }
}

void Bdhc_Reset(Bdhc *bdhc) {
    if (bdhc != NULL) {
        bdhc->loaded = FALSE;
        bdhc->points = NULL;
        bdhc->normals = NULL;
        bdhc->plates = NULL;
        bdhc->strips = NULL;
        bdhc->accessList = NULL;
    }
}

SysTask *Bdhc_StartLoadTask(NARC *narc, u32 unused, Bdhc *bdhc, u32 *pending, u8 **buffer, SysTask **waitTask) {
    BdhcLoadTask *loadTask = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(BdhcLoadTask));

    loadTask->state = BDHC_LOAD_HEADER;
    loadTask->narc = narc;
    loadTask->unused80 = unused;
    loadTask->bdhc = bdhc;
    loadTask->pending = pending;
    loadTask->cancel = FALSE;
    loadTask->unused64 = 0;
    loadTask->unused48 = 0;
    loadTask->buffer = *buffer;
    loadTask->waitTask = waitTask;
    return SysTask_CreateOnMainQueue(Task_LoadBdhc, loadTask, 1);
}

void Bdhc_CancelLoadTask(SysTask *task) {
    BdhcLoadTask *loadTask = SysTask_GetData(task);

    loadTask->cancel = TRUE;
}

void Bdhc_SetUnloaded(Bdhc *bdhc) {
    bdhc->loaded = FALSE;
}
