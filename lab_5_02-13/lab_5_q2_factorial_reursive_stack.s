    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001020
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_5_q2_factorial_reursive_stack, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler


; R0 input number n
; R1 result
; R2 result in RAM


Reset_Handler
    LDR R0, =SRC
    LDR R0, [R0]
    LDR R2, =RESULT


    MOV R1, #1                ; set result as 1
    
    BL FACTORIAL


STORE_IN_RAM
    STR R1, [R2]              ; this address gets pushed onto stack on first run of PUSHGT {LR, R0}


STOP
    B STOP


FACTORIAL
    CMP R0, #0x1              ; base case: n <= 1
    PUSHGT {LR, R0}           ; push n and link register onto stack as long as n > 1
    SUBGT R0, #0x1            ; decrement n
    BLGT FACTORIAL            ; upcoming POP instruction address gets stored in LR on the 2nd to nth run

    ; control reaching here means n (n - 1) (n - 2) ... 2 have been pushed onto the stack
    ; represents reaching base case
    POP {LR, R0}              ; retrieve LR and n
    MUL R1, R0                ; result = result * n
    BX LR                     ; goes to above POP for (n - 1) runs; STORE_IN_RAM on nth run


SRC DCD 4                     ; 4! = (24)10 = 0x18

    AREA DATASEG, DATA, READWRITE
RESULT DCD 0


    END
