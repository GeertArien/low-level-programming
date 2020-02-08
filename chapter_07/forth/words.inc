
; internal
; ############

section .data
program_stub: dq 0

native 'init', init
    mov [stack_base], rsp   ; save stack base address
    mov rstack, rstack_start
    mov qword[program_stub], xt_interpreter
    mov pc, program_stub
    jmp next

native 'docol', docol
    sub rstack, 8
    mov [rstack], pc
    add w, 8
    mov pc, w
    jmp next

native 'exit', exit
    mov pc, [rstack]
    add rstack, 8
    jmp next

native 'bye', bye
    mov rax, 60         ; use exit system call to shut down correctly
    xor rdi, rdi
    syscall

; ( call num a1 a2 a3 a4 a5 a6 -- new rax )
native 'syscall', syscall_l
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    jmp next

native 'lit', lit
    push qword[pc]
    add pc, 8
    jmp next

; ( xt -- )
native 'execute', execute
    pop w
    jmp [w]

native 'branch', branch
    mov rdi, [pc]
    add pc, 8
    add pc, rdi
    jmp next

native '0branch', 0branch
    pop rax
    test rax, rax
    jnz .no_offset
    mov rdi, [pc]
    add pc, rdi
.no_offset:
    add pc, 8
    jmp next



; variables
; ############

native 'mem', mem
    push qword user_mem
    jmp next

native 'here', here
    push qword here
    jmp next

native 'last word', last_word
    push qword last_word
    jmp next

native 'ibuf', ibuf
    push qword input_buf
    jmp next

native 'state', state
    push qword state
    jmp next


; compiler
; ############

colon ':', colon
    dq xt_ibuf, xt_word, xt_drop
    dq xt_lit, 0, xt_ibuf, xt_create
    dq xt_lit, i_docol, xt_comma
    dq xt_state, xt_lit, 1, xt_c_write
    dq xt_exit

colon ';', semicolon, 1
    dq xt_lit, xt_exit, xt_comma
    dq xt_state, xt_lit, 0, xt_c_write
    dq xt_exit

colon ',', comma
    dq xt_here, xt_read, xt_swap, xt_write
    dq xt_here, xt_dup, xt_read 
    dq xt_lit, 8, xt_plus, xt_write
    dq xt_exit

native 'c,', c_comma
    mov rcx, [here]
    pop rdi
    mov [rcx], sil
    inc rcx
    mov [here], rcx
    jmp next

; ( flags name -- )
colon 'create', create
    dq xt_last_word, xt_read                        ; save last_word value
    dq xt_last_word, xt_here, xt_read, xt_write     ; write here addres to last_word
    dq xt_comma                                     ; write prev last_word addres to here

    dq xt_here, xt_read, xt_swap, xt_string_copy        ; copy string to here address
    dq xt_lit, 1, xt_plus, xt_here, xt_read, xt_plus   ; increase here addres by length + 1
    dq xt_here, xt_swap, xt_write                       ; update here address

    dq xt_c_comma       ; add flags to here addres
    dq xt_exit


; stack manipulation
; ############

native '.S', dotS
    mov rcx, rsp
.loop:
    cmp rcx, [stack_base]
    jae next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop

; (a b c -- b c a)
native 'rot', rot
    pop rdi ;c
    pop rsi ;b
    pop rdx ;a
    push rsi
    push rdi
    push rdx
    jmp next

native 'swap', swap
    pop rdi
    pop rsi
    push rdi
    push rsi
    jmp next

native 'dup', dup
    mov rdi, [rsp]
    push rdi
    jmp next

native 'drop', drop
    add rsp, 8
    jmp next

native '.', dot
    pop rdi
    call print_int
    call print_newline
    jmp next

native '>r', from_r
    push qword[rstack]
    add rstack, 8
    jmp next
    
native 'r>', to_r
    sub rstack, 8
    pop qword[rstack]
    jmp next

native 'r@', r_copy
    push qword[rstack]
    jmp next

; arithmetic
; ############

native '+', plus
    pop rax
    add [rsp], rax
    jmp next
    
native '-', min
    pop rax
    sub [rsp], rax
    jmp next
    
native '*', mult
    pop rdi,
    pop rax,
    mul rdi
    push rax
    jmp next
    
native '/', divi
    pop rdi,
    pop rax,
    xor rdx, rdx
    div rdi
    push rax
    jmp next

native '%', quot
    pop rdi,
    pop rax,
    xor rdx, rdx
    div rdi
    push rdx
    jmp next

native '=', equal
    pop rdi,
    pop rax,
    cmp rdi, rax
    je .equal
    push 0
    jmp next
