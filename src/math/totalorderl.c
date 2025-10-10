#define __STDC_WANT_IEC_60559_EXT__
#include "libm.h"
#include <string.h>
#include <stdbit.h>

#ifdef __GNUC__
#define memcpy __builtin_memcpy
#endif

int    totalorderl(const long double *pa, const long double *pb)
{
    #if LDBL_MANT_DIG==53
    return totalorder((const double *)pa, (const double *)pb);
    #elif LDBL_MANT_DIG==64
    union ldshape lda = {*pa}, ldb = {*pb};
    int16_t ixa = lda.i.se;
    int16_t ixb = ldb.i.se;
    int c = ixa < ixb? -1 : ixa > ixb? 1 : lda.i.mant < ldb.i.mant? -1 : lda.i.mant > ldb.i.mant? 1 : 0;
    if (ixa < 0 && ixb < 0) c = -c;
    return c <= 0;
    #else
    int64_t ha, hb;
    uint64_t la, lb;

    memcpy(&ha, (const char *)pa + (__STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_BIG__) * 8, 8);
    memcpy(&la, (const char *)pa + (__STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__) * 8, 8);
    memcpy(&hb, (const char *)pb + (__STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_BIG__) * 8, 8);
    memcpy(&lb, (const char *)pb + (__STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__) * 8, 8);

    int c = ha < hb? -1 : ha > hb? 1 : la < lb? -1 : la > lb? 1: 0;
    if (ha < 0 && hb < 0) c = -c;
    return c <= 0;
    #endif
}
