	AREA	RESET, DATA, READONLY
	EXPORT __Vectors

__Vectors
	DCD 0x10001010
	DCD Reset_Handler
	
	ALIGN
	
	AREA lab_2_q2_add_128_bit_store_in_data_seg, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler


; num1 R0
; num2 R1
; i        R2
; tmp1 R3
; tmp2 R4
; tmp3 R7
; sum  R5
; sum in RAM R6


Reset_Handler
	LDR	R0, =NUM1
	LDR	R1, =NUM2
	LDR	R6, =DST


loop
	LDR R3, [R0], #4      ; store value of num1 in tmp1 and increment it's address
	LDR R4, [R1], #4      ; store value of num2 in tmp2 and increment it's address

    ADCS R5, R3, R4        ; sum = tmp1 + tmp2
	STR R5, [R6], #4         ; store computed sum for current 8 bits in ram and increment it's address for upcoming 8 bit sum

	ADD R2, #0x1          ; increment counter
	TEQ R2, #0x4    	   ; if i == 4, XOR is 0, Z = 1, exit loop
	BNE loop			       ; if Z == 0 goto loop


extract_and_save_carry_in_ram
    ADCS R7, R7
    STR R7, [R6]


STOP
	B STOP


NUM1 DCD 0x98765400, 0xAB000000, 0xCD000000, 0xEF000000
NUM2 DCD 0x00000001, 0x00000002, 0x00000002, 0x00000001

;NUM1 DCD 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF


	AREA DATASEG, DATA, READWRITE
DST DCD 0                  ; DST location in Data segment


	END
