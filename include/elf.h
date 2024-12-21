#ifndef __ELF_H
#define __ELF_H

#include <stdint.h>

typedef uint16_t Elf32_Half, Elf64_Half;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Addr, Elf32_Off, Elf32_Word, Elf64_Word;
typedef int64_t Elf64_Sxword;
typedef uint64_t Elf64_Addr, Elf64_Off, Elf64_Xword;

#define EI_NIDENT 16
typedef struct {
    unsigned char   e_ident[EI_NIDENT];
    Elf32_Half      e_type;
    Elf32_Half      e_machine;
    Elf32_Word      e_version;
    Elf32_Addr      e_entry;
    Elf32_Off       e_phoff;
    Elf32_Off       e_shoff;
    Elf32_Word      e_flags;
    Elf32_Half      e_ehsize;
    Elf32_Half      e_phentsize;
    Elf32_Half      e_phnum;
    Elf32_Half      e_shentsize;
    Elf32_Half      e_shnum;
    Elf32_Half      e_shstrndx;
} Elf32_Ehdr;

typedef struct {
    unsigned char   e_ident[EI_NIDENT];
    Elf64_Half      e_type;
    Elf64_Half      e_machine;
    Elf64_Word      e_version;
    Elf64_Addr      e_entry;
    Elf64_Off       e_phoff;
    Elf64_Off       e_shoff;
    Elf64_Word      e_flags;
    Elf64_Half      e_ehsize;
    Elf64_Half      e_phentsize;
    Elf64_Half      e_phnum;
    Elf64_Half      e_shentsize;
    Elf64_Half      e_shnum;
    Elf64_Half      e_shstrndx;
} Elf64_Ehdr;

#define ET_NONE         0
#define ET_REL          1
#define ET_EXEC         2
#define ET_DYN          3
#define ET_CORE         4
#define ET_LOOS         0xfe00
#define ET_HIOS         0xfeff
#define ET_LOPROC       0xff00
#define ET_HIPROC       0xffff

#define EM_NONE               0      /* No machine */
#define EM_M32                1      /* AT&T WE 32100 */
#define EM_SPARC              2      /* SPARC */
#define EM_386                3      /* Intel 80386 */
#define EM_68K                4      /* Motorola 68000 */
#define EM_88K                5      /* Motorola 88000 */
#define EM_IAMCU              6      /* Intel MCU */
#define EM_860                7      /* Intel 80860 */
#define EM_MIPS               8      /* MIPS I Architecture */
#define EM_S370               9      /* IBM System/370 Processor */
#define EM_MIPS_RS3_LE        10     /* MIPS RS3000 Little-endian */
                                     /* 11-14 Reserved for future use */
#define EM_PARISC             15     /* Hewlett-Packard PA-RISC */
                                     /* 16 Reserved for future use */
#define EM_VPP500             17     /* Fujitsu VPP500 */
#define EM_SPARC32PLUS        18     /* Enhanced instruction set SPARC */
#define EM_960                19     /* Intel 80960 */
#define EM_PPC                20     /* PowerPC */
#define EM_PPC64              21     /* 64-bit PowerPC */
#define EM_S390               22     /* IBM System/390 Processor */
#define EM_SPU                23     /* IBM SPU/SPC */
                                     /* 24-35 Reserved for future use */
#define EM_V800               36     /* NEC V800 */
#define EM_FR20               37     /* Fujitsu FR20 */
#define EM_RH32               38     /* TRW RH-32 */
#define EM_RCE                39     /* Motorola RCE */
#define EM_ARM                40     /* ARM 32-bit architecture (AARCH32) */
#define EM_ALPHA              41     /* Digital Alpha */
#define EM_SH                 42     /* Hitachi SH */
#define EM_SPARCV9            43     /* SPARC Version 9 */
#define EM_TRICORE            44     /* Siemens TriCore embedded processor */
#define EM_ARC                45     /* Argonaut RISC Core, Argonaut Technologies Inc. */
#define EM_H8_300             46     /* Hitachi H8/300 */
#define EM_H8_300H            47     /* Hitachi H8/300H */
#define EM_H8S                48     /* Hitachi H8S */
#define EM_H8_500             49     /* Hitachi H8/500 */
#define EM_IA_64              50     /* Intel IA-64 processor architecture */
#define EM_MIPS_X             51     /* Stanford MIPS-X */
#define EM_COLDFIRE           52     /* Motorola ColdFire */
#define EM_68HC12             53     /* Motorola M68HC12 */
#define EM_MMA                54     /* Fujitsu MMA Multimedia Accelerator */
#define EM_PCP                55     /* Siemens PCP */
#define EM_NCPU               56     /* Sony nCPU embedded RISC processor */
#define EM_NDR1               57     /* Denso NDR1 microprocessor */
#define EM_STARCORE           58     /* Motorola Star*Core processor */
#define EM_ME16               59     /* Toyota ME16 processor */
#define EM_ST100              60     /* STMicroelectronics ST100 processor */
#define EM_TINYJ              61     /* Advanced Logic Corp. TinyJ embedded processor family */
#define EM_X86_64             62     /* AMD x86-64 architecture */
#define EM_PDSP               63     /* Sony DSP Processor */
#define EM_PDP10              64     /* Digital Equipment Corp. PDP-10 */
#define EM_PDP11              65     /* Digital Equipment Corp. PDP-11 */
#define EM_FX66               66     /* Siemens FX66 microcontroller */
#define EM_ST9PLUS            67     /* STMicroelectronics ST9+ 8/16 bit microcontroller */
#define EM_ST7                68     /* STMicroelectronics ST7 8-bit microcontroller */
#define EM_68HC16             69     /* Motorola MC68HC16 Microcontroller */
#define EM_68HC11             70     /* Motorola MC68HC11 Microcontroller */
#define EM_68HC08             71     /* Motorola MC68HC08 Microcontroller */
#define EM_68HC05             72     /* Motorola MC68HC05 Microcontroller */
#define EM_SVX                73     /* Silicon Graphics SVx */
#define EM_ST19               74     /* STMicroelectronics ST19 8-bit microcontroller */
#define EM_VAX                75     /* Digital VAX */
#define EM_CRIS               76     /* Axis Communications 32-bit embedded processor */
#define EM_JAVELIN            77     /* Infineon Technologies 32-bit embedded processor */
#define EM_FIREPATH           78     /* Element 14 64-bit DSP Processor */
#define EM_ZSP                79     /* LSI Logic 16-bit DSP Processor */
#define EM_MMIX               80     /* Donald Knuth's educational 64-bit processor */
#define EM_HUANY              81     /* Harvard University machine-independent object files */
#define EM_PRISM              82     /* SiTera Prism */
#define EM_AVR                83     /* Atmel AVR 8-bit microcontroller */
#define EM_FR30               84     /* Fujitsu FR30 */
#define EM_D10V               85     /* Mitsubishi D10V */
#define EM_D30V               86     /* Mitsubishi D30V */
#define EM_V850               87     /* NEC v850 */
#define EM_M32R               88     /* Mitsubishi M32R */
#define EM_MN10300            89     /* Matsushita MN10300 */
#define EM_MN10200            90     /* Matsushita MN10200 */
#define EM_PJ                 91     /* picoJava */
#define EM_OPENRISC           92     /* OpenRISC 32-bit embedded processor */
#define EM_ARC_COMPACT        93     /* ARC International ARCompact processor (old spelling/synonym: EM_ARC_A5) */
#define EM_XTENSA             94     /* Tensilica Xtensa Architecture */
#define EM_VIDEOCORE          95     /* Alphamosaic VideoCore processor */
#define EM_TMM_GPP            96     /* Thompson Multimedia General Purpose Processor */
#define EM_NS32K              97     /* National Semiconductor 32000 series */
#define EM_TPC                98     /* Tenor Network TPC processor */
#define EM_SNP1K              99     /* Trebia SNP 1000 processor */
#define EM_ST200              100    /* STMicroelectronics (www.st.com) ST200 microcontroller */
#define EM_IP2K               101    /* Ubicom IP2xxx microcontroller family */
#define EM_MAX                102    /* MAX Processor */
#define EM_CR                 103    /* National Semiconductor CompactRISC microprocessor */
#define EM_F2MC16             104    /* Fujitsu F2MC16 */
#define EM_MSP430             105    /* Texas Instruments embedded microcontroller msp430 */
#define EM_BLACKFIN           106    /* Analog Devices Blackfin (DSP) processor */
#define EM_SE_C33             107    /* S1C33 Family of Seiko Epson processors */
#define EM_SEP                108    /* Sharp embedded microprocessor */
#define EM_ARCA               109    /* Arca RISC Microprocessor */
#define EM_UNICORE            110    /* Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University */
#define EM_EXCESS             111    /* eXcess: 16/32/64-bit configurable embedded CPU */
#define EM_DXP                112    /* Icera Semiconductor Inc. Deep Execution Processor */
#define EM_ALTERA_NIOS2       113    /* Altera Nios II soft-core processor */
#define EM_CRX                114    /* National Semiconductor CompactRISC CRX microprocessor */
#define EM_XGATE              115    /* Motorola XGATE embedded processor */
#define EM_C166               116    /* Infineon C16x/XC16x processor */
#define EM_M16C               117    /* Renesas M16C series microprocessors */
#define EM_DSPIC30F           118    /* Microchip Technology dsPIC30F Digital Signal Controller */
#define EM_CE                 119    /* Freescale Communication Engine RISC core */
#define EM_M32C               120    /* Renesas M32C series microprocessors */
                                     /* 121-130 Reserved for future use */
#define EM_TSK3000            131    /* Altium TSK3000 core */
#define EM_RS08               132    /* Freescale RS08 embedded processor */
#define EM_SHARC              133    /* Analog Devices SHARC family of 32-bit DSP processors */
#define EM_ECOG2              134    /* Cyan Technology eCOG2 microprocessor */
#define EM_SCORE7             135    /* Sunplus S+core7 RISC processor */
#define EM_DSP24              136    /* New Japan Radio (NJR) 24-bit DSP Processor */
#define EM_VIDEOCORE3         137    /* Broadcom VideoCore III processor */
#define EM_LATTICEMICO32      138    /* RISC processor for Lattice FPGA architecture */
#define EM_SE_C17             139    /* Seiko Epson C17 family */
#define EM_TI_C6000           140    /* The Texas Instruments TMS320C6000 DSP family */
#define EM_TI_C2000           141    /* The Texas Instruments TMS320C2000 DSP family */
#define EM_TI_C5500           142    /* The Texas Instruments TMS320C55x DSP family */
#define EM_TI_ARP32           143    /* Texas Instruments Application Specific RISC Processor, 32bit fetch */
#define EM_TI_PRU             144    /* Texas Instruments Programmable Realtime Unit */
                                     /* 145-159 Reserved for future use */
