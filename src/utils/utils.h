#ifndef __UTILS__H
#define __UTILS__H
#include "enums.h"
#include <string>
#include <fstream>
#include <location.hpp>

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
        case OPR_PRE_INC:
        case OPR_POST_INC:
            return "INC " + typeToQuad(type);
        case OPR_PRE_DEC:
        case OPR_POST_DEC:
            return "DEC " + typeToQuad(type);
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
        case OPR_UNARY_MINUS:
            return "NEG " + typeToQuad(type);
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
        ret.reserve(str.size());
        for (char c: str)
        {
            ret += (c == '\t') ? std::string(4, ' ') : std::string(1, c);
        }
        return ret;
    }

    inline bool isIntegerType(DataType type)
    {
        return (type == DTYPE_BOOL || type == DTYPE_CHAR || type == DTYPE_INT);
    }

    inline std::string getLine(const std::string& filename, int line) {
        std::ifstream file(filename);
        std::string s;

        for (int i = 1; i <= line; i++)
            std::getline(file, s);

        return replaceTabsWithSpaces(s);
    }

    inline void log(const std::string &message, const yy::location &loc, const std::string &logType)
    {
        const std::string& sourceCodeLine = getLine(*loc.begin.filename, loc.begin.line);
        int endColumn = loc.end.line != loc.begin.line ? sourceCodeLine.length() : loc.end.column;

        fprintf(stderr, "%s:%d:%d: %s: %s\n", loc.begin.filename->c_str(), loc.begin.line, loc.begin.column, logType.c_str(), message.c_str());
        fprintf(stderr, "%s\n", sourceCodeLine.c_str());
        fprintf(stderr, "%*s", loc.begin.column, "^");
        fprintf(stderr, "%s", std::string(std::max(0, endColumn - loc.begin.column - 1), '~').c_str());
        fprintf(stderr, "\n");
    }

}

#endif // __UTILS__H