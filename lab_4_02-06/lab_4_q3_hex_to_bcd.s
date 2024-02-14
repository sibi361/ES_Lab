    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_4_q3_hex_to_bcd, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler


; R0 input hexadecimal number
; R1 shift counter
; R2 quotient / carry
; R3 result
; R4 result in data segment after LSR


Reset_Handler
    LDR R0, =HEX
    LDR R0, [R0]
    MOV R1, #0x20           ; initialise (final) shift counter to 32


CONVERT
    BL DIVIDE_INIT
    CMP R0, #0x0            ; end if dividend is zero
    BNE CONVERT


STORE_IN_RAM
    LDR R4, =BCD
    LSR R3, R1
    STR R3, [R4]


STOP
    B STOP


DIVIDE_INIT
    MOV R2, #0              ; set quotient to zero


DIVIDE
    CMP R0, #0x0A
    BLO PACK_DIGIT
    
    SUB R0, #0x0A           ; if dividend is exceeds 10 decrement it by 10
    ADD R2, #0x1            ; and increment quotient by 1
    B DIVIDE                ; until dividend is less than 10


PACK_DIGIT
    ORR R3, R0
    ROR R3, #0x4
    MOV R0, R2              ; quotient becomes new dividend
    
    SUB R1, #0x4
    BX LR


HEX DCD 0x2E

    AREA DATASEG, DATA, READWRITE
BCD DCD 0                   ; DST location in Data segment


    END
