#include <stdint.h>
#include <sys/types.h>

#ifndef REQUIRES
#define REQUIRES

/*** Structs ***/

struct SUBNET {
    unsigned int ip: 32;
    unsigned int mask: 32;
};

#endif
