#ifndef __UTILS__H
#define __UTILS__H
#include "enums.h"
#include <string>

namespace Utils
{
    // Note: If you remove inline keyword, you will have to separate the header and the implementation file
    //       Otherwise you will get a linker error.
    inline bool isLogical(Operator op)
    {
        return (op >= OPR_AND && op <= OPR_NOT_EQUAL);
    }

    inline std::string TypeToQuad(DataType type)
    {
        switch (type)
        {
        case DTYPE_VOID:
            return "VOID";
        case DTYPE_BOOL:
            return "BOOL";
        case DTYPE_CHAR:
            return "CHAR";
        case DTYPE_INT:
            return "INT";
        case DTYPE_FLOAT:
            return "FLOAT";
        default:
            return "";
        }
    }

    inline std::string opToQuad(Operator op, DataType type)
    {
        switch (op)
        {
        case OPR_ADD:
            return "ADD " + TypeToQuad(type);
        case OPR_SUB:
            return "SUB " + TypeToQuad(type);
        case OPR_MUL:
            return "MUL " + TypeToQuad(type);
        case OPR_DIV:
            return "DIV " + TypeToQuad(type);
        case OPR_POW:
            return "POW " + TypeToQuad(type);
        case OPR_SHL:
            return "SHL " + TypeToQuad(type);
        case OPR_SHR:
            return "SHR " + TypeToQuad(type);
        case OPR_AND:
            return "AND " + TypeToQuad(type);
        case OPR_OR:
            return "OR " + TypeToQuad(type);
        case OPR_NOT:
            return "NOT " + TypeToQuad(type);
        case OPR_GREATER:
            return "GT " + TypeToQuad(type);
        case OPR_GREATER_EQUAL:
            return "GE " + TypeToQuad(type);
        case OPR_LESS:
            return "LT " + TypeToQuad(type);
        case OPR_LESS_EQUAL:
            return "LE " + TypeToQuad(type);
        case OPR_EQUAL:
            return "EQ " + TypeToQuad(type);
        case OPR_NOT_EQUAL:
            return "NEQ " + TypeToQuad(type);
        case OPR_PUSH:
            return "PUSH " + TypeToQuad(type);
        case OPR_POP:
            return "POP " + TypeToQuad(type);
        case OPR_RET:
            return "RET";
        case OPR_JMP:
            return "JMP";
        case OPR_JMPEQ:
            return "JMPEQ";
        case OPR_JMPNEQ:
            return "JMPNEQ";
        default:
            return "";
        }
    }

    inline std::string convTypeToQuad(DataType in_type, DataType out_type)
    {
        return (in_type != out_type ? ("CONV " + TypeToQuad(in_type) + " TO " + TypeToQuad(out_type) + "\n") : "");
    }
}

#endif // __UTILS__H