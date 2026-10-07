#include "to50_overlay96_r71u3_private.h"

void ov96_021FB400(void *course) {
    u8 *packed;
    s32 outer;
    s32 inner;
    u8 *copy;
    s32 base;
    u16 *row;
    Ov96R71Node head;
    Ov96R71Node nodes[12];

    copy = PokeathlonCourse_GetDataCopyArea(course);
    packed = (void *)ov96_021E8A20(copy + 0x28);
    outer = 0;
    head.prev = NULL;
    head.next = &nodes[0];
    head.h8 = 0;
    head.hA = 0;
    head.hC = 0;
    head.hE = 0;
    head.w10 = 0;
    copy += 0x50;
    base = outer;
    for (; outer < 4; outer++) {
        row = (void *)ov96_021E8A20(copy);
        for (inner = 0; inner < 3; inner++) {
            Ov96R71Node *node;
            Ov96R71Node *at = &head;
            s32 index = inner + base;
            node = (Ov96R71Node *)((u8 *)&nodes[-1] + (index + 1) * 20);
            s32 j;
            node->h8 = row[0];
            node->hC = row[5];
            node->hA = index;
            node->hE = row[8];
            node->w10 = 0;
            j = 0;
            node->next = (void *)j;
            if (index > 0) {
                do {
                    u32 inserted;
                    at = at->next;
                    if (node->hE != 0) {
                        inserted = ov96_021FB514(node, at);
                    } else {
                        inserted = ov96_021FB56C(node, at);
                    }
                    if (inserted != 0) {
                        break;
                    }
                    j++;
                } while (j < index);
            }
            if (j == index) {
                at->next = node;
                node->prev = at;
            }
            row++;
        }
        copy += 0x28;
        base += 3;
    }
    {
        u8 group = 0;
        u8 rank;
        Ov96R71Node *node;
        s32 i;
        node = &head;
        rank = 1;
        for (i = 0; i < 6; i++) {
            packed[0x1C + i] = 0;
        }
        for (i = 0; i < 12; i++) {
            node = node->next;
            if (node == NULL) {
                GF_AssertFail();
            }
            if (node->w10 == 0) {
                group += rank;
                rank = 1;
            } else {
                rank++;
            }
            ov96_021FB5C8(packed, (u8)node->hA, group);
        }
    }
}