#define EM_MMDSP_PLUS         160    /* STMicroelectronics 64bit VLIW Data Signal Processor */
#define EM_CYPRESS_M8C        161    /* Cypress M8C microprocessor */
#define EM_R32C               162    /* Renesas R32C series microprocessors */
#define EM_TRIMEDIA           163    /* NXP Semiconductors TriMedia architecture family */
#define EM_QDSP6              164    /* QUALCOMM DSP6 Processor */
#define EM_8051               165    /* Intel 8051 and variants */
#define EM_STXP7X             166    /* STMicroelectronics STxP7x family of configurable and extensible RISC processors */
#define EM_NDS32              167    /* Andes Technology compact code size embedded RISC processor family */
#define EM_ECOG1              168    /* Cyan Technology eCOG1X family */
#define EM_ECOG1X             168    /* Cyan Technology eCOG1X family */
#define EM_MAXQ30             169    /* Dallas Semiconductor MAXQ30 Core Micro-controllers */
#define EM_XIMO16             170    /* New Japan Radio (NJR) 16-bit DSP Processor */
#define EM_MANIK              171    /* M2000 Reconfigurable RISC Microprocessor */
#define EM_CRAYNV2            172    /* Cray Inc. NV2 vector architecture */
#define EM_RX                 173    /* Renesas RX family */
#define EM_METAG              174    /* Imagination Technologies META processor architecture */
#define EM_MCST_ELBRUS        175    /* MCST Elbrus general purpose hardware architecture */
#define EM_ECOG16             176    /* Cyan Technology eCOG16 family */
#define EM_CR16               177    /* National Semiconductor CompactRISC CR16 16-bit microprocessor */
#define EM_ETPU               178    /* Freescale Extended Time Processing Unit */
#define EM_SLE9X              179    /* Infineon Technologies SLE9X core */
#define EM_L10M               180    /* Intel L10M */
#define EM_K10M               181    /* Intel K10M */
                                     /* 182 Reserved for future Intel use */
#define EM_AARCH64            183    /* ARM 64-bit architecture (AARCH64) */
                                     /* 184 Reserved for future ARM use */
#define EM_AVR32              185    /* Atmel Corporation 32-bit microprocessor family */
#define EM_STM8               186    /* STMicroeletronics STM8 8-bit microcontroller */
#define EM_TILE64             187    /* Tilera TILE64 multicore architecture family */
#define EM_TILEPRO            188    /* Tilera TILEPro multicore architecture family */
#define EM_MICROBLAZE         189    /* Xilinx MicroBlaze 32-bit RISC soft processor core */
#define EM_CUDA               190    /* NVIDIA CUDA architecture */
#define EM_TILEGX             191    /* Tilera TILE-Gx multicore architecture family */
#define EM_CLOUDSHIELD        192    /* CloudShield architecture family */
#define EM_COREA_1ST          193    /* KIPO-KAIST Core-A 1st generation processor family */
#define EM_COREA_2ND          194    /* KIPO-KAIST Core-A 2nd generation processor family */
#define EM_ARC_COMPACT2       195    /* Synopsys ARCompact V2 */
#define EM_OPEN8              196    /* Open8 8-bit RISC soft processor core */
#define EM_RL78               197    /* Renesas RL78 family */
#define EM_VIDEOCORE5         198    /* Broadcom VideoCore V processor */
#define EM_78KOR              199    /* Renesas 78KOR family */
#define EM_56800EX            200    /* Freescale 56800EX Digital Signal Controller (DSC) */
#define EM_BA1                201    /* Beyond BA1 CPU architecture */
#define EM_BA2                202    /* Beyond BA2 CPU architecture */
#define EM_XCORE              203    /* XMOS xCORE processor family */
#define EM_MCHP_PIC           204    /* Microchip 8-bit PIC(r) family */
#define EM_INTEL205           205    /* Reserved by Intel */
#define EM_INTEL206           206    /* Reserved by Intel */
#define EM_INTEL207           207    /* Reserved by Intel */
#define EM_INTEL208           208    /* Reserved by Intel */
#define EM_INTEL209           209    /* Reserved by Intel */
#define EM_KM32               210    /* KM211 KM32 32-bit processor */
#define EM_KMX32              211    /* KM211 KMX32 32-bit processor */
#define EM_KMX16              212    /* KM211 KMX16 16-bit processor */
#define EM_KMX8               213    /* KM211 KMX8 8-bit processor */
#define EM_KVARC              214    /* KM211 KVARC processor */
#define EM_CDP                215    /* Paneve CDP architecture family */
#define EM_COGE               216    /* Cognitive Smart Memory Processor */
#define EM_COOL               217    /* Bluechip Systems CoolEngine */
#define EM_NORC               218    /* Nanoradio Optimized RISC */
#define EM_CSR_KALIMBA        219    /* CSR Kalimba architecture family */
#define EM_Z80                220    /* Zilog Z80 */
#define EM_VISIUM             221    /* Controls and Data Services VISIUMcore processor */
#define EM_FT32               222    /* FTDI Chip FT32 high performance 32-bit RISC architecture */
#define EM_MOXIE              223    /* Moxie processor family */
#define EM_AMDGPU             224    /* AMD GPU architecture */
                                     /* 225-242 Reserved for future use */
#define EM_RISCV              243    /* RISC-V */
#define EM_LANAI              244    /* Lanai processor */
#define EM_CEVA               245    /* CEVA Processor Architecture Family */
#define EM_CEVA_X2            246    /* CEVA X2 Processor Family */
#define EM_BPF                247    /* Linux BPF – in-kernel virtual machine */
#define EM_GRAPHCORE_IPU      248    /* Graphcore Intelligent Processing Unit */
#define EM_IMG1               249    /* Imagination Technologies */
#define EM_NFP                250    /* Netronome Flow Processor (NFP) */
#define EM_VE                 251    /* NEC Vector Engine */
#define EM_CSKY               252    /* C-SKY processor family */
#define EM_ARC_COMPACT3_64    253    /* Synopsys ARCv2.3 64-bit */
#define EM_MCS6502            254    /* MOS Technology MCS 6502 processor */
#define EM_ARC_COMPACT3       255    /* Synopsys ARCv2.3 32-bit */
#define EM_KVX                256    /* Kalray VLIW core of the MPPA processor family */
#define EM_65816              257    /* WDC 65816/65C816 */
#define EM_LOONGARCH          258    /* Loongson Loongarch */
#define EM_KF32               259    /* ChipON KungFu32 */
#define EM_U16_U8CORE         260    /* LAPIS nX-U16/U8 */
#define EM_TACHYUM            261    /* Reserved for Tachyum processor */
#define EM_56800EF            262    /* NXP 56800EF Digital Signal Controller (DSC) */
#define EM_SBF                263    /* Solana Bytecode Format */
#define EM_AIENGINE           264    /* AMD/Xilinx AIEngine architecture */
#define EM_SIMA_MLA           265    /* SiMa MLA */
#define EM_BANG               266    /* Cambricon BANG */
#define EM_LOONGGPU           267    /* Loongson Loongarch */

#define EV_NONE 0
#define EV_CURRENT 1

#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_ABIVERSION 8
#define EI_PAD 9

#define ELFMAG0     '\x7f'
#define ELFMAG1     'E'
#define ELFMAG2     'L'
#define ELFMAG3     'F'

#define ELFCLASSNONE    0
#define ELFCLASS32      1
#define ELFCLASS64      2

#define ELFDATANONE     0
#define ELFDATA2LSB     1
#define ELFDATA2MSB     2

#define ELFOSABI_NONE   0
#define ELFOSABI_HPUX   1
#define ELFOSABI_NETBSD 2
#define ELFOSABI_GNU    3
#define ELFOSABI_LINUX  3
#define ELFOSABI_SOLARIS 6
#define ELFOSABI_AIX    7
#define ELFOSABI_IRIX   8
#define ELFOSABI_FREEBSD 9
#define ELFOSABI_TRU64  10
#define ELFOSABI_MODESTO 11
#define ELFOSABI_OPENBSD 12
#define ELFOSABI_OPENVMS 13
#define ELFOSABI_NSK    14
#define ELFOSABI_AROS   15
#define ELFOSABI_FENIXOS 16
#define ELFOSABI_CLOUDABI 17
#define ELFOSABI_OPENVOS 18

#define SHN_UNDEF       0
#define SHN_LORESERVE   0xff00
#define SHN_LOPROC      0xff00
#define SHN_HIPROC      0xff1f
#define SHN_LOOS        0xff20
#define SHN_HIOS        0xff3f
#define SHN_ABS         0xfff1
#define SHN_COMMON      0xfff2
#define SHN_XINDEX      0xffff
#define SHN_HIRESERVE   0xffff

typedef struct {
    Elf32_Word  sh_name;
    Elf32_Word  sh_type;
    Elf32_Word  sh_flags;
    Elf32_Addr  sh_addr;
    Elf32_Off   sh_offset;
    Elf32_Word  sh_size;
    Elf32_Word  sh_link;
    Elf32_Word  sh_info;
    Elf32_Word  sh_addralign;
    Elf32_Word  sh_entsize;
} Elf32_Shdr;

typedef struct {
    Elf64_Word  sh_name;
    Elf64_Word  sh_type;
    Elf64_Xword sh_flags;
    Elf64_Addr  sh_addr;
    Elf64_Off   sh_offset;
    Elf64_Xword sh_size;
    Elf64_Word  sh_link;
    Elf64_Word  sh_info;
    Elf64_Xword sh_addralign;
    Elf64_Xword sh_entsize;
} Elf64_Shdr;

#define SHT_NULL            0
#define SHT_PROGBITS        1
#define SHT_SYMTAB          2
#define SHT_STRTAB          3
#define SHT_RELA            4
#define SHT_HASH            5
#define SHT_DYNAMIC         6
#define SHT_NOTE            7
#define SHT_NOBITS          8
#define SHT_REL             9
#define SHT_SHLIB           10
#define SHT_DYNSYM          11
#define SHT_INIT_ARRAY      14
#define SHT_FINI_ARRAY      15
#define SHT_PREINIT_ARRAY   16
#define SHT_GROUP           17
#define SHT_SYMTAB_SHNDX    18
#define SHT_LOOS            0x60000000
#define SHT_HIOS            0x6fffffff
#define SHT_LOPROC          0x70000000
#define SHT_HIPROC          0x7fffffff
#define SHT_LOUSER          0x80000000
#define SHT_HIUSER          0xffffffff

#define SHT_GNU_INCREMENTAL_INPUTS  0x6fff4700
#define SHT_LLVM_ODRTAB             0x6fff4c00
#define SHT_GNU_ATTRIBUTES          0x6ffffff5
#define SHT_GNU_HASH                0x6ffffff6
#define SHT_GNU_LIBLIST             0x6ffffff7
#define SHT_CHECKSUM	            0x6ffffff8
#define SHT_GNU_verdef              0x6ffffffd
#define SHT_GNU_verneed             0x6ffffffe
#define SHT_GNU_versym              0x6fffffff
#define SHT_X86_64_UNWIND           0x70000001

#define SHF_WRITE           0x1
#define SHF_ALLOC           0x2
#define SHF_EXECINSTR       0x4
#define SHF_MERGE           0x10
#define SHF_STRINGS         0x20
#define SHF_INFO_LINK       0x40
#define SHF_LINK_ORDER      0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP           0x200
#define SHF_TLS             0x400
#define SHF_COMPRESSED      0x800
#define SHF_MASKOS          0x0ff00000
#define SHF_MASKPROC        0xf0000000
#define SHF_X86_64_LARGE    0x10000000

