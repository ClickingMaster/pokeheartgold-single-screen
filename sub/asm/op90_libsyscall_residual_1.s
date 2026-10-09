	.include "asm/macros.inc"
	.include "global.inc"

.text
	thumb_func_start SVC_WaitVBlankIntr
SVC_WaitVBlankIntr: ; 0x038008BC
	mov r2, #0
	swi 5
	bx lr
	thumb_func_end SVC_WaitVBlankIntr

	non_word_aligned_thumb_func_start SVC_Halt
SVC_Halt: ; 0x038008C2
	swi 6
	bx lr
	thumb_func_end SVC_Halt

	non_word_aligned_thumb_func_start SVC_Sleep
SVC_Sleep: ; 0x038008C6
	swi 7
	bx lr
	thumb_func_end SVC_Sleep

	non_word_aligned_thumb_func_start SVC_ChangeSoundBias
SVC_ChangeSoundBias: ; 0x038008CA
	swi 8
	bx lr
	thumb_func_end SVC_ChangeSoundBias

	non_word_aligned_thumb_func_start SVC_SetSoundBias
SVC_SetSoundBias: ; 0x038008CE
	add r1, r0, #0
	mov r0, #1
	swi 8
	bx lr
	thumb_func_end SVC_SetSoundBias

	non_word_aligned_thumb_func_start SVC_ResetSoundBias
SVC_ResetSoundBias: ; 0x038008D6
	add r1, r0, #0
	mov r0, #0
	swi 8
	bx lr
	thumb_func_end SVC_ResetSoundBias

	non_word_aligned_thumb_func_start SVC_Div
SVC_Div: ; 0x038008DE
	swi 9
	bx lr
	thumb_func_end SVC_Div

	non_word_aligned_thumb_func_start SVC_DivRem
SVC_DivRem: ; 0x038008E2
	swi 9
	add r0, r1, #0
	bx lr
	thumb_func_end SVC_DivRem
