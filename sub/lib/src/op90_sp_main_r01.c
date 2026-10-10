#pragma force_active on
#include <nitro/os/alloc.h>
#include <nitro/os/common/interrupt_shared.h>
#include <nitro/os/common/system_shared.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 prefix[6];
    u16 firstText[10];
    u8 firstLength, reserved1b;
    u16 secondText[26];
    u8 secondLength, reserved51[0x64 - 0x51];
    u16 languageBits;
    u8 reserved66[0x70 - 0x66];
    u16 revision, baseCrc;
    u8 extensionTag, preferredLanguage;
    u16 allowedLanguages;
    u8 reserved78[0xfe - 0x78];
    u16 extensionCrc;
} FirmwareFields;

typedef union {
    u8 bytes[256];
    u16 halves[128];
    u32 words[64];
    FirmwareFields fields;
} FirmwarePage;

typedef char FirmwareFieldsMustBe256[(sizeof(FirmwareFields) == 256) ? 1 : -1];

typedef struct {
    u32 index, count, finalDelay, on[6], off[6], active, limit, repeats;
} Pulse;

extern void WVR_ShelterExtWram(void);
extern void OS_Init(void);
extern void OS_InitThread(void);
extern void NVRAM_ReadDataBytes(u32, u32, u8 *);
extern u16 GetRomValidLanguage(void);
extern u16 __Veneer_SVC_GetCRC16(u16, const void *, u32);
extern u16 WMSP_GetAllowedChannel(u32);
extern void MIi_CpuClear32(u32, u32 *, u32);
extern void MIi_CpuCopy32(const u32 *, u32 *, u32);
extern void MI_CpuCopy8(const void *, void *, u32);
extern void MI_CpuFill8(void *, u8, u32);
extern void PXI_Init(void);
extern void OS_Terminate(void);
extern void SND_Init(int);
extern void PAD_InitXYButton(void);
extern void VBlankIntr(void);
extern void OS_SetIrqFunction(OSIrqMask, OSIrqFunction);
extern OSIrqMask OS_EnableIrqMask(OSIrqMask);
extern OSIntrMode OS_EnableInterrupts(void);
extern void FS_Init(u32);
extern void RTC_Init(u32);
extern void WVR_Init(s32);
extern void SPI_Init(u32);
extern void __Veneer_SVC_Halt(void);
extern int CARD_SetThreadPriority(int);
extern BOOL OS_IsResetOccurred(void);
extern void CTRDG_VibPulseEdgeUpdate(Pulse *);
extern void OS_ResetSystem(void);
extern void CTRDG_CheckPullOut_Polling(void);
extern void CARD_CheckPullOut_Polling(void);
void NitroSpMain(void);

