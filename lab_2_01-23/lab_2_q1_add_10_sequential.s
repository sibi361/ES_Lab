	AREA	RESET, DATA, READONLY
	EXPORT __Vectors


__Vectors
	DCD 0X10001000
	DCD Reset_Handler
	
	ALIGN
	
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler

Reset_Handler
	LDR R0, =0x01
	ADD R0, #0x02
	ADD R0, #0x03
	ADD R0, #0x04
	ADD R0, #0x05
	ADD R0, #0x06
	ADD R0, #0x07
	ADD R0, #0x08
	ADD R0, #0x09
	ADD R0, #0x0A
	
	LDR R1, =0x10000000
	STR R0, [R1] 			; storing result in memory
	LDR R2, [R1] 			; retrieving result from memory

STOP
	B STOP

END