.equal:
    push 1
    jmp next





; logic
; ############

native '<', less
    pop rdi
    pop rsi
    cmp rdi, rsi
    jl .lower
    push 0
    jmp next
.lower:
    push 1
    jmp next

native 'and', and_l
    pop rdi
    pop rsi
    test rdi, rdi
    jz .false
    test rsi, rsi
    jz .false
    push 1
    jmp next
.false:
    push 0
    jmp next

native 'not', not_l
    pop rdi
    test rdi, rdi
    jz .true
    push 0
    jmp next
.true:
    push 1
    jmp next

colon '>', greater
    dq xt_swap
    dq xt_less
    dq xt_exit

colon 'or', or_l
    dq xt_dup
    dq xt_rot
    dq xt_dup
    dq xt_and_l
    dq xt_rot
    dq xt_rot
    dq xt_and_l
    dq xt_plus
    dq xt_not_l
    dq xt_not_l
    dq xt_exit




; input / output
; ############



native 'key', key
    call read_char
    push rax
    jmp next

native 'emit', emit
    pop rdi
    call print_char
    jmp next

; ( str -- num len )
native 'number', number
    pop rdi
    call parse_int
    push rax
    push rdx
    jmp next

native 'word', word
    pop rdi
    mov rsi, 1024
    call read_word
    mov rdi, rax
    call string_length
    push rax
    jmp next

native 'count', count
    pop rdi
    call string_length
    push rax
    jmp next

; ( str -- )
native 'prints', prints
    pop rdi
    call print_error
    jmp next


; (dest_addres src_address -- length)
native 'string copy', string_copy
    pop rdi             ; pointer to src str
    pop rsi             ; pointer to destination
    mov rdx, 1024       ; max buffer length
    call string_copy
    mov rdi, rax
    call string_length
    push rax
    jmp next


; memory
; ############

native '!', write
    pop rdi     ; data
    pop rsi     ; address
    mov [rsi], rdi
    jmp next

native 'c!', c_write
    pop rdi     ; data
    pop rsi     ; address
    mov [rsi], dil
    jmp next

native '@', read
    pop rdi     ; address
    mov rsi, [rdi]
    push rsi
    jmp next

native 'c@', c_read
    pop rdi     ; address
    xor rsi, rsi
    mov sil, [rdi]
    push rsi
    jmp next


; dictionary
; ############

; rdi points to string we try to find
; rsi points to last value of list
; ( str -- header_addr )
native 'find', find
    pop rdi
    mov rsi, last_word
.loop:
    test rsi, rsi
    jz .no_match
    push rdi
    push rsi
    add rsi, 8
    call string_equals
    pop rsi
    pop rdi
    test rax, rax
    jnz .match
    mov rsi, [rsi]
    jmp .loop

.no_match:
    push 0
    jmp next

.match:
    push rsi
    jmp next

; ( word_address -- xt )
native 'cfa', cfa
    pop rdi
    add rdi, 8
    push rdi
    call string_length
    pop rdi
    add rax, rdi
    add rax, 2     ; 1b for 0 of zero terminated str and 1b for header flags
    push rax
    jmp next




; interpreter
; ############

colon 'interpreter', interpreter
.start:
    dq xt_ibuf, xt_word             
    branch0 .end             

    dq xt_ibuf, xt_find, xt_dup
    branch0 .test_number
    dq xt_cfa

    dq xt_state, xt_c_read
    branch0 .execute_word

    dq xt_dup, xt_lit, 1, xt_min, xt_c_read         ; put flag value on stack
    dq xt_not_l
    branch0 .execute_word

    dq xt_comma, 
    branch .start       ; add address to word

.execute_word:
    dq xt_execute, 
    branch .start
    
.test_number:  
    dq xt_drop, xt_ibuf, xt_number          ; parse number
    branch0 .invalid_input

    dq xt_state, xt_c_read
    branch0 .start          ; number already on stack so back to start    

.compile_number:
    dq xt_here, xt_lit, 8, xt_min, xt_read
    dq xt_lit, xt_branch, xt_equal, xt_not_l
    branch0 .add_number

    dq xt_here, xt_lit, 8, xt_min, xt_read
    dq xt_lit, xt_0branch, xt_equal, xt_not_l
    branch0 .add_number

    dq xt_lit, xt_lit, xt_comma         ; add xt_lit to word

.add_number:
    dq xt_comma                         ; add number to word
    branch .start

.invalid_input:
    dq xt_lit, error_msg, xt_prints

.end:  
    dq xt_bye, xt_exit


