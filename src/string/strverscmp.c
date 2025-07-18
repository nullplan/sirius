#define _GNU_SOURCE
#include <string.h>
#include <ctype.h>

int strverscmp(const char *a, const char *b)
{
    /* If I understand the spec correctly, I can make do with the following implementation:
     *
     * 1. Find size of equal prefix (length of segment that's the same between both strings).
     * 2. If equal prefix equals string length, the strings are equal and we return 0.
     * 3. If either string has no digits at or after the point of difference, return byte value difference.
     * 4. While the equal prefix is nonempty and the last character is a digit, reduce equal prefix.
     * 5. Find size of only-'0' segment from that point forward.
     * 6. If they are differently sized, *longer* string is smaller.
     * 7. Else skip the zeroes, then parse the left over numbers and return the numerical difference.
     */
    size_t ep = 0;
    for (; a[ep] && a[ep] == b[ep]; ep++);
    if (((ep == 0 || !isdigit(a[ep-1])) && (!isdigit(a[ep]) || !isdigit(b[ep]))))
        return (unsigned char)a[ep] - (unsigned char)b[ep];

    size_t numdig = 0;
    for (; numdig < ep && isdigit(a[ep-numdig]); numdig++);

    size_t nza, nzb;
    for (nza = 0; a[ep-numdig+nza] == '0'; nza++);
    for (nzb = 0; a[ep-numdig+nzb] == '0'; nzb++);
    if (nza > nzb) return -1;
    if (nzb > nza) return 1;

    /* actually parsing the number is overkill. Having taken care of leading zeroes,
     * we know that the longer digit string must be the larger number.
     * And in case of equality, byte value difference wins.
     */
    size_t nda, ndb;
    for (nda = 0; isdigit(a[ep+nda]); nda++);
    for (ndb = 0; isdigit(b[ep+ndb]); ndb++);
    if (nda < ndb) return -1;
    if (ndb < nda) return 1;
    return (unsigned char)a[ep] - (unsigned char)b[ep];
}