#define SHF_GNU_RETAIN      0x00200000
#define SHF_ORDERED         0x40000000
#define SHF_EXCLUDE         0x80000000U

typedef struct {
    Elf32_Word  ch_type;
    Elf32_Word  ch_size;
    Elf32_Word  ch_addralign;
} Elf32_Chdr;

typedef struct {
    Elf64_Word  ch_type;
    Elf64_Word  ch_reserved;
    Elf64_Xword ch_size;
    Elf64_Xword ch_addralign;
} Elf64_Chdr;

#define ELFCOMPRESS_ZLIB        1
#define ELFCOMPRESS_LOOS        0x60000000
#define ELFCOMPRESS_HIOS        0x6fffffff
#define ELFCOMPRESS_LOPROC      0x70000000
#define ELFCOMPRESS_HIPROC      0x7fffffff

#define GRP_COMDAT          0x1
#define GRP_MASKOS          0x0ff00000
#define GRP_MASKPROC        0xf0000000

#define STN_UNDEF           0

typedef struct {
    Elf32_Word  st_name;
    Elf32_Addr  st_value;
    Elf32_Word  st_size;
    unsigned char st_info;
    unsigned char st_other;
    Elf32_Half  st_shndx;
} Elf32_Sym;

typedef struct {
    Elf64_Word  st_name;
    unsigned char st_info;
    unsigned char st_other;
    Elf64_Half  st_shndx;
    Elf64_Addr  st_value;
    Elf64_Xword st_size;
} Elf64_Sym;

#define ELF32_ST_BIND(i)    ((i)>>4)
#define ELF32_ST_TYPE(i)    ((i)&0xf)
#define ELF32_ST_INFO(b,t)  ((b)<<4|(t))
#define ELF32_ST_VISIBILITY(o)  ((o)&3)
#define ELF64_ST_BIND(i)    ((i)>>4)
#define ELF64_ST_TYPE(i)    ((i)&0xf)
#define ELF64_ST_INFO(b,t)  ((b)<<4|(t))
#define ELF64_ST_VISIBILITY(o)  ((o)&3)

#define STB_LOCAL   0
#define STB_GLOBAL  1
#define STB_WEAK    2
#define STB_LOOS    10
#define STB_HIOS    12
#define STB_LOPROC  13
#define STB_HIPROC  15

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_COMMON  5
#define STT_TLS     6
#define STT_LOOS    10
#define STT_HIOS    12
#define STT_LOPROC  13
#define STT_HIPROC  15

#define STT_GNU_IFUNC 10

#define STV_DEFAULT     0
#define STV_INTERNAL    1
#define STV_HIDDEN      2
#define STV_PROTECTED   3


typedef struct {
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
} Elf32_Rel;

typedef struct {
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
    Elf32_Sword r_addend;
} Elf32_Rela;

typedef struct {
    Elf64_Addr  r_offset;
    Elf64_Xword r_info;
} Elf64_Rel;

typedef struct {
    Elf64_Addr      r_offset;
    Elf64_Xword     r_info;
    Elf64_Sxword    r_addend;
} Elf64_Rela;

#define ELF32_R_SYM(i)  ((i)>>8)
#define ELF32_R_TYPE(i) ((i)&0xff)
#define ELF32_R_INFO(s,t)   ((s)<<8|(t))

#define ELF64_R_SYM(i)  ((i)>>32)
#define ELF64_R_TYPE(i) ((i)&0xffffffffU)
#define ELF64_R_INFO(s,t)   ((s)+0ull << 32 | (t))

#define R_386_NONE	   0		/* No reloc */
#define R_386_32	   1		/* Direct 32 bit  */
#define R_386_PC32	   2		/* PC relative 32 bit */
#define R_386_GOT32	   3		/* 32 bit GOT entry */
#define R_386_PLT32	   4		/* 32 bit PLT address */
#define R_386_COPY	   5		/* Copy symbol at runtime */
#define R_386_GLOB_DAT	   6		/* Create GOT entry */
#define R_386_JMP_SLOT	   7		/* Create PLT entry */
#define R_386_RELATIVE	   8		/* Adjust by program base */
#define R_386_GOTOFF	   9		/* 32 bit offset to GOT */
#define R_386_GOTPC	   10		/* 32 bit PC relative offset to GOT */
#define R_386_32PLT	   11
#define R_386_TLS_TPOFF	   14		/* Offset in static TLS block */
#define R_386_TLS_IE	   15		/* Address of GOT entry for static TLS block offset */
#define R_386_TLS_GOTIE	   16		/* GOT entry for static TLS block offset */
#define R_386_TLS_LE	   17		/* Offset relative to static TLS block */
#define R_386_TLS_GD	   18		/* Direct 32 bit for GNU version of general dynamic thread local data */
#define R_386_TLS_LDM	   19		/* Direct 32 bit for GNU version of local dynamic thread local data in LE code */
#define R_386_16	   20
#define R_386_PC16	   21
#define R_386_8		   22
#define R_386_PC8	   23
#define R_386_TLS_GD_32	   24		/* Direct 32 bit for general dynamic thread local data */
#define R_386_TLS_GD_PUSH  25		/* Tag for pushl in GD TLS code */
#define R_386_TLS_GD_CALL  26		/* Relocation for call to __tls_get_addr() */
#define R_386_TLS_GD_POP   27		/* Tag for popl in GD TLS code */
#define R_386_TLS_LDM_32   28		/* Direct 32 bit for local dynamic thread local data in LE code */
#define R_386_TLS_LDM_PUSH 29		/* Tag for pushl in LDM TLS code */
#define R_386_TLS_LDM_CALL 30		/* Relocation for call to __tls_get_addr() in LDM code */
#define R_386_TLS_LDM_POP  31		/* Tag for popl in LDM TLS code */
#define R_386_TLS_LDO_32   32		/* Offset relative to TLS block */
#define R_386_TLS_IE_32	   33		/* GOT entry for negated static TLS block offset */
#define R_386_TLS_LE_32	   34		/* Negated offset relative to static TLS block */
#define R_386_TLS_DTPMOD32 35		/* ID of module containing symbol */
#define R_386_TLS_DTPOFF32 36		/* Offset in TLS block */
#define R_386_TLS_TPOFF32  37		/* Negated offset in static TLS block */
#define R_386_SIZE32	   38 		/* 32-bit symbol size */
#define R_386_TLS_GOTDESC  39		/* GOT offset for TLS descriptor.  */
#define R_386_TLS_DESC_CALL 40		/* Marker of call through TLS descriptor for relaxation.  */
#define R_386_TLS_DESC     41		/* TLS descriptor containing pointer to code and to argument, returning the TLS offset for the symbol.  */
#define R_386_IRELATIVE	   42		/* Adjust indirectly by program base */
#define R_386_GOT32X	   43		/* Load from 32 bit GOT entry, relaxable. */

#define R_X86_64_NONE 0
#define R_X86_64_64 1
#define R_X86_64_PC32 2
#define R_X86_64_GOT32 3
#define R_X86_64_PLT32 4
#define R_X86_64_COPY 5
#define R_X86_64_GLOB_DAT 6
#define R_X86_64_JUMP_SLOT 7
#define R_X86_64_RELATIVE 8
#define R_X86_64_GOTPCREL 9
#define R_X86_64_32 10
#define R_X86_64_32S 11
#define R_X86_64_16 12
#define R_X86_64_PC16 13
#define R_X86_64_8 14
#define R_X86_64_PC8 15
#define R_X86_64_DTPMOD64 16
#define R_X86_64_DTPOFF64 17
#define R_X86_64_TPOFF64 18
#define R_X86_64_TLSGD 19
#define R_X86_64_TLSLD 20
#define R_X86_64_DTPOFF32 21
#define R_X86_64_GOTTPOFF 22
#define R_X86_64_TPOFF32 23
#define R_X86_64_PC64 24
#define R_X86_64_GOTOFF64 25
#define R_X86_64_GOTPC32 26
#define R_X86_64_GOT64 27
#define R_X86_64_GOTPCREL64 28
#define R_X86_64_GOTPC64 29
#define R_X86_64_PLTOFF64 31
#define R_X86_64_SIZE32 32
#define R_X86_64_SIZE64 †
#define R_X86_64_GOTPC32_TLSDESC 34
#define R_X86_64_TLSDESC_CALL 35
#define R_X86_64_TLSDESC 36
#define R_X86_64_IRELATIVE 37
#define R_X86_64_RELATIVE64 38
#define R_X86_64_GOTPCRELX 41
#define R_X86_64_REX_GOTPCRELX 42
#define R_X86_64_CODE_4_GOTPCRELX 43
#define R_X86_64_CODE_4_GOTTPOFF 44
#define R_X86_64_CODE_4_GOTPC32_TLSDESC 45
#define R_X86_64_CODE_5_GOTPCRELX 46
#define R_X86_64_CODE_5_GOTTPOFF 47
#define R_X86_64_CODE_5_GOTPC32_TLSDESC 48
#define R_X86_64_CODE_6_GOTPCRELX 49
#define R_X86_64_CODE_6_GOTTPOFF 50
#define R_X86_64_CODE_6_GOTPC32_TLSDESC 51

#define R_PPC_NONE		0
#define R_PPC_ADDR32		1	/* 32bit absolute address */
#define R_PPC_ADDR24		2	/* 26bit address, 2 bits ignored.  */
#define R_PPC_ADDR16		3	/* 16bit absolute address */
#define R_PPC_ADDR16_LO		4	/* lower 16bit of absolute address */
#define R_PPC_ADDR16_HI		5	/* high 16bit of absolute address */
#define R_PPC_ADDR16_HA		6	/* adjusted high 16bit */
#define R_PPC_ADDR14		7	/* 16bit address, 2 bits ignored */
#define R_PPC_ADDR14_BRTAKEN	8
#define R_PPC_ADDR14_BRNTAKEN	9
#define R_PPC_REL24		10	/* PC relative 26 bit */
#define R_PPC_REL14		11	/* PC relative 16 bit */
#define R_PPC_REL14_BRTAKEN	12
#define R_PPC_REL14_BRNTAKEN	13
#define R_PPC_GOT16		14
#define R_PPC_GOT16_LO		15
#define R_PPC_GOT16_HI		16
#define R_PPC_GOT16_HA		17
#define R_PPC_PLTREL24		18
#define R_PPC_COPY		19
#define R_PPC_GLOB_DAT		20
#define R_PPC_JMP_SLOT		21
#define R_PPC_RELATIVE		22
#define R_PPC_LOCAL24PC		23
#define R_PPC_UADDR32		24
#define R_PPC_UADDR16		25
#define R_PPC_REL32		26
#define R_PPC_PLT32		27
#define R_PPC_PLTREL32		28
#define R_PPC_PLT16_LO		29
#define R_PPC_PLT16_HI		30
#define R_PPC_PLT16_HA		31
#define R_PPC_SDAREL16		32
#define R_PPC_SECTOFF		33
#define R_PPC_SECTOFF_LO	34
#define R_PPC_SECTOFF_HI	35
#define R_PPC_SECTOFF_HA	36

