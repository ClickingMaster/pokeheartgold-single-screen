#include "global.h"

extern int Comm_GetLocalNetId(void);
extern int Comm_CountConnected(void);
extern void ov73_021E71E4(void *, u32, u32);
extern void ScheduleBgTilemapBufferTransfer(void *, u32);
extern int ov73_021E75FC(u32);
extern void RemoveTextPrinter(u8);
extern void ov73_021E756C(void *, u32, u32);
extern void ov73_021E781C(void *, int);
extern void ov73_021E66F0(void *);
void *ov73_021E6F1C(void *, void *);

void *ov73_021E6F1C(void *work, void *result) {
    if (Comm_GetLocalNetId() == 0) {
        ov73_021E71E4(work, 0, 2);
    }
    *(s32 *)((u8 *)work + 0x4A38) = -1;
    ov73_021E71E4(work, 1, 2);
    ScheduleBgTilemapBufferTransfer(*(void **)work, 1);
    if (*(u32 *)((u8 *)work + 0x4c) != 0xff && ov73_021E75FC(*(u32 *)((u8 *)work + 0x4c)) == 0) {
        RemoveTextPrinter((u8)*(u32 *)((u8 *)work + 0x4c));
    }
    ov73_021E756C(work, 15, 1);
    *(u32 *)((u8 *)work + 0x318) = 20;
    if (Comm_GetLocalNetId() == 0) {
        ov73_021E781C(work, 0);
    }
    *(u32 *)((u8 *)work + 0x4A2C) = Comm_CountConnected();
    *(u32 *)((u8 *)work + 0x4A34) = 0x12c;
    ov73_021E66F0(work);
    return result;
}
