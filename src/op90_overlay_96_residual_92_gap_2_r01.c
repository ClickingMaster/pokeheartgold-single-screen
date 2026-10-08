#pragma force_active on
#include "global.h"

typedef struct Item {
    u8 pad0[0x10];
    s16 score;
} Item;

typedef struct Node {
    struct Node *prev;
    struct Node *next;
    Item *data;
} Node;

u8 ov96_02208FF0(Item **items, Item **out);

// @fn ov96_02208FF0
u8 ov96_02208FF0(Item **items, Item **out) {
    Node nodes[49];
    u8 count;
    u8 i, j;
    Node *node;
    Node *cur;

    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    nodes[0].data = NULL;
    count = 0;
    for (i = 0; i < 48; i++) {
        node = &nodes[i + 1];
        node->data = items[i];
        node->next = NULL;
        node->prev = NULL;
        cur = &nodes[0];
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (cur->data->score < node->data->score) {
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
    {
    Node *it;
    s16 first;
    first = nodes[0].next->data->score;
    for (j = 0, it = &nodes[0]; j < 48; j++) {
        it = it->next;
        out[j] = it->data;
        if (first == out[j]->score) {
            count++;
        }
    }
    }
    GF_ASSERT(count != 0);
    return count;
}