/* PowerPC relocations defined for the TLS access ABI.  */
#define R_PPC_TLS		67 /* none	(sym+add)@tls */
#define R_PPC_DTPMOD32		68 /* word32	(sym+add)@dtpmod */
#define R_PPC_TPREL16		69 /* half16*	(sym+add)@tprel */
#define R_PPC_TPREL16_LO	70 /* half16	(sym+add)@tprel@l */
#define R_PPC_TPREL16_HI	71 /* half16	(sym+add)@tprel@h */
#define R_PPC_TPREL16_HA	72 /* half16	(sym+add)@tprel@ha */
#define R_PPC_TPREL32		73 /* word32	(sym+add)@tprel */
#define R_PPC_DTPREL16		74 /* half16*	(sym+add)@dtprel */
#define R_PPC_DTPREL16_LO	75 /* half16	(sym+add)@dtprel@l */
#define R_PPC_DTPREL16_HI	76 /* half16	(sym+add)@dtprel@h */
#define R_PPC_DTPREL16_HA	77 /* half16	(sym+add)@dtprel@ha */
#define R_PPC_DTPREL32		78 /* word32	(sym+add)@dtprel */
#define R_PPC_GOT_TLSGD16	79 /* half16*	(sym+add)@got@tlsgd */
#define R_PPC_GOT_TLSGD16_LO	80 /* half16	(sym+add)@got@tlsgd@l */
#define R_PPC_GOT_TLSGD16_HI	81 /* half16	(sym+add)@got@tlsgd@h */
#define R_PPC_GOT_TLSGD16_HA	82 /* half16	(sym+add)@got@tlsgd@ha */
#define R_PPC_GOT_TLSLD16	83 /* half16*	(sym+add)@got@tlsld */
#define R_PPC_GOT_TLSLD16_LO	84 /* half16	(sym+add)@got@tlsld@l */
#define R_PPC_GOT_TLSLD16_HI	85 /* half16	(sym+add)@got@tlsld@h */
#define R_PPC_GOT_TLSLD16_HA	86 /* half16	(sym+add)@got@tlsld@ha */
#define R_PPC_GOT_TPREL16	87 /* half16*	(sym+add)@got@tprel */
#define R_PPC_GOT_TPREL16_LO	88 /* half16	(sym+add)@got@tprel@l */
#define R_PPC_GOT_TPREL16_HI	89 /* half16	(sym+add)@got@tprel@h */
#define R_PPC_GOT_TPREL16_HA	90 /* half16	(sym+add)@got@tprel@ha */
#define R_PPC_GOT_DTPREL16	91 /* half16*	(sym+add)@got@dtprel */
#define R_PPC_GOT_DTPREL16_LO	92 /* half16*	(sym+add)@got@dtprel@l */
#define R_PPC_GOT_DTPREL16_HI	93 /* half16*	(sym+add)@got@dtprel@h */
#define R_PPC_GOT_DTPREL16_HA	94 /* half16*	(sym+add)@got@dtprel@ha */
#define R_PPC_TLSGD		95 /* none	(sym+add)@tlsgd */
#define R_PPC_TLSLD		96 /* none	(sym+add)@tlsld */

/* The remaining relocs are from the Embedded ELF ABI, and are not
   in the SVR4 ELF ABI.  */
#define R_PPC_EMB_NADDR32	101
#define R_PPC_EMB_NADDR16	102
#define R_PPC_EMB_NADDR16_LO	103
#define R_PPC_EMB_NADDR16_HI	104
#define R_PPC_EMB_NADDR16_HA	105
#define R_PPC_EMB_SDAI16	106
#define R_PPC_EMB_SDA2I16	107
#define R_PPC_EMB_SDA2REL	108
#define R_PPC_EMB_SDA21		109	/* 16 bit offset in SDA */
#define R_PPC_EMB_MRKREF	110
#define R_PPC_EMB_RELSEC16	111
#define R_PPC_EMB_RELST_LO	112
#define R_PPC_EMB_RELST_HI	113
#define R_PPC_EMB_RELST_HA	114
#define R_PPC_EMB_BIT_FLD	115
#define R_PPC_EMB_RELSDA	116	/* 16 bit relative offset in SDA */

/* Diab tool relocations.  */
#define R_PPC_DIAB_SDA21_LO	180	/* like EMB_SDA21, but lower 16 bit */
#define R_PPC_DIAB_SDA21_HI	181	/* like EMB_SDA21, but high 16 bit */
#define R_PPC_DIAB_SDA21_HA	182	/* like EMB_SDA21, adjusted high 16 */
#define R_PPC_DIAB_RELSDA_LO	183	/* like EMB_RELSDA, but lower 16 bit */
#define R_PPC_DIAB_RELSDA_HI	184	/* like EMB_RELSDA, but high 16 bit */
#define R_PPC_DIAB_RELSDA_HA	185	/* like EMB_RELSDA, adjusted high 16 */

/* GNU extension to support local ifunc.  */
#define R_PPC_IRELATIVE		248

/* GNU relocs used in PIC code sequences.  */
#define R_PPC_REL16		249	/* half16   (sym+add-.) */
#define R_PPC_REL16_LO		250	/* half16   (sym+add-.)@l */
#define R_PPC_REL16_HI		251	/* half16   (sym+add-.)@h */
#define R_PPC_REL16_HA		252	/* half16   (sym+add-.)@ha */

/* This is a phony reloc to handle any old fashioned TOC16 references
   that may still be in object files.  */
#define R_PPC_TOC16		255

/* PowerPC specific values for the Dyn d_tag field.  */
#define DT_PPC_GOT		(DT_LOPROC + 0)
#define DT_PPC_OPT		(DT_LOPROC + 1)
#define DT_PPC_NUM		2

/* PowerPC specific values for the DT_PPC_OPT Dyn entry.  */
#define PPC_OPT_TLS		1

/* PowerPC64 relocations defined by the ABIs */
#define R_PPC64_NONE		R_PPC_NONE
#define R_PPC64_ADDR32		R_PPC_ADDR32 /* 32bit absolute address */
#define R_PPC64_ADDR24		R_PPC_ADDR24 /* 26bit address, word aligned */
#define R_PPC64_ADDR16		R_PPC_ADDR16 /* 16bit absolute address */
#define R_PPC64_ADDR16_LO	R_PPC_ADDR16_LO	/* lower 16bits of address */
#define R_PPC64_ADDR16_HI	R_PPC_ADDR16_HI	/* high 16bits of address. */
#define R_PPC64_ADDR16_HA	R_PPC_ADDR16_HA /* adjusted high 16bits.  */
#define R_PPC64_ADDR14		R_PPC_ADDR14 /* 16bit address, word aligned */
#define R_PPC64_ADDR14_BRTAKEN	R_PPC_ADDR14_BRTAKEN
#define R_PPC64_ADDR14_BRNTAKEN	R_PPC_ADDR14_BRNTAKEN
#define R_PPC64_REL24		R_PPC_REL24 /* PC-rel. 26 bit, word aligned */
#define R_PPC64_REL14		R_PPC_REL14 /* PC relative 16 bit */
#define R_PPC64_REL14_BRTAKEN	R_PPC_REL14_BRTAKEN
#define R_PPC64_REL14_BRNTAKEN	R_PPC_REL14_BRNTAKEN
#define R_PPC64_GOT16		R_PPC_GOT16
#define R_PPC64_GOT16_LO	R_PPC_GOT16_LO
#define R_PPC64_GOT16_HI	R_PPC_GOT16_HI
#define R_PPC64_GOT16_HA	R_PPC_GOT16_HA

#define R_PPC64_COPY		R_PPC_COPY
#define R_PPC64_GLOB_DAT	R_PPC_GLOB_DAT
#define R_PPC64_JMP_SLOT	R_PPC_JMP_SLOT
#define R_PPC64_RELATIVE	R_PPC_RELATIVE

#define R_PPC64_UADDR32		R_PPC_UADDR32
#define R_PPC64_UADDR16		R_PPC_UADDR16
#define R_PPC64_REL32		R_PPC_REL32
#define R_PPC64_PLT32		R_PPC_PLT32
#define R_PPC64_PLTREL32	R_PPC_PLTREL32
#define R_PPC64_PLT16_LO	R_PPC_PLT16_LO
#define R_PPC64_PLT16_HI	R_PPC_PLT16_HI
#define R_PPC64_PLT16_HA	R_PPC_PLT16_HA

#define R_PPC64_SECTOFF		R_PPC_SECTOFF
#define R_PPC64_SECTOFF_LO	R_PPC_SECTOFF_LO
#define R_PPC64_SECTOFF_HI	R_PPC_SECTOFF_HI
#define R_PPC64_SECTOFF_HA	R_PPC_SECTOFF_HA
#define R_PPC64_ADDR30		37 /* word30 (S + A - P) >> 2 */
#define R_PPC64_ADDR64		38 /* doubleword64 S + A */
#define R_PPC64_ADDR16_HIGHER	39 /* half16 #higher(S + A) */
#define R_PPC64_ADDR16_HIGHERA	40 /* half16 #highera(S + A) */
#define R_PPC64_ADDR16_HIGHEST	41 /* half16 #highest(S + A) */
#define R_PPC64_ADDR16_HIGHESTA	42 /* half16 #highesta(S + A) */
#define R_PPC64_UADDR64		43 /* doubleword64 S + A */
#define R_PPC64_REL64		44 /* doubleword64 S + A - P */
#define R_PPC64_PLT64		45 /* doubleword64 L + A */
#define R_PPC64_PLTREL64	46 /* doubleword64 L + A - P */
#define R_PPC64_TOC16		47 /* half16* S + A - .TOC */
#define R_PPC64_TOC16_LO	48 /* half16 #lo(S + A - .TOC.) */
#define R_PPC64_TOC16_HI	49 /* half16 #hi(S + A - .TOC.) */
#define R_PPC64_TOC16_HA	50 /* half16 #ha(S + A - .TOC.) */
#define R_PPC64_TOC		51 /* doubleword64 .TOC */
#define R_PPC64_PLTGOT16	52 /* half16* M + A */
#define R_PPC64_PLTGOT16_LO	53 /* half16 #lo(M + A) */
#define R_PPC64_PLTGOT16_HI	54 /* half16 #hi(M + A) */
#define R_PPC64_PLTGOT16_HA	55 /* half16 #ha(M + A) */

#define R_PPC64_ADDR16_DS	56 /* half16ds* (S + A) >> 2 */
#define R_PPC64_ADDR16_LO_DS	57 /* half16ds  #lo(S + A) >> 2 */
#define R_PPC64_GOT16_DS	58 /* half16ds* (G + A) >> 2 */
#define R_PPC64_GOT16_LO_DS	59 /* half16ds  #lo(G + A) >> 2 */
#define R_PPC64_PLT16_LO_DS	60 /* half16ds  #lo(L + A) >> 2 */
#define R_PPC64_SECTOFF_DS	61 /* half16ds* (R + A) >> 2 */
#define R_PPC64_SECTOFF_LO_DS	62 /* half16ds  #lo(R + A) >> 2 */
#define R_PPC64_TOC16_DS	63 /* half16ds* (S + A - .TOC.) >> 2 */
#define R_PPC64_TOC16_LO_DS	64 /* half16ds  #lo(S + A - .TOC.) >> 2 */
#define R_PPC64_PLTGOT16_DS	65 /* half16ds* (M + A) >> 2 */
#define R_PPC64_PLTGOT16_LO_DS	66 /* half16ds  #lo(M + A) >> 2 */

