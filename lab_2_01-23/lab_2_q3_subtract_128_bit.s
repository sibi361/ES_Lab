    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_2_q3_subtract_128_bit, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler
 
 
; num1 R0
; num2 R1
; i        R2
; tmp1 R3
; tmp2 R4
; tmp3 R7
; diff    R5
; diff in RAM R6


Reset_Handler
    LDR    R0, =NUM1
    LDR    R1, =NUM2
    LDR    R6, =DST
    
    LDR R3, =0x20000000
    MSR XPSR, R3           ; set carry to 1 for initial SBC run


loop
    LDR R3, [R0], #4       ; store value of num1 in tmp1 and increment it's address
    LDR R4, [R1], #4       ; store value of num2 in tmp2 and increment it's address

    SBCS R5, R3, R4        ; diff = tmp1 - tmp2
    STR R5, [R6], #4       ; store computed diff for current 8 bits in ram and increment it's address for upcoming 8 bit sum

    ADD R2, #0x1           ; increment counter
    TEQ R2, #0x4           ; if i == 4, XOR is 0, Z = 1, exit loop
    BNE loop               ; if Z == 0 goto loop


extract_and_save_carry_in_ram
    ADCS R7, R7
    STR R7, [R6]


STOP
    B STOP


NUM1 DCD 0x98765400, 0xAB000000, 0xCD000000, 0xEF001F00
NUM2 DCD 0x00005400, 0x0B000000, 0x0D000000, 0x0F000E00


    AREA DATASEG, DATA, READWRITE
DST DCD 0                  ; DST location in Data segment


    END
