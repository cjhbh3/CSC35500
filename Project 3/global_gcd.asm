    global _gcd
    section .text
_gcd:
    push rbp
    mov rbp, rsp

    cmp dword [rbp+12], 0
    jne .recurse

.BaseCase:
    mov rax, [rbp+8]
    leave       ; or jump to .done to do this...
    ret

; calculate (a%b), do a recursive function call,
; then quit this function.
.recurse:
    mov rax, [rbp+8]    ; eax <-- 32-bit "a"
    xor rdx, rdx        ; edx <-- upper 0 bits
    div dword [rbp+12]
    push rdx            ; remainder is new "b"
    mov rax, [rbp+12]
    push rax            ; old "b" is new "a"
    call _gcd           ; tail-recursive call
.done:
    leave
    ret