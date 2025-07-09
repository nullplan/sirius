/* glibc rules: the first one that matches wins:
 *      - U+00AD has width 1
 *      - U+115F has width 2
 *      - All Hangul syllables with the second column being V or T have with 0
 *      - All non-spacing and enclsing marks (categories Mn and Me from UnicodeData.txt) have with 0
 *      - All Default_Ignorable_Code_Points have width 0
 *      - All double-width characters from EastAsianWidth.txt (type W or F) have width 2
 *      - All others have width 1.
 *
 * That's what they write, but also, they define that wcwidth(0) == 0 and !iswprint(wc) => wcwidth(wc) = -1
 */
/* musl rules:
 *      - the null byte has width 0
 *      - bytes from the C0 and C1 ranges have width -1
 *      - all other characters from ISO-8859-1 have width 1.
 *      - planes 0 and 1 contain both non-spacing and double width characters and are treated accordingly (constant sets).
 *      - code point xxFFFE in all planes has width -1
 *      - everything from planes 2 and 3 is double width
 *      - code points E0001, E0020..E007F, and E0100..E01EF have width 0
 *      - all others have width 1.
 */

/* Per POSIX: "The application shall ensure that wc is a valid character". Therefore, non-character wc is undefined.
 * I suppose that means at least iswcntrl(wc) || iswprint(wc) is a given.
 *
 * Therefore the FFFE codepoints are not valid inputs.
 *
 * I'm otherwise going with the glibc ruleset.
 */

#include <wchar.h>
#include <wctype.h>

static const unsigned char zerowidth[] = {
#include "zerowidth.h"
};

static const unsigned char doublewidth[] = {
#include "doublewidth.h"
};

int wcwidth(wchar_t wc)
{
    if (!wc) return 0;
    if (iswcntrl(wc)) return -1;
    if (__is_in_set(wc, zerowidth, sizeof zerowidth)) return 0;
    if (__is_in_set(wc, doublewidth, sizeof doublewidth)) return 2;
    return 1;
}
