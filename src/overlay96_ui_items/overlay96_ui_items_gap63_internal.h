#ifndef OVERLAY96_UI_ITEMS_GAP63_INTERNAL_H
#define OVERLAY96_UI_ITEMS_GAP63_INTERNAL_H
#include "global.h"
typedef struct Ov96R63Item {
    int active;
    void *handle;
    int delay;
    int played;
} Ov96R63Item;
int ov96_021F8978(void *);
void ov96_021F895C(void *);
void PlaySE(int);
void ov96_021F893C(void *, int *, int *);
void ov96_021F8948(void *, int, int);
void ov96_021F8128(Ov96R63Item *, int, int);
int ov96_021F8180(Ov96R63Item *, int);
#endif
