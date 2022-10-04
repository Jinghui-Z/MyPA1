def_EHelper(auipc)
{
  rtl_li(s, ddest, id_src1->imm + s->pc);
}
def_EHelper(addi)
{
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(addiw)
{
  rtl_addiw(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(jal)
{
  rtl_addi(s, ddest, &s->pc, 4);
  rtl_addi(s, &s->dnpc, &s->pc, id_src1->imm);
}
def_EHelper(jalr)
{
  rtl_addi(s, ddest, &s->pc, 4);
  rtl_addi(s, &s->dnpc, dsrc1, id_src2->imm);
}
def_EHelper(beq)
{
  if (*dsrc1 == *ddest)
  {
    rtl_addi(s, &s->dnpc, &s->pc, id_src2->imm);
  }
}
def_EHelper(bne)
{
  rtl_jrelop(s, RELOP_NE, dsrc1, ddest, s->pc + id_src2->imm);
}
def_EHelper(blt)
{
  rtl_jrelop(s, RELOP_LT, dsrc1, ddest, s->pc + id_src2->imm);
}
def_EHelper(bltu)
{
  rtl_jrelop(s, RELOP_LTU, dsrc1, ddest, s->pc + id_src2->imm);
}
def_EHelper(bgeu)
{
  rtl_jrelop(s, RELOP_GEU, dsrc1, ddest, s->pc + id_src2->imm);
}

def_EHelper(bge)
{
  rtl_jrelop(s, RELOP_GE, dsrc1, ddest, s->pc + id_src2->imm);
}
def_EHelper(add)
{
  rtl_add(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mul)
{
  rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}
def_EHelper(addw)
{
  rtl_addw(s, ddest, dsrc1, dsrc2);
}
def_EHelper(sub)
{
  rtl_sub(s, ddest, dsrc1, dsrc2);
}
def_EHelper(subw)
{
  rtl_subw(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mulw)
{
  rtl_mulw(s, ddest, dsrc1, dsrc2);
}
def_EHelper(divw)
{
  rtl_divw(s, ddest, dsrc1, dsrc2);
}
def_EHelper(remw)
{
  rtl_remw(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltiu)
{
  rtl_setrelopi(s, RELOP_LTU, ddest, dsrc1, id_src2->imm);
}

def_EHelper(slliw)
{
  rtl_slliw(s, ddest, dsrc1, id_src2->imm & 0b111111u);
}

def_EHelper(sraiw)
{
  rtl_sraiw(s, ddest, dsrc1, id_src2->imm & 0b111111u);
}

def_EHelper(srai)
{
  rtl_srai(s, ddest, dsrc1, id_src2->imm & 0b111111u);
}
def_EHelper(andi)
{
  rtl_andi(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(sllw)
{
  rtl_sllw(s, ddest, dsrc1, dsrc2);
}
def_EHelper(sraw)
{
  rtl_sraw(s, ddest, dsrc1, dsrc2);
}
def_EHelper(srlw)
{
  rtl_srlw(s, ddest, dsrc1, dsrc2);
}
def_EHelper(and)
{
  rtl_and(s, ddest, dsrc1, dsrc2);
}
def_EHelper(sltu)
{
  rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
}
def_EHelper(slt)
{
  rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
}
def_EHelper(xori)
{
  rtl_xori(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(or)
{
  rtl_or(s, ddest, dsrc1, dsrc2);
}
def_EHelper(slli)
{
  rtl_slli(s, ddest, dsrc1, id_src2->imm & 0b111111u);
}
def_EHelper(srli)
{
  rtl_srli(s, ddest, dsrc1, id_src2->imm & 0b111111u);
}
def_EHelper(srliw)
{
  rtl_srliw(s, ddest, dsrc1, id_src2->imm & 0b111111u);
}
def_EHelper(lui)
{
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(mulhu)
{
  rtl_mulu_hi(s, ddest, dsrc1, dsrc2);
}
