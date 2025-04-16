#include <netdb.h>

hidden void __dns_process(const unsigned char *result, size_t reslen, int (*cb)(const unsigned char *, size_t, const unsigned char *, size_t, int, void *), void *ctx)
{
    /* for now, simply reject nonzero rcodes */
    if (reslen < 12) return;
    if (result[3] & 15) return;
    int qdcount = result[4] << 8 | result[5];
    int ancount = result[6] << 8 | result[7];
    int nscount = result[8] << 8 | result[9];
    int arcount = result[10] << 8 | result[11];
    /* I only send packets with a single question, so that's what the result must contain. */
    if (qdcount != 1) return;
    /* every query takes at least 5 bytes, and every other RR takes at least 9 */
    /* note: we may relax this to allow processing of complete RRs in truncated packets. */
    if (qdcount * 5 + (ancount + nscount + arcount) * 9 + 12 > reslen) return;
    const unsigned char *p = result + 12;
    const unsigned char *end = result + reslen;

    /* skip question, abort on error */
    /* XXX: djbdns verifies that the question name matches the query, and the result RRs. Should I do the same? */
    while (*p && *p < 63)
    {
        size_t labellen = *p;
        if (end - p - 2 < labellen) return;
        p += labellen + 1;
    }
    if (*p) p++;
    p++;
    if (end - p < 4) return;
    p += 4;

    int count = ancount + nscount + arcount; /* just lumping them all together */
    while (count--)
    {
        const unsigned char *q = p;
        while (*q && *q < 63) {
            size_t labellen = *q;
            if (end - q - 2 < labellen) return;
            q += labellen + 1;
        }
        if (*q) q++;
        q++;
        if (end - q < 10) return;
        int type = q[0] << 8 | q[1];
        int rdlength = q[8] << 8 | q[9];
        if (end - q - 10 < rdlength) return;
        if (cb(q + 10, rdlength, result, reslen, type, ctx)) return;
        p = q + 10 + rdlength;
        if (p == end) return;
    }
}
