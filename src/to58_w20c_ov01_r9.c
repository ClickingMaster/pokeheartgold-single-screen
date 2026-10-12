#include "global.h"
#include "heap.h"
#include "filesystem.h"

extern int MapMatrix_GetWidth(void *);
extern int MapMatrix_GetMatrixId(void *);
extern int MapMatrix_GetMapAltitude(void *, int, u16, u16, int);
void ov01_021F5FB8(int position, int width, void *matrix, int *vector);
extern int MapMatrix_GetHeight(void *);
extern void *MapLoadBuffers_New(int);
extern void ov01_021F5F34(void *);
extern void GF_AssertFail(void);
extern const u32 ov01_02206BC0[];
extern const u32 ov01_02206BD0[];
extern const u32 ov01_02206BC8[];
void *MapLoadManager_New(void *matrix, void *field, void *arg2, void *arg3, void *arg4, int mode, void *arg6);

void ov01_021F5FB8(int position, int width, void *matrix, int *vector) {
    vector[0] = 1 << 20;
    vector[2] = 1 << 20;
    if (position != -1) {
        u16 x = position % width;
        u16 y = position / width;
        int matrixWidth = MapMatrix_GetWidth(matrix);
        int matrixId = MapMatrix_GetMatrixId(matrix);
        vector[1] = MapMatrix_GetMapAltitude(matrix, matrixId, x, y, matrixWidth) << 15;
        vector[0] += x << 21;
        vector[2] += y << 21;
    }
}

#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
void *MapLoadManager_New(void *matrix, void *field, void *arg2, void *arg3, void *arg4, int mode, void *arg6) {
    u8 *manager = Heap_Alloc((enum HeapID)4, 0x110);
    int flag;
    MI_CpuFill8(manager, 0, 0x110);
    if (mode == 0) {
        W(manager, 0xfc) = (u32)ov01_02206BC0;
        flag = 1;
    } else if (mode == 1) {
        W(manager, 0xfc) = (u32)ov01_02206BD0;
        flag = 0;
    } else if (mode == 2) {
        W(manager, 0xfc) = (u32)ov01_02206BC8;
        flag = 1;
    } else {
        GF_AssertFail();
    }
    W(manager, 0x104) = (u32)arg6;
    W(manager, 0) = (u32)MapLoadBuffers_New(flag);
    W(manager, 0xb8) = (u32)field;
    W(manager, 0xc0) = (u32)matrix;
    W(manager, 0xbc) = (u32)arg2;
    W(manager, 0xc4) = MapMatrix_GetWidth(matrix);
    W(manager, 0xc8) = MapMatrix_GetHeight(matrix);
    W(manager, 0xcc) = W(manager, 0xc4) << 5;
    W(manager, 0xf4) = (u32)arg3;
    W(manager, 0xf8) = (u32)arg4;
    W(manager, 0xf0) = 1;
    ov01_021F5F34(manager);
    W(manager, 0xb0) = 0;
    W(manager, 0xb4) = 2;
    W(manager, 0x100) = (u32)NARC_New((NarcId)0x41, (enum HeapID)4);
    W(manager, 0x108) = 0;
    return manager;
}
