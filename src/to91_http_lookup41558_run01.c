#include "global.h"

#include "to91_http_lookup41558_private.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef union HttpLookupKey {
    int id;
    const void *pointer;
} HttpLookupKey;

typedef union HttpLookupValue {
    void *object;
    const void *bytes;
} HttpLookupValue;

typedef struct HttpQueueNode {
    struct HttpQueueNode *prev, *next;
    HttpLookupKey key;
    HttpLookupValue value;
    int size;
} HttpQueueNode;

typedef char HttpQueueNodeSize20[(sizeof(HttpQueueNode) == 20) ? 1 : -1];
extern void *ov38_02226230;

void *ov38_02221EE0(int id) {
    HttpQueueNode *res = 0;
    HttpQueueNode *const head = ov38_02226230;
    if (head) {
        if (head->key.id == id) {
            res = head;
        } else {
            HttpQueueNode *n = head->next;
            while (n != head) {
                if (n->key.id == id) {
                    res = n;
                    break;
                }
                n = n->next;
            }
        }
    }
    return res;
}
