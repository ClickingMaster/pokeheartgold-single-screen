#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct { u8 data[80]; } AgbBoxMonX;
typedef struct { u32 currentBox; AgbBoxMonX boxes[14][30]; } AgbStorageX;
typedef struct { void *mon; void *item; void *extra; } SlotSprites;
typedef struct {
    u8 pad0[0x1a8];
    SlotSprites slots[30];
    u8 pad1[0xe880 - 0x1a8 - 30 * 12];
    AgbStorageX *storage;
    int box;
    u8 pad2[0x12604 - 0xe888];
    void (*cb)(void *);
} Work;
extern void *NARC_New(int,int);
extern void NARC_Delete(void *);
extern void Heap_Free(void *);
extern void ov74_02231F30(int,int,int,int,void *,void *,void *);
extern void ov74_02231FB0(void *);
extern void ov74_02232678(Work *,int);
extern void ov74_0223262C(Work *);
extern int AGB_GetBoxMonData(void *,int,void *);
extern int ov74_02231D70(Work *,int,int);
extern int ov74_02231D94(Work *,int,int);
extern int ov74_02231DB8(Work *,int,int);
extern int TranslateAgbSpecies(int);
extern int ov74_02231E54(int,int,u8);
extern void Sprite_SetDrawFlag(void *,int);
void ov74_02231FF4(Work *w);
// @fn ov74_02231FF4
void ov74_02231FF4(Work *w){
    int species, held, form;
    void *buffer;
    void *narc;
    int i;
    narc = NARC_New(20, 76);
    buffer = Heap_AllocAtEnd((enum HeapID)76, 4096);
    for (i = 0; i < 30; i++) {
        if (AGB_GetBoxMonData(&w->storage->boxes[w->box][i], 5, NULL)) {
            species = ov74_02231D70(w, w->box, i);
            held = ov74_02231D94(w, w->box, i);
            form = ov74_02231DB8(w, w->box, i);
            form = ov74_02231E54(TranslateAgbSpecies(species), form, gSystem.unk6A);
            ov74_02231F30(species, held, form, i, w->slots[i].mon, buffer, narc);
            Sprite_SetDrawFlag(w->slots[i].mon, 1);
            if (AGB_GetBoxMonData(&w->storage->boxes[w->box][i], 12, NULL)) {
                Sprite_SetDrawFlag(w->slots[i].item, 1);
            } else {
                Sprite_SetDrawFlag(w->slots[i].item, 0);
            }
        } else {
            ov74_02231F30(species, held, form, i, NULL, buffer, narc);
            Sprite_SetDrawFlag(w->slots[i].mon, 0);
            Sprite_SetDrawFlag(w->slots[i].item, 0);
        }
    }
    Heap_Free(buffer);
    NARC_Delete(narc);
    w->cb = ov74_02231FB0;
    ov74_02232678(w, w->box);
    ov74_0223262C(w);
}
