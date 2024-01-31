	AREA	RESET, DATA, READONLY
	EXPORT __Vectors

__Vectors
	DCD 0X10001010
	DCD Reset_Handler
	
	ALIGN
	
	AREA lab_3_q2_gcd, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler


Reset_Handler
	LDR	R1, =NUM1
	LDR	R2, =NUM2
	LDR R1, [R1]			   ; load value of num1
	LDR R2, [R2]			   ; load value of num2


GCD
	CMP R1, R2
	BEQ STORE_IN_RAM
	
	BLLO REDUCE_NUM2
	
	BLHI REDUCE_NUM1
	
	B GCD
	

REDUCE_NUM1
	SUB R1, R2
	BX LR


REDUCE_NUM2
	SUB R2, R1
	BX LR


STORE_IN_RAM
	LDR R3, =DST
	STR R2, [R3]


STOP
	B STOP


NUM1 DCD 0x8  ; 8
NUM2 DCD 0xE  ; 14 gives 2


	AREA DATASEG, DATA, READWRITE
DST DCD 0                  ; DST location in Data segment


	END