/* PowerPC64 relocations defined for the TLS access ABI.  */
#define R_PPC64_TLS		67 /* none	(sym+add)@tls */
#define R_PPC64_DTPMOD64	68 /* doubleword64 (sym+add)@dtpmod */
#define R_PPC64_TPREL16		69 /* half16*	(sym+add)@tprel */
#define R_PPC64_TPREL16_LO	70 /* half16	(sym+add)@tprel@l */
#define R_PPC64_TPREL16_HI	71 /* half16	(sym+add)@tprel@h */
#define R_PPC64_TPREL16_HA	72 /* half16	(sym+add)@tprel@ha */
#define R_PPC64_TPREL64		73 /* doubleword64 (sym+add)@tprel */
#define R_PPC64_DTPREL16	74 /* half16*	(sym+add)@dtprel */
#define R_PPC64_DTPREL16_LO	75 /* half16	(sym+add)@dtprel@l */
#define R_PPC64_DTPREL16_HI	76 /* half16	(sym+add)@dtprel@h */
#define R_PPC64_DTPREL16_HA	77 /* half16	(sym+add)@dtprel@ha */
#define R_PPC64_DTPREL64	78 /* doubleword64 (sym+add)@dtprel */
#define R_PPC64_GOT_TLSGD16	79 /* half16*	(sym+add)@got@tlsgd */
#define R_PPC64_GOT_TLSGD16_LO	80 /* half16	(sym+add)@got@tlsgd@l */
#define R_PPC64_GOT_TLSGD16_HI	81 /* half16	(sym+add)@got@tlsgd@h */
#define R_PPC64_GOT_TLSGD16_HA	82 /* half16	(sym+add)@got@tlsgd@ha */
#define R_PPC64_GOT_TLSLD16	83 /* half16*	(sym+add)@got@tlsld */
#define R_PPC64_GOT_TLSLD16_LO	84 /* half16	(sym+add)@got@tlsld@l */
#define R_PPC64_GOT_TLSLD16_HI	85 /* half16	(sym+add)@got@tlsld@h */
#define R_PPC64_GOT_TLSLD16_HA	86 /* half16	(sym+add)@got@tlsld@ha */
#define R_PPC64_GOT_TPREL16_DS	87 /* half16ds*	(sym+add)@got@tprel */
#define R_PPC64_GOT_TPREL16_LO_DS 88 /* half16ds (sym+add)@got@tprel@l */
#define R_PPC64_GOT_TPREL16_HI	89 /* half16	(sym+add)@got@tprel@h */
#define R_PPC64_GOT_TPREL16_HA	90 /* half16	(sym+add)@got@tprel@ha */
#define R_PPC64_GOT_DTPREL16_DS	91 /* half16ds*	(sym+add)@got@dtprel */
#define R_PPC64_GOT_DTPREL16_LO_DS 92 /* half16ds (sym+add)@got@dtprel@l */
#define R_PPC64_GOT_DTPREL16_HI	93 /* half16	(sym+add)@got@dtprel@h */
#define R_PPC64_GOT_DTPREL16_HA	94 /* half16	(sym+add)@got@dtprel@ha */
#define R_PPC64_TPREL16_DS	95 /* half16ds*	(sym+add)@tprel */
#define R_PPC64_TPREL16_LO_DS	96 /* half16ds	(sym+add)@tprel@l */
#define R_PPC64_TPREL16_HIGHER	97 /* half16	(sym+add)@tprel@higher */
#define R_PPC64_TPREL16_HIGHERA	98 /* half16	(sym+add)@tprel@highera */
#define R_PPC64_TPREL16_HIGHEST	99 /* half16	(sym+add)@tprel@highest */
#define R_PPC64_TPREL16_HIGHESTA 100 /* half16	(sym+add)@tprel@highesta */
#define R_PPC64_DTPREL16_DS	101 /* half16ds* (sym+add)@dtprel */
#define R_PPC64_DTPREL16_LO_DS	102 /* half16ds	(sym+add)@dtprel@l */
#define R_PPC64_DTPREL16_HIGHER	103 /* half16	(sym+add)@dtprel@higher */
#define R_PPC64_DTPREL16_HIGHERA 104 /* half16	(sym+add)@dtprel@highera */
#define R_PPC64_DTPREL16_HIGHEST 105 /* half16	(sym+add)@dtprel@highest */
#define R_PPC64_DTPREL16_HIGHESTA 106 /* half16	(sym+add)@dtprel@highesta */
#define R_PPC64_TLSGD		107 /* none	(sym+add)@tlsgd */
#define R_PPC64_TLSLD		108 /* none	(sym+add)@tlsld */
#define R_PPC64_TOCSAVE		109 /* none */

/* Added when HA and HI relocs were changed to report overflows.  */
#define R_PPC64_ADDR16_HIGH	110
#define R_PPC64_ADDR16_HIGHA	111
#define R_PPC64_TPREL16_HIGH	112
#define R_PPC64_TPREL16_HIGHA	113
#define R_PPC64_DTPREL16_HIGH	114
#define R_PPC64_DTPREL16_HIGHA	115

/* GNU extension to support local ifunc.  */
#define R_PPC64_JMP_IREL	247
#define R_PPC64_IRELATIVE	248
#define R_PPC64_REL16		249	/* half16   (sym+add-.) */
#define R_PPC64_REL16_LO	250	/* half16   (sym+add-.)@l */
#define R_PPC64_REL16_HI	251	/* half16   (sym+add-.)@h */
#define R_PPC64_REL16_HA	252	/* half16   (sym+add-.)@ha */

/* e_flags bits specifying ABI.
   1 for original function descriptor using ABI,
   2 for revised ABI without function descriptors,
   0 for unspecified or not using any features affected by the differences.  */
#define EF_PPC64_ABI	3

/* PowerPC64 specific values for the Dyn d_tag field.  */
#define DT_PPC64_GLINK  (DT_LOPROC + 0)
#define DT_PPC64_OPD	(DT_LOPROC + 1)
#define DT_PPC64_OPDSZ	(DT_LOPROC + 2)
#define DT_PPC64_OPT	(DT_LOPROC + 3)
#define DT_PPC64_NUM    4

/* PowerPC64 specific bits in the DT_PPC64_OPT Dyn entry.  */
#define PPC64_OPT_TLS		1
#define PPC64_OPT_MULTI_TOC	2
#define PPC64_OPT_LOCALENTRY	4

/* PowerPC64 specific values for the Elf64_Sym st_other field.  */
#define STO_PPC64_LOCAL_BIT	5
#define STO_PPC64_LOCAL_MASK	(7 << STO_PPC64_LOCAL_BIT)
#define PPC64_LOCAL_ENTRY_OFFSET(other)				\
 (((1 << (((other) & STO_PPC64_LOCAL_MASK) >> STO_PPC64_LOCAL_BIT)) >> 2) << 2)

/* AArch64 relocs.  */

#define R_AARCH64_NONE            0	/* No relocation.  */

/* ILP32 AArch64 relocs.  */
#define R_AARCH64_P32_ABS32		  1	/* Direct 32 bit.  */
#define R_AARCH64_P32_COPY		180	/* Copy symbol at runtime.  */
#define R_AARCH64_P32_GLOB_DAT		181	/* Create GOT entry.  */
#define R_AARCH64_P32_JUMP_SLOT		182	/* Create PLT entry.  */
#define R_AARCH64_P32_RELATIVE		183	/* Adjust by program base.  */
#define R_AARCH64_P32_TLS_DTPMOD	184	/* Module number, 32 bit.  */
#define R_AARCH64_P32_TLS_DTPREL	185	/* Module-relative offset, 32 bit.  */
#define R_AARCH64_P32_TLS_TPREL		186	/* TP-relative offset, 32 bit.  */
#define R_AARCH64_P32_TLSDESC		187	/* TLS Descriptor.  */
#define R_AARCH64_P32_IRELATIVE		188	/* STT_GNU_IFUNC relocation. */

