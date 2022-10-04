#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(auipc) f(ld) f(sd) f(inv) f(nemu_trap) f(addi) f(jal) f(jalr) f(beq)                     \
    f(lw) f(addw) f(sub) f(sltiu) f(bne) f(addiw) f(add) f(slliw) f(sh) f(srai) f(lbu) f(andi)                   \
        f(sllw) f(and) f(sltu) f(xori) f(or) f(sb) f(slli) f(srli) f(blt) f(slt) f(bge) f(sw) f(mulw)            \
            f(divw) f(remw) f(lh) f(lhu) f(subw) f(sraiw) f(mul) f(lui) f(srliw) f(sraw) f(srlw) f(bltu) f(bgeu) \
                f(mulhu)

def_all_EXEC_ID();
