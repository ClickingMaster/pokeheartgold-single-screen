.include "asm/macros.inc"
.extern ov18_021EE35C
.extern FillWindowPixelBuffer
.extern Pokedex_CheckMonCaughtFlag
.extern ov18_021F9648
.extern ov18_021E590C
.extern ov18_021F95FC
.extern String_Delete
.extern PlayerProfile_GetPlayerName_NewString
.extern ov18_021EEA84
.extern ov18_021EEB34
.extern PlayerProfile_GetTrainerGender
.extern ScheduleWindowCopyToVram
.extern ov18_021F9DE4
.text

	thumb_func_start ov18_021F05E8
ov18_021F05E8: ; 0x021F05E8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	ldr r1, _021F0820 ; =ov18_021F9DE4
	mov r2, #0xd
	add r5, r0, #0
	bl ov18_021EE35C
	mov r6, #0
	add r4, r5, #0
	add r4, #0xc
	add r7, r6, #0
_021F05FE:
	add r0, r4, #0
	add r1, r7, #0
	bl FillWindowPixelBuffer
	add r6, r6, #1
	add r4, #0x10
	cmp r6, #0xd
	blo _021F05FE
	ldr r1, _021F0824 ; =0x000018A2
	ldr r0, [r5]
	ldrh r1, [r5, r1]
	ldr r0, [r0]
	bl Pokedex_CheckMonCaughtFlag
	cmp r0, #0
	beq _021F0622
	mov r4, #2
	b _021F0624
_021F0622:
	mov r4, #1
_021F0624:
	mov r3, #0
	str r3, [sp]
	mov r0, #4
	str r0, [sp, #4]
	ldr r0, _021F0828 ; =0x00020100
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0xc
	mov r2, #0x8f
	bl ov18_021F9648
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0828 ; =0x00020100
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0x3c
	mov r2, #0x88
	mov r3, #0x30
	bl ov18_021F9648
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0828 ; =0x00020100
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0x4c
	mov r2, #0xa
	mov r3, #0x10
	bl ov18_021F9648
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0828 ; =0x00020100
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0x6c
	mov r2, #0xa
	mov r3, #0x10
	bl ov18_021F9648
	ldr r0, _021F0824 ; =0x000018A2
	mov r1, #2
	ldrh r0, [r5, r0]
	mov r2, #0x25
	bl ov18_021E590C
	add r6, r0, #0
	mov r3, #0
	ldr r0, _021F0828 ; =0x00020100
	str r3, [sp]
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	add r0, r5, #0
	add r0, #0x1c
	add r1, r6, #0
	mov r2, #0x20
	bl ov18_021F95FC
	add r0, r6, #0
	bl String_Delete
	ldr r0, [r5]
	mov r1, #0x25
	ldr r0, [r0, #4]
	bl PlayerProfile_GetPlayerName_NewString
	add r6, r0, #0
	mov r3, #0
	ldr r0, _021F0828 ; =0x00020100
	str r3, [sp]
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	add r0, r5, #0
	add r0, #0x2c
	add r1, r6, #0
	mov r2, #0x20
	bl ov18_021F95FC
	add r0, r6, #0
	bl String_Delete
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, _021F0828 ; =0x00020100
	ldr r1, _021F0824 ; =0x000018A2
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	ldrh r1, [r5, r1]
	add r0, r5, #0
	add r2, r4, #0
	mov r3, #5
	bl ov18_021EEA84
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0830 ; =0x00050900
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0x8c
	mov r2, #0x89
	mov r3, #0x30
	bl ov18_021F9648
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0834 ; =0x000F0500
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0x9c
	mov r2, #0xb
	mov r3, #0x10
	bl ov18_021F9648
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0834 ; =0x000F0500
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0xbc
	mov r2, #0xb
	mov r3, #0x10
	bl ov18_021F9648
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, _021F0834 ; =0x000F0500
	ldr r1, _021F0824 ; =0x000018A2
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	ldrh r1, [r5, r1]
	add r0, r5, #0
	add r2, r4, #0
	mov r3, #0xa
	bl ov18_021EEB34
	ldr r0, [r5]
	ldr r0, [r0, #4]
	bl PlayerProfile_GetTrainerGender
	cmp r0, #0
	ldr r1, _021F082C ; =0x0000065C
	bne _021F07D0
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0828 ; =0x00020100
	mov r2, #0x8a
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0x7c
	mov r3, #0x20
	bl ov18_021F9648
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0834 ; =0x000F0500
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0xcc
	mov r2, #0x8c
	mov r3, #0x20
	bl ov18_021F9648
	b _021F080A
_021F07D0:
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0828 ; =0x00020100
	mov r2, #0x8b
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0x7c
	mov r3, #0x20
	bl ov18_021F9648
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021F0834 ; =0x000F0500
	ldr r1, _021F082C ; =0x0000065C
	str r0, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r5, #0
	ldr r1, [r5, r1]
	add r0, #0xcc
	mov r2, #0x8d
	mov r3, #0x20
	bl ov18_021F9648
_021F080A:
	mov r4, #0
	add r5, #0xc
_021F080E:
	add r0, r5, #0
	bl ScheduleWindowCopyToVram
	add r4, r4, #1
	add r5, #0x10
	cmp r4, #0xd
	blo _021F080E
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F0820: .word ov18_021F9DE4
_021F0824: .word 0x000018A2
_021F0828: .word 0x00020100
_021F082C: .word 0x0000065C
_021F0830: .word 0x00050900
_021F0834: .word 0x000F0500
	thumb_func_end ov18_021F05E8
