#ifndef ENUMS
#define ENUMS

enum DataType
{
    DTYPE_VOID = 100,
    DTYPE_BOOL,
    DTYPE_CHAR,
    DTYPE_INT,
    DTYPE_FLOAT
};

enum EntryType
{
    TYPE_VAR = 200,
    TYPE_CONST,
    TYPE_FUNC,
};

enum Operator
{
    OPR_ASSIGN = 300,  // a = b
    OPR_ADD,           // a + b
    OPR_SUB,           // a - b
    OPR_MUL,           // a * b
    OPR_DIV,           // a / b
    OPR_SHL,           // a << 1
    OPR_SHR,           // a >> 1
    OPR_AND,           // a && b
    OPR_OR,            // a || b
    OPR_NOT,           // !a
    OPR_GREATER,       // a > b
    OPR_GREATER_EQUAL, // a >= b
    OPR_LESS,          // a < b
    OPR_LESS_EQUAL,    // a <= b
    OPR_EQUAL,         // a == b
    OPR_NOT_EQUAL,     // a != b
};

#endif