    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_4_q2_bcd_to_hex, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler

; R0 input number
; R1 temp for digit
; R2 multiplier
; R3 result
; R4 constant 10 for MUL
; R5 result in data memory

Reset_Handler
    LDR R0, =SRC
    LDR R0, [R0]
    MOV R2, #0x1
    MOV R3, #0x0
    MOV R4, #0xA

LOOP
    AND R1, R0, MASK                    ; extract LSB digit
    MUL R1, R2                          ; get actual numeric value of current place
    ADD R3, R1                          ; add to result
    LSR R0, #4

    MUL R2, R4                          ; set multiplier for upcoming place; m = mp * 10

    CMP R0, #0
    BNE LOOP


STORE_IN_RAM
    LDR R5, =DST
    STR R3, [R5]


STOP
    B STOP


SRC DCD 0x98
MASK EQU 0x0F

    AREA DATASEG, DATA, READWRITE
DST DCD 0                  ; DST location in Data segment


    END
