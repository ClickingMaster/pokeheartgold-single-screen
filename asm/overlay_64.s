	.include "asm/macros.inc"
	.include "overlay_64.inc"
	.include "global.inc"
	.public HallOfFameShowcase_Init
	.public HallOfFameShowcase_Main
	.public _021E6E7C
	.public ov64_021E5B10
	.public ov64_021E5CD0
	.public ov64_021E607C
	.public ov64_021E6170
	.public ov64_021E62C8
	.public ov64_021E652C
	.public ov64_021E677C
	.public ov64_021E6B84
	.public ov64_021E6C1C
	.public ov64_021E6E88
	.public ov64_021E6E98
	.public ov64_021E6EA8
	.public ov64_021E6EB8
	.public ov64_021E6ECC
	.public ov64_021E6ECE
	.public ov64_021E6EE4
	.public ov64_021E6EFC
	.public ov64_021E6F18
	.public ov64_021E6F34
	.public ov64_021E6F50
	.public ov64_021E6F6C
	.public ov64_021E6F8C
	.public ov64_021E6FAC
	.public ov64_021E6FD4
	.public ov64_021E7008
	.public ov64_021E7040
	.public ov64_021E70C8
	.public ov64_021E70FC
	.public ov64_021E73A0
	.public ov64_021E73D4
	.public ov64_021E7408
	.public ov64_021E743C


	.text
	.public HallOfFameShowcase_Exit
	.public ov64_021E5A88
	.public ov64_021E5AAC
	.public ov64_021E5AC8
	.public ov64_021E5AE4
	.public ov64_021E5B00
	.public ov64_021E5CA4
	.public ov64_021E6010
	.public ov64_021E602C
	.public ov64_021E605C
	.public ov64_021E6118
	.public ov64_021E620C
	.public ov64_021E622C
	.public ov64_021E6260
	.public ov64_021E6270
	.public ov64_021E6274
	.public ov64_021E6278
	.public ov64_021E6280
	.public ov64_021E6288
	.public ov64_021E62A8
	.public ov64_021E64F8
	.public ov64_021E6754
	.public ov64_021E6BD8
	.public ov64_021E6E30

	thumb_func_start HallOfFameShowcase_Init
HallOfFameShowcase_Init: ; 0x021E5900
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	bl HBlankInterruptDisable
	mov r0, #0
	bl GfGfx_EngineASetPlanes
	mov r0, #0
	bl GfGfx_EngineBSetPlanes
	ldr r0, _021E59B4 ; =0x04000050
	mov r1, #0
	strh r1, [r0]
	ldr r0, _021E59B8 ; =0x04001050
	strh r1, [r0]
	bl sub_020210BC
	mov r0, #4
	bl sub_02021148
	ldr r2, _021E59BC ; =0x04000304
	ldr r0, _021E59C0 ; =0xFFFF7FFF
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
	mov r2, #1
	mov r0, #3
	mov r1, #0x3b
	lsl r2, r2, #0x12
	bl Heap_Create
	mov r1, #0x1d
	add r0, r5, #0
	lsl r1, r1, #4
	mov r2, #0x3b
	bl OverlayManager_CreateAndGetData
	mov r2, #0x1d
	mov r1, #0
	lsl r2, r2, #4
	add r4, r0, #0
	bl MI_CpuFill8
	add r0, r5, #0
	bl OverlayManager_GetArgs
	str r0, [r4]
	bl Save_HOF_GetNumRecords
	mov r1, #0x6e
	lsl r1, r1, #2
	str r0, [r4, r1]
	mov r0, #0xb4
	mov r1, #0x3b
	bl NARC_New
	mov r1, #0x61
	lsl r1, r1, #2
	str r0, [r4, r1]
	bl ov64_021E5B00
	add r0, r4, #0
	bl ov64_021E5B10
	add r0, r4, #0
	bl ov64_021E5CD0
	add r0, r4, #0
	bl ov64_021E607C
	add r0, r4, #0
	bl ov64_021E6170
	add r0, r4, #0
	bl ov64_021E622C
	add r0, r4, #0
	mov r1, #0
	bl ov64_021E652C
	ldr r0, _021E59C4 ; =ov64_021E5A88
	add r1, r4, #0
	bl Main_SetVBlankIntrCB
	mov r0, #1
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021E59B4: .word 0x04000050
_021E59B8: .word 0x04001050
_021E59BC: .word 0x04000304
_021E59C0: .word 0xFFFF7FFF
_021E59C4: .word ov64_021E5A88
	thumb_func_end HallOfFameShowcase_Init
