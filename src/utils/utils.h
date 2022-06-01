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

    inline std::string scopeToString(ScopeType scope)
    {
        switch (scope)
        {
            case SCOPE_BLOCK:
                return "Block";
            case SCOPE_FUNCTION:
                return "Function";
            case SCOPE_LOOP:
                return "Loop";
            case SCOPE_IF:
                return "If";
            case SCOPE_SWITCH:
                return "Switch";
            default:
                return "";
        }
    }

    inline std::string typeToQuad(DataType type)
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
            return "ADD " + typeToQuad(type);
        case OPR_SUB:
            return "SUB " + typeToQuad(type);
        case OPR_MUL:
            return "MUL " + typeToQuad(type);
        case OPR_DIV:
            return "DIV " + typeToQuad(type);
        case OPR_POW:
            return "POW " + typeToQuad(type);
        case OPR_SHL:
            return "SHL " + typeToQuad(type);
        case OPR_SHR:
            return "SHR " + typeToQuad(type);
        case OPR_AND:
            return "AND " + typeToQuad(type);
        case OPR_OR:
            return "OR " + typeToQuad(type);
        case OPR_NOT:
            return "NOT " + typeToQuad(type);
        case OPR_GREATER:
            return "GT " + typeToQuad(type);
        case OPR_GREATER_EQUAL:
            return "GE " + typeToQuad(type);
        case OPR_LESS:
            return "LT " + typeToQuad(type);
        case OPR_LESS_EQUAL:
            return "LE " + typeToQuad(type);
        case OPR_EQUAL:
            return "EQ " + typeToQuad(type);
        case OPR_NOT_EQUAL:
            return "NEQ " + typeToQuad(type);
        case OPR_PUSH:
            return "PUSH " + typeToQuad(type);
        case OPR_POP:
            return "POP " + typeToQuad(type);
        case OPR_RET:
            return "RET";
        case OPR_JMP:
            return "JMP";
        case OPR_JMPZ:
            return "JMPZ";
        case OPR_JMPNZ:
            return "JMPNZ";
        default:
            return "";
        }
    }

    inline std::string convTypeToQuad(DataType in_type, DataType out_type)
    {
        return (in_type != out_type ? ("CONV " + typeToQuad(in_type) + " TO " + typeToQuad(out_type) + "\n") : "");
    }

    inline std::string replaceTabsWithSpaces(const std::string &str)
    {
        std::string ret;
        for (int i = 0; i < str.size(); ++i)
        {
            if (str[i] == '\t')
            {
                ret += "    ";
            }
            else
            {
                ret += str[i];
            }
        }
        return ret;
    }

    inline bool isIntegerType(DataType type)
    {
        return (type == DTYPE_BOOL || type == DTYPE_CHAR || type == DTYPE_INT);
    }
}

#endif // __UTILS__H