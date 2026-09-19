#include "overlay40_pc_box_wallpaper_private.h"
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void SpriteSystem_LoadPaletteBufferFromOpenNarc(void *, int, void *, void *, void *, int, int, int, int, int);
void SpriteSystem_LoadCharResObjFromOpenNarc(void *, void *, void *, int, int, int, int);
void SpriteSystem_LoadCellResObjFromOpenNarc(void *, void *, void *, int, int, int);
void SpriteSystem_LoadAnimResObjFromOpenNarc(void *, void *, void *, int, int, int);
void ov40_02243A28(u8 *);
void ov40_02243D64(u8 *);
void *SpriteSystem_NewSprite(void *, void *, const void *);
void ManagedSprite_SetAnim(void *, int);
void ManagedSprite_TickFrame(void *);
void ManagedSprite_SetOamMode(void *, int);
void ManagedSprite_SetAffineOverwriteMode(void *, int);
void ov40_02243EEC(u8 *);
int ov40_02244054(int, int);
void ov40_02243C54(u8 *);
typedef struct R41SpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    int pal;
    int vram;
    int resIdList[6];
    int bgPriority;
    int vramTransfer;
} R41SpriteTemplate;
typedef struct R41WorkTail {
    u8 pad[0x220];
    void *narc;
    void *system;
    void *manager;
    void *bgConfig;
    void *sprite;
} R41WorkTail;
