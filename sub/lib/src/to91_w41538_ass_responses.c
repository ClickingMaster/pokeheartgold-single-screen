#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
void *AllocateHeapBuf(void *, u32);
void SetFatalErr(u32);
void InitManHeader(void *, const void *);
u32 SetSupRateSet(void *);
u8 WL_ReadByte(const void *);
void WL_WriteByte(void *, u32);
u32 CAM_AllocateAID(u32);
u8 *CAM_GetMacAdrs(u32);

typedef struct {
    u8 allocator[12];
    u16 station;
    u8 gap0e[8];
    u16 dataLength;
    u8 gap18[10];
    u16 frameLength, control;
    u8 gap26[22];
    u16 capability, status, aid;
    u8 tail[42];
} W40784Frame;

typedef char W40784FrameSize[(sizeof(W40784Frame) == 108) ? 1 : -1];
u8 *MakeAssResFrame(u32 station, u32 status, const void *element);

u8 *MakeAssResFrame(u32 station, u32 status, const void *element) {
    const u8 *extra = element;
    W40784Frame *frame = (W40784Frame *)AllocateHeapBuf(BASE + 0x188, 96);
    u8 *p = (u8 *)frame;
    u32 aid;
    if (!p) {
        SetFatalErr(2);
        return p;
    }
    frame->station = 65535;
    if (status == 0) {
        aid = CAM_AllocateAID(station);
        if (aid == 0) {
            status = 19;
        }
    } else {
        aid = 0;
    }
    InitManHeader(p + 16, CAM_GetMacAdrs(station));
    frame->capability = H(BASE, 0x3c0);
    frame->status = status;
    frame->aid = aid;
    if (aid) {
        frame->aid |= 0xc000;
    }
    frame->dataLength = SetSupRateSet(p + 0x42) + 6;
    frame->frameLength = frame->dataLength + 28;
    frame->control = 0x10;
    {
        const u8 *source;
        u32 i;
        u8 *dest;
        u32 length;
        dest = p + 0x24 + frame->frameLength;
        if (extra) {
            length = WL_ReadByte(extra + 1);
            WL_WriteByte(dest++, WL_ReadByte(extra));
            WL_WriteByte(dest++, length & 255u);
            i = 0;
            if (i < length) {
                source = extra + 2;
                while (i < length) {
                    WL_WriteByte(dest, WL_ReadByte(source + i));
                    i++;
                    dest++;
                }
            }
        } else {
            WL_WriteByte(dest, 0);
            WL_WriteByte(dest + 1, 0);
        }
    }
    return p + 16;
}

u8 *MakeReAssResFrame(u32 station, u32 status, const u8 *extra);

u8 *MakeReAssResFrame(u32 station, u32 status, const u8 *extra) {
    W40784Frame *frame = (W40784Frame *)AllocateHeapBuf(BASE + 0x188, 96);
    u8 *p = (u8 *)frame;
    u32 aid;
    if (!p) {
        SetFatalErr(2);
        return p;
    }
    frame->station = 65535;
    if (status == 0) {
        aid = CAM_AllocateAID(station);
        if (aid == 0) {
            status = 19;
        }
    } else {
        aid = 0;
    }
    InitManHeader(p + 16, CAM_GetMacAdrs(station));
    frame->capability = H(BASE, 0x3c0);
    frame->status = status;
    frame->aid = aid | 0xc000;
    frame->dataLength = SetSupRateSet(p + 0x42) + 6;
    frame->frameLength = frame->dataLength + 28;
    frame->control = 0x30;
    {
        u32 i;
        u8 *dest;
        u32 length;
        dest = p + 0x24 + frame->frameLength;
        if (extra) {
            length = WL_ReadByte(extra + 1);
            WL_WriteByte(dest, WL_ReadByte(extra));
            WL_WriteByte(dest + 1, length & 255u);
            dest += 2;
            extra += 2;
            for (i = 0; i < length; i++) {
                WL_WriteByte(dest, WL_ReadByte(extra + i));
                dest++;
            }
        } else {
            WL_WriteByte(dest, 0);
            WL_WriteByte(dest + 1, 0);
        }
    }
    return p + 16;
}
