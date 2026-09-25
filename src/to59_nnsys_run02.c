typedef unsigned char u8;
typedef unsigned short u16;

typedef struct List {
    void *head;
    void *tail;
    u16 count;
    u16 offset;
} List;

typedef struct Link {
    void *prev;
    void *next;
} Link;

#define GetLink(list, object) ((Link *)((u8 *)(object) + (list)->offset))

void NNS_FndInitList(List *list, u16 offset);
void SetFirstObject(List *list, void *object);
void NNS_FndAppendListObject(List *list, void *object);
void NNS_FndPrependListObject(List *list, void *object);
void NNS_FndInsertListObject(List *list, void *target, void *object);
void NNS_FndRemoveListObject(List *list, void *object);
void *NNS_FndGetNextListObject(List *list, void *object);
void *NNS_FndGetPrevListObject(List *list, void *object);

void NNS_FndRemoveListObject(List *list, void *object) {
    Link *link = GetLink(list, object);
    if (!link->prev) {
        list->head = link->next;
    } else {
        GetLink(list, link->prev)->next = link->next;
    }
    if (!link->next) {
        list->tail = link->prev;
    } else {
        GetLink(list, link->next)->prev = link->prev;
    }
    link->prev = 0;
    link->next = 0;
    list->count--;
}

void *NNS_FndGetNextListObject(List *list, void *object) {
    void *result;
    if (object == 0) {
        result = list->head;
    } else {
        result = GetLink(list, object)->next;
    }
    return result;
}

void *NNS_FndGetPrevListObject(List *list, void *object) {
    void *result;
    if (object == 0) {
        result = list->tail;
    } else {
        result = GetLink(list, object)->prev;
    }
    return result;
}
