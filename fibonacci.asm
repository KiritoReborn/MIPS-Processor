.text
.globl main
main:
    # Initialize registers
    li $1, 10       # Input: which Fibonacci number to calculate
    li $2, 2       # Start counter at 2 since we already have the first two terms
    li $3, 1       # First Fibonacci number
    li $4, 1       # Result storage (initially the second term)
    li $5, 1       # Increment value
    li $6, 1       # Second Fibonacci number (same as $4 initially)
    
    # Special case for n <= 2
    slti $t0, $1, 3       # Check if input is 1 or 2
    bne $t0, $0, exit     # If so, result is already 1, exit
    
loop:
    beq $2, $1, exit     # If counter reaches the limit, exit
    add $4, $3, $6       # Add the two previous numbers
    add $3, $0, $6       # Move second number to first position
    add $6, $0, $4       # Update second number to the sum
    add $2, $2, $5       # Increment counter
    j loop               # Jump back to loop
    
exit:
    # Program ends, result is in $4
    
    # Add system call to display result for testing
    li $v0, 1
    move $a0, $4
    syscall
    
    # Exit program
    li $v0, 10
    syscall
