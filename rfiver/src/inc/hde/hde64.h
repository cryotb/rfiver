#pragma once

#ifndef _HDE64_H_
#define _HDE64_H_

#define F_MODRM         0x00000001
#define F_SIB           0x00000002
#define F_IMM8          0x00000004
#define F_IMM16         0x00000008
#define F_IMM32         0x00000010
#define F_IMM64         0x00000020
#define F_DISP8         0x00000040
#define F_DISP16        0x00000080
#define F_DISP32        0x00000100
#define F_RELATIVE      0x00000200
#define F_ERROR         0x00001000
#define F_ERROR_OPCODE  0x00002000
#define F_ERROR_LENGTH  0x00004000
#define F_ERROR_LOCK    0x00008000
#define F_ERROR_OPERAND 0x00010000
#define F_PREFIX_REPNZ  0x01000000
#define F_PREFIX_REPX   0x02000000
#define F_PREFIX_REP    0x03000000
#define F_PREFIX_66     0x04000000
#define F_PREFIX_67     0x08000000
#define F_PREFIX_LOCK   0x10000000
#define F_PREFIX_SEG    0x20000000
#define F_PREFIX_REX    0x40000000
#define F_PREFIX_ANY    0x7f000000

#define PREFIX_SEGMENT_CS   0x2e
#define PREFIX_SEGMENT_SS   0x36
#define PREFIX_SEGMENT_DS   0x3e
#define PREFIX_SEGMENT_ES   0x26
#define PREFIX_SEGMENT_FS   0x64
#define PREFIX_SEGMENT_GS   0x65
#define PREFIX_LOCK         0xf0
#define PREFIX_REPNZ        0xf2
#define PREFIX_REPX         0xf3
#define PREFIX_OPERAND_SIZE 0x66
#define PREFIX_ADDRESS_SIZE 0x67

#pragma pack(push,1)

typedef struct {
    u8 len;
    u8 p_rep;
    u8 p_lock;
    u8 p_seg;
    u8 p_66;
    u8 p_67;
    u8 rex;
    u8 rex_w;
    u8 rex_r;
    u8 rex_x;
    u8 rex_b;
    u8 opcode;
    u8 opcode2;
    u8 modrm;
    u8 modrm_mod;
    u8 modrm_reg;
    u8 modrm_rm;
    u8 sib;
    u8 sib_scale;
    u8 sib_index;
    u8 sib_base;
    union {
        u8 imm8;
        u16 imm16;
        u32 imm32;
        u64 imm64;
    } imm;
    union {
        u8 disp8;
        u16 disp16;
        u32 disp32;
    } disp;
    u32 flags;
} hde64s;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

    /* __cdecl */
    unsigned int hde64_disasm(const void* code, hde64s* hs);

#ifdef __cplusplus
}
#endif

#endif /* _HDE64_H_ */
