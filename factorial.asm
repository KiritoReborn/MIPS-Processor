.text
.globl main
main:
    # Initialize registers
    li $1, 5        # Input: number to calculate factorial (n = 5)
    li $2, 1        # Counter, starts at 1
    li $4, 1        # Result storage (initial value 1)

loop:
    # If counter > n, exit loop.
    # We'll use slt to set $t0 = 1 if n < counter.
    slt $t0, $1, $2  # $t0 = 1 if $1 (n) < $2 (counter)
    bne $t0, $zero, exit  # if $t0 != 0 then counter > n, so exit

    mul $4, $4, $2  # Multiply result by counter: $4 = $4 * $2
    addi $2, $2, 1  # Increment counter: $2 = $2 + 1
    j loop         # Jump back to loop

exit:
    # Final result (n!) is in $4.
    li $v0, 1      # syscall code for print integer
    move $a0, $4
    syscall

    li $v0, 10     # syscall code for exit
    syscall
