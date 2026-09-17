#include "overlay96_scene_nodes_gap60_internal.h"
void ov96_021F74D0(Ov96R60Node *node, void *a, void *b) {
    int i;
    for (i = 0; i < 29; i++, node++) {
        node->a = ov96_021EB3E4(a, b, 3, 0x65, 2);
        ov96_021EB52C(node->a, 1, 1);
        ov96_021EB564(node->a, 8);
        node->b = ov96_021EB3E4(a, b, 3, 0x65, 5);
        node->c = ov96_021EB3E4(a, b, 3, 0x65, 6);
        ov96_021EB630(node->a, 0x14);
        ov96_021EB630(node->b, 7);
        ov96_021EB630(node->c, 8);
    }
}
void ov96_021F7544(Ov96R60Node *node) {
    int i;
    VecFx32 pos;
    const u16 *xy = ov96_0221C21C;
    for (i = 0; i < 29; i++, xy += 2, node++) {
        pos.x = xy[0] << 15;
        pos.y = xy[1] << 15;
        pos.z = 0;
        ov96_021EB588(node->a, &pos);
        ov96_021EB588(node->b, &pos);
        ov96_021EB588(node->c, &pos);
        node->pos = pos;
    }
}
void ov96_021F7598(u8 *work) {
    u32 counter = *(u32 *)(work + 0x32c);
    if (counter == 0) {
        ov96_021F7620(work);
        *(u32 *)(work + 0x32c) = 0xf0;
    } else {
        *(u32 *)(work + 0x32c) = counter - 1;
    }
}
void ov96_021F75BC(Ov96R60Node *node, int index) {
    node[index].zero = 0;
    ov96_021EB564(node[index].a, 8);
}
