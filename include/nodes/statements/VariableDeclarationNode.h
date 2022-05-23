#ifndef VARIABLE_DECLARATION_NODE
#define VARIABLE_DECLARATION_NODE


#include "utils/enums.h"
#include "nodes/expressions/IdentifierNode.h"

class VariableDeclarationNode : public Node
{
    ExpressionNode* value;
    EntryType entryType;
    DataType type;
    IdentifierNode* identifier;

public:
    VariableDeclarationNode(yy::location loc, DataType type, IdentifierNode* identifier, ExpressionNode* value = nullptr, bool constant = false)
    : Node(loc) {
        this->type = type;
        this->identifier = identifier;
        this->value = value;
        this->entryType = constant ? EntryType::TYPE_CONST : EntryType::TYPE_VAR;
    }

    bool analyzeSemantic() override
    {
        SymbolTable* table = SymbolTable::getInstance();
        return table->insert(identifier->name, type, entryType);
    }

    string generateCode() override
    {
        throw "Not Implemented yet";
    }
};


#endif //VARIABLE_DECLARATION_NODE
