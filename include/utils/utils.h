#ifndef __UTILS__H
#define __UTILS__H
#include "enums.h"

namespace Utils
{
    // Note: If you remove inline keyword, you will have to separate the header and the implementation file
    //       Otherwise you will get a linker error.
    inline bool isLogical(Operator op)
    {
        return (op >= OPR_AND && op <= OPR_NOT_EQUAL);
    }
}

#endif // __UTILS__H