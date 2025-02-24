        .data
n:      .word 10               # Number of Fibonacci numbers to generate
fib:    .word 0:10             # Array to store the Fibonacci numbers

        .text
        .globl main

main:
        # Initialize registers
        li $t0, 0              # i = 0 (counter)
        li $t1, 0              # Fibonacci(0) = 0
        li $t2, 1              # Fibonacci(1) = 1
        la $t3, fib            # Load address of fib array

loop:
        bge $t0, 10, exit      # If i >= 10, exit the loop
        sw $t1, 0($t3)         # Store Fibonacci number in array
        addi $t3, $t3, 4       # Move to next element in fib array
        
        # Calculate next Fibonacci number
        add $t4, $t1, $t2      # temp = Fibonacci(i-1) + Fibonacci(i-2)
        move $t1, $t2          # Fibonacci(i-1) = Fibonacci(i-2)
        move $t2, $t4          # Fibonacci(i) = temp

        addi $t0, $t0, 1       # Increment counter (i)
        j loop                 # Repeat the loop

exit:
        # Exit program
        li $v0, 10             # syscall number for exit
        syscall
