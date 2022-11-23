section .data
prompt: db 'Enter a string: '
buffer: resb 100                ; "reserve" 100 bytes
        section .text ; code
        global  _start
_start:
        mov     rax, 4      ; write
        mov     rbx, 1      ; stdout
        mov     rcx, prompt ; where characters start
        mov     rdx, 0x10   ; 16 characters
        int     0x80
        mov     rax, 3      ; read
        mov     rbx, 0      ; from stdin
        mov     rcx, buffer ; start of storage
        mov     rdx, 100    ; no more than 0x64 chars
        int     0x80
        ; dump buffer that was read
        mov     rdx, rax   ; # chars read
        mov     rax, 4     ; write
        mov     rbx, 1     ; to stdout
        mov     rcx, buffer; starting point
        int     0x80
       ; return to Linux 
        mov     rax, 1
        mov     rbx, 0
        int     0x80