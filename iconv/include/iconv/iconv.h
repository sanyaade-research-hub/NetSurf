#ifndef _LIB_ICONV_H
#define _LIB_ICONV_H

#ifndef EILSEQ
#define EILSEQ ENOENT
#endif

#undef iconv_t
typedef void *iconv_t;

/*
 * Initialise the iconv library
 */
int iconv_initialise(const char *aliases_file);

/*
 * Finalise the iconv library
 */
void iconv_finalise(void);

/*
 * Allocate a conversion descriptor suitable for converting byte sequences
 * from encoding from code to encoding tocode.
 * The resulting conversion descriptor may be used any number of times and
 * remains valid until deallocated with iconv_close.
 * A conversion descriptor contains a conversion state. After creation,
 * the state is in the initial state. Using iconv modifies the descriptor's
 * conversion state. The state may be reset by calling iconv with NULL
 * as the inbuf argument.
 *
 * Returns the conversion descriptor on success. On error, (iconv_t)(-1) is
 * returned and errno is set. If errno is set to EINVAL, the implementation
 * does not provide support for conversion between fromcode and tocode.
 */
extern iconv_t iconv_open(const char *tocode, const char *fromcode);

/* Perform character set conversion
 * cd must be a conversion descriptor as allocated by iconv_open.
 *
 * If inbuf is not NULL and *inbuf is not NULL, the multibyte sequence
 * starting at *inbuf is converted into a multibyte sequence starting at
 * *outbuf. At most *inbytesleft bytes, starting at *inbuf, will be read.
 * At most *outbytesleft bytes, starting at *outbuf, will be written.
 *
 * One multibyte character is converted at a time.  For each conversion,
 * *inbuf is incremented and *inbytesleft is decremented by the number of
 * converted input bytes. Similarly, *outbuf is incremented and
 * *outbytesleft is decremented by the number of converted output bytes.
 *
 * Conversion can stop for four reasons:
 *
 * 1. An invalid multibyte input sequence is encountered. In this case,
 *    errno is set to EILSEQ and (size_t)(-1) is returned. *inbuf points
 *    to the start of the illegal sequence.
 *
 * 2. The input sequence has been entirely converted. In this case, the
 *    number of non-reversible conversions performed is returned.
 *
 * 3. An incomplete multibyte sequence is encountered in the input and
 *    the input terminates after it. In this case, errno is set to EINVAL
 *    and (size_t)(-1) is returned.
 *
 * 4. The output buffer has no room for the next converted character.
 *    In this case, errno is set to E2BIG and (size_t)(-1) is returned.
 *
 * If inbuf is NULL or *inbuf is NULL but outbuf is not NULL and *outbuf is
 * not NULL, the function attempts to set cd's conversion state to the
 * initial state and store a corresponding shift sequence in *outbuf.
 * At most *outbytesleft will be written. If the output buffer is too small
 * for this reset sequence, errno is set to E2BIG and (size_t)(-1) is
 * returned. Otherwise *outbuf is incremented and *outbytesleft is
 * decremented by the number of bytes written.
 *
 * If inbuf or *inbuf and outbuf or *outbuf are NULL, cd is reset to the
 * initial conversion state.
 */
extern size_t iconv(iconv_t cd, char **inbuf, size_t *inbytesleft,
		char **outbuf, size_t *outbytesleft);

/* Deallocate a conversion descriptor cd.
 * Returns 0 on success, -1 on error and sets errno
 */
extern int iconv_close(iconv_t cd);


#endif

