#ifndef TO45_OV96_R36B_PRIVATE_H
#define TO45_OV96_R36B_PRIVATE_H

#include "global.h"

typedef struct Ov96R36SpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    int palette;
    int vram;
    int resources[6];
    int bgPriority;
    int vramTransfer;
} Ov96R36SpriteTemplate;

typedef struct Ov96R36Work {
    u32 heapId;
    u8 filler04[0x14];
    void *spriteSystem;
    void *spriteManager;
    void *sprites[0x26];
} Ov96R36Work;

#define FIELD_U8(p, off) (*(u8 *)((u8 *)(p) + (off)))
#define PTR(p, off)      (*(void **)((u8 *)(p) + (off)))

void *SpriteSystem_NewSpriteWithYOffset(void *, void *, const Ov96R36SpriteTemplate *, int);
void ManagedSprite_SetAnimateFlag(void *, int);
void ManagedSprite_SetPaletteOverride(void *, int);
void ManagedSprite_SetDrawFlag(void *, int);
void *PokeathlonCourse_GetHeapAllocPtr4(void *);
void SpriteSystem_LoadCharResObj(void *, void *, int, int, int, int, int);
void SpriteSystem_LoadPlttResObj(void *, void *, int, int, int, int, int, int);
void SpriteSystem_LoadCellResObj(void *, void *, int, int, int, int);
void SpriteSystem_LoadAnimResObj(void *, void *, int, int, int, int);
void ov96_021ECA18(void *);
int ov96_021EDC38(u8, int);
void ov96_021EC5C0(void *);
void ov96_021EC68C(void *);
void ov96_021EC70C(void *);
void ov96_021EC82C(void *);
void ov96_021EC790(void *, u32);

typedef struct Ov96ParticipantViewR36 {
    u16 species;
    u16 field2;
    u16 special;
    u8 kind;
    u8 field7;
    u32 participant;
    u32 fieldC;
} Ov96ParticipantViewR36;

typedef struct Ov96PokepicTemplateR36 {
    u16 narcID;
    u16 charDataID;
    u16 palDataID;
    u16 species;
    u8 isAnimated;
    u8 filler09[3];
    u32 personality;
} Ov96PokepicTemplateR36;

typedef struct Ov96PaletteDataR36 {
    u32 fmt;
    u32 extended;
    u32 size;
    void *rawData;
} Ov96PaletteDataR36;

void *PokeathlonCourse_GetParticipantUnk04(void *, u32);
void *PokeathlonCourse_GetParticipantData(void *, u32);
void PokeathlonCourse_GetMonInfo(void *, u32, u32, Ov96ParticipantViewR36 *);
void ov96_021EEBF8(void *, Ov96ParticipantViewR36 *, u32, u32, u32, u32);
void GetMonSpriteCharAndPlttNarcIdsEx(Ov96PokepicTemplateR36 *, u16, u8, u32, u8, u8, u32);
void *GfGfxLoader_GetPlttData(u32, s32, Ov96PaletteDataR36 **, u32);
void ov96_021EAF60(void *, u8, u32, void *);
void Heap_Free(void *);

#endif
