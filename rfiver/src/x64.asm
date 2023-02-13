;
;
;

.DATA

fxs_region BYTE 1024 dup (?)

;
;
;

EXTERN GADGET_TRAMP_BEGIN : DQ
EXTERN GADGET_TRAMP_END : DQ

EXTERN o_fire_weapon_def : DQ
EXTERN hk_x_fire_weapon_def : PROC

.CODE

pushaq macro
	push    rax
    push    rcx
    push    rdx
    push    rbx
    push    -1      ; dummy for rsp
    push    rbp
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
endm

popaq macro
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rbp
    add     rsp, 8    ; dummy for rsp
    pop     rbx
    pop     rdx
    pop     rcx
    pop     rax
endm

msa_conn proc
	int 3h
	ret
msa_conn endp

asm_tramp_x_fire_weapon_def proc
    fxsave [fxs_region]

    sub rsp, 48h ; register storage
    pushaq

	sub rsp, 20h ; shadow space

    call hk_x_fire_weapon_def 

	add rsp, 20h

    popaq
    add rsp, 48h

    fxrstor[fxs_region]

	jmp o_fire_weapon_def
asm_tramp_x_fire_weapon_def endp

;
;
;

set_ss macro
	pushfq
	pop rcx
	mov rax, rcx
	bts rax, 8
	push rax
	popfq
endm

__rs_jmp proc
	mov rbx, qword ptr [rsp]

	push rbx
	mov rax, qword ptr [rcx]
	mov rbx, GADGET_TRAMP_END
	mov qword ptr [rsp+8h], rbx
	pop rbx

	mov r9, qword ptr [rcx+20h]
	mov r8, qword ptr [rcx+18h]
	mov rdx, qword ptr [rcx+10h]
	mov rcx, qword ptr [rcx+8h]

	jmp GADGET_TRAMP_BEGIN
__rs_jmp endp

EXTERN fn_trace_ray : DQ

__trace_ray proc
    push rbp
    mov rbp, rsp
    and rsp, -16
    
    sub rsp, 40h
    
    ; arg1
    mov rax, qword ptr[rbp+8h+28h]
    mov qword ptr[rsp+20h], rax
    
    mov rax, qword ptr [rcx]
    call qword ptr [rax+18h]

	add rsp, 40h
    
    mov rsp, rbp
	pop rbp
	ret
__trace_ray endp

END
