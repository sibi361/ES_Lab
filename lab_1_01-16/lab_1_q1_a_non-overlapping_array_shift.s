    AREA    RESET, DATA, READONLY
    EXPORT __Vectors

__Vectors
    DCD 0x10001010
    DCD Reset_Handler
    ALIGN
    
    AREA lab_1_q1_a_non_overlapping_array_shift, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler

Reset_Handler
    LDR    R0, =SRC_ARR      ; load code segment array address into register
    LDR    R1, =DST_ARR      ; load data segment array address into register

; arr_src R0
; arr_dst R1
; tmp R2
; i   R3
; arr_dst_2 R4

loop_store_in_ram
    LDR R2, [R0], #4      ; store value in tmp R2 and increment src array's address in code segment

    STR R2, [R1], #4      ; store value in data segment and increment it's address in data segment

    ADD R3, #0x1          ; R3 is i; increment i
    TEQ R3, #0xA          ; if i == 10, XOR is 0, Z = 1, exit loop
    BNE loop_store_in_ram              ; else if Z == 0 goto loop

    LDR    R1, =DST_ARR      ; load (again) data segment array address into register

    LDR R4, =DST_ARR      ; temporarily copy first data segment array to R4
    ADD R4, #0x4E         ; setup address for non-overlapping second data segment array after 78 bits, 48 bits offset after first array

    MOV R3, #0            ; reset counter to 0

loop_non_overlapping_shift
    LDR R2, [R1], #4      ; store value in tmp R2 and increment src array's address in data segment

    STR R2, [R4], #4      ; store value in data segment and increment it's address in data segment

    ADD R3, #0x1          ; R3 is i; increment i
    TEQ R3, #0xA          ; if i == 10, XOR is 0, Z = 1, exit loop
    BNE loop_non_overlapping_shift              ; else if Z == 0 goto loop


STOP
    B STOP

SRC_ARR DCD 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A


    AREA DATASEG, DATA, READWRITE
DST_ARR DCD 0              ;DST location in Data segment

    END
