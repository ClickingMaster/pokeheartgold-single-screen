#include "overlay96_exact_bank_internal.h"
#include "overlay96_exact_bank_prototypes.h"

void ov96_02203BC0(u8 *p, int i) {
    PokeathlonCourseSpritePoolSprite_Show(*(void **)(p + i * 4 + 0x60), 1);
}
