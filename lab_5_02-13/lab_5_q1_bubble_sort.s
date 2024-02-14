    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_5_q1_bubble_sort, CODE, READONLY
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
; R6 n - i - 1
; R7 #0x4
; R8 tmp1
; R9 tmp2
; R10 tmp3 LDR base addr


SORT_INIT
    LDR R2, =ARR_RAM
    MOV R4, #-1
    SUB R3, R1, #0x1                  ; n - 1
    MOV R7, #0x4


SORT
    ADD R4, #0x1                      ; increment i by 1
    CMP R4, R3
    BGE STOP                          ; exit if i >= n - 1

    MOV R5, #-1                       ; init j to -1


SORT_INNER_LOOP
    ADD R5, #0x1                      ; increment j by 1
    SUB R6, R3, R4                    ; n - i - 1
    CMP R5, R6
    BGE SORT                          ; continue inner loop as long as j < (n - i - 1)
    
    MLA R10, R5, R7, R2               ; offset = j * 4 + base_ram_address
    LDR R8, [R10]                     ; fetch arr[j]
    LDR R9, [R10, #4]                 ; fetch arr[j + 1]
    
    CMP R8, R9
    BLGT SWAP                         ; swap if arr[j] > arr[j + 1]
    B SORT_INNER_LOOP


STOP
    B STOP


SWAP
    STR R9, [R10]
    STR R8, [R10, #4]
    BX LR


ARR DCD 2, -1, 5, 3, 4
ARR_LEN DCD 5

    AREA DATASEG, DATA, READWRITE
ARR_RAM DCD 0


    END
