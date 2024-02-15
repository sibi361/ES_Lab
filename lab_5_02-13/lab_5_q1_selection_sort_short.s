    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_5_q1_selection_sort_short, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler


Reset_Handler
    LDR R0, =ARR
    LDR R1, =ARR_LEN
    LDR R2, =ARR_RAM
    LDR R1, [R1]


LOOP_COPY_TO_RAM
    LDR R3, [R0], #4
    STR R3, [R2], #4

    ADD R4, #0x1
    TEQ R4, R1
    BNE LOOP_COPY_TO_RAM


; R0 input array
; R1 array length (n)
; R2 array in data segment
; R3 i
; R4 j
; R5 tmp1 for smallest value
; R6 tmp2 for current value
; R7 tmp3 for address


SORT_INIT
    LDR R2, =ARR_RAM
    SUB R3, R1, #0x1                ; initialize i to n - 1


SORT_OUTER
    MOV R4, R3                      ; j = i
    MOV R7, R2                      ; copy array address


SORT
    LDR R5, [R2]                    ; load arr[i]
    LDR R6, [R7, #4]!               ; load arr[j]

    CMP R5, R6                      ; swap current idx with i if arr[i] > arr[j]
    STRGT R5, [R7]
    STRGT R6, [R2]

    SUB R4, #0x1                    ; decrement j
    TEQ R4, #0x0
    BNE SORT                        ; end of inner loop when j == 0

    ADD R2, #0x4
    SUB R3, #0x1                    ; decrement i
    TEQ R3, #0x0
    BNE SORT_OUTER                  ; end of outer loop when i == 0


STOP
    B STOP


ARR DCD 2, -1, 5, 3, 4
;ARR DCD -1, 2, 3, 4,5
ARR_LEN DCD 5

    AREA DATASEG, DATA, READWRITE
ARR_RAM DCD 0


    END
