#include <string.h>

#include "global.h"

typedef struct OverlayManager OverlayManager;

extern void *OverlayManager_GetArgs(OverlayManager *);
extern void *OverlayManager_CreateAndGetData(OverlayManager *, u32, u32);
extern void *OverlayManager_GetData(OverlayManager *);
extern void OverlayManager_FreeData(OverlayManager *);
extern void Heap_Create(u32, u32, u32);
extern void Heap_Destroy(u32);
extern void PlazaGameRoster_Init(void *, u8, u32, u8, void *);
extern void ov91_0225CBE4(void *, u32);
extern int PlazaGameRoster_CheckConnectionLost(void *);
extern void ov91_0225CC38(void *);
extern void sub_020398D4(int, int);
extern void sub_02037AC0(int);
extern int sub_02037B38(int);
extern int Comm_CountConnected(void);
extern int sub_020347A0(void);

int ov91_0225C540(OverlayManager *manager, int *state);
int ov91_0225C9EC(OverlayManager *manager, int *state);

int ov91_0225C540(OverlayManager *manager, int *state) {
    u8 *args = OverlayManager_GetArgs(manager);
    u8 *work;
    Heap_Create(3, 0x6A, 3 << 17);
    work = OverlayManager_CreateAndGetData(manager, 0x88, 0x6A);
    memset(work, 0, 0x88);
    PlazaGameRoster_Init(work + 8, args[0x39], *(u32 *)(args + 0x34), args[0x38], args);
    ov91_0225CBE4(work, 0x6A);
    return 1;
}
