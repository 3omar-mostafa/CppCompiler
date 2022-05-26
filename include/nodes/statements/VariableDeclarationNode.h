#ifndef VARIABLE_DECLARATION_NODE
#define VARIABLE_DECLARATION_NODE

#include "utils/enums.h"
#include "nodes/expressions/IdentifierNode.h"
#include <iostream>

class VariableDeclarationNode : public Node
{
    ExpressionNode *value;
    EntryType entryType;
    DataType type;
    IdentifierNode *identifier;

public:
    VariableDeclarationNode(yy::location loc, DataType type, IdentifierNode *identifier, ExpressionNode *value = nullptr, bool constant = false)
        : Node(loc)
    {
        this->type = type;
        this->identifier = identifier;
        this->value = value;
        this->entryType = constant ? EntryType::TYPE_CONST : EntryType::TYPE_VAR;
    }

    bool analyzeSemantic() override
    {
        if (value != nullptr)
        {
            if (!value->analyzeSemantic())
                return false;
        }

        SymbolTable *table = SymbolTable::getInstance();
        return table->insert(identifier->name, type, entryType);
    }

    string generateCode() override
    {
        string quad;
        if (value != nullptr)
            quad += value->generateCode();
        quad += Utils::convTypeToQuad(value->type, type);
        quad += Utils::opToQuad(OPR_POP, type) + " " + identifier->name + "\n";
        return quad;
    }
};

#endif // VARIABLE_DECLARATION_NODE
