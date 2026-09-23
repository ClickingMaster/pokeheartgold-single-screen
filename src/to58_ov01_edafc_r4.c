#include "field_move_tutor_menu_internal.h"
#include "system.h"
#include "constants/sndseq.h"
extern int IsPaletteFadeFinished(void);
extern void ov01_021EE634(MoveTutorMenu *);
extern void ov01_021EE568(MoveTutorMenu *);
void ov01_021EE49C(SysTask *, MoveTutorMenu *);
void ov01_021EE49C(SysTask *task, MoveTutorMenu *menu)
{
    if (menu->delay != 0) {
        menu->delay--;
        return;
    }
    if (IsPaletteFadeFinished()) {
        u16 oldRow;
        int input = ListMenu_ProcessInput(menu->listMenu);
        oldRow = menu->lastRow;
        ListMenuGetCurrentItemArrayId(menu->listMenu, &menu->lastRow);
        if (oldRow != menu->lastRow) PlaySE(SEQ_SE_DP_SELECT);
        int keys = gSystem.newAndRepeatedKeys;
        if ((keys & 0x40) || (keys & 0x80) || (keys & 0x20) || (keys & 0x10)) ov01_021EE634(menu);
        switch (input) {
        case -2:
            if (menu->cancellable == 1) {
                PlaySE(SEQ_SE_DP_SELECT);
                *menu->ret_p = 0xfffe;
                ov01_021EE568(menu);
                return;
            }
            break;
        case -1:
            break;
        default:
            PlaySE(SEQ_SE_DP_SELECT);
            *menu->ret_p = input;
            ov01_021EE568(menu);
            break;
        }
    }
}
