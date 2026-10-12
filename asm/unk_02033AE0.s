	.include "asm/macros.inc"
	.include "unk_02033AE0.inc"
	.include "global.inc"

	.bss

	.public gCommCommandTable
gCommCommandTable:
	.space 0x4

	.rodata

	.public _020F692C
_020F692C:
	.byte 0x01, 0x01, 0x01, 0x01
	.byte 0x03, 0x03, 0x03, 0x04, 0x03, 0x04, 0x07, 0x03, 0x07, 0x04, 0x01, 0x04, 0x01, 0x01, 0x04, 0x01
	.byte 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x04, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x03, 0x01, 0x03
	.byte 0x00, 0x01, 0x01, 0x01, 0x03
	.public _020F6955
_020F6955:
	.byte 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01
	.byte 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01
	.byte 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00
	.public gCommBaseCommands
gCommBaseCommands:
	.word 0, CommCmd_GetSize0, 0
	.word 0, CommCmd_GetSize0, 0
	.word sub_0203776C, CommCmd_GetSize0, 0
	.word CommPlayerInfo_RecvPlayerEntry, CommPlayerInfo_GetEntrySize, 0
	.word CommPlayerInfo_RecvEntry, CommPlayerInfo_GetEntrySize, 0
	.word sub_02034524, CommCmd_GetSize0, 0
	.word CommState_RecvJoinRequest, CommState_GetJoinMessageSize, 0
	.word CommState_RecvJoinReply, CommState_GetJoinMessageSize, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word sub_02037618, CommCmd_GetSize1, 0
	.word sub_02037640, CommCmd_GetSize1, 0
	.word sub_02037668, CommCmd_GetSize1, 0
	.word CommCmd_RecvReady, CommCmd_GetSize0, 0
	.word CommCmd_RecvShutdown, CommCmd_GetSize0, 0
	.word CommCmd_RecvNotReady, CommCmd_GetSize0, 0
	.word LinkSync_RecvRequest, CommCmd_GetSize1, 0
	.word LinkSync_RecvDone, CommCmd_GetSize1, 0
	.word LinkSync_RecvState, CommCmd_GetSize2, 0
	.word LinkSync_RecvValue, LinkSync_GetValueSize, 0
	.word LinkSync_RecvPacket, CommState_GetSyncPacketSize, 0
	.word CommState_RecvWifiDisconnect, CommCmd_GetSize0, 0

	.text

	thumb_func_start ByteRing_GetReadableSize
ByteRing_GetReadableSize: ; 0x02033BC4
	mov r1, #4
	mov r3, #6
	ldrsh r1, [r0, r1]
	ldrsh r2, [r0, r3]
	cmp r1, r2
	ble _02033BDC
	mov r2, #0xa
	ldrsh r2, [r0, r2]
	ldrsh r0, [r0, r3]
	add r0, r2, r0
	sub r0, r0, r1
	bx lr
_02033BDC:
	ldrsh r0, [r0, r3]
	sub r0, r0, r1
	bx lr
	.balign 4, 0
	thumb_func_end ByteRing_GetReadableSize
	thumb_func_start sub_02033BE4
sub_02033BE4: ; 0x02033BE4
	push {r4, lr}
	add r4, r0, #0
	bl ByteRing_GetReadableSize
	mov r1, #0xa
	ldrsh r1, [r4, r1]
	sub r0, r1, r0
	pop {r4, pc}
	thumb_func_end sub_02033BE4
	thumb_func_start ByteRing_GetWritableSize
ByteRing_GetWritableSize: ; 0x02033BF4
	mov r1, #4
	mov r2, #8
	ldrsh r3, [r0, r1]
	ldrsh r1, [r0, r2]
	cmp r3, r1
	ble _02033C06
	ldrsh r0, [r0, r2]
	sub r0, r3, r0
	bx lr
_02033C06:
	mov r1, #0xa
	ldrsh r1, [r0, r1]
	ldrsh r0, [r0, r2]
	sub r0, r0, r3
	sub r0, r1, r0
	bx lr
	.balign 4, 0
	thumb_func_end ByteRing_GetWritableSize
	thumb_func_start ByteRing_Wrap
ByteRing_Wrap: ; 0x02033C14
	push {r3, lr}
	add r2, r0, #0
	add r0, r1, #0
	mov r1, #0xa
	ldrsh r1, [r2, r1]
	bl _s32_div_f
	add r0, r1, #0
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end ByteRing_Wrap
	thumb_func_start ByteRing_Commit
ByteRing_Commit: ; 0x02033C28
	mov r1, #8
	ldrsh r1, [r0, r1]
	strh r1, [r0, #6]
	bx lr
	thumb_func_end ByteRing_Commit
	thumb_func_start PacketPool_GetFreeNode
PacketPool_GetFreeNode: ; 0x02033C30
	ldr r3, [r0, #0x1c]
	mov r2, #0
	ldr r1, [r0, #0x18]
	cmp r3, #0
	ble _02033C4C
_02033C3A:
	ldrb r0, [r1, #0xe]
	cmp r0, #0
	bne _02033C44
	add r0, r1, #0
	bx lr
_02033C44:
	add r2, r2, #1
	add r1, #0x10
	cmp r2, r3
	blt _02033C3A
_02033C4C:
	mov r0, #0
	bx lr
	thumb_func_end PacketPool_GetFreeNode
	thumb_func_start PacketPool_AllNodesFree
PacketPool_AllNodesFree: ; 0x02033C50
	ldr r3, [r0, #0x1c]
	mov r2, #0
	ldr r1, [r0, #0x18]
	cmp r3, #0
	ble _02033C6C
_02033C5A:
	ldrb r0, [r1, #0xe]
	cmp r0, #0
	beq _02033C64
	mov r0, #0
	bx lr
_02033C64:
	add r2, r2, #1
	add r1, #0x10
	cmp r2, r3
	blt _02033C5A
_02033C6C:
	mov r0, #1
	bx lr
	thumb_func_end PacketPool_AllNodesFree
