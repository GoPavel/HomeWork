                section         .text

                global          _start
_start:

                sub             rsp, 2 * 128 * 8
                lea             rdi, [rsp + 128 * 8]
                mov             rcx, 128
                call            read_long
                mov             rdi, rsp
                call            read_long
                lea             rsi, [rsp + 128 * 8]

;create and initialize buffer
; rbx -- address buffer
                sub             rsp, 2 * 128 * 8
                mov             rbx, rsp

                push            rcx
                push            rdi
                lea             rcx, [2 * 128]
                mov             rdi, rbx
                call            set_zero
                pop             rdi
                pop             rcx
; buffer's initialized
                call            mul_long_long

                push            rdi
                push            rcx
                mov             rdi, rbx
                lea             rcx, [2  * 128]
                call            write_long
                pop             rcx
                pop             rdi

                mov             al, 0x0a
                call            write_char

                jmp             exit

; mul two long numbers
;    rdi -- address of #1
;    rsi -- address of #2
;    rcx -- length of long numbers in qwiords
;    rbx -- buffer
;inside:
;    r8 -- index for rdi
;    r9 -- index for rsi
;result:
;    rbx = rdi * rsi
mul_long_long:
; begin double cycle
                push            r8
                push            r9

                push            rdi
                mov             r8, 0
.loop_frist: ; for(rdi)
                push            rsi

;                push            rax
;                mov             rax, r8
;                lea             rax, [rax * 8]
;                add             rsi, rax
;                pop             rax

                mov             r9, 0
.loop_second:; for(rsi)
; begin body cycle
;       multiply [rdi] and [rsi] with shift
;       and add to buffer
                push            rax
                push            r8
                push            rcx
                push            rdx

                mov             rax, [rdi]
                mov             rdx, [rsi]
                mul             rdx ; [rdx][rax] = rdx * rax
                add             r8, r9
                mov             rcx, 2 * 128
                call            add_long_short_with_shift
                mov             rax, rdx
                inc             r8
                call            add_long_short_with_shift

                pop             rdx
                pop             rcx
                pop             r8
                pop             rax
; end body cycle
                lea             rsi, [rsi + 8]
                inc             r9
                cmp             r9, rcx
                jne              .loop_second

                pop             rsi
; end frist cycle
                lea             rdi, [rdi + 8]
                inc             r8
                cmp             r8, rcx
                jne             .loop_frist

                pop             rdi
; end second cycle
                pop             r8
                pop             r9
; end double cycle
                ret

; adds 64-bit number to long number with shift
;    rbx -- address of summand #1 (long number)
;    rcx -- length of summand #1 in qwords
;    rax -- summand #2 (64-bit unsigned)
;    r8 -- shift
; result:
;    sum is written to rbx
add_long_short_with_shift:
                push            rbx
                push            rax
                push            rcx
                push            r8
                push            rdx

                xor             rdx,rdx
                sub             rcx, r8
                mov             rdx, r8
                lea             rdx, [rdx * 8]
                add             rbx, rdx
                xor             rdx, rdx
.loop:
                add             [rbx], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rbx, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             r8
                pop             rcx
                pop             rax
                pop             rbx
                ret

; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx
.loop:
                add             [rdi], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_short:
                push            rax
                push            rdi
                push            rcx
;                push            rsi

                xor             rsi, rsi
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, rsi
                adc             rdx, 0
                mov             [rdi], rax
                add             rdi, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

;                pop             rsi
                pop             rcx
                pop             rdi
                pop             rax
                ret

; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx
                mov             [rdi], rax
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              .done
                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            mul_long_short
                call            add_long_short
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            rcx

                mov             rax, 20
                mul             rcx
                mov             rbp, rsp
                sub             rsp, rax

                mov             rsi, rbp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl
                call            is_zero
                jnz             .loop

                mov             rdx, rbp
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret

; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1
                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall

                cmp             rax, 1
                jne             .error
                xor             rax, rax
                mov             al, [rsp]
                add             rsp, 1

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret

; write one char to stdout, errors are ignored
;    al -- char
write_char:
                sub             rsp, 1
                mov             [rsp], al

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
                ret

exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, 1
                mov             rdi, 1
                syscall

                pop             rax
                ret


                section         .rodata
invalid_char_msg:
                db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg
