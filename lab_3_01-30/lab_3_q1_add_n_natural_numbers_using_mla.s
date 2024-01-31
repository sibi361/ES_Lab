	AREA	RESET, DATA, READONLY
	EXPORT __Vectors

__Vectors
	DCD 0X10001010
	DCD Reset_Handler
	
	ALIGN
	
	AREA lab_3_q1_add_n_natural_numbers_using_mla, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler


Reset_Handler
	LDR	R0, =N
	LDR R0, [R0]			   ; load value of n

	MLA R1, R0, R0, R0		   ; n * n + n = n * (n + 1)
	
	LSR R2, R1, #0x1           ; right shift by one bit divides by 2


STORE_IN_RAM
	LDR R3, =DST
	STR R2, [R3]


STOP
	B STOP


N DCD 10


	AREA DATASEG, DATA, READWRITE
DST DCD 0                  ; DST location in Data segment


	END
