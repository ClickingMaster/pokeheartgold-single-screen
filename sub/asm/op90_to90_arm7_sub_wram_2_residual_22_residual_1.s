	.include "asm/macros.inc"
	.include "global.inc"
	.extern WMSP_Init
	.extern wvrWlStaElement
	.extern wvrWlWork
	.public __VENEER_WMSP_Init

.text
	arm_func_start __VENEER_WMSP_Init
__VENEER_WMSP_Init: ; 0x03805B10
	ldr pc, _03805B14 ; =WMSP_Init
	.align 2, 0
_03805B14: .word WMSP_Init
	arm_func_end __VENEER_WMSP_Init
