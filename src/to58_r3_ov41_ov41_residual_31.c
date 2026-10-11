#include "global.h"

void ov41_02248164(void *work, int index);

void DressUpGfx_SetSlotPriority(void *, int);
void ov41_02249700(void *, int);

typedef struct Ov41Node {
    void *value;
    int selected;
    struct Ov41Node *next;
} Ov41Node;

void ov41_02248164(void *work, int index)
{
    Ov41Node *node = *(Ov41Node **)((u8 *)work + 0x1c);
    Ov41Node *end = (Ov41Node *)((u8 *)work + 0x14);
    while (node != end) {
        if (node->selected == 0) {
            DressUpGfx_SetSlotPriority(node->value, index);
        } else {
            ov41_02249700(node->value, index);
        }
        node = node->next;
        index--;
    }

    node = *(Ov41Node **)((u8 *)work + 0xc);
    end = (Ov41Node *)((u8 *)work + 4);
    index -= 8;
    while (node != end) {
        if (node->selected == 0) {
            DressUpGfx_SetSlotPriority(node->value, index);
        } else {
            ov41_02249700(node->value, index);
        }
        node = node->next;
        index--;
    }
}
