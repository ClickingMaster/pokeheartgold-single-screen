	.include "asm/macros.inc"
	.include "overlay_91.inc"
	.include "global.inc"
	.text
	.public ov91_0225D3B4
	.extern ov91_0225F3A0

	thumb_func_start ov91_0225D3B4
ov91_0225D3B4: ; 0x0225D3B4
	ldr r3, _0225D3BC ; =0x000087CC
	add r0, r0, r3
	ldr r3, _0225D3C0 ; =ov91_0225F3A0
	bx r3
	.balign 4, 0
_0225D3BC: .word 0x000087CC
_0225D3C0: .word ov91_0225F3A0
	thumb_func_end ov91_0225D3B4
