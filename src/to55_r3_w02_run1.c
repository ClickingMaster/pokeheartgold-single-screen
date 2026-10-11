#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "nnsys.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "oam_manager.h"

typedef struct Ov112BoxMon {
    u32 species;
    u32 personality;
    u16 isEgg;
    u16 form;
    u16 unkC;
    u16 unkE;
    u16 isShiny;
    u16 gender;
    u16 unk14;
    u16 unk16;
    u16 nickname[12];
} Ov112BoxMon;

extern const GraphicsBanks ov112_021FF0FC;
extern const GraphicsModes ov112_021FF0DC;
extern const ObjCharTransferTemplate ov112_021FF0EC;

extern void PokewalkerApp_HideSprite(void *, int);
extern void PokewalkerApp_Teardown(void *);
extern void *ov112_021F039C(int);
extern void ov112_021F03BC(void *, void *);
extern void ov112_021F050C(void *);
extern void ov112_021F051C(void *);
extern void PokewalkerApp_Setup(void *);
extern void ov112_021EF31C(void *, void *);
extern void PokewalkerApp_Teardown(void *);
extern void ov112_021EF300(void *, void *, u32);
extern void ov112_021EF310(void *);
extern void ov112_021EF3F8(void);
extern void *sub_020932E0(int, int, int);
extern void sub_02093354(void *);
extern void sub_02093440(void *, void *, void *, void *, int, int, int, int, void *, void *, void *, void *, void *);
extern int sub_020935E0(void *);
extern void sub_02093A40(void *);
extern void sub_02093594(void *);

int ov112_021EEF30(void *p);
int ov112_021EEF68(void *p);
int ov112_021EF03C(void *p);
int ov112_021EF100(void *p);
void ov112_021EF15C(void);
void ov112_021EF17C(void *unused);
void ov112_021EF19C(void);
void ov112_021EF1CC(void *unused);
BOOL ov112_021EF1F0(PCStorage *storage, u32 boxno, u32 slotno, Ov112BoxMon *ptr);
BOOL ov112_021EF298(PCStorage *storage, u32 boxno, u32 slotno, Ov112BoxMon *ptr);

int ov112_021EEF30(void *p) {
    FillBgTilemapRect(*(BgConfig **)((u8 *)p + 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(*(void **)((u8 *)p + 0x18), 1);
    PokewalkerApp_HideSprite(p, 6);
    return 11;
}
