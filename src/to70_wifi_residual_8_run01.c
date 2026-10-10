#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef struct Socket Socket;

struct Socket {
    u8 pad[0x70];
    s16 flags;
    u8 pad72[10];
    Socket *next;
};

typedef struct Config {
    u8 pad[0x1c];
    void (*free)(void *);
    u8 pad20[8];
    void *external;
} Config;

extern Config *_02110628;
extern void *_021D4424;
extern void *_021D4670;
extern Socket *_021D442C;
extern Socket *_021D448C;
extern void *_021D4490;
extern u8 _021D4430[];
int SOCL_CalmDown(void);
int SOCLi_CleanupCommandPacketQueue(void);
void CPS_Cleanup(void);
void CPS_SetScavengerCallback(void (*)(void));
int SOCL_Close(Socket *);
int SOCL_Cleanup(void);
int SOCL_CloseAll(void);

int SOCL_Cleanup(void) {
    int result;
    if (!_021D4424) {
        _021D4424 = _021D4670;
    }
    while (SOCL_CalmDown() == -26) {
        OS_Sleep(100);
    }
    result = SOCLi_CleanupCommandPacketQueue();
    if (result >= 0) {
        CPS_Cleanup();
        CPS_SetScavengerCallback(0);
        if (!_02110628->external) {
            _02110628->free(*(void **)(_021D4430 + 0x1c));
        }
        _02110628 = 0;
    }
    return result;
}

int SOCL_CloseAll(void) {
    Socket *p;
    while (1) {
        OSIntrMode irq = OS_DisableInterrupts();
        p = _021D448C;
        while (p) {
            if (p != _021D442C && !(p->flags & 0x10)) {
                break;
            }
            p = p->next;
        }
        OS_RestoreInterrupts(irq);
        if (!p) {
            break;
        }
        SOCL_Close(p);
    }
    if (!_021D448C || (_021D448C == _021D442C && !_021D448C->next)) {
        if (!_021D4490) {
            return 0;
        }
    }
    return -26;
}
