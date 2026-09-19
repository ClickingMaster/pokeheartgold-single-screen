#include "to52_overlay70_entry_state_machine_private.h"

#define FAIL_BEGIN()                        \
    do {                                    \
        _02246800.state = 24;               \
        ov38_0221C014();                    \
        _02246800.result = ov70_02238398(); \
        ov38_0221BFEC();                    \
        return;                             \
    } while (0)
#define FINISH()         \
    do {                 \
        ov38_0221BFEC(); \
        return;          \
    } while (0)

void ov70_022378DC(void) {
    u32 received;

    switch (_02246800.state) {
    case 2:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            switch (ov70_02246920.mode) {
            case 1:
                goto c2_r0;
            case 2:
                goto c2_rm5;
            case 3:
                goto c2_rm4;
            case 7:
                goto c2_rm1;
            case 8:
                goto c2_rm8;
            case 9:
                goto c2_rm9;
            case 10:
                goto c2_rm10;
            case 11:
                goto c2_rm11;
            case 12:
                goto c2_rm6;
            case 13:
                goto c2_rm7;
            case 14:
                goto c2_rm2;
            default:
                goto c2_rm13;
            }
c2_r0:
            _02246800.result = 0;
            goto c2_done;
c2_rm5:
            _02246800.result = -5;
            goto c2_done;
c2_rm4:
            _02246800.result = -4;
            goto c2_done;
c2_rm1:
            _02246800.result = -1;
            goto c2_done;
c2_rm6:
            _02246800.result = -6;
            goto c2_done;
c2_rm7:
            _02246800.result = -7;
            goto c2_done;
c2_rm8:
            _02246800.result = -8;
            goto c2_done;
c2_rm9:
            _02246800.result = -9;
            goto c2_done;
c2_rm10:
            _02246800.result = -10;
            goto c2_done;
c2_rm11:
            _02246800.result = -11;
            goto c2_done;
c2_rm2:
            _02246800.result = -2;
            goto c2_done;
c2_rm13:
            _02246800.result = -13;
c2_done:
            FINISH();
        default:
            return;
        }
    case 4:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            switch (ov70_02246920.mode) {
            case 1:
                goto c12_r0;
            case 2:
                goto c12_rm5;
            case 3:
                goto c12_rm4;
            case 5:
                goto c12_rm3;
            case 14:
                goto c12_rm2;
            default:
                goto c12_rm13;
            }
c12_r0:
            _02246800.result = 0;
            goto c12_done;
c12_rm5:
            _02246800.result = -5;
            goto c12_done;
c12_rm4:
            _02246800.result = -4;
            goto c12_done;
c12_rm3:
            _02246800.result = -3;
            goto c12_done;
c12_rm2:
            _02246800.result = -2;
            goto c12_done;
c12_rm13:
            _02246800.result = -13;
c12_done:
            FINISH();
        default:
            return;
        }
    case 6:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            if (ov38_0221C020() == 0x124) {
                _02246800.result = 0;
            } else {
                switch (ov70_02246900.response[0]) {
                case 5:
                    _02246800.result = -3;
                    break;
                case 3:
                    _02246800.result = -4;
                    break;
                case 14:
                    _02246800.result = -2;
                    break;
                default:
                    _02246800.result = -13;
                    break;
                }
            }
            FINISH();
        default:
            return;
        }
    case 8:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            if (ov38_0221C020() == 0x124) {
                _02246800.result = 1;
            } else {
                switch (ov70_02246900.response[0]) {
                case 5:
                    _02246800.result = -3;
                    break;
                case 3:
                    _02246800.result = -4;
                    break;
                case 4:
                    _02246800.result = 0;
                    break;
                case 14:
                    _02246800.result = -2;
                    break;
                default:
                    _02246800.result = -13;
                    break;
                }
            }
            FINISH();
        default:
            return;
        }
    case 10:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            switch (ov70_02246920.mode) {
            case 1:
                goto c10_r0;
            case 3:
                goto c10_rm4;
            case 5:
                goto c10_rm3;
            case 14:
                goto c10_rm2;
            default:
                goto c10_rm13;
            }
c10_r0:
            _02246800.result = 0;
            goto c10_done;
c10_rm3:
            _02246800.result = -3;
            goto c10_done;
c10_rm4:
            _02246800.result = -4;
            goto c10_done;
c10_rm2:
            _02246800.result = -2;
            goto c10_done;
