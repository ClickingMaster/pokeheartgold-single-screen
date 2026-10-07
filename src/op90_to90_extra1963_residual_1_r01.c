#pragma force_active on
#include "global.h"
typedef struct Node {
    struct Node *prev;
    struct Node *next;
    u16 key;
    u16 idx;
} Node;
typedef struct {
    u8 pad[0x1c];
    u8 levels[6];
} Data;
typedef struct {
    u8 pad[0x228];
    void *x228;
} Work;
void ov96_021FC2B4(void *a, int b);
void ov96_021FC248(void *a, int idx, int x);
void ov96_021FC28C(void *a, int idx, int v);
void ov96_021FC2E0(void *a, int idx, int v);
void ov96_021FAFFC(Work *work, Data *data);
// @fn ov96_021FAFFC
void ov96_021FAFFC(Work *work, Data *data) {
    Node nodes[13];
    int i;
    int j;
    Node *node;
    Node *cur;
    Node *p;
    u8 v;
    i = 0;
    nodes[0].next = &nodes[1];
    nodes[0].prev = NULL;
    nodes[0].key = 0;
    nodes[0].idx = 0;
    for (; i < 12; i++) {
        node = &nodes[i + 1];
        node->key = (u8)((data->levels[(u8)(i / 2)] >> ((u8)(i % 2) * 4)) & 0xf);
        node->idx = i;
        node->next = NULL;
        cur = &nodes[0];
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (cur->key > node->key) {
                cur->prev->next = node;
                node->prev = cur->prev;
                node->next = cur;
                cur->prev = node;
                break;
            } else if (cur->key == node->key && cur->idx > node->idx) {
                cur->prev->next = node;
                node->prev = cur->prev;
                node->next = cur;
                cur->prev = node;
                break;
            }
        }
        if (j == i) {
            cur->next = node;
            node->prev = cur;
        }
    }
    ov96_021FC2B4(work->x228, 1);
    p = &nodes[0];
    for (i = 0; i < 12; i++) {
        p = p->next;
        v = p->key;
        if (v > 8) {
            v = 8;
        }
        ov96_021FC248(work->x228, p->idx, i * 0x20 + 0x10);
        ov96_021FC28C(work->x228, p->idx, v);
        ov96_021FC2E0(work->x228, p->idx, 1);
    }
}
