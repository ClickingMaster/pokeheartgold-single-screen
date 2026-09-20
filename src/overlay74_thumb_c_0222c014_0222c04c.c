#include "global.h"

#define S8AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTRAT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov74Work {
    u8 pad_0000[0x3014];
    void *sprite1;
    u8 pad_3018[0x48];
    void *sprite2;
} Ov74Work;

extern void GfGfx_EngineATogglePlanes(u32, u32);
extern void GfGfx_EngineBTogglePlanes(u32, u32);
extern void ov74_0222BF08(void *);
extern void ov74_0222AF28(void *);
extern void ov74_0222B288(void *);
extern void ov74_0222B3A4(void *);
extern void ov74_0222B144(void *);
extern BOOL ov74_0222BF2C(void *, int, int);
extern BOOL ov74_0222BFA0(void *, int, int);
extern void Sprite_TryChangeAnimSeq(void *, int);
extern void Sprite_SetDrawFlag(void *, int);
extern s32 *Sprite_GetMatrixPtr(void *);
extern int Sprite_GetDrawFlag(void *);
extern void ov74_0222B344(void *);
extern void ov74_0222B2C4(void *);
extern void ov74_0222B5F8(void *);
extern void ov74_0222BF18(void);
extern void PlaySE(int);
extern void ov74_0222B19C(void *);
extern void ov74_0222B1F4(void *);
extern void ov74_0222B30C(void *);
extern void ov74_0222B760(void *);
extern void ov74_0222B374(void *);
extern void ov74_0222B20C(void *);
extern void ov74_0222B224(void *);
extern void SysTask_Destroy(void *);
extern void Heap_Free(void *);

void ov74_0222C014(void *p);
void ov74_0222C04C(void *task, void *p);

void ov74_0222C014(void *p) {
    S8AT(p, 8) = 0;
    ov74_0222BF08(p);
    ov74_0222AF28(p);
    ov74_0222B288(p);
    ov74_0222B3A4(p);
    ov74_0222B144(p);
    GfGfx_EngineATogglePlanes(0x10, TRUE);
    GfGfx_EngineBTogglePlanes(0x10, TRUE);
}

void ov74_0222C04C(void *task, void *p) {
    int count;
    int ready;
    int i;
    u8 *walker;
    s32 *m1;
    s32 *m2;

    switch (U32AT(p, 0)) {
    case 0:
        ov74_0222C014(p);
        U32AT(p, 0) = 1;
        break;
    case 1:
        if (ov74_0222BF2C(p, 1, 2)) {
            U32AT(p, 0) = 2;
        }
        break;
    case 2:
        count = 0;
        ready = TRUE;
        i = 0;
        walker = p;
        while (i < 80) {
            if (PTRAT(walker, 0x90) != NULL) {
                count++;
            } else {
                ready = FALSE;
            }
            i++;
            walker += 0x4c;
        }
        if (count > 50) {
            Sprite_TryChangeAnimSeq(((Ov74Work *)p)->sprite1, 4);
        } else if (count > 30) {
            Sprite_TryChangeAnimSeq(((Ov74Work *)p)->sprite1, 3);
        } else if (count > 7) {
            Sprite_SetDrawFlag(((Ov74Work *)p)->sprite1, TRUE);
        }
        if (ready || count == 79) {
            U32AT(p, 0) = 3;
            ov74_0222B344(p);
        }
        break;
    case 3:
        if (U32AT(PTRAT(p, 0x30a0), 0) == 2) {
            ov74_0222B2C4(p);
            ov74_0222B5F8(p);
            GfGfx_EngineATogglePlanes(2, FALSE);
            ov74_0222BF18();
            U32AT(p, 0) = 4;
        }
        break;
    case 4:
        count = 0;
        ready = TRUE;
        i = 0;
        walker = p;
        while (i < 80) {
            if (PTRAT(walker, 0x94) != NULL) {
                if (PTRAT(walker, 0x90) != NULL) {
                    count++;
                } else {
                    ready = FALSE;
                }
            }
            i++;
            walker += 0x4c;
        }
        ov74_0222BFA0(p, 2, 2);
        if (ready || count == 19) {
            PlaySE(0x61a);
            U32AT(p, 0) = 5;
            ov74_0222B344(p);
            ov74_0222B19C(p);
        }
        break;
    case 5:
        if (ov74_0222BFA0(p, 2, 2)) {
            ov74_0222BFA0(p, 1, 2);
            U32AT(p, 0) = 6;
        }
        break;
    case 6:
        if (ov74_0222BFA0(p, 1, 2)) {
            m1 = Sprite_GetMatrixPtr(((Ov74Work *)p)->sprite1);
            m2 = Sprite_GetMatrixPtr(((Ov74Work *)p)->sprite2);
            if (m1[1] < 0xe0000) {
                m1[1] += 0x8000;
            }
            if (m1[1] - 0xc0000 > 0) {
                if (m2[1] < 0x180000) {
                    m2[1] += 0x8000;
                    if (!Sprite_GetDrawFlag(((Ov74Work *)p)->sprite2)) {
                        Sprite_SetDrawFlag(((Ov74Work *)p)->sprite2, TRUE);
                    }
                } else {
                    U32AT(p, 0) = 7;
                    ov74_0222B1F4(p);
                    ov74_0222B30C(p);
                    ov74_0222B760(p);
                    GfGfx_EngineATogglePlanes(2, TRUE);
                }
            }
        }
        break;
    case 7:
        count = 0;
        ready = TRUE;
        i = 0;
        walker = p;
        while (i < 80) {
            if (PTRAT(walker, 0x1850) != NULL) {
                count++;
            } else {
                ready = FALSE;
            }
            i++;
            walker += 0x4c;
        }
        if (count > 7) {
            ov74_0222BF2C(p, 2, 1);
        }
        if (count > 50) {
            Sprite_SetDrawFlag(((Ov74Work *)p)->sprite2, FALSE);
        } else if (count > 30) {
            Sprite_TryChangeAnimSeq(((Ov74Work *)p)->sprite2, 2);
        } else if (count > 7) {
            Sprite_TryChangeAnimSeq(((Ov74Work *)p)->sprite2, 3);
        }
        if (ready) {
            U32AT(p, 0) = 8;
            ov74_0222B374(p);
            ov74_0222B20C(p);
        }
        break;
    case 8:
        if (ov74_0222BF2C(p, 2, 1)) {
            U32AT(p, 0) = 9;
        }
        break;
    case 9:
        if (ov74_0222BF2C(p, 0, 2)) {
            U32AT(p, 0) = 0xff;
        }
        break;
    default:
        U32AT(PTRAT(p, 0x30a0), 0) = 0;
        SysTask_Destroy(task);
        Heap_Free(p);
        return;
    }
    ov74_0222B224(p);
}
