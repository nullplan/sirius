/* got to store r4-r11, r13 (sp), r14 (lr), makes 10 GPRs. */
/* also have to preserve d8-d15 with VFP, makes 8 double-words */
/* so in total 13 double-words */
typedef long long jmp_buf[25];
