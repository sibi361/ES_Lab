    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_5_q2_factorial_iterative, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler


; R0 input number n
; R1 result
; R2 result in RAM


Reset_Handler
    LDR R0, =SRC
    LDR R0, [R0]
    LDR R2, =RESULT

    MOV R1, #1              ; result


FACTORIAL
    CMP R0, #0x1
    BLE STORE_IN_RAM        ; quit if n <= 1

    MUL R1, R0              ; result = result * n
    SUB R0, #0x1            ; decrement n

    BNE FACTORIAL


STORE_IN_RAM
    STR R1, [R2]


STOP
    B STOP


SRC DCD 4

    AREA DATASEG, DATA, READWRITE
RESULT DCD 0


    END
