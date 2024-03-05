    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    ALIGN
    
    AREA armstrong_number_checker, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler


Reset_Handler
    LDR R0, =SRC
    LDR R1, =NUM_LEN
    LDR R2, =DST
    
    LDR R0, [R0]
    LDR R1, [R1]

    MOV R4, R1 ; backup len

    MOV R5, #0 ; sum
    MOV R6, #0 ; actual number converted from BCD
    MOV R8, #0 ; digit
    MOV R10, #0 ; power result
    MOV R7, #1 ; multiplier for bcd to hex conversion
    MOV R11, #10 ; constant


PERFORM_SUM
    CMP R0, #0
    BEQ CHECK

    AND R8, R0, #0x0F

    MOV R10, R8
    BL POWER
    ADD R5, R10
    MOV R1, R4

    MUL R8, R7
    ADD R6, R8
    MUL R7, R11

    LSR R0, #4

    B PERFORM_SUM


CHECK
    CMP R5, R6
    
    MOVEQ R9, #1
    MOVNE R9, #-1

    STR R9, [R2]


STOP
    B STOP


POWER
    CMP R1, #1
    BXEQ LR

    MUL R10, R8
    SUB R1, #1
    B POWER


;SRC DCD 0x1634
SRC DCD 0x153
;SRC DCD 0x152

;NUM_LEN DCD 0x4
NUM_LEN DCD 0x3


    AREA DATASEG, DATA, READWRITE
DST DCD 0              ;DST location in Data segment

    END
