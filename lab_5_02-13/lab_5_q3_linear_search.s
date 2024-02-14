    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    
    ALIGN
    
    AREA lab_5_q3_linear_search, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler


; R0 input array
; R1 array length
; R2 search key
; R3 result index stored in RAM
; R3 will be set to (-1)10 i.e. 0xFFFFFFFF if key is NOT found 


Reset_Handler
    LDR R0, =ARR
    LDR R1, =ARR_LEN
    LDR R2, =KEY
    LDR R3, =RESULT

    LDR R1, [R1]
    LDR R2, [R2]

    MOV R4, #-1             ; index
    MOV R5, #0x0            ; temp


SEARCH
    CMP R4, R1
    BGE NOT_FOUND           ; quit if index >= array_length

    ADD R4, #0x1            ; increment index

    LDR R5, [R0], #0x4      ; load value from array and increment address
    CMP R5, R2              ; compare current array value with search key

    BNE SEARCH


STORE_IN_RAM
    STR R4, [R3]


STOP
    B STOP


NOT_FOUND
    MOV R4, #-1
    B STORE_IN_RAM


ARR DCD 100, 64, -45, 40, 55
ARR_LEN DCD 5
KEY DCD -45

    AREA DATASEG, DATA, READWRITE
RESULT DCD 0


    END
