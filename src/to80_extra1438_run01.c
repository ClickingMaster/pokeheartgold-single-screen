#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Node {
    u8 pad[0x24];
    struct Node *next;
} Node;

typedef struct {
    u8 pad[0x814];
    Node *head, *tail;
} List;

extern const char ov45_022565C0[];
extern const char ov45_022565D0[];
extern const char ov45_02255F74[];
extern const char ov45_02256644[];
void __msl_assertion_failed(const char *, const char *, const char *, int);
void ov45_02249904(Node *);
void ov45_02249958(List *, Node *);

typedef struct {
    int type, unused;
    void *arg, *arg2;
} Request;

void ov45_02249A0C(List *, Request *, void *);
void ov45_0224A14C(List *, Request *);
extern const char ov45_02256654[];
extern const char ov45_02256010[];
extern const char ov45_0225665C[];
extern const char ov45_02256694[];
extern const char ov45_02256690[];

void ov45_02249958(List *list, Node *target) {
    Node *previous = NULL, *head, *node;
    if (!list) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255F74, 0x19f);
    }
    if (!target) {
        __msl_assertion_failed(ov45_02256644, ov45_022565D0, ov45_02255F74, 0x1a1);
    }
    head = list->head;
    node = head;
    while (node) {
        if (node == target) {
            if (head == node) {
                list->head = node->next;
            }
            if (list->tail == node) {
                list->tail = previous;
            }
            if (previous) {
                previous->next = node->next;
            }
            ov45_02249904(node);
            return;
        }
        previous = node;
        node = node->next;
    }
}
