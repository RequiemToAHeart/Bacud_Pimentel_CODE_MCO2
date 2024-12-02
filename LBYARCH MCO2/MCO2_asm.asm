section .text
bits 64
default rel
global MCO2_asm

MCO2_asm:
    ; Perform the DAXPY operation: B * X[i] + Y[i] for double-precision floats

main_loop: ; n=rcx, x_ptr=rdx, y_ptr=r8, z_ptr=r9, scalar_b=rsp+40

    movsd XMM4, [rsp+40]           ; Load scalar B into XMM4
    movsd XMM1, [rdx]              ; Load X[i] from x_ptr into XMM1
    mulsd XMM1, XMM4               ; Compute X[i] * B, result in XMM1
    movsd XMM2, [r8]               ; Load Y[i] from y_ptr into XMM2
    addsd XMM1, XMM2               ; Compute Y[i] + (X[i] * B), result in XMM1
    movsd [r9], XMM1               ; Store result into z_ptr (Z[i])

    add rdx, 8                     ; Advance x_ptr to next element (8 bytes)
    add r8, 8                      ; Advance y_ptr to next element (8 bytes)
    add r9, 8                      ; Advance z_ptr to next element (8 bytes)
    dec rcx                        ; Decrement loop counter (n)
    jnz main_loop                  ; Repeat the loop if rcx is not zero

    ret
