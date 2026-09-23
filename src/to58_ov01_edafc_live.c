#include "field_move_tutor_menu_internal.h"
extern int IsPaletteFadeFinished(void);
extern int Handle2dMenuInput(struct ListMenu2D *);
extern void ov01_021EDF38(MoveTutorMenu *);
void ov01_021EDE8C(SysTask *task, MoveTutorMenu *menu);
void ov01_021EDE8C(SysTask *task, MoveTutorMenu *menu)
{
    if (menu->delay != 0) {
        menu->delay--;
        return;
    }
    if (IsPaletteFadeFinished()) {
        int input = Handle2dMenuInput(menu->menu2dPtr);
        switch (input) {
        case -2:
            if (menu->cancellable == 1) {
                *menu->ret_p = 0xfffe;
                ov01_021EDF38(menu);
                return;
            }
            break;
        case -1:
            break;
        default:
            *menu->ret_p = input;
            ov01_021EDF38(menu);
            break;
        }
    }
}
