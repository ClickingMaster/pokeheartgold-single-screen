#ifndef UNK_02089D68_PRIVATE_H
#define UNK_02089D68_PRIVATE_H
#include "global.h"
typedef struct SummaryState89 {
    void *bgConfig;
    u8 data[0x7B8];
} SummaryState89;
void PokemonSummary_RemovePageWindows(SummaryState89 *);
void sub_0208B448(SummaryState89 *);
void PokemonSummary_UpdateTypeSprites(SummaryState89 *);
void PokemonSummary_UpdatePokeathlonStars(SummaryState89 *);
void PokemonSummary_UpdateShinyLeafSprites(SummaryState89 *);
void PokemonSummary_ShowRibbonSprites(SummaryState89 *);
void PokemonSummary_InitSheenSparkles(SummaryState89 *);
void PokemonSummary_AddPageWindows(SummaryState89 *);
void PokemonSummary_PrintPage(SummaryState89 *);
void sub_02089F98(SummaryState89 *);
void sub_0208A950(SummaryState89 *);
void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
void BgCommitTilemapBufferToVram(void *, int);
void PlaySE(int);
void sub_02089D68(SummaryState89 *, int);
void sub_02089E14(SummaryState89 *, int);
void sub_02089E30(SummaryState89 *, int);
#endif
