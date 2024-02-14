    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_4_q1_hex_number_to_ascii, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler


; R0 input hexadecimal number
; R1 quotient / carry
; R2 ASCII result in data segment


Reset_Handler
    LDR R0, =HEX
    LDR R0, [R0]
    LDR R2, =ASCII


CONVERT
    BL EXTRACT_DIGITS_INIT
    CMP R0, #0x0             ; end if dividend is zero
    BNE CONVERT


STOP
    B STOP


EXTRACT_DIGITS_INIT
    MOV R1, #0               ; set quotient to zero


EXTRACT_DIGITS
    CMP R0, #0x0A
    BLO STORE_DIGIT_AS_ASCII
    
    SUB R0, #0x0A            ; if dividend is exceeds 10 decrement it by 10
    ADD R1, #0x1             ; and increment quotient by 1
    B EXTRACT_DIGITS         ; until dividend is less than 10


STORE_DIGIT_AS_ASCII
    ADD R0, #0x30            ; Get ASCII value for current digit in hex form

    STRB R0, [R2], #1        ; storing as a single byte as value falls in range [30, 39]

    MOV R0, R1               ; quotient becomes new dividend
    
    BX LR


HEX DCD 0x1A                 ; (26) => 32 36

    AREA DATASEG, DATA, READWRITE
ASCII DCD 0                  ; DST location in Data segment


    END
