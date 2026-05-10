#ifndef TLS_H
#define TLS_H

#include <stddef.h>
#include <features.h>

struct tls_module {
    size_t off;
    size_t size;
    size_t align;
    void *image;
    size_t len;
    struct tls_module *next;
};

hidden void __add_tls(struct tls_module *);
struct tls_data {
    size_t size;
    size_t align;
};
hidden struct tls_data __get_tls_data(void);
/* programming model: call __copy_tls() with zeroed out memory, with size and alignment given in the TLS data,
 * and it returns the TD placed correctly and TLS and DTV (and __pthread::dtv) filled out correctly
 * However, TSD is not part of this and might need to be allocated separately.
 */
struct __pthread;
hidden struct __pthread *__copy_tls(void *, size_t);

hidden size_t __tls_cnt(void);

struct tls_state {
    size_t raw_size;
    size_t raw_align;
    size_t tls_cnt;
    struct tls_module *tail;
};

hidden void __tls_save_state(struct tls_state *);
hidden void __tls_restore_state(const struct tls_state *);
#endif
