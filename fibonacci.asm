section .data
    n db 10            ; Number of Fibonacci numbers to generate
    result db 0        ; Placeholder for the result

section .text
    global _start

_start:
    mov ecx, 0         ; Initialize counter i (for loop variable)
    mov eax, 0         ; Fibonacci(0)
    mov ebx, 1         ; Fibonacci(1)

loop_start:
    cmp ecx, [n]       ; Compare counter (i) with n
    jge loop_end       ; If counter >= n, exit the loop

    ; Print the current Fibonacci number
    ; (The result is in eax)
    
    ; Next Fibonacci number:
    add eax, ebx       ; New Fibonacci = Fibonacci(i-1) + Fibonacci(i-2)
    mov edx, eax       ; Copy new Fibonacci number to edx (for the next iteration)
    mov eax, ebx       ; Move the previous Fibonacci value to eax
    mov ebx, edx       ; Move the new Fibonacci value to ebx (for next iteration)

    inc ecx            ; Increment the counter
    jmp loop_start     ; Repeat the loop

loop_end:
    ; Exit program (exit system call)
    mov eax, 1         ; syscall number (sys_exit)
    xor ebx, ebx       ; exit status code
    int 0x80           ; call kernel
