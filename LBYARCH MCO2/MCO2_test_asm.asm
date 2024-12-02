section .text
bits 64
default rel
global MCO2_asm

MCO2_asm:
    ; Arguments:
    ; count     -> rcx (number of elements)
    ; x_ptr     -> rdx (pointer to input array X)
    ; y_ptr     -> r8  (pointer to input array Y)
    ; result_ptr-> r9  (pointer to output array Z)
    ; scalar_b  -> passed on the stack at [rsp+40]

    ; Load scalar B from stack into XMM4 and broadcast it across all elements
    movsd XMM4, [rsp+40]          ; Load scalar B into XMM4
    shufpd XMM4, XMM4, 0x00       ; Broadcast scalar B across all positions in XMM4

    ; Prepare for SIMD loop to process 2 elements at a time (2 double-precision floats)
    mov rax, rcx                  ; Copy 'count' to rax for scalar remainder processing
    shr rcx, 1                    ; Divide count by 2 for SIMD iteration count
    jz scalar_process             ; If no full groups of 2, jump to scalar processing

simd_loop:
    ; Process 2 elements of X and Y in parallel
    movupd XMM0, [rdx]            ; Load 2 elements from X into XMM0
    mulpd XMM0, XMM4              ; Multiply each element in XMM0 by scalar B
    movupd XMM1, [r8]             ; Load 2 elements from Y into XMM1
    addpd XMM0, XMM1              ; Compute Y + (X * B) for 2 elements
    movupd [r9], XMM0             ; Store result into Z (result_ptr)

    ; Advance pointers for next SIMD iteration
    add rdx, 16                   ; Advance x_ptr by 2 elements (16 bytes)
    add r8, 16                    ; Advance y_ptr by 2 elements (16 bytes)
    add r9, 16                    ; Advance result_ptr by 2 elements (16 bytes)

    dec rcx                       ; Decrement SIMD iteration counter
    jnz simd_loop                 ; Repeat SIMD loop if rcx != 0

scalar_process:
    ; Handle remaining elements (if count is odd)
    and rax, 1                    ; Compute remaining elements (mod 2)
    jz end_daxpy                  ; If no remaining elements, exit

scalar_loop:
    ; Process single elements
    movsd XMM1, [rdx]             ; Load single element X[i] into XMM1
    mulsd XMM1, XMM4              ; Multiply X[i] by scalar B
    movsd XMM2, [r8]              ; Load single element Y[i] into XMM2
    addsd XMM1, XMM2              ; Compute Y[i] + (X[i] * B)
    movsd [r9], XMM1              ; Store result in Z[i]

    ; Advance pointers for next scalar iteration
    add rdx, 8                    ; Advance x_ptr to next element (8 bytes)
    add r8, 8                     ; Advance y_ptr to next element (8 bytes)
    add r9, 8                     ; Advance result_ptr to next element (8 bytes)
    dec rax                       ; Decrement scalar iteration counter
    jnz scalar_loop               ; Repeat scalar loop if rax != 0

end_daxpy:
    ret