/* LP64 AArch64 relocs.  */
#define R_AARCH64_ABS64         257	/* Direct 64 bit. */
#define R_AARCH64_ABS32         258	/* Direct 32 bit.  */
#define R_AARCH64_ABS16		259	/* Direct 16-bit.  */
#define R_AARCH64_PREL64	260	/* PC-relative 64-bit.	*/
#define R_AARCH64_PREL32	261	/* PC-relative 32-bit.	*/
#define R_AARCH64_PREL16	262	/* PC-relative 16-bit.	*/
#define R_AARCH64_MOVW_UABS_G0	263	/* Dir. MOVZ imm. from bits 15:0.  */
#define R_AARCH64_MOVW_UABS_G0_NC 264	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_UABS_G1	265	/* Dir. MOVZ imm. from bits 31:16.  */
#define R_AARCH64_MOVW_UABS_G1_NC 266	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_UABS_G2	267	/* Dir. MOVZ imm. from bits 47:32.  */
#define R_AARCH64_MOVW_UABS_G2_NC 268	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_UABS_G3	269	/* Dir. MOV{K,Z} imm. from 63:48.  */
#define R_AARCH64_MOVW_SABS_G0	270	/* Dir. MOV{N,Z} imm. from 15:0.  */
#define R_AARCH64_MOVW_SABS_G1	271	/* Dir. MOV{N,Z} imm. from 31:16.  */
#define R_AARCH64_MOVW_SABS_G2	272	/* Dir. MOV{N,Z} imm. from 47:32.  */
#define R_AARCH64_LD_PREL_LO19	273	/* PC-rel. LD imm. from bits 20:2.  */
#define R_AARCH64_ADR_PREL_LO21	274	/* PC-rel. ADR imm. from bits 20:0.  */
#define R_AARCH64_ADR_PREL_PG_HI21 275	/* Page-rel. ADRP imm. from 32:12.  */
#define R_AARCH64_ADR_PREL_PG_HI21_NC 276 /* Likewise; no overflow check.  */
#define R_AARCH64_ADD_ABS_LO12_NC 277	/* Dir. ADD imm. from bits 11:0.  */
#define R_AARCH64_LDST8_ABS_LO12_NC 278	/* Likewise for LD/ST; no check. */
#define R_AARCH64_TSTBR14	279	/* PC-rel. TBZ/TBNZ imm. from 15:2.  */
#define R_AARCH64_CONDBR19	280	/* PC-rel. cond. br. imm. from 20:2. */
#define R_AARCH64_JUMP26	282	/* PC-rel. B imm. from bits 27:2.  */
#define R_AARCH64_CALL26	283	/* Likewise for CALL.  */
#define R_AARCH64_LDST16_ABS_LO12_NC 284 /* Dir. ADD imm. from bits 11:1.  */
#define R_AARCH64_LDST32_ABS_LO12_NC 285 /* Likewise for bits 11:2.  */
#define R_AARCH64_LDST64_ABS_LO12_NC 286 /* Likewise for bits 11:3.  */
#define R_AARCH64_MOVW_PREL_G0	287	/* PC-rel. MOV{N,Z} imm. from 15:0.  */
#define R_AARCH64_MOVW_PREL_G0_NC 288	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_PREL_G1	289	/* PC-rel. MOV{N,Z} imm. from 31:16. */
#define R_AARCH64_MOVW_PREL_G1_NC 290	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_PREL_G2	291	/* PC-rel. MOV{N,Z} imm. from 47:32. */
#define R_AARCH64_MOVW_PREL_G2_NC 292	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_PREL_G3	293	/* PC-rel. MOV{N,Z} imm. from 63:48. */
#define R_AARCH64_LDST128_ABS_LO12_NC 299 /* Dir. ADD imm. from bits 11:4.  */
#define R_AARCH64_MOVW_GOTOFF_G0 300	/* GOT-rel. off. MOV{N,Z} imm. 15:0. */
#define R_AARCH64_MOVW_GOTOFF_G0_NC 301	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_GOTOFF_G1 302	/* GOT-rel. o. MOV{N,Z} imm. 31:16.  */
#define R_AARCH64_MOVW_GOTOFF_G1_NC 303	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_GOTOFF_G2 304	/* GOT-rel. o. MOV{N,Z} imm. 47:32.  */
#define R_AARCH64_MOVW_GOTOFF_G2_NC 305	/* Likewise for MOVK; no check.  */
#define R_AARCH64_MOVW_GOTOFF_G3 306	/* GOT-rel. o. MOV{N,Z} imm. 63:48.  */
#define R_AARCH64_GOTREL64	307	/* GOT-relative 64-bit.  */
#define R_AARCH64_GOTREL32	308	/* GOT-relative 32-bit.  */
#define R_AARCH64_GOT_LD_PREL19	309	/* PC-rel. GOT off. load imm. 20:2.  */
#define R_AARCH64_LD64_GOTOFF_LO15 310	/* GOT-rel. off. LD/ST imm. 14:3.  */
#define R_AARCH64_ADR_GOT_PAGE	311	/* P-page-rel. GOT off. ADRP 32:12.  */
#define R_AARCH64_LD64_GOT_LO12_NC 312	/* Dir. GOT off. LD/ST imm. 11:3.  */
#define R_AARCH64_LD64_GOTPAGE_LO15 313	/* GOT-page-rel. GOT off. LD/ST 14:3 */
#define R_AARCH64_TLSGD_ADR_PREL21 512	/* PC-relative ADR imm. 20:0.  */
#define R_AARCH64_TLSGD_ADR_PAGE21 513	/* page-rel. ADRP imm. 32:12.  */
#define R_AARCH64_TLSGD_ADD_LO12_NC 514	/* direct ADD imm. from 11:0.  */
#define R_AARCH64_TLSGD_MOVW_G1	515	/* GOT-rel. MOV{N,Z} 31:16.  */
#define R_AARCH64_TLSGD_MOVW_G0_NC 516	/* GOT-rel. MOVK imm. 15:0.  */
#define R_AARCH64_TLSLD_ADR_PREL21 517	/* Like 512; local dynamic model.  */
#define R_AARCH64_TLSLD_ADR_PAGE21 518	/* Like 513; local dynamic model.  */
#define R_AARCH64_TLSLD_ADD_LO12_NC 519	/* Like 514; local dynamic model.  */
#define R_AARCH64_TLSLD_MOVW_G1	520	/* Like 515; local dynamic model.  */
#define R_AARCH64_TLSLD_MOVW_G0_NC 521	/* Like 516; local dynamic model.  */
#define R_AARCH64_TLSLD_LD_PREL19 522	/* TLS PC-rel. load imm. 20:2.  */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G2 523 /* TLS DTP-rel. MOV{N,Z} 47:32.  */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G1 524 /* TLS DTP-rel. MOV{N,Z} 31:16.  */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC 525 /* Likewise; MOVK; no check.  */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G0 526 /* TLS DTP-rel. MOV{N,Z} 15:0.  */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC 527 /* Likewise; MOVK; no check.  */
#define R_AARCH64_TLSLD_ADD_DTPREL_HI12 528 /* DTP-rel. ADD imm. from 23:12. */
#define R_AARCH64_TLSLD_ADD_DTPREL_LO12 529 /* DTP-rel. ADD imm. from 11:0.  */
#define R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC 530 /* Likewise; no ovfl. check.  */
#define R_AARCH64_TLSLD_LDST8_DTPREL_LO12 531 /* DTP-rel. LD/ST imm. 11:0.  */
#define R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC 532 /* Likewise; no check.  */
#define R_AARCH64_TLSLD_LDST16_DTPREL_LO12 533 /* DTP-rel. LD/ST imm. 11:1.  */
#define R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC 534 /* Likewise; no check.  */
#define R_AARCH64_TLSLD_LDST32_DTPREL_LO12 535 /* DTP-rel. LD/ST imm. 11:2.  */
#define R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC 536 /* Likewise; no check.  */
#define R_AARCH64_TLSLD_LDST64_DTPREL_LO12 537 /* DTP-rel. LD/ST imm. 11:3.  */
#define R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC 538 /* Likewise; no check.  */
#define R_AARCH64_TLSIE_MOVW_GOTTPREL_G1 539 /* GOT-rel. MOV{N,Z} 31:16.  */
#define R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC 540 /* GOT-rel. MOVK 15:0.  */
#define R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21 541 /* Page-rel. ADRP 32:12.  */
#define R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC 542 /* Direct LD off. 11:3.  */
#define R_AARCH64_TLSIE_LD_GOTTPREL_PREL19 543 /* PC-rel. load imm. 20:2.  */
#define R_AARCH64_TLSLE_MOVW_TPREL_G2 544 /* TLS TP-rel. MOV{N,Z} 47:32.  */
#define R_AARCH64_TLSLE_MOVW_TPREL_G1 545 /* TLS TP-rel. MOV{N,Z} 31:16.  */
#define R_AARCH64_TLSLE_MOVW_TPREL_G1_NC 546 /* Likewise; MOVK; no check.  */
#define R_AARCH64_TLSLE_MOVW_TPREL_G0 547 /* TLS TP-rel. MOV{N,Z} 15:0.  */
#define R_AARCH64_TLSLE_MOVW_TPREL_G0_NC 548 /* Likewise; MOVK; no check.  */
#define R_AARCH64_TLSLE_ADD_TPREL_HI12 549 /* TP-rel. ADD imm. 23:12.  */
#define R_AARCH64_TLSLE_ADD_TPREL_LO12 550 /* TP-rel. ADD imm. 11:0.  */
#define R_AARCH64_TLSLE_ADD_TPREL_LO12_NC 551 /* Likewise; no ovfl. check.  */
#define R_AARCH64_TLSLE_LDST8_TPREL_LO12 552 /* TP-rel. LD/ST off. 11:0.  */
#define R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC 553 /* Likewise; no ovfl. check. */
#define R_AARCH64_TLSLE_LDST16_TPREL_LO12 554 /* TP-rel. LD/ST off. 11:1.  */
#define R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC 555 /* Likewise; no check.  */
#define R_AARCH64_TLSLE_LDST32_TPREL_LO12 556 /* TP-rel. LD/ST off. 11:2.  */
#define R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC 557 /* Likewise; no check.  */
#define R_AARCH64_TLSLE_LDST64_TPREL_LO12 558 /* TP-rel. LD/ST off. 11:3.  */
#define R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC 559 /* Likewise; no check.  */
#define R_AARCH64_TLSDESC_LD_PREL19 560	/* PC-rel. load immediate 20:2.  */
#define R_AARCH64_TLSDESC_ADR_PREL21 561 /* PC-rel. ADR immediate 20:0.  */
#define R_AARCH64_TLSDESC_ADR_PAGE21 562 /* Page-rel. ADRP imm. 32:12.  */
#define R_AARCH64_TLSDESC_LD64_LO12 563	/* Direct LD off. from 11:3.  */
#define R_AARCH64_TLSDESC_ADD_LO12 564	/* Direct ADD imm. from 11:0.  */
#define R_AARCH64_TLSDESC_OFF_G1 565	/* GOT-rel. MOV{N,Z} imm. 31:16.  */
#define R_AARCH64_TLSDESC_OFF_G0_NC 566	/* GOT-rel. MOVK imm. 15:0; no ck.  */
#define R_AARCH64_TLSDESC_LDR	567	/* Relax LDR.  */
#define R_AARCH64_TLSDESC_ADD	568	/* Relax ADD.  */
#define R_AARCH64_TLSDESC_CALL	569	/* Relax BLR.  */
#define R_AARCH64_TLSLE_LDST128_TPREL_LO12 570 /* TP-rel. LD/ST off. 11:4.  */
#define R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC 571 /* Likewise; no check.  */
#define R_AARCH64_TLSLD_LDST128_DTPREL_LO12 572 /* DTP-rel. LD/ST imm. 11:4. */
#define R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC 573 /* Likewise; no check.  */
#define R_AARCH64_COPY         1024	/* Copy symbol at runtime.  */
#define R_AARCH64_GLOB_DAT     1025	/* Create GOT entry.  */
#define R_AARCH64_JUMP_SLOT    1026	/* Create PLT entry.  */
#define R_AARCH64_RELATIVE     1027	/* Adjust by program base.  */
#define R_AARCH64_TLS_DTPMOD   1028	/* Module number, 64 bit.  */
#define R_AARCH64_TLS_DTPREL   1029	/* Module-relative offset, 64 bit.  */
#define R_AARCH64_TLS_TPREL    1030	/* TP-relative offset, 64 bit.  */
#define R_AARCH64_TLSDESC      1031	/* TLS Descriptor.  */
#define R_AARCH64_IRELATIVE	1032	/* STT_GNU_IFUNC relocation.  */

/* MTE memory tag segment type.  */
#define PT_AARCH64_MEMTAG_MTE	(PT_LOPROC + 2)

/* AArch64 specific values for the Dyn d_tag field.  */
#define DT_AARCH64_BTI_PLT	(DT_LOPROC + 1)
#define DT_AARCH64_PAC_PLT	(DT_LOPROC + 3)
#define DT_AARCH64_VARIANT_PCS	(DT_LOPROC + 5)
#define DT_AARCH64_NUM		6

/* AArch64 specific values for the st_other field.  */
#define STO_AARCH64_VARIANT_PCS 0x80

typedef struct {
    Elf32_Word  p_type;
    Elf32_Off   p_offset;
    Elf32_Addr  p_vaddr;
    Elf32_Addr  p_paddr;
    Elf32_Word  p_filesz;
    Elf32_Word  p_memsz;
    Elf32_Word  p_flags;
    Elf32_Word  p_align;
} Elf32_Phdr;

typedef struct {
    Elf64_Word  p_type;
    Elf64_Word  p_flags;
    Elf64_Off   p_offset;
    Elf64_Addr  p_vaddr;
    Elf64_Addr  p_paddr;
    Elf64_Xword p_filesz;
    Elf64_Xword p_memsz;
    Elf64_Xword p_align;
} Elf64_Phdr;

