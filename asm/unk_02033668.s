	.include "asm/macros.inc"
	.include "unk_02032844.inc"
	.include "global.inc"

	.text
	thumb_func_start Wireless_StartParent
Wireless_StartParent: ; 0x02033668
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r6, r1, #0
	add r7, r2, #0
	add r4, r3, #0
	bl sub_0203993C
	bl sub_020340C4
	cmp r0, #0
	beq _0203368C
	ldr r0, _0203372C ; =Wireless_OnLifeTimeSet
	mov r2, #0x64
	ldr r1, _02033730 ; =0x0000FFFF
	mov r3, #5
	str r2, [sp]
	bl WM_SetLifeTime
_0203368C:
	ldr r1, _02033734 ; =gWirelessManager
	mov r0, #7
	ldr r2, [r1, #4]
	ldr r1, _02033738 ; =0x00001308
	lsl r0, r0, #6
	str r0, [r2, r1]
	ldr r0, _02033734 ; =gWirelessManager
	mov r3, #0xe0
	ldr r2, [r0, #4]
	sub r0, r1, #4
	str r3, [r2, r0]
	ldr r0, _02033734 ; =gWirelessManager
	add r1, #0xc
	ldr r0, [r0, #4]
	str r5, [r0, r1]
	mov r0, #3
	bl Wireless_SetState
	ldr r1, _02033734 ; =gWirelessManager
	cmp r5, #0
	ldr r0, [r1, #4]
	strh r6, [r0, #0xc]
	ldr r0, [r1, #4]
	strh r7, [r0, #0x32]
	add r0, sp, #8
	ldrh r2, [r0, #0x10]
	ldr r0, [r1, #4]
	strh r2, [r0, #0x18]
	beq _020336CC
	cmp r5, #4
	beq _020336E6
	b _020336F2
_020336CC:
	ldr r0, [r1, #4]
	mov r2, #0xc0
	strh r2, [r0, #0x34]
	cmp r4, #5
	blo _020336DE
	ldr r0, [r1, #4]
	mov r2, #0xc
	strh r2, [r0, #0x36]
	b _020336F2
_020336DE:
	ldr r0, [r1, #4]
	mov r2, #0x26
	strh r2, [r0, #0x36]
	b _020336F2
_020336E6:
	ldr r0, [r1, #4]
	mov r2, #0x64
	strh r2, [r0, #0x34]
	ldr r0, [r1, #4]
	mov r2, #0xc
	strh r2, [r0, #0x36]
_020336F2:
	ldr r1, _02033734 ; =gWirelessManager
	ldr r0, [r1, #4]
	strh r4, [r0, #0x10]
	ldr r2, [r1, #4]
	mov r0, #0
	strh r0, [r2, #0x16]
	ldr r2, [r1, #4]
	cmp r5, #2
	strh r0, [r2, #0x12]
	ldr r2, [sp, #0x1c]
	ldr r1, [r1, #4]
	strh r2, [r1, #0xe]
	bne _0203370E
	mov r0, #1
_0203370E:
	ldr r1, _02033734 ; =gWirelessManager
	cmp r5, #0
	ldr r1, [r1, #4]
	strh r0, [r1, #0x14]
	beq _02033720
	cmp r5, #2
	beq _02033720
	cmp r5, #4
	bne _02033726
_02033720:
	bl Wireless_SendParentParameter
	pop {r3, r4, r5, r6, r7, pc}
_02033726:
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0203372C: .word Wireless_OnLifeTimeSet
_02033730: .word 0x0000FFFF
_02033734: .word gWirelessManager
_02033738: .word 0x00001308
	thumb_func_end Wireless_StartParent
	thumb_func_start Wireless_StartChild
Wireless_StartChild: ; 0x0203373C
	push {r3, r4, r5, r6, lr}
	sub sp, #4
	add r5, r0, #0
	add r4, r1, #0
	bl sub_0203993C
	bl sub_020340C4
	cmp r0, #0
	beq _0203375E
	ldr r0, _020337BC ; =Wireless_OnLifeTimeSet
	mov r2, #0x64
	ldr r1, _020337C0 ; =0x0000FFFF
	mov r3, #5
	str r2, [sp]
	bl WM_SetLifeTime
_0203375E:
	ldr r2, _020337C4 ; =gWirelessManager
	mov r3, #2
	ldr r0, [r2, #4]
	ldr r1, _020337C8 ; =0x00001308
	lsl r3, r3, #8
	str r3, [r0, r1]
	sub r0, r1, #4
	ldr r3, [r2, #4]
	mov r6, #0x40
	str r6, [r3, r0]
	ldr r0, [r2, #4]
	add r1, #0xc
	str r5, [r0, r1]
	mov r0, #3
	bl Wireless_SetState
	cmp r5, #1
	beq _0203378A
	cmp r5, #3
	beq _0203378A
	cmp r5, #5
	bne _020337B4
_0203378A:
	ldr r1, _020337C4 ; =gWirelessManager
	add r0, r4, #0
	ldr r2, [r1, #4]
	ldr r1, _020337CC ; =0x00001220
	add r1, r2, r1
	mov r2, #0xc0
	bl MI_CpuCopy8
	ldr r0, _020337C4 ; =gWirelessManager
	ldr r1, [r0, #4]
	ldr r0, _020337CC ; =0x00001220
	add r0, r1, r0
	mov r1, #0xc0
	bl DC_FlushRange
	bl DC_WaitWriteBufferEmpty
	bl Wireless_StartConnect
	add sp, #4
	pop {r3, r4, r5, r6, pc}
_020337B4:
	mov r0, #0
	add sp, #4
	pop {r3, r4, r5, r6, pc}
	nop
_020337BC: .word Wireless_OnLifeTimeSet
_020337C0: .word 0x0000FFFF
_020337C4: .word gWirelessManager
_020337C8: .word 0x00001308
_020337CC: .word 0x00001220
	thumb_func_end Wireless_StartChild
