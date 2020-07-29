#ifndef _SBRK_H
#define _SBRK_H

#include <sys/types.h>
#include <errno.h>

// ----------------------------------------------------------------------------

caddr_t
_sbrk(int incr);

#endif
