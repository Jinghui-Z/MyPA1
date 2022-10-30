def_EHelper(inv)
{
    rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap)
{
    rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}

def_EHelper(ecall)
{
    rtl_j(s, isa_raise_intr(EX_ecall_m, s->snpc));
}

def_EHelper(mret)
{
    GET_MSTATUS(mstatus);
    mstatus.mie = mstatus.mpie;
    mstatus.mpie = 1;
    SET_MSTATUS(mstatus);
    rtl_j(s, cpu.mepc);
}

def_EHelper(csrrw)
{
    rtl_csr_to_gpr(s, s0, s->src2.imm);
    rtl_gpr_to_csr(s, dsrc1, s->src2.imm);
    rtl_mv(s, ddest, s0);
}
def_EHelper(csrrs)
{
    rtl_csr_to_gpr(s, ddest, s->src2.imm);
    rtl_or(s, s0, ddest, dsrc1); // s->dest now stores csr old value
    rtl_gpr_to_csr(s, s0, s->src2.imm);
}

def_EHelper(csrrc)
{
    rtl_csr_to_gpr(s, ddest, s->src2.imm);
    rtl_not(s, s0, dsrc1);
    rtl_or(s, s0, ddest, s0); // s->dest now stores csr old value
    // csr(idx) = (~rs1) & csr_old
    rtl_gpr_to_csr(s, s0, s->src2.imm);
}

def_EHelper(csrrwi)
{
    // TODO: privilege checking, but not necessary in PA
    rtl_csr_to_gpr(s, ddest, s->src2.imm);
    // read first, rd==$0 is handled in decode stage
    rtl_li(s, s0, s->src1.imm);
    rtl_gpr_to_csr(s, s0, s->src2.imm);
}

def_EHelper(csrrsi)
{
    rtl_csr_to_gpr(s, ddest, s->src2.imm);
    rtl_li(s, s0, s->src1.imm);
    rtl_or(s, s0, ddest, s0); // s->dest now stores csr old value
    rtl_gpr_to_csr(s, s0, s->src2.imm);
}

def_EHelper(csrrci)
{
    rtl_csr_to_gpr(s, ddest, s->src2.imm);
    rtl_li(s, s0, s->src1.imm);
    rtl_not(s, s0, s0);
    rtl_or(s, s0, ddest, s0); // s->dest now stores csr old value
    // csr(idx) = (~rs1) & csr_old
    rtl_gpr_to_csr(s, s0, s->src2.imm);
}