#define PT_NULL     0
#define PT_LOAD     1
#define PT_DYNAMIC  2
#define PT_INTERP   3
#define PT_NOTE     4
#define PT_SHLIB    5
#define PT_PHDR     6
#define PT_TLS      7
#define PT_LOOS     0x60000000
#define PT_HIOS     0x6fffffff
#define PT_LOPROC   0x70000000
#define PT_HIPROC   0x7fffffff
/* more constants defined in Linux: */
#define PT_GNU_EH_FRAME (PT_LOOS + 0x0474e550)
#define PT_GNU_STACK    (PT_LOOS + 0x0474e551)
#define PT_GNU_RELRO    (PT_LOOS + 0x0474e552)
#define PT_GNU_PROPERTY (PT_LOOS + 0x0474e553)

#define PT_AARCH64_MEMTAG_MTE   (PT_LOPROC + 0x2)

#define PN_XNUM     0xffff

#define PF_X        1
#define PF_W        2
#define PF_R        4
#define PF_MASKOS   0x0ff00000
#define PF_MASKPROC 0xf0000000

typedef struct {
	Elf32_Sword	d_tag;
   	union {
   		Elf32_Word	d_val;
   		Elf32_Addr	d_ptr;
	} d_un;
} Elf32_Dyn;

typedef struct {
	Elf64_Sxword	d_tag;
   	union {
   		Elf64_Xword	d_val;
   		Elf64_Addr	d_ptr;
	} d_un;
} Elf64_Dyn;

#define DT_NULL     0
#define DT_NEEDED   1
#define DT_PLTRELSZ     2
#define DT_PLTGOT   3
#define DT_HASH     4
#define DT_STRTAB   5
#define DT_SYMTAB   6
#define DT_RELA     7
#define DT_RELASZ   8
#define DT_RELAENT  9
#define DT_STRSZ    10
#define DT_SYMENT   11
#define DT_INIT     12
#define DT_FINI     13
#define DT_SONAME   14
#define DT_RPATH    15
#define DT_SYMBOLIC     16
#define DT_REL  17
#define DT_RELSZ    18
#define DT_RELENT   19
#define DT_PLTREL   20
#define DT_DEBUG    21
#define DT_TEXTREL  22
#define DT_JMPREL   23
#define DT_BIND_NOW     24
#define DT_INIT_ARRAY   25
#define DT_FINI_ARRAY   26
#define DT_INIT_ARRAYSZ     27
#define DT_FINI_ARRAYSZ     28
#define DT_RUNPATH  29
#define DT_FLAGS    30
#define DT_ENCODING     32
#define DT_PREINIT_ARRAY    32
#define DT_PREINIT_ARRAYSZ  33
#define DT_SYMTAB_SHNDX     34
#define DT_RELRSZ	35
#define DT_RELR		36
#define DT_RELRENT	37
#define	DT_NUM		38
#define DT_LOOS     0x6000000d
#define DT_HIOS     0x6ffff000
#define DT_VALRNGLO 0x6ffffd00
#define DT_GNU_PRELINKED 0x6ffffdf5	/* Prelinking timestamp */
#define DT_GNU_CONFLICTSZ 0x6ffffdf6	/* Size of conflict section */
#define DT_GNU_LIBLISTSZ 0x6ffffdf7	/* Size of library list */
#define DT_CHECKSUM	0x6ffffdf8
#define DT_PLTPADSZ	0x6ffffdf9
#define DT_MOVEENT	0x6ffffdfa
#define DT_MOVESZ	0x6ffffdfb
#define DT_FEATURE_1	0x6ffffdfc	/* Feature selection (DTF_*).  */
#define DT_POSFLAG_1	0x6ffffdfd	/* Flags for DT_* entries, effecting
					   the following DT_* entry.  */
#define DT_SYMINSZ	0x6ffffdfe	/* Size of syminfo table (in bytes) */
#define DT_SYMINENT	0x6ffffdff	/* Entry size of syminfo */
#define DT_VALRNGHI 0x6ffffdff
#define DT_ADDRRNGLO    0x6ffffe00
#define DT_GNU_HASH	0x6ffffef5	/* GNU-style hash table.  */
#define DT_TLSDESC_PLT	0x6ffffef6
#define DT_TLSDESC_GOT	0x6ffffef7
#define DT_GNU_CONFLICT	0x6ffffef8	/* Start of conflict section */
#define DT_GNU_LIBLIST	0x6ffffef9	/* Library list */
#define DT_CONFIG	0x6ffffefa	/* Configuration information.  */
#define DT_DEPAUDIT	0x6ffffefb	/* Dependency auditing.  */
#define DT_AUDIT	0x6ffffefc	/* Object auditing.  */
#define	DT_PLTPAD	0x6ffffefd	/* PLT padding.  */
#define	DT_MOVETAB	0x6ffffefe	/* Move table.  */
#define DT_SYMINFO	0x6ffffeff	/* Syminfo table.  */
#define DT_ADDRRNGHI    0x6ffffeff
#define DT_VERSYM   0x6ffffff0
#define DT_RELACOUNT    0x6ffffff9
#define DT_RELCOUNT 0x6ffffffa
#define DT_FLAGS_1  0x6ffffffb
#define DT_VERDEF   0x6ffffffc
#define DT_VERDEFNUM    0x6ffffffd
#define DT_VERNEED  0x6ffffffe
#define DT_VERNEEDNUM   0x6fffffff
#define OLD_DT_HIOS     0x6fffffff
#define DT_LOPROC   0x70000000
#define DT_HIPROC   0x7fffffff

#define DT_X86_64_PLT       0x70000000
#define DT_X86_64_PLTSZ     0x70000001
#define DT_X86_64_PLTENT    0x70000002

#define DF_ORIGIN	0x00000001	/* Object may use DF_ORIGIN */
#define DF_SYMBOLIC	0x00000002	/* Symbol resolutions starts here */
#define DF_TEXTREL	0x00000004	/* Object contains text relocations */
#define DF_BIND_NOW	0x00000008	/* No lazy binding for this object */
#define DF_STATIC_TLS	0x00000010	/* Module uses the static TLS model */

#define DF_1_NOW	0x00000001	/* Set RTLD_NOW for this object.  */
#define DF_1_GLOBAL	0x00000002	/* Set RTLD_GLOBAL for this object.  */
#define DF_1_GROUP	0x00000004	/* Set RTLD_GROUP for this object.  */
#define DF_1_NODELETE	0x00000008	/* Set RTLD_NODELETE for this object.*/
#define DF_1_LOADFLTR	0x00000010	/* Trigger filtee loading at runtime.*/
#define DF_1_INITFIRST	0x00000020	/* Set RTLD_INITFIRST for this object*/
#define DF_1_NOOPEN	0x00000040	/* Set RTLD_NOOPEN for this object.  */
#define DF_1_ORIGIN	0x00000080	/* $ORIGIN must be handled.  */
#define DF_1_DIRECT	0x00000100	/* Direct binding enabled.  */
#define DF_1_TRANS	0x00000200
#define DF_1_INTERPOSE	0x00000400	/* Object is used to interpose.  */
#define DF_1_NODEFLIB	0x00000800	/* Ignore default lib search path.  */
#define DF_1_NODUMP	0x00001000	/* Object can't be dldump'ed.  */
#define DF_1_CONFALT	0x00002000	/* Configuration alternative created.*/
#define DF_1_ENDFILTEE	0x00004000	/* Filtee terminates filters search. */
#define	DF_1_DISPRELDNE	0x00008000	/* Disp reloc applied at build time. */
#define	DF_1_DISPRELPND	0x00010000	/* Disp reloc applied at run-time.  */
#define	DF_1_NODIRECT	0x00020000	/* Object has no-direct binding. */
#define	DF_1_IGNMULDEF	0x00040000
#define	DF_1_NOKSYMS	0x00080000
#define	DF_1_NOHDR	0x00100000
#define	DF_1_EDITED	0x00200000	/* Object is modified after built.  */
#define	DF_1_NORELOC	0x00400000
#define	DF_1_SYMINTPOSE	0x00800000	/* Object has individual interposers.  */
#define	DF_1_GLOBAUDIT	0x01000000	/* Global auditing required.  */
#define	DF_1_SINGLETON	0x02000000	/* Singleton symbols are used.  */
#define	DF_1_STUB	0x04000000
#define	DF_1_PIE	0x08000000
#define	DF_1_KMOD       0x10000000
#define	DF_1_WEAKFILTER 0x20000000
#define	DF_1_NOCOMMON   0x40000000

#define DTF_1_PARINIT	0x00000001
#define DTF_1_CONFEXP	0x00000002

#define DF_P1_LAZYLOAD	0x00000001	/* Lazyload following object.  */
#define DF_P1_GROUPPERM	0x00000002	/* Symbols from next object are not generally available.  */
/* ELF note types */
#define NT_PRSTATUS	1		/* Contains copy of prstatus struct */
#define NT_PRFPREG	2		/* Contains copy of fpregset struct */
#define NT_FPREGSET	2		/* Contains copy of fpregset struct */
#define NT_PRPSINFO	3		/* Contains copy of prpsinfo struct */
#define NT_PRXREG	4		/* Contains copy of prxregset struct */
#define NT_TASKSTRUCT	4		/* Contains copy of task structure */
#define NT_PLATFORM	5		/* String from sysinfo(SI_PLATFORM) */
#define NT_AUXV		6		/* Contains copy of auxv array */
#define NT_GWINDOWS	7		/* Contains copy of gwindows struct */
#define NT_ASRS		8		/* Contains copy of asrset struct */
#define NT_PSTATUS	10		/* Contains copy of pstatus struct */
#define NT_PSINFO	13		/* Contains copy of psinfo struct */
#define NT_PRCRED	14		/* Contains copy of prcred struct */
#define NT_UTSNAME	15		/* Contains copy of utsname struct */
#define NT_LWPSTATUS	16		/* Contains copy of lwpstatus struct */
#define NT_LWPSINFO	17		/* Contains copy of lwpinfo struct */
#define NT_PRFPXREG	20		/* Contains copy of fprxregset struct */
#define NT_SIGINFO	0x53494749	/* Contains copy of siginfo_t,
					   size might increase */
#define NT_FILE		0x46494c45	/* Contains information about mapped
					   files */
#define NT_PRXFPREG	0x46e62b7f	/* Contains copy of user_fxsr_struct */
#define NT_PPC_VMX	0x100		/* PowerPC Altivec/VMX registers */
#define NT_PPC_SPE	0x101		/* PowerPC SPE/EVR registers */
#define NT_PPC_VSX	0x102		/* PowerPC VSX registers */
#define NT_PPC_TAR	0x103		/* Target Address Register */
#define NT_PPC_PPR	0x104		/* Program Priority Register */
#define NT_PPC_DSCR	0x105		/* Data Stream Control Register */
#define NT_PPC_EBB	0x106		/* Event Based Branch Registers */
#define NT_PPC_PMU	0x107		/* Performance Monitor Registers */
#define NT_PPC_TM_CGPR	0x108		/* TM checkpointed GPR Registers */
#define NT_PPC_TM_CFPR	0x109		/* TM checkpointed FPR Registers */
#define NT_PPC_TM_CVMX	0x10a		/* TM checkpointed VMX Registers */
#define NT_PPC_TM_CVSX	0x10b		/* TM checkpointed VSX Registers */
#define NT_PPC_TM_SPR	0x10c		/* TM Special Purpose Registers */
#define NT_PPC_TM_CTAR	0x10d		/* TM checkpointed Target Address
					   Register */
