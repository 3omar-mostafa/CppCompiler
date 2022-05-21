#ifndef UTILS
#define UTILS
#include "enums.h"

namespace Utils
{
    bool isLogical(Operator op)
    {
        return (op >= OPR_AND && op <= OPR_NOT_EQUAL);
    }
}
#endif