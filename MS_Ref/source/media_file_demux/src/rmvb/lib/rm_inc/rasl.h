

#ifndef RASL_H
#define RASL_H

#include "helix_types.h"

/* Must regenerate table if these are changed! */
#define RA50_NCODEBYTES         18.5            /* bytes per 5.0kbps sl frame */
#define RA65_NCODEBYTES         14.5            /* bytes per 6.5kbps sl frame */
#define RA85_NCODEBYTES         19              /* bytes per 8.5kbps sl frame */
#define RA160_NCODEBYTES        20              /* bytes per 16.0kbps sl frame */

#define RA50_NCODEBITS          148             /* bits per 5.0kbps sl frame */
#define RA65_NCODEBITS          116             /* bits per 6.5kbps sl frame */
#define RA85_NCODEBITS          152             /* bits per 8.5kbps sl frame */
#define RA160_NCODEBITS         160             /* bits per 16.0kbps sl frame */

#define RASL_MAXCODEBYTES       20              /* max bytes per  sl frame */
#define RASL_NFRAMES            16             /* frames per block - must match codec */
#define RASL_NBLOCKS            6              /* blocks to interleave across */
#define RASL_BLOCK_NUM(i) ((i) >> 4)    /* assumes RASL_NFRAMES = 16 */
#define RASL_BLOCK_OFF(i) ((i) & 0xf)   /* assumes RASL_NFRAMES = 16 */


void RASL_DeInterleave(char *buf, unsigned long ulBufSize, int type, ULONG32 * pFlags);
                                                                                 
void ra_bitcopy(unsigned char* toPtr,
                unsigned long  ulToBufSize,
                unsigned char* fromPtr,
                unsigned long  ulFromBufSize,
                int            bitOffsetTo,
                int            bitOffsetFrom,
                int            numBits);

#endif /* #ifndef RASL_H */
