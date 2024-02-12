	AREA	RESET, DATA, READONLY
	EXPORT __Vectors

__Vectors
	DCD 0x10001010
	DCD Reset_Handler
	
	ALIGN
	
	AREA lab_2_q1_add_ten_from_code_segment_store_in_data_segment, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler


; arr R0
; i   R1
; tmp R2
; sum R3


Reset_Handler
	LDR	R0, =SRC_ARR


loop
	LDR R2, [R0], #4      ; store value in tmp R2 and increment it's address

    ADD R3, R2            ; sum = sum  + tmp

	ADD R1, #0x1          ; R1 is i; increment i
	TEQ R1, #0xA    	  ; if i == 10, XOR is 0, Z = 1, exit loop
	BNE loop			  ; if Z == 0 goto loop


store_in_ram
	LDR R4, =DST
	STR R3, [R4]


STOP
	B STOP


SRC_ARR DCD 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A


	AREA DATASEG, DATA, READWRITE
DST DCD 0                  ; DST location in Data segment


	END