void NitroSpMain(void) {
    u8 feature;
    u16 channels;
    u8 mac[6];
    u32 offset;
    FirmwarePage pages[2];
    void *lo, *hi;
    OSHeapHandle heap;
    FirmwarePage *shared = (FirmwarePage *)0x027ffc80;
    WVR_ShelterExtWram();
    OS_Init();
    OS_InitThread();
    NVRAM_ReadDataBytes(0x20, 2, (u8 *)&offset);
    offset <<= 3;
    NVRAM_ReadDataBytes(offset, 256, pages[0].bytes);
    NVRAM_ReadDataBytes(offset + 256, 256, pages[1].bytes);
    {
        s32 valid;
        u16 i;
        valid = 0;
        NVRAM_ReadDataBytes(0x1d, 1, &feature);
        if (feature == 255 ? 0 : ((feature & 0x50) != 0)) {
            u32 chinese;
            u32 languages;
            languages = GetRomValidLanguage();
            chinese = languages & 0x40;
            for (i = 0; i < 2; i++) {
                FirmwarePage *p = &pages[i];
                u16 crc = __Veneer_SVC_GetCRC16(0xffff, p, 0x70);
                if (crc != pages[i].fields.baseCrc) {
                    continue;
                }
                if (pages[i].fields.revision >= 0x80) {
                    continue;
                }
                crc = __Veneer_SVC_GetCRC16(0xffff, p->bytes + 0x74, 0x8a);
                if (crc != pages[i].fields.extensionCrc) {
                    continue;
                }
                if (!((1u << pages[i].fields.preferredLanguage) & pages[i].fields.allowedLanguages)) {
                    continue;
                }
                if (languages & pages[i].fields.allowedLanguages) {
                    p->fields.languageBits = (p->fields.languageBits & ~7) | (pages[i].fields.preferredLanguage & 7);
                }
                if (chinese & ~pages[i].fields.allowedLanguages) {
                    valid = 3;
                    goto selected;
                }
                valid |= 1u << i;
            }
        } else {
            if (GetRomValidLanguage() & 0x40) {
                valid = 3;
                goto selected;
            }
            for (i = 0; i < 2; i++) {
                u16 crc = __Veneer_SVC_GetCRC16(0xffff, &pages[i], 0x70);
                if (crc == pages[i].fields.baseCrc && pages[i].fields.revision < 0x80) {
                    valid |= 1u << i;
                }
            }
        }
        if (valid != 1 && valid != 2) {
            if (valid == 3) {
                if (((pages[0].fields.revision + 1) & 0x7f) == pages[1].fields.revision) {
                    valid = 2;
                } else {
                    valid = 1;
                }
            } else {
                valid = 0;
            }
        }
selected:
        if (valid >= 3) {
            MIi_CpuClear32(~0u, shared->words, 0x74);
        } else if (valid) {
            int j;
            if (pages[valid - 1].fields.firstLength < 10) {
                for (j = 10; j > pages[valid - 1].fields.firstLength; j--) {
                    pages[valid - 1].fields.firstText[j - 1] = 0;
                }
            }
            if (pages[valid - 1].fields.secondLength < 26) {
                for (j = 26; j > pages[valid - 1].fields.secondLength; j--) {
                    pages[valid - 1].fields.secondText[j - 1] = 0;
                }
            }
            MIi_CpuCopy32(pages[valid - 1].words, shared->words, 0x74);
        } else {
            MIi_CpuClear32(0, shared->words, 0x74);
        }
    }
    NVRAM_ReadDataBytes(0x36, 6, mac);
    {
        u8 *dst = shared->bytes + 0x74;
        MI_CpuCopy8(mac, dst, 6);
        NVRAM_ReadDataBytes(0x3c, 2, (u8 *)&channels);
        *(u16 *)(dst + 6) = WMSP_GetAllowedChannel(((u32)channels << 15) >> 16);
    }
    PXI_Init();
    hi = OS_GetArenaHi(OS_ARENA_WRAM_SUBPRIV);
    lo = OS_GetArenaLo(OS_ARENA_WRAM_SUBPRIV);
    lo = OS_InitAlloc(OS_ARENA_WRAM_SUBPRIV, lo, hi, 1);
    hi = OS_GetArenaHi(OS_ARENA_WRAM_SUBPRIV);
    MI_CpuFill8(lo, 0, (u8 *)hi - (u8 *)lo);
    OS_SetArenaLo(OS_ARENA_WRAM_SUBPRIV, lo);
    hi = OS_GetArenaHi(OS_ARENA_WRAM_SUBPRIV);
    lo = OS_GetArenaLo(OS_ARENA_WRAM_SUBPRIV);
    heap = OS_CreateHeap(OS_ARENA_WRAM_SUBPRIV, lo, hi);
    if (heap < 0) {
        OS_Terminate();
    }
    OS_SetCurrentHeap(OS_ARENA_WRAM_SUBPRIV, heap);
    if ((u32)OS_CheckHeap(OS_ARENA_WRAM_SUBPRIV, heap) < 0x2100) {
        OS_Terminate();
    }
    SND_Init(6);
    PAD_InitXYButton();
    OS_SetIrqFunction(1, VBlankIntr);
    OS_EnableIrqMask(1);
    {
        volatile u16 *disp = (volatile u16 *)0x04000004;
        volatile u16 *ime = (volatile u16 *)0x04000208;
        u16 old = *disp;
        old = *disp;
        *disp = old | 8;
        old = *ime;
        *ime = 1;
    }
    OS_EnableInterrupts();
    FS_Init(~0u);
    CARD_SetThreadPriority(15);
    RTC_Init(12);
    WVR_Init(heap);
    SPI_Init(2);
    for (;;) {
        __Veneer_SVC_Halt();
        if (OS_IsResetOccurred()) {
            CTRDG_VibPulseEdgeUpdate(0);
            OS_ResetSystem();
        }
        CTRDG_CheckPullOut_Polling();
        CARD_CheckPullOut_Polling();
    }
}
