
; internal
; ############

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

native 'lit', lit
    push qword[pc]
    add pc, 8
    jmp next




; variables
; ############

native 'here', here_l
    push qword here
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
    dq xt_ibuf, xt_word
    dq xt_lit, 0, xt_ibuf, xt_create
    dq xt_lit, i_docol, xt_comma
    dq xt_state, xt_lit, 1, xt_c_write
    dq xt_exit

colon ';', semicolon, 1
    dq xt_lit, xt_exit, xt_comma
    dq xt_state, xt_lit, 0, xt_c_write
    dq xt_exit

native ',', comma
    mov rcx, [here]
    pop rdi
    mov [rcx], rdi
    add rcx, 8
    mov [here], rcx

native 'c,', c_comma
    mov rcx, [here]
    pop rdi
    mov [rcx], sil
    inc rcx
    mov [here], rcx

; ( flags name -- )
native 'create', create
    mov rcx, [here]
    mov rdi, [last_word]
    mov [rcx], rdi
    mov [last_word], rcx
    add rcx, 8

    pop rdi             ; pointer to name str
    mov rsi, rcx      ; pointer to destination
    call string_copy
    mov rdi, rax
    call string_length
    add rcx, rax   
    inc rcx             ; add 1 because null terminated str

    pop rdi             ; flags in lowest byte
    mov [rcx], sil
    inc rcx
    
    mov [here], rcx
    jmp next



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

native 'number', number
    mov rdi, input_buf
    mov rsi, 1024
    call read_word
    mov rdi, input_buf
    call parse_int
    test rdx, rdx
    jz .end
    push rax
.end:
    jmp next

native 'word', word
    pop rdi
    mov rsi, 1024
    call read_word
    mov rdi, rax
    call string_length
    push rax
    jmp next






; memory
; ############

native 'mem', mem
    push qword user_mem
    jmp next

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









