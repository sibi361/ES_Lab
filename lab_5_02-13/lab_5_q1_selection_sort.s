    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_5_q1_selection_sort, CODE, READONLY
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
; R3 n - 1
; R4 i
; R5 j
; R6 smallest idx
; R7 #0x4
; R8 tmp1 for smallest value
; R9 tmp2 for current value
; R10 tmp3 for address


SORT_INIT
    LDR R2, =ARR_RAM
    MOV R4, #-1
    SUB R3, R1, #0x1                ; n - 1
    MOV R6, #0x0
    MOV R7, #0x4


SORT
    ADD R4, #0x1                    ; increment i by 1
    CMP R4, R3
    BGE STOP                        ; exit if i >= n - 1

    MOV R5, R4                      ; init j to i

    MLA R10, R5, R7, R2             ; address = i * 4 + base_address
    LDR R8, [R10]                   ; fetch arr[i] i.e. arr[smallest]


SORT_INNER_LOOP
    ADD R5, #0x1                    ; increment j by 1

    CMP R5, R1
    BGE SWAP                        ; continue inner loop as long as j < n
    
    MLA R10, R5, R7, R2                ; address = j * 4 + base_address
    LDR R9, [R10]                   ; fetch arr[j + 1]
    
    CMP R9, R8
    MOVLT R6, R5                    ; smallest = j if arr[j] < arr[smallest]
    MLALT R10, R6, R7, R2           ; address = smallest * 4 + base_address
    LDRLT R8, [R10]                 ; update smallest value

    B SORT_INNER_LOOP


STOP
    B STOP


SWAP
    MLA R10, R4, R7, R2             ; address = i * 4 + base_address
    LDR R9, [R10]                   ; backup arr[i]
    STR R8, [R10]                   ; store arr[smallest] at i-th idx

    MLA R10, R6, R7, R2             ; address = smallest * 4 + base_address
    STR R9, [R10]                   ; store arr[i] at smallest idx

    B SORT


ARR DCD 2, -1, 5, 3, 4
;ARR DCD -1, 2, 3, 4,5
ARR_LEN DCD 5

    AREA DATASEG, DATA, READWRITE
ARR_RAM DCD 0


    END
