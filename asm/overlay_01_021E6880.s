#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/flags.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "constants/phone_constants.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "asm/macros.inc"
	.include "overlay_01_021E6880.inc"
	.include "global.inc"

	.text

	.rodata

	.public ov01_02206378
ov01_02206378:
	.word ov01_021E5924, ov01_021E5BE4, ov01_021E5C24, 0xFFFFFFFF

	.public sBGMVolume
sBGMVolume:
	.byte 0x60, 0x40, 0x20, 0x00

	.public sSoundplateVolume
sSoundplateVolume: ; 0x0220638C
	.byte 0x40, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x40, 0x60, 0x7F
	.byte 0x2E, 0x40, 0x60
	.byte 0x40, 0x60, 0x6C
	.byte 0x2E, 0x60, 0x6C
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x48, 0x6C
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x40, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F

	.public sSoundplateSounds
sSoundplateSounds: ; 0x022063BC
	.short SEQ_SE_GS_N_SESERAGI, 0x0001
	.short SEQ_SE_GS_N_HUUSHA, 0x0000
	.short SEQ_SE_GS_N_UMIBE, 0x0000
	.short SEQ_SE_GS_N_HASHIRA, 0x0001
	.short SEQ_SE_GS_N_UZUSIO, 0x0000
	.short SEQ_SE_GS_N_TAKI, 0x0000
	.short SEQ_SE_GS_N_YOUGAN, 0x0001
	.short SEQ_SE_GS_N_KANSEI, 0x0000
	.short SEQ_SE_GS_N_KITEKI, 0x0000
	.short SEQ_SE_GS_KABIGON_IBIKI, 0x0001
	.short SEQ_SE_GS_N_MOTER, 0x0001
	.short SEQ_SE_GS_N_KANE, 0x0001
	.short SEQ_SE_GS_KYOUHUU, 0x0001
	.short SEQ_SE_GS_N_ENGINE, 0x0001
	.short SEQ_SE_GS_N_HUNSUI, 0x0000
	.short SEQ_SE_GS_DENGEKIBARIA, 0x0000
