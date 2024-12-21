s/^TYPEDEF \(.*\) \([_a-zA-z][_a-zA-Z0-9]*\);/#if defined __NEED_\2 \&\& !defined __DEF_\2\
#define __DEF_\2\
typedef \1 \2;\
#endif/
s/^STRUCT \([_a-zA-z][_a-zA-Z0-9]*\) \(.*\);/#if defined __NEED_struct_\1 \&\& !defined __DEF_struct_\1\
#define __DEF_struct_\1\
struct \1 \2;\
#endif/

