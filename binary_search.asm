        .data
array:  .word 1, 3, 5, 7, 9, 11, 13, 15, 17, 19   # Sorted array of numbers
target: .word 11                                   # The number to search for
size:   .word 10                                   # Size of the array

        .text
        .globl main

main:
        # Initialize registers
        la $t0, array            # Load the address of the array
        la $t1, target           # Load the address of the target
        lw $t2, 0($t1)           # Load target value into $t2
        lw $t3, size             # Load the size of the array into $t3
        li $t4, 0                # Set left index to 0 (t4 = left)
        sub $t5, $t3, 1          # Set right index to size-1 (t5 = right)
        
binary_search:
        bgt $t4, $t5, not_found  # If left > right, element is not found
        
        # Calculate middle index
        add $t6, $t4, $t5        # t6 = left + right
        sra $t6, $t6, 1          # t6 = (left + right) / 2, middle index
        
        # Load value at the middle index
        sll $t7, $t6, 2          # Multiply middle index by 4 (word size)
        add $t7, $t7, $t0        # t7 = address of array[mid]
        lw $t8, 0($t7)           # Load array[mid] into $t8
        
        # Compare middle value with target
        beq $t8, $t2, found      # If array[mid] == target, found
        blt $t8, $t2, move_right # If array[mid] < target, move right
        bgt $t8, $t2, move_left  # If array[mid] > target, move left

move_right:
        addi $t4, $t6, 1         # Set left = mid + 1
        j binary_search          # Repeat the search

move_left:
        sub $t5, $t6, 1          # Set right = mid - 1
        j binary_search          # Repeat the search

found:
        # Print success message
        li $v0, 4                # syscall for print_string
        la $a0, found_msg        # Load address of success message
        syscall
        
        j exit_program           # Jump to exit

not_found:
        # Print failure message
        li $v0, 4                # syscall for print_string
        la $a0, not_found_msg    # Load address of failure message
        syscall
        
exit_program:
        # Exit program
        li $v0, 10               # syscall for exit
        syscall

        .data
found_msg:  .asciiz "Target found in the array.\n"
not_found_msg: .asciiz "Target not found in the array.\n"