c10_rm13:
            _02246800.result = -13;
c10_done:
            FINISH();
        default:
            return;
        }
    case 12:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            switch (ov70_02246920.mode) {
            case 1:
                goto c12b_r0;
            case 2:
                goto c12b_rm5;
            case 3:
                goto c12b_rm4;
            case 5:
                goto c12b_rm3;
            case 14:
                goto c12b_rm2;
            default:
                goto c12b_rm13;
            }
c12b_r0:
            _02246800.result = 0;
            goto c12b_done;
c12b_rm3:
            _02246800.result = -3;
            goto c12b_done;
c12b_rm5:
            _02246800.result = -5;
            goto c12b_done;
c12b_rm4:
            _02246800.result = -4;
            goto c12b_done;
c12b_rm2:
            _02246800.result = -2;
            goto c12b_done;
c12b_rm13:
            _02246800.result = -13;
c12b_done:
            FINISH();
        default:
            return;
        }
    case 14:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            received = ov38_0221C020();
            if (received >= 0x124) {
                _02246800.result = ov38_0221C020() / 0x124;
            } else if (received == 0) {
                _02246800.result = 0;
            } else if (ov70_02246900.response[0] == 14) {
                _02246800.result = -2;
            } else {
                _02246800.result = -13;
            }
            FINISH();
        default:
            return;
        }
    case 16:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            if (ov38_0221C020() == 0x124) {
                _02246800.result = 0;
            } else {
                switch (ov70_02246900.response[0]) {
                case 2:
                    goto c16_rm5;
                case 8:
                    goto c16_rm8;
                case 9:
                    goto c16_rm9;
                case 10:
                    goto c16_rm10;
                case 11:
                    goto c16_rm11;
                case 12:
                    goto c16_rm6;
                case 13:
                    goto c16_rm7;
                case 14:
                    goto c16_rm2;
                default:
                    goto c16_rm13;
                }
            }
            goto c16_done;
c16_rm5:
            _02246800.result = -5;
            goto c16_done;
c16_rm6:
            _02246800.result = -6;
            goto c16_done;
c16_rm7:
            _02246800.result = -7;
            goto c16_done;
c16_rm8:
            _02246800.result = -8;
            goto c16_done;
c16_rm9:
            _02246800.result = -9;
            goto c16_done;
c16_rm10:
            _02246800.result = -10;
            goto c16_done;
c16_rm11:
            _02246800.result = -11;
            goto c16_done;
c16_rm2:
            _02246800.result = -2;
            goto c16_done;
c16_rm13:
            _02246800.result = -13;
c16_done:
            FINISH();
        default:
            return;
        }
    case 18:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            switch (ov70_02246920.mode) {
            case 1:
                goto c18_r0;
            case 2:
                goto c18_rm5;
            case 14:
                goto c18_rm2;
            default:
                goto c18_done;
            }
c18_r0:
            _02246800.result = 0;
            goto c18_done;
c18_rm2:
            _02246800.result = -2;
            goto c18_done;
c18_rm5:
            _02246800.result = -5;
c18_done:
            FINISH();
        default:
            return;
        }
    case 20:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            switch (ov70_02246920.mode) {
            case 1:
                _02246800.result = 0;
                break;
            case 6:
                _02246800.result = 1;
                break;
            case 7:
                _02246800.result = 2;
                break;
            case 14:
                _02246800.result = -2;
                break;
            default:
                _02246800.result = -13;
                break;
            }
            FINISH();
        default:
            return;
        }
    case 22:
        switch (ov38_0221BF48()) {
        case 1:
            FAIL_BEGIN();
        case 7:
            _02246800.state = 24;
            if (ov38_0221C020() == 8) {
                _02246800.result = 0;
            } else {
                switch (ov70_02246920.mode) {
                case 1:
                    _02246800.result = 0;
                    break;
                case 6:
                    _02246800.result = 1;
                    break;
                case 7:
                    _02246800.result = 2;
                    break;
                case 14:
                    _02246800.result = -2;
                    break;
                default:
                    _02246800.result = -13;
                    break;
                }
            }
            FINISH();
        default:
            return;
        }
    case 3:
    case 5:
    case 7:
    case 9:
    case 11:
    case 13:
    case 15:
    case 17:
    case 19:
    case 21:
    case 23:
        _02246800.state = 24;
        _02246800.result = -12;
        FINISH();
    case 0:
    case 1:
    case 24:
    default:
        return;
    }
}
