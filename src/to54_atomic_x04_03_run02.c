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

int ov91_0225C9EC(OverlayManager *manager, int *state) {
    void *work = OverlayManager_GetData(manager);
    int done;
    OverlayManager_GetArgs(manager);
    switch (*state) {
    case 0:
        done = PlazaGameRoster_CheckConnectionLost((u8 *)work + 8);
        ov91_0225CC38(work);
        OverlayManager_FreeData(manager);
        Heap_Destroy(0x6A);
        sub_020398D4(0, 1);
        if (done == 1) {
            return 1;
        }
        sub_02037AC0(4);
        (*state)++;
        break;
    case 1:
        if (sub_02037B38(4) != 0 || Comm_CountConnected() < sub_020347A0()) {
            return 1;
        }
        break;
    }
    return 0;
}
