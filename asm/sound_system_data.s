#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "sound_system_data.inc"
	.include "global.inc"

	.rodata

	.bss

	.public gSoundSystemFlags
gSoundSystemFlags:
	.space 0x10

	.public gSoundMicAutoParam
gSoundMicAutoParam:
	.space 0x28

	.public sSoundChatotBuffer
sSoundChatotBuffer:
	.space 0x7D0

