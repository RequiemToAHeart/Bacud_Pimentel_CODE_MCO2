# Bacud_Pimentel_CODE_MCO2
**LBYARCH MCO2 Project**

**=SPECS:=**

![LBY specs](https://github.com/user-attachments/assets/8bf41155-644b-4f48-89d6-c5618e86ef75)

**=COMMAND PROMPT COMPILATION & EXECUTION:=**
==========================================================
*For  N = {2^20, 2^24, and 2^28}: inputs*
           
nasm -f win64 MCO2_test_asm.asm

gcc -c MCO2_test_C.c -o MCO2_test_C.obj -m64

gcc MCO2_test_C.obj MCO2_test_asm.obj -o MCO2_test_C.exe -m64

MCO2_test_C.exe
==========================================================
*For Self-input*
        
nasm -f win64 MCO2_asm.asm

gcc -c MCO2_C.c -o MCO2_C.obj -m64

gcc MCO2_C.obj MCO2_asm.obj -o MCO2_C.exe -m64

MCO2_C.exe
==========================================================

**=COMPATIVE ANALYSIS:=**


When comparing the performance of a C kernel and x86-64 assembly for handling large vector sizes in double-precision arithmetic, it’s clear that assembly has a significant edge in speed. The C kernel is much easier to write because it takes care of things like memory management and function calls automatically. It’s also portable, meaning you can run the same code on different platforms without needing to make changes. However, this convenience comes with a tradeoff: the compiler has to translate the C code into machine instructions, which adds extra overhead and slows things down. However in, x86-64 assembly gives you complete control over the hardware, allowing you to optimize your code specifically for the CPU. For example, you can use SIMD instructions like in "mulpd" and in "addpd" to handle multiple double-precision numbers all at once. Assembly itself also lets you manage registers and memory directly, which reduces unnecessary operations and leads to better performance. Finally, writing assembly code for double-precision calculations is more challenging and time-consuming, and it’s not portable across different processor types. But for applications where performance is critical—like in scientific or engineering workloads involving large datasets—the effort pays off. Assembly allows you to fully utilize the CPU’s capabilities, making it much faster than C kernel on some other cases.

**=Execution time of Manual Input:=**

![LBY4](https://github.com/user-attachments/assets/c5300773-ba2d-429d-9c2a-cff53f18c987)

**=Execution time of vector size N = {2^20, 2^24, and 2^28}:=**

![LBY1](https://github.com/user-attachments/assets/689b5d8a-072f-4aa8-8fd7-4f260e6c4f18)
![LBY2](https://github.com/user-attachments/assets/9517e6da-662c-47d7-9b56-f47b8bc1d8d3)
![LBY3](https://github.com/user-attachments/assets/82367b07-3b29-4628-a4a8-a0b87ba38fec)



