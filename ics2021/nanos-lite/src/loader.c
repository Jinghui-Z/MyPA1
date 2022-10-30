#include <proc.h>
#include <elf.h>
#include <fs.h>
#include <common.h>
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
// void *new_page(size_t nr_page);
#define NR_PAGE 8
#define PAGESIZE 4096
#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Phdr Elf32_Phdr
#endif

uintptr_t loader(PCB *pcb, const char *filename)
{
  Elf_Ehdr ehdr, *ptr_ehdr = &ehdr;
  Elf_Phdr phdr, *ptr_phdr = &phdr;
  // uint32_t phoff;
  // int fd;
  // fd = fs_open(filename, 0, 0);
  // assert(fd != -1);
  // fs_read(fd, ptr_ehdr, sizeof(Elf_Ehdr));
  // assert(*((uint32_t *)ptr_ehdr) == 0x464c457f);
  // assert(ehdr.e_ident[EI_CLASS] == ELFCLASS64);
  // assert(ehdr.e_ident[EI_DATA] == ELFDATA2LSB);
  // for (int i = 0; i < ehdr.e_phnum; ++i)
  // {
  //   phoff = ehdr.e_phoff + ehdr.e_phentsize * i;
  //   fs_lseek(fd, phoff, SEEK_SET);
  //   fs_read(fd, ptr_phdr, sizeof(Elf_Phdr));
  //   if (phdr.p_type == PT_LOAD)
  //   {
  //     fs_lseek(fd, phdr.p_offset, SEEK_SET);
  //     fs_read(fd, (void *)phdr.p_vaddr, phdr.p_memsz);
  //     memset((void *)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
  //   }
  // }
  size_t len = ramdisk_read(ptr_ehdr, 0, sizeof(Elf_Ehdr));
  Log("len = 0x%x", len);
  for (int i = 0; i < ehdr.e_phnum; ++i)
  {
    uint64_t ph_offset = ehdr.e_phoff + ehdr.e_phentsize * i;
    ramdisk_read(ptr_phdr, ph_offset, ehdr.e_phentsize);
    if (phdr.p_type == PT_LOAD)
    {
      ramdisk_read((void *)phdr.p_vaddr, phdr.p_offset, phdr.p_memsz);
      memset((void *)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
    }
  }
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename)
{
  // printf("1111\n");
  uintptr_t entry = loader(pcb, filename);
  // printf("2222\n");
  Log("Jump to entry = %p", entry);
  ((void (*)())entry)();
}
