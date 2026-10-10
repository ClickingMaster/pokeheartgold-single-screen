#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    void *state;
    u32 unused, value;
} Interface09;

typedef struct {
    u32 kind;
    Interface09 *iface;
    void *state;
    u32 value, flags;
} Request09;

typedef struct {
    u8 existing[0x1000];

    struct {
        u8 before[0x54c];
        Interface09 *iface;
        void *state;
        u8 tail[0x3c];
    } extension;
} Manager09;

extern u8 wmspW[0x1590];
#define MANAGER09 ((Manager09 *)(void *)wmspW)
extern void WMSPi_CommonInit(u32);
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
void WMSP_Enable(void *);
typedef char Size09[(sizeof(Manager09) == 0x1590 && sizeof(Request09) == 20 && sizeof(Interface09) == 12) ? 1 : -1];

void WMSP_Enable(void *raw) {
    void *state;
    Interface09 *iface;
    Request09 *request = raw;
    iface = request->iface;
    MANAGER09->extension.iface = iface;
    state = request->state;
    MANAGER09->extension.state = state;
    iface->state = state;
    iface->value = request->value;
    WMSPi_CommonInit(request->flags);
    {
        u16 *reply = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        reply[0] = 3;
        reply[1] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(reply);
    }
}
