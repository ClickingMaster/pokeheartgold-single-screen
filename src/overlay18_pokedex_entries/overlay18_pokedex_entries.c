#include "private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov18_021EEC34(void *work, u32 species, u32 index, u32 language)
{
    u8 *windowBase = (u8 *)work + 0xC;
    u32 offset = index << 4;
    int mode;

    FillWindowPixelBuffer(windowBase + offset, 0);
    switch (language) {
    case 1:
        mode = 0;
        break;
    default:
        mode = 1;
        break;
    }
    BufferIntegerAsString(PTR(work, 0x660), 0,
        Pokedex_ConvertToCurrentDexNo(*((u8 *)work + 0x1858), species), 3, 2, mode);
    ov18_021EE3AC(work, PTR(work, 0x65C), index, 9, 0, 0, 0, 0x20100, 0);
    ScheduleWindowCopyToVram(windowBase + offset);
}

void ov18_021EECB0(void *work, u32 species, u32 index)
{
    u8 *windowBase = (u8 *)work + 0xC;
    u32 offset = index << 4;
    void *string;

    FillWindowPixelBuffer(windowBase + offset, 0);
    string = ov18_021E590C(species, *((u8 *)work + 0x185C), 0x25);
    ov18_021F95FC(windowBase + offset, string, 0, 0, 0, 0x20100, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(windowBase + offset);
}

void ov18_021EED00(void *work, u32 species, u32 slot, u32 index)
{
    u8 *windowBase = work;
    u32 offset = index << 4;
    void *string;

    windowBase += 0xC;
    FillWindowPixelBuffer(windowBase + offset, 0);
    if (*(u16 *)((u8 *)work + slot * 4 + 0x1032) == 2) {
        string = ov18_021E595C(species, *((u8 *)work + 0x185C), 0x25);
    } else {
        string = ov18_021E595C(0, *((u8 *)work + 0x185C), 0x25);
    }
    ov18_021F95FC(windowBase + offset, string, 0x7C, 0, 0, 0x20100, 1);
    String_Delete(string);
    ScheduleWindowCopyToVram(windowBase + offset);
}

void ov18_021EED70(To47Work18 *work, u32 species, u32 slot)
{
    ClearWindowTilemapAndScheduleTransfer(&work->windows[86]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[88]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[85]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[87]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[89]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[90]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[91]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[92]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[93]);

    if (species != 0) {
        ov18_021EE984(work, species, slot, 0x55);
        if (work->language == 2) {
            ov18_021EE9FC(work, 0x56);
            ov18_021EEA40(work, 0x58);
            ov18_021EEAE4(work, species, slot, 0x57);
            ov18_021EEB94(work, species, slot, 0x59);
        } else {
            ov18_021EEBE4(work, 0x5A);
            ov18_021EEC34(work, species, 0x5B, work->language);
            ov18_021EECB0(work, species, 0x5C);
            ov18_021EED00(work, species, slot, 0x5D);
        }
    }
}