#define NT_PPC_TM_CPPR	0x10e		/* TM checkpointed Program Priority
					   Register */
#define NT_PPC_TM_CDSCR	0x10f		/* TM checkpointed Data Stream Control
					   Register */
#define NT_PPC_PKEY	0x110		/* Memory Protection Keys
					   registers.  */
#define NT_386_TLS	0x200		/* i386 TLS slots (struct user_desc) */
#define NT_386_IOPERM	0x201		/* x86 io permission bitmap (1=deny) */
#define NT_X86_XSTATE	0x202		/* x86 extended state using xsave */
#define NT_S390_HIGH_GPRS	0x300	/* s390 upper register halves */
#define NT_S390_TIMER	0x301		/* s390 timer register */
#define NT_S390_TODCMP	0x302		/* s390 TOD clock comparator register */
#define NT_S390_TODPREG	0x303		/* s390 TOD programmable register */
#define NT_S390_CTRS	0x304		/* s390 control registers */
#define NT_S390_PREFIX	0x305		/* s390 prefix register */
#define NT_S390_LAST_BREAK	0x306	/* s390 breaking event address */
#define NT_S390_SYSTEM_CALL	0x307	/* s390 system call restart data */
#define NT_S390_TDB	0x308		/* s390 transaction diagnostic block */
#define NT_S390_VXRS_LOW	0x309	/* s390 vector registers 0-15
					   upper half.  */
#define NT_S390_VXRS_HIGH	0x30a	/* s390 vector registers 16-31.  */
#define NT_S390_GS_CB	0x30b		/* s390 guarded storage registers.  */
#define NT_S390_GS_BC	0x30c		/* s390 guarded storage
					   broadcast control block.  */
#define NT_S390_RI_CB	0x30d		/* s390 runtime instrumentation.  */
#define NT_ARM_VFP	0x400		/* ARM VFP/NEON registers */
#define NT_ARM_TLS	0x401		/* ARM TLS register */
#define NT_ARM_HW_BREAK	0x402		/* ARM hardware breakpoint registers */
#define NT_ARM_HW_WATCH	0x403		/* ARM hardware watchpoint registers */
#define NT_ARM_SYSTEM_CALL	0x404	/* ARM system call number */
#define NT_ARM_SVE	0x405		/* ARM Scalable Vector Extension
					   registers */
#define NT_ARM_PAC_MASK	0x406		/* ARM pointer authentication
					   code masks.  */
#define NT_ARM_PACA_KEYS	0x407	/* ARM pointer authentication
					   address keys.  */
#define NT_ARM_PACG_KEYS	0x408	/* ARM pointer authentication
					   generic key.  */
#define NT_ARM_TAGGED_ADDR_CTRL	0x409	/* AArch64 tagged address
					   control.  */
#define NT_ARM_PAC_ENABLED_KEYS	0x40a	/* AArch64 pointer authentication
					   enabled keys.  */
#define NT_VMCOREDD	0x700		/* Vmcore Device Dump Note.  */
#define NT_MIPS_DSP	0x800		/* MIPS DSP ASE registers.  */
#define NT_MIPS_FP_MODE	0x801		/* MIPS floating-point mode.  */
#define NT_MIPS_MSA	0x802		/* MIPS SIMD registers.  */
typedef struct {
    Elf32_Word n_namesz;
    Elf32_Word n_descsz;
    Elf32_Word n_type;
} Elf32_Nhdr, Elf64_Nhdr;

#define GNU_PROPERTY_STACK_SIZE             1
#define GNU_PROPERTY_NO_COPY_ON_PROTECTED   2
#define GNU_PROPERTY_UINT32_AND_LO          0xb0000000
#define GNU_PROPERTY_UINT32_AND_HI          0xb0007fff
#define GNU_PROPERTY_UINT32_OR_LO           0xb0008000
#define GNU_PROPERTY_UINT32_OR_HI           0xb000ffff
#define GNU_PROPERTY_LOPROC                 0xc0000000
#define GNU_PROPERTY_HIPROC                 0xdfffffff
#define GNU_PROPERTY_LOUSER                 0xe0000000
#define GNU_PROPERTY_HIUSER                 0xffffffff

#define GNU_PROPERTY_1_NEEDED               0xb0008000
#define GNU_PROPERTY_AARCH64_FEATURE_1_AND	0xc0000000
#define GNU_PROPERTY_AARCH64_FEATURE_1_BTI	(1U << 0)
#define GNU_PROPERTY_X86_UINT32_AND_LO      0xc0000002
#define GNU_PROPERTY_X86_UINT32_AND_HI      0xc0007fff
#define GNU_PROPERTY_X86_UINT32_OR_LO       0xc0008000
#define GNU_PROPERTY_X86_UINT32_OR_HI       0xc000ffff
#define GNU_PROPERTY_X86_UINT32_OR_AND_LO   0xc0010000
#define GNU_PROPERTY_X86_UINT32_OR_AND_HI   0xc0017fff
#define GNU_PROPERTY_X86_FEATURE_1_AND      0xc0000002
#define GNU_PROPERTY_X86_FEATURE_2_USED     0xc0010001
#define GNU_PROPERTY_X86_FEATURE_2_NEEDED   0xc0008001
#define GNU_PROPERTY_X86_ISA_1_USED         0xc0010002
#define GNU_PROPERTY_X86_ISA_1_NEEDED       0xc0008002
#define GNU_PROPERTY_X86_FEATURE_1_IBT      (1U << 0)
#define GNU_PROPERTY_X86_FEATURE_1_SHSTK    (1U << 1)
#define GNU_PROPERTY_X86_FEATURE_2_X86      (1U << 0)
#define GNU_PROPERTY_X86_FEATURE_2_X87      (1U << 1)
#define GNU_PROPERTY_X86_FEATURE_2_MMX      (1U << 2)
#define GNU_PROPERTY_X86_FEATURE_2_XMM      (1U << 3)
#define GNU_PROPERTY_X86_FEATURE_2_YMM      (1U << 4)
#define GNU_PROPERTY_X86_FEATURE_2_ZMM      (1U << 5)
#define GNU_PROPERTY_X86_FEATURE_2_FXSR     (1U << 6)
#define GNU_PROPERTY_X86_FEATURE_2_XSAVE    (1U << 7)
#define GNU_PROPERTY_X86_FEATURE_2_XSAVEOPT (1U << 8)
#define GNU_PROPERTY_X86_FEATURE_2_XSAVEC   (1U << 9)
#define GNU_PROPERTY_X86_FEATURE_2_TMM      (1U << 10)
#define GNU_PROPERTY_X86_FEATURE_2_MASK     (1U << 11)
#define GNU_PROPERTY_X86_ISA_1_BASELINE     (1U << 0)
#define GNU_PROPERTY_X86_ISA_1_V2           (1U << 1)
#define GNU_PROPERTY_X86_ISA_1_V3           (1U << 2)
#define GNU_PROPERTY_X86_ISA_1_V4           (1U << 3)

typedef struct {
    Elf32_Word a_type;
    union {
        Elf32_Word a_val;
        void *a_ptr;
        void (*a_fnc)();
    } a_un;
} Elf32_Ahdr;

typedef struct {
    Elf64_Xword a_type;
    union {
        Elf64_Xword a_val;
        void *a_ptr;
        void (*a_fnc)();
    } a_un;
} Elf64_Ahdr;

#define AT_NULL   0
#define AT_IGNORE 1
#define AT_EXECFD 2
#define AT_PHDR   3
#define AT_PHENT  4
#define AT_PHNUM  5
#define AT_PAGESZ 6
#define AT_BASE   7
#define AT_FLAGS  8
#define AT_ENTRY  9
#define AT_NOTELF 10
#define AT_UID    11
#define AT_EUID   12
#define AT_GID    13
#define AT_EGID   14
#define AT_PLATFORM 15
#define AT_HWCAP  16
#define AT_CLKTCK 17
#define AT_FPUCW		18
#define AT_DCACHEBSIZE		19
#define AT_ICACHEBSIZE		20
#define AT_UCACHEBSIZE		21
#define AT_IGNOREPPC		22
#define AT_SECURE 23
#define AT_BASE_PLATFORM 24
#define AT_RANDOM 25
#define AT_HWCAP2 26
#define AT_RSEQ_FEATURE_SIZE	27
#define AT_RSEQ_ALIGN		28
#define AT_HWCAP3 29
#define AT_HWCAP4 30
#define AT_EXECFN  31
#define AT_SYSINFO      32
#define AT_SYSINFO_EHDR	33
#define AT_L1I_CACHESHAPE	34
#define AT_L1D_CACHESHAPE	35
#define AT_L2_CACHESHAPE	36
#define AT_L3_CACHESHAPE	37
#define AT_L1I_CACHESIZE	40
#define AT_L1I_CACHEGEOMETRY	41
#define AT_L1D_CACHESIZE	42
#define AT_L1D_CACHEGEOMETRY	43
#define AT_L2_CACHESIZE		44
#define AT_L2_CACHEGEOMETRY	45
#define AT_L3_CACHESIZE		46
#define AT_L3_CACHEGEOMETRY	47
#define AT_ADI_BLKSZ	48
#define AT_ADI_NBITS	49
#define AT_ADI_UEONADI	50
#define AT_MINSIGSTKSZ	51	/* stack needed for signal delivery */

typedef struct {
    Elf32_Half    vd_version;
    Elf32_Half    vd_flags;
    Elf32_Half    vd_ndx;
    Elf32_Half    vd_cnt;
    Elf32_Word    vd_hash;
    Elf32_Word    vd_aux;
    Elf32_Word    vd_next;
} Elf32_Verdef, Elf64_Verdef;

typedef struct {
    Elf32_Word    vda_name;
    Elf32_Word    vda_next;
} Elf32_Verdaux, Elf64_Verdaux;

typedef struct {
    Elf32_Half    vn_version;
    Elf32_Half    vn_cnt;
    Elf32_Word    vn_file;
    Elf32_Word    vn_aux;
    Elf32_Word    vn_next;
} Elf32_Verneed;

typedef struct {
    Elf32_Word    vna_hash;
    Elf32_Half    vna_flags;
    Elf32_Half    vna_other;
    Elf32_Word    vna_name;
    Elf32_Word    vna_next;
} Elf32_Vernaux, Elf64_Vernaux;

#define VER_DEF_NONE	0		/* No version */
#define VER_DEF_CURRENT	1		/* Current version */
#define VER_DEF_NUM	2		/* Given version number */

#define VER_FLG_BASE	0x1		/* Version definition of file itself */
#define VER_FLG_WEAK	0x2		/* Weak version identifier */

#define	VER_NDX_LOCAL		0	/* Symbol is local.  */
#define	VER_NDX_GLOBAL		1	/* Symbol is global.  */
#define	VER_NDX_LORESERVE	0xff00	/* Beginning of reserved entries.  */
#define	VER_NDX_ELIMINATE	0xff01	/* Symbol is to be eliminated.  */

#define VER_NEED_NONE	 0		/* No version */
#define VER_NEED_CURRENT 1		/* Current version */
#define VER_NEED_NUM	 2		/* Given version number */

#endif
