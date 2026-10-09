	.include "asm/macros.inc"
	.include "global.inc"
	.public __Veneer_SVC_GetCRC16
	.public __Veneer_SVC_Halt

.text
	.arm
	.balign 4, 0
__Veneer_SVC_GetCRC16: ; 0x037F8470
	ldr ip, _037F8478 ; =SVC_GetCRC16
	bx ip
	.align 2, 0
_037F8478: .word SVC_GetCRC16
	arm_func_end __Veneer_SVC_GetCRC16

	.arm
	.balign 4, 0
__Veneer_SVC_Halt: ; 0x037F847C
	ldr ip, _037F8484 ; =SVC_Halt
	bx ip
	.align 2, 0
_037F8484: .word SVC_Halt
	arm_func_end __Veneer_SVC_Halt
