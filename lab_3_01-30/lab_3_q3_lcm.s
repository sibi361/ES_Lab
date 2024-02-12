	AREA	RESET, DATA, READONLY
	EXPORT __Vectors

__Vectors
	DCD 0x10001010
	DCD Reset_Handler
	
	ALIGN
	
	AREA lab_3_q3_lcm, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler


Reset_Handler
	LDR	R1, =NUM1
	LDR	R2, =NUM2
	LDR R1, [R1]			   ; load value of num1
	LDR R2, [R2]			   ; load value of num2
	
	CMP R1, R2
	BLLO SWAP				   ; swap if num1 < num2
	
	MOV R6, #0x1  ; i = 1


LOOP
	MUL R7, R6, R1
	
	BL MOD   ; mod in r5
	ADD R6, #0x1
	
	TEQ R5, #0x0;
	BNE LOOP


STORE_IN_RAM
	LDR R4, =DST
	STR R7, [R4]


STOP
	B STOP


SWAP
	MOV R6, R1
	MOV R1, R2
	MOV R2, R6
	BX LR


MOD
	UDIV R3, R7, R2
	MUL R3, R2
	SUB R5, R7, R3
	BX LR


NUM1 DCD 2
NUM2 DCD 9               ; LCM (2, 9) = 18 i.e. 0x12


	AREA DATASEG, DATA, READWRITE
DST DCD 0                  ; DST location in Data segment


	END
