#include "global.h"

#include "constants/apricorns.h"

#include "apricorn_tree.h"
#include "save_misc_data.h"

static const u8 sTreeApricorns[NUM_APRICORN_TREE] = {
    APRICORN_BLK, // Route 1
    APRICORN_PNK, // Route 2 North
    APRICORN_YLW, // Route 8
    APRICORN_GRN, // Route 11
    APRICORN_BLU, // Route 26
    APRICORN_GRN, // Route 29
    APRICORN_GRN, // Route 30
    APRICORN_PNK, // Route 30
    APRICORN_BLK, // Route 31
    APRICORN_PNK, // Route 33
    APRICORN_BLK, // Route 33
    APRICORN_GRN, // Route 35
    APRICORN_BLU, // Route 36
    APRICORN_RED, // Route 37
    APRICORN_BLU, // Route 37
    APRICORN_BLK, // Route 37
    APRICORN_WHT, // Route 38
    APRICORN_GRN, // Route 39
    APRICORN_PNK, // Route 42
    APRICORN_GRN, // Route 42
    APRICORN_YLW, // Route 42
    APRICORN_BLK, // Route 43
    APRICORN_RED, // Route 44
    APRICORN_GRN, // Route 45
    APRICORN_GRN, // Route 46
    APRICORN_YLW, // Route 46
    APRICORN_BLU, // Pewter City
    APRICORN_WHT, // Pewter City
    APRICORN_RED, // Fuchsia City
    APRICORN_YLW, // Violet City
    APRICORN_WHT, // Azalea Town
};

void ApricornTree_Clear(APRICORN_TREE *tree);

void ApricornTrees_Init(APRICORN_TREE *trees) {
    int i;
    MI_CpuClear8(trees, sizeof(APRICORN_TREE) * MAX_APRICORN_TREE);
    for (i = 0; i < MAX_APRICORN_TREE; i++) {
        trees[i].hasApricorn = 0;
    }
}

void ApricornTrees_GrowAll(APRICORN_TREE *trees) {
    int i;
    for (i = 0; i < MAX_APRICORN_TREE || i < NUM_APRICORN_TREE; i++) {
        ApricornTree_Clear(&trees[i]);
        trees[i].hasApricorn = 1;
        trees[i].unk_1 = 1;
    }
}

void ApricornTree_Clear(APRICORN_TREE *tree) {
    tree->hasApricorn = 0;
    tree->unk_1 = 0;
}

u8 ApricornTrees_HasApricorn(const APRICORN_TREE *trees, int idx) {
    return trees[idx].hasApricorn;
}

// Returns the Apricorn type or APRICORN_NONE if undefined.
u8 ApricornTrees_TryGetApricorn(const APRICORN_TREE *trees, int idx) {
#pragma unused(trees)
    return sTreeApricorns[idx];
}

// Returns the Apricorn type. Defaults to APRICORN_RED if Apricorn type is
// undefined.
int ApricornTrees_GetApricorn(const APRICORN_TREE *trees, int idx) {
#pragma unused(trees)
    int ret;
    ret = sTreeApricorns[idx];
    if (ret <= 0) {
        GF_ASSERT(FALSE);
        return 0;
    }
    return ret - 1;
}

void ApricornTrees_PickApricorn(APRICORN_TREE *trees, int idx) {
    ApricornTree_Clear(&trees[idx]);
}

void ApricornTrees_RegrowEmpty(APRICORN_TREE *trees) {
    int i;
    for (i = 0; i < MAX_APRICORN_TREE; i++) {
        if (!trees[i].hasApricorn) {
            trees[i].hasApricorn = 1;
            trees[i].unk_1 = 1;
        }
    }
}
