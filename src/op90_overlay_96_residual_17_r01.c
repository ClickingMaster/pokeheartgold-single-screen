#pragma force_active on
#include "global.h"
typedef struct {
    int time;
    u8 pad4[5];
    u8 rank;
    u8 padA[2];
    int score;
    u8 pad10[8];
} Ent;
typedef struct Node {
    struct Node *prev;
    struct Node *next;
    Ent *ent;
} Node;
typedef struct {
    int count;
    u8 pad4[8];
    int order;
    u8 pad10[0x14];
    Ent ents[12];
    Ent *sorted[12];
} Work;
void ov96_021E93B4(Work *work, int mode);
// @fn ov96_021E93B4
void ov96_021E93B4(Work *work, int mode) {
    int order;
    Node nodes[13];
    int i;
    int j;
    Node *node;
    Node *cur;
    Node *p;
    int prevScore;
    int rank;
    int tie;
    nodes[0].next = nodes[0].prev = &nodes[1];
    nodes[0].ent = NULL;
    switch (mode) {
    case 0:
        order = 1;
        break;
    case 1:
        order = 1;
        break;
    case 2:
        order = 2;
        break;
    default:
        GF_AssertFail();
        break;
    }
    work->order = order;
    for (i = 0; i < work->count; i++) {
        node = &nodes[i + 1];
        node->ent = &work->ents[i];
        node->next = NULL;
        node->prev = NULL;
        cur = &nodes[0];
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (order == 1) {
                if (cur->ent->score < node->ent->score) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                    break;
                } else if (cur->ent->score == node->ent->score && cur->ent->time > node->ent->time) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                }
            } else {
                if (cur->ent->score > node->ent->score) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                    break;
                } else if (cur->ent->score == node->ent->score && cur->ent->time > node->ent->time) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                }
            }
        }
        if (j == i) {
            cur->next = node;
            node->prev = cur;
        }
    }
    p = &nodes[0];
    prevScore = -1;
    tie = 0;
    rank = -1;
    for (i = 0; i < work->count; i++) {
        p = p->next;
        work->sorted[i] = p->ent;
        if (prevScore != p->ent->score) {
            prevScore = p->ent->score;
            if (tie) {
                rank = i;
                tie = 0;
            } else {
                rank++;
            }
        } else {
            tie = 1;
        }
        work->sorted[i]->rank = rank;
    }
}
