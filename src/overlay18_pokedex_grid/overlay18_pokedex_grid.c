#include "global.h"
#include "constants/heap.h"
#include "menu_input_state.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02020A0C.h"

typedef struct GridInputHandler GridInputHandler;
typedef struct GridCallbacks GridCallbacks;
GridInputHandler *GridInputHandler_Create(const TouchscreenHitbox *, const DpadMenuBox *, const GridCallbacks *, void *, BOOL, u8, enum HeapID);
int GridInputHandler_HandleInput_AllowHold(GridInputHandler *);
const DpadMenuBox *GridInputHandler_GetDpadBox(GridInputHandler *, int);
void GridInputHandler_SetNextLastUnk0FInputs(GridInputHandler *, u8, u8, u8);

typedef struct Ov18GridState {
    u8 raw[0x1864];
    GridInputHandler *grid;
    u8 gap[0x34];
    int previousInput;
} Ov18GridState;

extern const TouchscreenHitbox sSearchNameHitboxes[];
extern const DpadMenuBox sSearchNameDpadBoxes[];
extern const GridCallbacks sSearchNameGridCallbacks;

#define PTR(p, off, T) (*(T **)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

void PlaySE(u32 seqNo);
void PokedexApp_SetSpriteVisible(void *, int, int);
void PokedexApp_SetSpriteAnim(void *, int, int);
void PokedexApp_UpdateSearchNameCursor(void *, int);
void PokedexApp_CreateSearchNameGrid(void *);
int PokedexApp_HandleSearchNameInput(void *);
void PokedexApp_OnSearchNameGridMove(Ov18GridState *, int, int);

void PokedexApp_CreateSearchNameGrid(void *p) {
    PTR(p, 0x1864, GridInputHandler) = GridInputHandler_Create(sSearchNameHitboxes, sSearchNameDpadBoxes, &sSearchNameGridCallbacks, p, TRUE, 0, (enum HeapID)0x25);
    PokedexApp_SetSpriteVisible(p, 0, 1);
    PokedexApp_UpdateSearchNameCursor(p, 0);
}

int PokedexApp_HandleSearchNameInput(void *p) {
    MenuInputStateMgr_SetState(*(MenuInputStateMgr **)((u8 *)PTR(p, 0, void) + 0xC), MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 0x1B;
    }
    return GridInputHandler_HandleInput_AllowHold(PTR(p, 0x1864, GridInputHandler));
}

void PokedexApp_UpdateSearchNameCursor(void *p, int target) {
    u8 pos[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(PTR(p, 0x1864, GridInputHandler), target);
    DpadMenuBox_GetPosition(box, &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(PTR(p, 0x670, ManagedSprite), pos[1], pos[0]);
    if (target >= 0x1B) {
        PokedexApp_SetSpriteAnim(p, 0, 0x23);
    } else {
        PokedexApp_SetSpriteAnim(p, 0, 0x28);
    }
}

void PokedexApp_OnSearchNameGridMove(Ov18GridState *p, int target, int input) {
    if ((u32)(input - 0x1B) <= 1) {
        int previous = p->previousInput;
        u8 left = sSearchNameDpadBoxes[previous].left;

        if (target == 0 || target == 6) {
            int i;
            for (i = 0; i <= 0x1A; i++) {
                if (left == sSearchNameDpadBoxes[i].left) {
                    target = i;
                    GridInputHandler_SetNextLastUnk0FInputs(p->grid, target, input, input);
                    break;
                }
            }
        } else if (target == 0x15 || target == 0x1A) {
            int i;
            for (i = 0x1A; i >= 0; i--) {
                if (left == sSearchNameDpadBoxes[i].left) {
                    target = i;
                    GridInputHandler_SetNextLastUnk0FInputs(p->grid, target, input, input);
                    break;
                }
            }
        }
    }
    PokedexApp_UpdateSearchNameCursor(p, target);
    p->previousInput = input;
    PlaySE(0x8E8);
}
