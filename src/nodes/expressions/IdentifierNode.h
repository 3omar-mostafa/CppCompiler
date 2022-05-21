#ifndef IDENTIFIER_NODE
#define IDENTIFIER_NODE
#include "../Node.h"

class IdentifierNode : public ExpressionNode
{
    string name;

public:
    IdentifierNode(yy::location loc, const string &name) : ExpressionNode(loc)
    {
        this->name = name;
    }

    bool analyzeSemantic() override
    {
        SymbolTable *table = SymbolTable::getInstance();
        EntryInfo info;
        if (!table->lookup(name, info))
            return false;
        type = info.type;
        entryType = info.entryType;
        return true;
    }

    // TODO:Implement
    string generateCode() override
    {
    }
};
#endif