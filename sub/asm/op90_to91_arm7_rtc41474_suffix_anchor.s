	.include "asm/macros.inc"
	.include "global.inc"

	.extern EXIi_SelectRcnt
	.extern RTCi_GpioEnd
	.extern RTCi_GpioSendCommand
	.extern RTCi_GpioSendData
	.extern RTCi_GpioStart
	.extern RtcChangeAlarmFormat12to24
	.extern RtcChangeAlarmFormat24to12
	.extern RtcGpioTransfer

.text
