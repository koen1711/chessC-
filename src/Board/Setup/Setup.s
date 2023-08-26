# Convert the FEN parsed to a std::map<std::string, std::list<int>>
.data
fen_map: .quad 0
fen_map_size: .quad 0
fen_map_capacity: .quad 0
fen_map_end: .quad 0
fen_map_begin: .quad 0
fen_map_bucket_count: .quad 0
.text
.global loadFenToMap

loadFenToMap:
    # Get the FEN string from the stack
    movq %rdi, %r8

    # Print the FEN string
    movq $1, %rax
    movq $1, %rdi
    movq %r8, %rsi
    # get the length of the string

    # loop until the null character is found
    cld
    repne scasb
    # rcx now contains the length of the string
    # add 1 to include the null character
    inc %rcx
    movq %rcx, %rdx
    # write the command (4, sys_write) in the eax register
    movl $4, %eax
    syscall

    // return error code 0
    movq $0, %rax
    ret


