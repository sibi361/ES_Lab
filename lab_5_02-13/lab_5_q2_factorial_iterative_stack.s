    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_5_q2_factorial_iterative_stack, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler


; R0 input number n
; R1 input number n backup
; R2 result
; R3 stack pop counter
; R4 result in RAM


Reset_Handler
    LDR R0, =SRC
    LDR R0, [R0]
    LDR R4, =RESULT

    MOV R1, R0
    MOV R2, #1


FACTORIAL_PUSH
    CMP R0, #0x1
    BLE FACTORIAL_CALC        ; keep pushing into stack until n <= 1

    PUSH {R0}
    SUB R0, #0x1              ; decrement n

    B FACTORIAL_PUSH


FACTORIAL_CALC
    ADD R3, #0x1              ; increment counter

    CMP R3, R1
    BGE STORE_IN_RAM          ; keep popping from stack until counter >= original n

    POP {R0}
    MUL R2, R0                ; result = result * (popped value)

    B FACTORIAL_CALC


STORE_IN_RAM
    STR R2, [R4]


STOP
    B STOP


SRC DCD 4

    AREA DATASEG, DATA, READWRITE
RESULT DCD 0


    END
